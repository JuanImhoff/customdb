#include <gtest/gtest.h>
#include "kv/store.h"

TEST(SmokeTest, Basic){ // just testing the configuration
    EXPECT_EQ(1+1,2);
}

/*
INITIAL STATE
size is zero
retrieving value return nullopt
erasing returns false
*/

TEST(StoreTest, InitialState){
    kv::Store store;
    EXPECT_EQ(store.size(),0u);
    ASSERT_FALSE(store.get("missing").has_value());
    ASSERT_FALSE(store.erase("missing"));
}

/*
PUT INSERTION
Test that when inserting a value, retrieving gives the same value
*/

TEST(StoreTest, PutGetResult){ // get should return the input string

    kv::Store store;
    std::string test_value_input = "test_value_john";
    bool put_result = store.put("test_key","test_value_john");
    ASSERT_TRUE(put_result);
    std::optional<std::string> test_value_output = store.get("test_key");
    ASSERT_TRUE(test_value_output.has_value());
    EXPECT_EQ(test_value_input, test_value_output.value());

}

/*
Multiple keys, put and retrieve
*/

/*

GET
Test:
retrieveing a value: already tested in PUT tests
retrieving a non-existent value
retrieving an empty string "" should return the empty string

*/

TEST(StoreTest, GetInexistingKey){ // get a non-existent key should return nullopt

    kv::Store store;
    std::optional<std::string> test_value = store.get("inexistent_key");
    ASSERT_FALSE(test_value.has_value());

}

TEST(StoreTest, EmptyStringValue){
    kv::Store store;
    bool put_result = store.put("test_key","");
    ASSERT_TRUE(put_result);
    EXPECT_EQ(store.size(),1u);
    ASSERT_TRUE(store.get("test_key").has_value());
    EXPECT_EQ(store.get("test_key").value(),"");
}

/*
SIZE: put a certain number of values with dinstinc keys increases the size accordingly.
*/

TEST(StoreTest, PutSize){ // size should return the size of the input string
    std::size_t test_size = 10;
    kv::Store store;
    std::string test_value_input = "test_value_john";
    std::string it_test_key;
    for (size_t i=0;i<test_size;i++){
        it_test_key = "test_key_" + std::to_string(i);
        bool put_result = store.put(it_test_key,"test_value_john");
        ASSERT_TRUE(put_result);    
    }
    std::size_t store_size = store.size();
    EXPECT_EQ(store_size, test_size);

}

TEST(StoreTest, EraseExistingKey){ // getting erased value should return nullopt
    
    kv::Store store;
    std::string test_value_input = "test_value_john";
    bool put_result = store.put("test_key","test_value_john");
    ASSERT_TRUE(put_result);
    bool erase_result = store.erase("test_key");
    ASSERT_TRUE(erase_result);
    std::optional<std::string> deleted_get_value = store.get("test_key");
    ASSERT_FALSE(deleted_get_value.has_value());

}

TEST(StoreTest, EraseMissingKey){
    kv::Store store;
    std::string empty = "";
    bool put_result = store.put("test_key",empty);
    ASSERT_TRUE(put_result);
    std::optional<std::string> empty_get_value = store.get("test_key");
    ASSERT_TRUE(empty_get_value.has_value());
    EXPECT_EQ(empty_get_value, empty);
}

/*
OVERWRITE: when doing overwrite the value returned should be the second
*/

TEST(StoreTest, OverWrite){
    
    kv::Store store;
    std::string test_value_input = "test_value_john";
    std::string other_test_value = "other_value_john";
    bool first_put_result = store.put("test_key", test_value_input);
    size_t expected_size = store.size();
    bool second_put_result = store.put("test_key",other_test_value);
    EXPECT_EQ(store.size(),expected_size); // size is unchanged by overwrote
    ASSERT_TRUE(first_put_result);
    ASSERT_FALSE(second_put_result);
    std::optional<std::string> stored_value = store.get("test_key");
    ASSERT_TRUE(stored_value.has_value());
    EXPECT_EQ(stored_value.value(),other_test_value);


}

/*
INDEPENDENT STORES
Test the independence of the behavior of two kv::Store objects
*/

TEST(StoreTest, TwoStoresAreIndependent){
    kv::Store store_a;
    kv::Store store_b;
    // put-get
    bool put_result_a = store_a.put("test_key","test_value_a");
    bool put_result_b = store_b.put("test_key","test_value_b");
    ASSERT_TRUE(put_result_a);
    ASSERT_TRUE(put_result_b);
    ASSERT_TRUE(store_a.get("test_key").has_value());
    ASSERT_TRUE(store_b.get("test_key").has_value());
    EXPECT_EQ(store_a.get("test_key").value(),"test_value_a");
    EXPECT_EQ(store_b.get("test_key").value(),"test_value_b");
    EXPECT_EQ(store_a.size(),1u);
    EXPECT_EQ(store_b.size(),1u);
    // erase one value
    bool erase_result_a = store_a.erase("test_key");
    ASSERT_TRUE(erase_result_a);
    ASSERT_TRUE(store_b.get("test_key").has_value());
    EXPECT_EQ(store_b.get("test_key").value(),"test_value_b");
    EXPECT_FALSE(store_a.get("test_key").has_value());

}

/*
LIST KEYS
Test the listing keys function
*/

TEST(StoreTest, ListKeys){
    std::size_t test_size = 10;
    kv::Store store;
    std::string test_value_input = "test_value_john";
    std::string it_test_key;
    std::list<std::string> test_list_of_keys;
    std::list<std::string> list_of_keys;
    for (size_t i=0;i<test_size;i++){
        it_test_key = "test_key_" + std::to_string(i);
        test_list_of_keys.push_back(it_test_key);
        bool put_result = store.put(it_test_key,"test_value_john");
        ASSERT_TRUE(put_result);    
    }
    store.listKeys(list_of_keys);
    // compare the two lists
    bool same = true;
    std::unordered_map<std::string,int> freq_map;
    for (const auto& key : list_of_keys){
        freq_map[key]++;
    }
    for (const auto& key : test_list_of_keys){
        if (freq_map[key]-- <0){
            same = false;
        }
    }
    ASSERT_TRUE(same);
}

TEST(StoreTest, Stats){

    kv::Store store;
    // do 2 gets (one missing and one ok), 1 put and 1 erase. assert the results
    std::optional<std::string> miss_opt = store.get("test_key");
    ASSERT_FALSE(miss_opt.has_value());
    bool put_result = store.put("test_key","test_value");
    ASSERT_TRUE(put_result);
    std::optional<std::string> get_value = store.get("test_key");
    ASSERT_TRUE(get_value.has_value());
    bool erase_result = store.erase("test_key");
    ASSERT_TRUE(erase_result);
    // check that the counts correspond.
    kv::Stats stats = store.getStats();
    EXPECT_EQ(stats.put_count, 1u);
    EXPECT_EQ(stats.get_count, 2);
    EXPECT_EQ(stats.hit_count, 1u);
    EXPECT_EQ(stats.miss_count, 1u);
    EXPECT_EQ(stats.erase_count, 1u);
}

