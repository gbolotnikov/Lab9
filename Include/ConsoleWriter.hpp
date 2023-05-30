#ifndef CONSOLE_WRITER_HPP
#define CONSOLE_WRITER_HPP

#include <iostream>
#include "Interfaces/Writer.hpp"

class ConsoleWriter : public IWriter {
public:
    ~ConsoleWriter() {
        // std::cout << "Destroy Writer" << std::endl;
    }

    void write(const std::string& str) const override {
        std::cout << str << std::endl;
    }
};

#endif