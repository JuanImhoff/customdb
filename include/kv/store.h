#pragma once
#include <string>
#include <iostream>

namespace kv {
    class Store{
        public:
            Store() = default;
            bool put(const std::string& key, const std::string& value);
            std::string get(const std::string& key);
            bool erase(const std::string& key);
            std::size_t size(const std::string& key);
    };
}