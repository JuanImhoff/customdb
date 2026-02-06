#include "kv/store.h"

namespace kv{
    bool Store::put(const std::string& key, const std::string& value){
        std::cout << key << std::endl << value << std::endl;
        return true;
    }

    std::optional<std::string> Store::get(const std::string& key){
        std::cout << key << std::endl;
        return "default_return_value";
    }

    bool Store::erase(const std::string& key){
        std::cout << key << std::endl;
        return true;
    }

    std::size_t Store::size(const std::string& key){
        std::cout << key << std::endl;
        std::size_t size = 0;
        return size;

    }
}