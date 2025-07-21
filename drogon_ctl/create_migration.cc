/**
 *
 *  create_migration.cc
 *  An Tao
 *
 *  Copyright 2018, An Tao.  All rights reserved.
 *  https://github.com/an-tao/drogon
 *  Use of this source code is governed by a MIT license
 *  that can be found in the License file.
 *
 *  Drogon
 *
 */

#include "create_migration.h"
#include "cmd.h"
#include <drogon/DrTemplateBase.h>
#include <drogon/utils/Utilities.h>
#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <regex>
#include <unordered_map>
#include <ctime>
#ifdef _WIN32
#else
#include <sys/stat.h>
#endif



using namespace drogon_ctl;

void create_migration::handleCommand(std::vector<std::string> &parameters)
{
    if(parameters.empty()){
        std::cout<<"error: migration name needed"<<std::endl;
        exit(1);
    }

    std::cout<<"create_migration::handleCommand"<<std::endl;

    std::string migrationName = *parameters.begin();

    std::cout<<"migration name: "+migrationName<<std::endl;

    parameters.erase(parameters.begin());

    std::unordered_map<std::string, std::string> tableColumnNameToTypeMap;

    for (auto iter = parameters.begin(); iter != parameters.end(); ++iter)
    {
        std::string entry = *iter;

        if (entry.find(":") == std::string::npos) {
            std::cout<<"incorrect format: entry ("+entry+") needs to be columnName:type "<<std::endl;
            exit(1);
        }

        std::stringstream ss(entry);
        std::vector<std::string> items;

        std::string columnName;
        std::string columnType;
        
        std::getline(ss, columnName, ':');
        std::getline(ss, columnType, ':');

        tableColumnNameToTypeMap.insert({columnName, columnType});
    }

    createMigration(migrationName, tableColumnNameToTypeMap);
}

void create_migration::createMigration(std::string migrationName, std::unordered_map<std::string,std::string> &migrationColumnsAndTypes){

    std::string sourceFileText = R"(
    class AddUsersTableMigration : public Migration {
    public:
        void up() override {
            createTable("users", [](TableBuilder& table) {
    )";

    //put other stuff here

    sourceFileText.append(R"(}

        void down() override {
            dropTable("users");
        }
    };)");

    std::time_t t = std::time(nullptr);
    std::tm *utc_tm = std::gmtime(&t);

    std::stringstream ss;
    ss << std::put_time(utc_tm, "%Y%m%d%H%M%S");

    std::string filename = ss.str();
    filename.append("_add_"+migrationName+"_table.cpp");

    std::cout<<filename<<std::endl;

    std::ofstream out(filename);
    
    if (!out) {
        std::cerr << "Failed to open file "+filename << std::endl;
        exit(1);
    }

    out << sourceFileText;
    out.close();


    #ifdef _WIN32 
    #else
        chmod(filename.c_str(), 0644);
    #endif
}