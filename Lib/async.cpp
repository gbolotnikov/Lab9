#include "async.h"
#include <sstream>
#include "CommandManager.hpp"
#include "ConsoleWriter.hpp"
#include "FileWriter.hpp"
#include "ConsoleReader.hpp"

class Manager {
public:    
    Manager(std::size_t bulk) : 
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

    ~Manager() {
        // или здесь вызвать метод join _commandManager, чтобы дождаться потоков
        // std::cout << "Destroy Manager" << std::endl;
    }

private:

    ConsoleWriter _consoleWriter;
    FileWriter _fileWriter = {"bulk"};
    CommandManager _commandManager;
    ConsoleReader _consoleReader;
};


namespace async {

template<class T>
struct maybe_deleter{
  bool _delete;
  explicit maybe_deleter(bool doit = true) : _delete(doit){}

  void operator()(T* p) const{
    if(_delete) delete p;
  }
};

template<class T>
using set_unique_ptr = std::unique_ptr<T, maybe_deleter<T>>;

set_unique_ptr<Manager> make_find_ptr(handle_t raw){
    return set_unique_ptr<Manager>(reinterpret_cast<Manager*>(raw), maybe_deleter<Manager>(false));
}

static std::unordered_map<set_unique_ptr<Manager>, std::string> _connections;
static std::mutex g_mutex;

handle_t connect(std::size_t bulk) {
    std::lock_guard lock(g_mutex);
    auto [it, b] = _connections.emplace(set_unique_ptr<Manager>(new Manager(bulk)), std::string());
    return it->first.get();
}

void receive(handle_t handle, const char *data, std::size_t size) {
    std::lock_guard lock(g_mutex);
    auto it = _connections.find(make_find_ptr(handle));
    if (it == _connections.end()) {
        return;
    }    
    it->second += std::string(data, size);
    std::string_view view(data, size);
    std::size_t pos = view.rfind('\n');
    if (pos == std::string_view::npos) {
        return;
    }
    std::istringstream input(it->second);
    it->second = std::string();
    it->first->receive(input);
}

void disconnect(handle_t handle) {
    std::lock_guard lock(g_mutex);
    auto it = _connections.find(make_find_ptr(handle));
    if (it == _connections.end()) {
        return;
    }  
    it->first->end();
    _connections.erase(make_find_ptr(handle));
}

}
