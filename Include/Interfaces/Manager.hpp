#ifndef MANAGER_HPP
#define MANAGER_HPP

#include "stdint.h"
#include <vector>
#include <string>
#include "Writer.hpp"

class IManager {
    public: 
        virtual void endCommand() = 0;
        virtual void newCommand(std::string_view cmd) = 0;
        virtual void addWriter(const IWriter& writer) = 0;        
        virtual ~IManager() = default;
};

#endif