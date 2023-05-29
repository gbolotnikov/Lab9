#ifndef COMMAND_MANAGER_HPP
#define COMMAND_MANAGER_HPP

#include <unordered_set>
#include "ThreadPool.hpp"
#include "FileWriter.hpp"
#include "Interfaces/Manager.hpp"

class CommandManager: public IManager{
public:
    void addWriter(const IWriter& writer) override;
    void addWriter(const FileWriter& writer);
    void newCommand(std::string_view cmd) override;
    void endCommand() override;

    ~CommandManager() = default;
private:

    void notifyWriter();

    ThreadPool _tWriters = {1};
    ThreadPool _tFileWriters = {2};
    

    size_t _firstCmdtimeStamp;
    std::vector<std::string> _commands;
    std::unordered_set<const IWriter*> _writers;
    std::unordered_set<const FileWriter*> _fileWriters;

};

#endif