cmake_minimum_required(VERSION 3.10)
project(GNet-Test VERSION 1.0 LANGUAGES CXX)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Assuming GNet's build directory is at a fixed relative path
# Adjust the path as necessary
add_subdirectory(../GNet ${CMAKE_BINARY_DIR}/GNet)

# Add the executable that uses the GNet library
add_executable(GNet-Test main.cpp)

# Link against GNet
target_link_libraries(GNet-Test PRIVATE GNet)

# If GNet requires specifying the include directory for its headers
target_include_directories(GNet-Test PRIVATE ../)
