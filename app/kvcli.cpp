#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <iomanip>
#include "kv/store.h"

/*
Helper functions: functions calling the store API to execute commands
*/

/*
Handle a command: Parse a line and call helper functions to fulfil the command
* CLI commands:
SET
GET
SIZE
DEL
EXIT
* CLI outputs:
ERR missing key
ERR unknown command
string value
uint size

*/

int handle_command(std::string &line, kv::Store& store){
    // parse the command ID
    std::istringstream iss(line);
    std::string command;
    iss >> command; 
    // handle SET
    if (command == "SET"){
        std::string key;
        if (!(iss >> key)){ // read the key, and display err if the key is missing
            std::cout << "ERR Missing key" << std::endl;
            return 0;
        }
        // parse the value
        std::string value;
        iss >> std::ws;
        std::getline(iss,value);
        // call put API
        bool put_result = store.put(key, value);
        if (put_result){
            std::cout << "value inserted" << std::endl;
        }
        else {
            std::cout << "value overwritten" << std::endl;
        }
        return 1;
    }

    if (command == "GET"){
        std::string key;
        if (!(iss >> key)){
            std::cout << "ERR Missing key" << std::endl;
            return 0;
        }
        std::string rest;
        if (iss >> rest){
            std::cout << "ERR key shouldn't have a space" << std::endl;
            return 0;
        }
        else{ // call API get
            std::optional<std::string> value_opt = store.get(key);
            if (value_opt.has_value()){
                std::string value = value_opt.value();
                std::cout << "key: " << key << "| value: " << value << std::endl;
                return 1;
            }
        }
    }

    if (command == "DEL"){
        std::string key;
        if (!(iss >> key)){
            std::cout << "ERR Missing key" << std::endl;
            return 0;
        }
        std::string rest;
        if (iss >> rest){
            std::cout << "ERR key shouldn't have a space" << std::endl;
            return 0;
        }
        else{ // call API get
            std::optional<std::string> value_opt = store.get(key);
            bool erase_result = store.erase(key);
            if (erase_result){
                std::cout << "value erased from storage" << std::endl;
            }
            else{
                std::cout << "key not in storage" << std::endl;
            }
            if (value_opt.has_value()){
                std::string value = value_opt.value();
                std::cout << "deleted: key: " << key << "| value: " << value << std::endl;
            }
            return 1;
        }
    }

    if (command == "SIZE"){
        std::size_t size = store.size();
        std::cout << "storage size: " << size << std::endl;
        return 1;
    }

    if (command == "HELP"){ // print help info (in txt file)
        std:: fstream file("app/help.txt");
        
        if (!file.is_open()) {
        std::cout << "Error opening file!" << std::endl;
        return 0;
    }

        std::string line;
        while (std::getline(file, line)){
            std::cout << line << std::endl;
        }
        file.close();
        return 1;
    }

    if (command == "EXIT"){
        return 2;
    }

    if (command == "DISPLAY"){
        std::list<std::string> list_of_keys;
        store.listKeys(list_of_keys);
        for (auto& key : list_of_keys){
            std::optional<std::string> value_opt = store.get(key);
            if (value_opt.has_value()){
                std::cout  << "| \033[31m" << std::setw(15)<< key << "\033[0m | " << "\033[33m" << std::setw(15) << value_opt.value() << "\033[0m | " << std::endl;
            }
        }
        return 1;
    }

    else{
        std::cout << "Unknow command" << std::endl;
        return 0;
    }



    

}


/*
Main CLI function

*/
int main(){
    std::cout << "app kv-cli starting" << std::endl;
    // get line
    std::string line;
    kv::Store store;
    while (true){
        std::cout << "Enter command: " << std::endl;
        std::getline(std::cin, line);
        int res = handle_command(line, store);
        if (res ==1) std::cout << "command succeeded" << std::endl;
        if (res==0) std::cout << "command failed" << std::endl;
        if (res==2) {
            std::cout << "exiting CLI" << std::endl;
            return 0;
        }
    }
    return 0;
}
