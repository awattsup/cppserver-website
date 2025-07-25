cmake_minimum_required(VERSION 3.14)
project(cppserver)
set(CMAKE_CXX_STANDARD 17)

# include(FetchContent)
# FetchContent_Declare(
#   crow
#   GIT_REPOSITORY https://github.com/CrowCpp/Crow.git
#   GIT_TAG v1.0+1
# )
# FetchContent_MakeAvailable(crow)

# Prefer pthreads for threading
set(THREADS_PREFER_PTHREAD_FLAG ON)


# Find Threads library
find_package(Threads REQUIRED)

# Enable static linking for Boost
set(Boost_USE_STATIC_LIBS ON)

# Find Boost libraries
find_package(Boost 1.88 REQUIRED COMPONENTS system filesystem)



# Collect all source files in src/
file(GLOB SOURCES src/*.cpp)

add_executable(cppserver ${SOURCES} main.cpp)

# include_directories(/opt/homebrew/include)
# link_directories(/opt/homebrew/include)

# Include directories
target_include_directories(cppserver PRIVATE ${Boost_INCLUDE_DIRS} include)
target_link_libraries(cppserver PRIVATE /opt/homebrew/lib/libjsoncpp.dylib)


# target_link_libraries(cppserver PRIVATE crow::crow)
# Link against Boost and threading libraries
find_package(Boost REQUIRED COMPONENTS log log_setup filesystem system thread)
target_link_libraries(cppserver
    PRIVATE
        Boost::log
        Boost::log_setup
        Boost::filesystem
        Boost::system
        Boost::thread
        Threads::Threads
)