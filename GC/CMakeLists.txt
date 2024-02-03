cmake_minimum_required(VERSION 3.10)
project(Packets VERSION 1.0 LANGUAGES CXX)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Assuming GNet is a sibling directory to Packets and has been built before Packets
# Adjust the relative path to GNet as necessary
add_subdirectory(../GNet ${CMAKE_BINARY_DIR}/GNet)

file(GLOB_RECURSE SOURCE_FILES "Packets/*.cpp")


# Define the library target
add_library(GC STATIC ${SOURCE_FILES})


# Specify include directories for this library
target_include_directories(GC PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        # If GNet has public headers that Packets need to include
        ../
)

# Link the library with GNet
target_link_libraries(GC PRIVATE GNet)
