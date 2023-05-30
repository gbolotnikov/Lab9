
#include <ctime>
#include <string>
#include <algorithm>
#include "CommandManager.hpp"

void CommandManager::addWriter(const IWriter& writer) {
    _writers.emplace(&writer);
}

void CommandManager::addWriter(const FileWriter& writer) {
    _fileWriters.emplace(&writer);
}

void CommandManager::newCommand(std::string_view cmd) {
    if (_commands.empty()) {
        _firstCmdtimeStamp = std::time(nullptr);
    }
    _commands.push_back(std::move(cmd.data()));
}

void CommandManager::endCommand() {
    notifyWriter();
    _commands.clear();
}

void CommandManager::notifyWriter() {
    if (_commands.empty()) {
        return;
    }
    std::string out("bulk: ");
    for (auto it = _commands.begin(); it != _commands.end(); ++it) {
        if (it != _commands.begin()) {
            out += ", ";
        }
        out += *it;
    }

    for (const IWriter* writer : _writers) {
        auto func = [writer, out](uint8_t id) {
            // std::this_thread::sleep_for(std::chrono::seconds{1});
            writer->write(out);
        };
        _tWriters.enqueue(func);
    }

    for (const FileWriter* writer : _fileWriters) {
        auto func = [writer, out, timeStamp = _firstCmdtimeStamp](uint8_t id) {
            // std::this_thread::sleep_for(std::chrono::seconds{1});
            writer->write(out, timeStamp, id);
        };
        _tFileWriters.enqueue(func);
    }    
}