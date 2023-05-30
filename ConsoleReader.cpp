#include <iostream>
#include "ConsoleReader.hpp"

ConsoleReader::ConsoleReader(IManager& cmdManager, size_t commandCount): 
_cmdManager(cmdManager),
_commandCount(commandCount)
{

}

void ConsoleReader::setStream(std::istream& input) {
    _input = &input;
}

void ConsoleReader::read() {
    std::string line;
    while (std::getline(*_input, line)) {
        if (line == "{") {
            openBrace();
        }
        else if (line == "}") {
            closeBrace();
        } else {
            ++_linesCount;
            notifyNewCommand(line);
            if (_linesCount == _commandCount && _bracesCount == 0) {
                _linesCount = 0;
                notifyEndCommand();
            }
        }
    }
}

void ConsoleReader::notifyNewCommand(std::string_view cmd) {
    _cmdManager.newCommand(cmd);
}

void ConsoleReader::notifyEndCommand() {
    _cmdManager.endCommand();
}

void ConsoleReader::openBrace() {
    _linesCount = 0;
    ++_bracesCount;
    if (_bracesCount == 1) {
        notifyEndCommand();
    }
}

void ConsoleReader::closeBrace() {
    _linesCount = 0;
    --_bracesCount;
    if (_bracesCount == 0) {
        notifyEndCommand();
    }
}