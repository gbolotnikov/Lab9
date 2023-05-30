#include "async.h"
#include <sstream>
#include "CommandManager.hpp"
#include "ConsoleWriter.hpp"
#include "FileWriter.hpp"
#include "ConsoleReader.hpp"

class Process {
public:    
    Process(std::size_t bulk) : 
        _consoleReader(_commandManager, bulk)
    {
        _commandManager.addWriter(_consoleWriter);
        _commandManager.addWriter(_fileWriter);
    }

    void receive(std::istream& input) {
        _consoleReader.setStream(input);
        _consoleReader.read();
    }

    void end() {
        _consoleReader.notifyEndCommand();
    }

    ~Process() {
        // или здесь вызвать метод join _commandManager, чтобы дождаться потоков
        // std::cout << "Destroy Process" << std::endl;
    }

private:

    ConsoleWriter _consoleWriter;
    FileWriter _fileWriter = {"bulk"};
    CommandManager _commandManager;
    ConsoleReader _consoleReader;
};


namespace async {

// template<class T>
// struct maybe_deleter{
//   bool _delete;
//   explicit maybe_deleter(bool doit = true) : _delete(doit){}

//   void operator()(T* p) const{
//     if(_delete) delete p;
//   }
// };

// template<class T>
// using set_unique_ptr = std::unique_ptr<T, maybe_deleter<T>>;

// set_unique_ptr<Process> make_find_ptr(handle_t raw){
//     return set_unique_ptr<Process>(reinterpret_cast<Process*>(raw), maybe_deleter<Process>(false));
// }

static std::unordered_map<handle_t, std::pair<std::unique_ptr<Process>, std::string>> _connections;
static std::mutex g_mutex;

handle_t connect(std::size_t bulk) {
    std::lock_guard lock(g_mutex);
    auto process = std::make_unique<Process>(bulk);
    auto handle = process.get();
    auto [it, b] = _connections.emplace(handle, std::make_pair(std::move(process), std::string()));
    return handle;
}

void receive(handle_t handle, const char *data, std::size_t size) {
    std::lock_guard lock(g_mutex);
    auto it = _connections.find(handle);
    if (it == _connections.end()) {
        return;
    }    
    auto& pair = it->second;
    pair.second += std::string(data, size);
    std::string_view view(data, size);
    std::size_t pos = view.rfind('\n');
    if (pos == std::string_view::npos) {
        return;
    }
    std::istringstream input(pair.second);
    pair.second = std::string();
    pair.first->receive(input);
}

void disconnect(handle_t handle) {
    std::lock_guard lock(g_mutex);
    auto it = _connections.find(handle);
    if (it == _connections.end()) {
        return;
    }  
    it->second.first->end();
    _connections.erase(it);
}

}
