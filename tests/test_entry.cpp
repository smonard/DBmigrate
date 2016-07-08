//
// Created by josilvam on 6/30/16.
//
#include "gtest/gtest.h"
#include <entry.cpp>

using namespace std;

struct entryPoint : testing::Test
{

    entryPoint()
    {

    }
    virtual ~entryPoint()
    {

    }
};

//parseFileID function

TEST(entryPoint, test_parseFile1digits){
    EXPECT_EQ(parseFileID("01.CreateSomething.sql"), 1);
}

TEST(entryPoint, test_parseFile1digit){
    EXPECT_EQ(parseFileID("1.CreateSomething.sql"), 1);
}

TEST(entryPoint, test_parseFile2digits){
    EXPECT_EQ(parseFileID("10.CreateSomething.sql"), 10);
}

TEST(entryPoint, test_parseFile3digits){
    EXPECT_EQ(parseFileID("312.CreateSomething.sql"), 312);
}

//getLastApplied

TEST(entryPoint, test_getLastApplied_if_not_migration_applied){
    createDB();
    EXPECT_EQ(getLastApplied(), 0);
}

TEST(entryPoint, test_getLastApplied){
    odb::transaction transaction ((*db)->begin());
    (*db)->execute("INSERT INTO schema_version(id, last_applied, last_applied_id, date) VALUES(1,\'05.AlterSomething.sql\', 5, \'2016-06-06\');");
    transaction.commit();
    EXPECT_EQ(getLastApplied(), 5);
    EXPECT_EQ(getLastApplied(), 5);
}

TEST(entryPoint, test_getLastApplied_twice){
    odb::transaction transaction ((*db)->begin());
    (*db)->execute("INSERT INTO schema_version(id, last_applied, last_applied_id, date) VALUES(2,\'06.AlterSomething.sql\', 6, \'2016-06-06\');");
    transaction.commit();
    EXPECT_EQ(getLastApplied(), 6);
    EXPECT_EQ(getLastApplied(), 6);
}

//should shouldApply

TEST(entryPoint, test_shouldApply){
    EXPECT_EQ(shouldApply("10.CreateSomething.sql"), true);
}

TEST(entryPoint, test_shouldApply_if_not){
    EXPECT_EQ(shouldApply("1.CreateSomething.sql"), false);
}

TEST(entryPoint, test_shouldApply_if_equal){
    EXPECT_EQ(shouldApply("6.CreateSomething.sql"), false);
}

//saveSchemaState

TEST(entryPoint, test_saveStatus){
    string filename("234.ALTERSomething.sql");
    saveSchemaVersionStatus(filename);
    transaction t ((*db)->begin());
    odb::result<schema_db_version> r((*db)->query<schema_db_version>(false));
    unsigned long lastApplied = (r.size() != 0)? r.begin()->last_installed_id : 0;
    t.commit();
    EXPECT_EQ(lastApplied, 234);
}

TEST(entryPoint, test_saveStatus_){
    string filename("238.ALTERSomething.sql");
    saveSchemaVersionStatus(filename);
    transaction t ((*db)->begin());
    odb::result<schema_db_version> r((*db)->query<schema_db_version>(false));
    unsigned long lastApplied = (r.size() != 0)? r.begin()->last_installed_id : 0;
    t.commit();
    EXPECT_EQ(lastApplied, 238);
}

TEST(entryPoint, test_dropElements){
    odb::transaction transaction2 ((*db)->begin());
    (*db)->execute("delete from schema_version;");
    transaction2.commit();
}
