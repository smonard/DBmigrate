#include <iostream>
#include <fstream>
#include <dirent.h>

#include <memory>
#include <odb/pgsql/database.hxx>
#include "entity/SchemaVersion.hpp"
#include "entity/SchemaVersion-odb.hxx"
#include "entity/SchemaVersion-odb.cxx"


using namespace std;
using namespace odb;

int setUpDatabase();
int runMigrations();
unsigned long getLastApplied();
unsigned long parseFileID(string filename);
bool shouldApply(string filename);
void createDB();

void saveSchemaVersionStatus(string &basic_string);

unsigned long lastApplied = 0;
auto_ptr<odb::database>* db;

int entryPoint(){
    createDB();
    setUpDatabase();
    int returnValue = runMigrations();
    delete db;
    return  returnValue;
}


void createDB(){
    string user = "hdih";
    string password= "123456";
    string database = "test_hdih";
    string host = "localhost";
    unsigned int port = 5432;
    db = new auto_ptr<odb::database>( new odb::pgsql::database (user, password, database, host, port) );
}

int setUpDatabase(){
    string script = " CREATE TABLE IF NOT EXISTS schema_version( id serial not null primary key, last_applied TEXT not null, last_applied_id integer not null, date date not null );";
    transaction t ((*db)->begin ());
    (*db)->execute(script);
    t.commit ();
}

int runMigrations () {
    DIR *directory;
    struct dirent* ent;
    const char* migrationsPath= "./migrations/";
    cout << endl << "Read files from directory: " << migrationsPath << endl;
    if ((directory = opendir (migrationsPath)) != NULL) {
        while ((ent = readdir(directory)) != NULL) {
            string d_name = ent->d_name;
            if(d_name != "." && d_name != ".." && shouldApply(d_name)) {
                cout << "Running: \"" << d_name << "\"\n";
                ifstream sqlFile((migrationsPath + d_name).c_str());
                string scriptBody( (std::istreambuf_iterator<char>(sqlFile)),
                                   (std::istreambuf_iterator<char>()) );
                transaction t ((*db)->begin ());
                (*db)->execute(scriptBody);
                t.commit ();
                saveSchemaVersionStatus(d_name);
                sqlFile.close();
            }
        }
    }
    else
        return 1;
    cout << endl << "Done!" << endl;
    return 0;
}

void saveSchemaVersionStatus(string &basic_string) {
    time_t times;
    time(&times);
    struct tm tm = *(localtime(&times));
    string date_str(std::to_string(tm.tm_year + 1900) + "-" + std::to_string(tm.tm_mon + 1) + "-" + std::to_string(tm.tm_mday));
    unsigned long fileID = parseFileID(basic_string);
    std::string insertScript("insert into schema_version(last_applied, last_applied_id, date) values('" + basic_string + "','" + std::to_string(fileID) + "','" + date_str + "');");
    transaction t ((*db)->begin ());
    (*db)->execute(insertScript);
    t.commit ();
}

bool shouldApply(string filename){
    return getLastApplied() < parseFileID(filename);
}

unsigned long getLastApplied(){
    typedef odb::result<schema_db_version> result;
    transaction t ((*db)->begin());
    result r((*db)->query<schema_db_version>(false));
    lastApplied = (r.size() != 0)? r.begin()->last_installed_id : 0;
    t.commit();
    return lastApplied;
}

unsigned long parseFileID(string filename){
    unsigned long firtsCut = filename.find(".");
    char fileId_str[firtsCut];
    filename.copy(fileId_str, firtsCut);
    unsigned long fileId = atol(fileId_str);
    return  fileId;
}