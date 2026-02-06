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
        return m_data.erase(key) > 0;
    }

    std::size_t Store::size(){
        std::size_t size = m_data.size();
        return size;

    }
}