#include <fstream>
#include <optional>

class Reader {
    public:
        Reader(const std::string& file_path);
        
        uint16_t Read(const uint8_t bit_count);
        std::string FileName() const;
        size_t FileSize();        
        bool IsEOF();

    private:
        std::ifstream file_;
        std::string file_path_;
        size_t file_size_ = 0;
        size_t bytes_read_ = 0;
        uint16_t buffer_ = 0;
        size_t available_bits_ = 0; 
        bool ReadByte();
};
