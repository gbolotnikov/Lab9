#include <iostream>
#include "CommandManager.hpp"
#include "ConsoleWriter.hpp"
#include "FileWriter.hpp"
#include "ConsoleReader.hpp"


int main(int argc, char* argv[])
{
    // if (argc > 1) {
        size_t count = 3;//std::atoi(argv[1]);
        ConsoleWriter consoleWriter;
        FileWriter fileWriter("bulk");
        CommandManager commandManager;
        commandManager.addWriter(consoleWriter);
        commandManager.addWriter(fileWriter);
        ConsoleReader consoleReader(commandManager, count);
        consoleReader.read();
    // } else {
    //     std::cout << "Неверное количество параметров" << std::endl;
    // }
    
}
