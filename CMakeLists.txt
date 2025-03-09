cmake_minimum_required(VERSION 3.28)
project(RayTracing LANGUAGES CXX)

# set(CMAKE_CXX_STANDARD 11)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.0
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

add_executable(space space.cpp)
# target_compile_features(main PRIVATE cxx_std_17)
target_link_libraries(space PRIVATE SFML::Graphics)

# add_compile_options(-fopenmp)
message(STATUS, "CXXFLAGS: ${CMAKE_CXX_FLAGS}")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -fopenmp" )
message(STATUS, "CXXFLAGS: ${CMAKE_CXX_FLAGS}")