#include "kv/store.h"

namespace kv{
    bool Store::put(const std::string& key, const std::string& value){
        std::unique_lock lock(m_mutex);
        std::cout << key << std::endl << value << std::endl;
        auto [it, inserted] = m_data.insert_or_assign(key, value);
        m_stats.put_count++;
        return inserted;
    }

    std::optional<std::string> Store::get(const std::string& key){
        std::shared_lock lock(m_mutex);
        auto it = m_data.find(key);
        m_stats.get_count++;
        if (it == m_data.end()){
            m_stats.miss_count++;
            return std::nullopt;
        }
        else{
            std::string value = it->second;
            m_stats.hit_count++;
            return value;
        }
    }

    bool Store::erase(const std::string& key){
        std::unique_lock lock(m_mutex);
        std::cout << key << std::endl;
        if (m_data.erase(key) > 0){
            m_stats.erase_count++;
            return true;
        }
        else{
            return false;
        }
    }

    void Store::listKeys(std::list<std::string>& list_of_keys){
        list_of_keys.clear();
        for (auto it = m_data.begin(); it!= m_data.end(); it++){
            list_of_keys.push_back(it->first);
        }
        return;
    }

    std::size_t Store::size(){
        std::shared_lock lock(m_mutex);
        std::size_t size = m_data.size();
        return size;

    }
}