#ifndef CONSOLE_READER_HPP
#define CONSOLE_READER_HPP

#include "Interfaces/Manager.hpp"

class ConsoleReader {
public:
    void read();
    ConsoleReader(IManager& cmdManager, size_t commandCount);
private:    
    void openBrace();
    void closeBrace();
    void notifyEndCommand();
    void notifyNewCommand(std::string_view cmd);
    
    size_t _linesCount = 0;
    size_t _bracesCount = 0;
    size_t _commandCount;
    IManager& _cmdManager;

};

#endif