#include <map>
#include "minesweeper.h"
#include <queue>

int Random(int min, int max)  // range : [min, max]
{
    return min + rand() % ((max + 1) - min);
}

Minesweeper::CellStatus Minesweeper::GetCell(const Minesweeper::Cell& cell) const {
    return cells_[cell.y * width_ + cell.x];
}

void Minesweeper::SetCellOpened(const Minesweeper::Cell& cell, bool is_opened) {
    cells_[cell.y * width_ + cell.x].is_cell_opened = is_opened;
}

void Minesweeper::SetMine(const Minesweeper::Cell& cell, bool is_mine) {
    cells_[cell.y * width_ + cell.x].is_mine = is_mine;
}

void Minesweeper::SetFlagSet(const Minesweeper::Cell& cell, bool is_flag_set) {
    cells_[cell.y * width_ + cell.x].is_flag_set = is_flag_set;
}

Minesweeper::Minesweeper(size_t width, size_t height, const std::vector<Cell>& cells_with_mines) {
    NewGame(width, height, cells_with_mines);
}

Minesweeper::Minesweeper(size_t width, size_t height, size_t mines_count) {
    NewGame(width, height, mines_count);
}

void Minesweeper::NewGame(size_t width, size_t height, size_t mines_count) {
    MemoryDestroy();
    srand(time(NULL));
    this->width_ = width;
    this->height_ = height;
    cells_ = new CellStatus[width * height];
    start_time_ = time(NULL);
    game_status_ = GameStatus::NOT_STARTED;
    int mn = 0;
    for (size_t i = 0; i < mines_count; ++i) {
        int r = Random(mn, width * height - (mines_count - i));
        cells_[r].is_mine = true;
        mn = r + 1;
    }
}

void Minesweeper::NewGame(size_t width, size_t height, const std::vector<Cell>& cells_with_mines) {
    MemoryDestroy();
    srand(time(NULL));
    this->width_ = width;
    this->height_ = height;
    cells_ = new CellStatus[width * height];
    start_time_ = time(NULL);
    game_status_ = GameStatus::NOT_STARTED;

    for (const Cell& cell : cells_with_mines) {
        cells_[cell.y * width + cell.x].is_mine = true;
    }
}

void Minesweeper::OpenCell(const Cell& cell) {
    if (game_status_ == GameStatus::NOT_STARTED) {
        game_status_ = GameStatus::IN_PROGRESS;
    }
    if (game_status_ == GameStatus::VICTORY || game_status_ == GameStatus::DEFEAT) {
        return;
    }
    if (GetCell(cell).is_flag_set) {
        return;
    }
    std::queue<Cell> q;
    q.push(cell);
    std::map<std::pair<size_t, size_t>, bool> used;
    while (!q.empty()) {
        Cell cur = q.front();
        size_t x = cur.x;
        size_t y = cur.y;
        used[{x, y}] = true;
        SetCellOpened(cur, true);
        q.pop();
        if (GetCell(cur).is_mine) {
            for (size_t x = 0; x < width_; ++x) {
                for (size_t y = 0; y < height_; ++y) {
                    SetCellOpened(Cell{x, y}, true);
                }
            }
            game_status_ = GameStatus::DEFEAT;
            return;
        } else {
            bool has_neighbour_mine = false;
            for (int dx = -1; dx <= 1; ++dx) {
                for (int dy = -1; dy <= 1; ++dy) {
                    if (dx == 0 && dy == 0) {
                        continue;
                    }
                    if (x + dx >= 0 && x + dx < width_ && y + dy >= 0 && y + dy < height_) {
                        if (GetCell(Cell{x + dx, y + dy}).is_mine) {
                            has_neighbour_mine = true;
                        }
                    }
                }
            }
            if (!has_neighbour_mine) {
                for (int dx = -1; dx <= 1; ++dx) {
                    for (int dy = -1; dy <= 1; ++dy) {
                        if (dx == 0 && dy == 0) {
                            continue;
                        }
                        if (x + dx >= 0 && x + dx < width_ && y + dy >= 0 && y + dy < height_) {
                            if (!GetCell(Cell{x + dx, y + dy}).is_flag_set && !used[{x + dx, y + dy}]) {
                                used[{x + dx, y + dy}] = true;
                                q.push(Cell{x + dx, y + dy});
                            }
                        }
                    }
                }
            }
        }
    }
    bool won = true;
    for (size_t x = 0; x < width_; ++x) {
        for (size_t y = 0; y < height_; ++y) {
            Minesweeper::CellStatus status = GetCell(Cell{x, y});
            if (!status.is_cell_opened && !status.is_mine) {
                won = false;
            }
        }
    }
    if (won) {
        game_status_ = GameStatus::VICTORY;
    }
};

void Minesweeper::MarkCell(const Cell& cell) {
    if (game_status_ == GameStatus::VICTORY || game_status_ == GameStatus::DEFEAT) {
        return;
    }
    bool is_flag_set = GetCell(cell).is_flag_set;
    SetFlagSet(cell, !is_flag_set);
}

Minesweeper::RenderedField Minesweeper::RenderField() const {
    std::vector<std::string> result;
    for (size_t y = 0; y < height_; ++y) {
        std::string row;
        for (size_t x = 0; x < width_; ++x) {
            CellStatus status = GetCell(Cell{x, y});
            if (status.is_flag_set) {
                row += "?";
            } else if (!status.is_cell_opened) {
                row += "-";
            } else if (status.is_mine) {
                row += "*";
            } else {
                char neighbour_mines = '0';
                for (int dx = -1; dx <= 1; ++dx) {
                    for (int dy = -1; dy <= 1; ++dy) {
                        if (x + dx >= 0 && x + dx < width_ && y + dy >= 0 && y + dy < height_) {
                            if (GetCell(Cell{x + dx, y + dy}).is_mine) {
                                neighbour_mines++;
                            }
                        }
                    }
                }
                if (neighbour_mines == '0') {
                    row += ".";
                } else {
                    row += neighbour_mines;
                }
            }
        }
        result.push_back(row);
    }
    return result;
}

time_t Minesweeper::GetGameTime() const {
    return time(NULL) - start_time_;
};

Minesweeper::GameStatus Minesweeper::GetGameStatus() const {
    return game_status_;
}

void Minesweeper::MemoryDestroy() {
    if (cells_ != nullptr) {
        delete[] cells_;
        cells_ = nullptr;
    }
}

Minesweeper::~Minesweeper() {
    Minesweeper::MemoryDestroy();
}
