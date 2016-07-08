cmake_minimum_required(VERSION 3.4)
project(databaseMigrations)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES entry.cpp entity/SchemaVersion.hpp
entity/SchemaVersion-odb.cxx entity/SchemaVersion-odb.hxx entity/SchemaVersion-odb.ixx )

find_library(OBDPGSQL_LIBRARY odb-pgsql)
find_library(POSTGRES_DRIVER pq)
find_library(OBD_LIBRARY odb)

add_library(libDatabaseMigrations STATIC ${SOURCE_FILES})

target_link_libraries(libDatabaseMigrations  ${OBD_LIBRARY} ${OBDPGSQL_LIBRARY} ${POSTGRES_DRIVER})
