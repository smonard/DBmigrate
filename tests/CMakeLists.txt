cmake_minimum_required(VERSION 3.3)

project(databaseMigrations_Tests)


add_subdirectory(lib/google-test)

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
add_executable(databaseMigrations_Tests
        test_entry.cpp)
target_link_libraries(databaseMigrations_Tests gtest gtest_main)
target_link_libraries(databaseMigrations_Tests libDatabaseMigrations)