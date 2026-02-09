#pragma once
#include <string>
#include <iostream>
#include <optional>
#include <unordered_map>
#include <shared_mutex>
#include <mutex>

namespace kv {
    class Store{
        public:
            Store(){m_data.reserve(1024);} // in prod one shouldn't use this default constructor, but set the reserve size
            Store(std::size_t reserve_size){m_data.reserve(reserve_size);}
            bool put(const std::string& key, const std::string& value);
            std::optional<std::string> get(const std::string& key);
            bool erase(const std::string& key);
            std::size_t size();

        private:
            std::unordered_map<std::string,std::string> m_data;
            std::shared_mutex m_mutex;

    };
}