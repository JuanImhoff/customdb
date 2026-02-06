#include <gtest/gtest.h>
#include "kv/store.h"

TEST(SmokeTest, Basic){ // just testing the configuration
    EXPECT_EQ(1+1,2);
}

TEST(StoreTest, PutGetResult){ // get should return the input string

    kv::Store store;
    std::string test_value_input = "test_value_john";
    bool putResult = store.put("test_key","test_value_john");
    ASSERT_TRUE(putResult);
    std::optional<std::string> test_value_output = store.get("test_key");
    ASSERT_TRUE(test_value_output.has_value());
    EXPECT_EQ(test_value_input, test_value_output.value());

}

TEST(StoreTest, GetInexistingKey){ // get a non-existent key should return nullopt

    kv::Store store;
    std::optional<std::string> test_value = store.get("inexistent_key");
    ASSERT_FALSE(test_value.has_value());

}

TEST(StoreTest, PutSize){ // size should return the size of the input string

    kv::Store store;
    std::string test_value_input = "test_value_john";
    bool putResult = store.put("test_key","test_value_john");
    ASSERT_TRUE(putResult);
    std::size_t test_value_size = store.size("test_key");
    std::size_t size_output = test_value_input.size();
    EXPECT_EQ(test_value_size, size_output);

}

TEST(StoreTest, Erase){ // getting erased value should return empty string
    
    kv::Store store;
    std::string test_value_input = "test_value_john";
    std::string empty = "";
    bool putResult = store.put("test_key","test_value_john");
    ASSERT_TRUE(putResult);
    bool eraseResult = store.erase("test_key");
    ASSERT_TRUE(eraseResult);
    std::optional<std::string> deletedGetValue = store.get("test_key");
    ASSERT_FALSE(deletedGetValue.has_value());
    EXPECT_EQ("",deletedGetValue);

}

TEST(StoreTest, OverWrite){
    
    kv::Store store;
    std::string test_value_input = "test_value_john";
    std::string other_test_value = "other_value_john";
    bool firstPutResult = store.put("test_key", test_value_input);
    bool secondPutResult = store.put("test_key",other_test_value);
    ASSERT_TRUE(firstPutResult);
    ASSERT_TRUE(secondPutResult);
    std::optional<std::string> stored_value = store.get("test_key");
    ASSERT_TRUE(stored_value.has_value());
    EXPECT_EQ(stored_value.value(),other_test_value);


}

