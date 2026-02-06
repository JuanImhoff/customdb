#include "kv/store.h"

namespace kv{
    bool Store::put(const std::string& key, const std::string& value){
        std::cout << key << std::endl << value << std::endl;
        auto [it, inserted] = m_data.insert_or_assign(key, value);
        return inserted;
    }

    std::optional<std::string> Store::get(const std::string& key){
        std::cout << key << std::endl;
        auto it = m_data.find(key);
        if (it == m_data.end()){
            return std::nullopt;
        }
        else{
            std::string value = it->second;
            return value;
        }
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