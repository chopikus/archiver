#include <fstream>
#include <optional>

class Reader {
    public:
        Reader(const std::string& file_path);
        
        bool Read1();
        uint16_t Read9();
        uint8_t Read8();
        std::string FileName() const;
        size_t FileSize();        
        bool IsEOF();

    private:
        std::ifstream file_;
        std::string file_path_;
        size_t file_size_ = 0;
        size_t bytes_read_ = 0;
        uint8_t buf_;
        size_t buf_pos_ = 8; 
        unsigned char ReadByte();
};
