#ifndef FILE_WRITER_HPP
#define FILE_WRITER_HPP

#include <string>
#include "Interfaces/Writer.hpp"

class FileWriter : public IWriter {
public:
    FileWriter(std::string_view name);
    void write(const std::string& str) const override;
    void write(const std::string& str, uint64_t timestamp, uint8_t id) const;
    
    ~FileWriter() = default;
private:
    std::string _name;
    static constexpr std::string_view FileExtension {".log"};
};

#endif