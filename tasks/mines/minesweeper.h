#pragma once

#include <string>
#include <vector>

class Minesweeper {
public:
    struct Cell {
        size_t x = 0;
        size_t y = 0;
    };

    enum class GameStatus {
        NOT_STARTED,
        IN_PROGRESS,
        VICTORY,
        DEFEAT,
    };

    using RenderedField = std::vector<std::string>;

    Minesweeper(size_t width, size_t height, size_t mines_count);
    Minesweeper(size_t width, size_t height, const std::vector<Cell>& cells_with_mines);

    void NewGame(size_t width, size_t height, size_t mines_count);
    void NewGame(size_t width, size_t height, const std::vector<Cell>& cells_with_mines);

    void OpenCell(const Cell& cell);
    void MarkCell(const Cell& cell);

    GameStatus GetGameStatus() const;
    time_t GetGameTime() const;

    RenderedField RenderField() const;
    ~Minesweeper();

private:
    struct CellStatus {
        bool is_mine = false;
        bool is_cell_opened = false;
        bool is_flag_set = false;
    };
    size_t width_ = 0, height_ = 0;
    GameStatus game_status_ = GameStatus::NOT_STARTED;
    CellStatus* cells_ = nullptr;
    time_t start_time_;
    CellStatus GetCell(const Cell& cell) const;
    void SetCellOpened(const Cell& cell, bool is_opened);
    void SetMine(const Cell& cell, bool is_mine);
    void SetFlagSet(const Cell& cell, bool is_flag_set);
    void MemoryDestroy();
};
