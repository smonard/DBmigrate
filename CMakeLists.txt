cmake_minimum_required(VERSION 3.4)
project(HDIH_WEB_API)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp )
add_executable(databaseMigrations_run ${SOURCE_FILES})

include_directories(src)

link_directories(/usr/local/lib)
link_directories(/usr/local)

add_subdirectory(src)
add_subdirectory(tests)

target_link_libraries(databaseMigrations_run libDatabaseMigrations)
