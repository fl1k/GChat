cmake_minimum_required(VERSION 3.10)
project(GC-Client VERSION 1.0 LANGUAGES CXX)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# If GC is a sibling directory and you have access to its CMakeLists.txt
# Otherwise, you might need to find GC using find_package() or similar
add_subdirectory(../GC ${CMAKE_BINARY_DIR}/GC)

# Add the executable target for GC-Client
add_executable(GC-Client main.cpp Config/Config.cpp)

# Specify include directories for GC-Client
target_include_directories(GC-Client PRIVATE Config)

# Link the executable with the GC library
target_link_libraries(GC-Client PRIVATE GC)
