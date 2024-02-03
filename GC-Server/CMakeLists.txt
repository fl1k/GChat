cmake_minimum_required(VERSION 3.10)
project(GC-Server VERSION 1.0 LANGUAGES CXX)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Assuming GC and GNet are sibling directories to GC-Server
# and they have been built before building GC-Server
add_subdirectory(../GC ${CMAKE_BINARY_DIR}/GC)

# Collect all source files
file(GLOB_RECURSE SERVER_SOURCES
        "Misc/*.cpp"
        "Server/*.cpp"
        "Server/Client/*.cpp"
        "Server/Command/*.cpp"
        "Server/Database/*.cpp"
        main.cpp)

# Define the executable target
add_executable(GC-Server ${SERVER_SOURCES})

# Specify include directories for this executable
target_include_directories(GC-Server PRIVATE
        Misc
        Server
        Server/Client
        Server/Command
        Server/Database
)

find_package(SQLite3 REQUIRED) # Finds the SQLite3 library

# Link SQLite3 with your target (e.g., GC-Server executable)
target_link_libraries(GC-Server PRIVATE SQLite::SQLite3)


# Link the executable with GC and GNet libraries
target_link_libraries(GC-Server PRIVATE GC GNet)
