#pragma once
#include <string>
#include <iostream>
#include <optional>
#include <unordered_map>
#include <shared_mutex>
#include <mutex>
#include <list>

namespace kv {
    /**
    * @brief Thread-safe in-memory key-value storage
    *
    *        Provides O(1) insert, lookup and erase
    * @warning no persistency
    */
    struct Stats{
        std::size_t put_count=0;
        std::size_t get_count=0;
        std::size_t erase_count=0;
        std::size_t miss_count=0;
        std::size_t hit_count=0;    
    };

    class Store{
        public:
            Store(){m_data.reserve(1024);} // in prod one shouldn't use this default constructor, but set the reserve size
            Store(std::size_t reserve_size){m_data.reserve(reserve_size);}
            /**
            * @brief insert or overwrite a key-value pair
            * @return true if the value was inserted, false if it was overwritten
            */
            bool put(const std::string& key, const std::string& value);
            /**
            * @brief retrieve the value for a given key
            * @return std::optional containing the value if it exists with the associated key
            *         std::nullopt if the key is not existent  
            */
            std::optional<std::string> get(const std::string& key);
            /**
            * @brief erase a key-value from the storage
            * @return true if the value was erased, false if it was not there at first
            */
            bool erase(const std::string& key);
            /**
            * @brief get a list of the keys in the table
            * @return void, modify as reference
            */
            void listKeys(std::list<std::string>& list_of_keys);
            /**
            * @brief return the number of elements stored
            * @return the size of the storage
            */
            std::size_t size();
            /**
            * @brief get stats of storage usage
            * @return a Stats struct with statistic data (uints)
            */
            Stats getStats(){return m_stats;};

        private:
            std::unordered_map<std::string,std::string> m_data;
            std::shared_mutex m_mutex;
            Stats m_stats;

    };
}