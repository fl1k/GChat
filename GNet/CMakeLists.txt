cmake_minimum_required(VERSION 3.10)
project(GNet VERSION 1.0 LANGUAGES CXX)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Collect all source files
file(GLOB_RECURSE SOURCE_FILES "Core/*.cpp")

# Include directories for the library
include_directories(Core)

# Create the library
add_library(GNet STATIC ${SOURCE_FILES})

# Specify include directory for the users of the library
target_include_directories(GNet PUBLIC ${PROJECT_SOURCE_DIR})
