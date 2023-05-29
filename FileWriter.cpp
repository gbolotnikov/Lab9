#include <chrono>
#include <fstream>
#include "FileWriter.hpp"


FileWriter::FileWriter(std::string_view name): 
    _name(std::move(name.data()))
{

}

void FileWriter::write(const std::string& str) const {
    std::string filename = _name + FileExtension.data();
    std::ofstream file(filename);
    file << str << '\n';
}

void FileWriter::write(const std::string& str, uint64_t timestamp, uint8_t id) const {
    auto addPrefix = "_" + std::to_string(id);
    std::string filename = _name + std::to_string(timestamp) + addPrefix  + FileExtension.data();
    std::ofstream file(filename);
    file << str << '\n';
}