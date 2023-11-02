cmake_minimum_required(VERSION 3.24)
project(Networking)

set(CMAKE_CXX_STANDARD 20)
find_package(Boost REQUIRED)
file(GLOB_RECURSE SOURCES src/*.cpp)
add_library(${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME}
        PUBLIC
            $<INSTALL_INTERFACE:include>
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        PRIVATE
            ${BOOST_INCLUDE_DIRS}
)
target_link_directories(${PROJECT_NAME}
        PRIVATE
            ${BOOST_LIBRARIES}
)