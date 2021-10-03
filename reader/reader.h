#include <fstream>
#include <optional>

class Reader {
    public:
        Reader(const std::string& file_path);

        unsigned char read_next_byte();
        bool has_next_byte() const;
        std::string file_name() const;
        size_t file_size();

    private:
        std::ifstream file_;
        std::string file_path_;
        size_t file_size_ = 0;
        size_t bytes_read_ = 0;
};
