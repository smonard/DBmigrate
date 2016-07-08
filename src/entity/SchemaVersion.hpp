//
// Created by josilvam on 6/30/16.
//

#ifndef HDIH_WEB_API_SCHEMAVERSION_H
#define HDIH_WEB_API_SCHEMAVERSION_H

#include <string>
#include <cstddef> // std::size_t

#include <odb/core.hxx>
#include <odb/nullable.hxx>

// Include TR1 <memory> header in a compiler-specific fashion. Fall back
// on the Boost implementation if the compiler does not support TR1.
//
#include <odb/tr1/memory.hxx>


#pragma db view query("SELECT last_applied_id " \
                        "FROM schema_version ORDER BY last_applied_id DESC "\
                        "LIMIT 1")
struct schema_db_version
{
#pragma db type("INTEGER")
    unsigned long last_installed_id;

};


#endif //HDIH_WEB_API_SCHEMAVERSION_H
