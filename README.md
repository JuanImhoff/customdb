# customdb

A lightweight key–value storage engine written in **C++17**. Designed to be used in the development of a custom database system.

This project is built as a learning and portfolio exercise to demonstrate:
- modern C++ practices
- CMake-based builds
- automated testing
- professional version control workflows

It includes:
- in-memory thread-safe key-value storage
- CLI with access to all API operations + table display of the stored data
- [in the future Phase II] a WAL persistence,and regular snapshots for crash recovery
- [in future Phase II] a DB platform based on the storage tool

---

## Requirements

### Build tools
- CMake ≥ 3.16
- A C++17-compatible compiler
  - GCC (recommended, tested)

### Tested environments
- Linux (Ubuntu, incl. WSL)
- GitHub Actions (Ubuntu)

---

## Project structure

### Store API
A simple key-value storage, with API defined in Store.h

#### Technical choices
- Use of std::unordered_map to allow fast insertion and access to stored data
- Store empty at initalization
- Get returns a std::optional object : allows to handle the inexisting keys without a heavy error-management
- Put overwrites automatically when the key is already in the storage. I a safety is needed in the future it can be added in higher layers.
- Concurrency: thread-safety ensured by a shared_mutex, forbidding any action simultaneous to put and erase. Allowing simultaneous get (as it has no modification of the memory). The mutex is locked using std::lock, to ensure automatic unlock when the lock gets out of scope.


### Storage CLI
A command-line interface creating a storage instance and allowing all operations

![App Screenshot](images/CLI_screeshot.png)

### Tests
Units tests using GoogleTest covering the API methods.
The CLI is not tested, as it is simple, and is only used for demo of Phase I output. 

# Workflows
## Tests

Unit tests using GoogleTest: 
- done manually when building
- Run automatically in GitHub actions at every push and every merge

Doxygen documentation of the storage library
- Done when merging in main

