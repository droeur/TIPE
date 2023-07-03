#include "parser.hpp"

#include <yaml-cpp/yaml.h>

using namespace std;

void settings_parse(){
    YAML::Node config = YAML::LoadFile("config.yaml");
    if (config["player1"]) {
        for(YAML::const_iterator u_yaml=config.begin();u_yaml != config.end();++u_yaml){
            
        }
    }
}

#include <fstream>
#include<iostream>

bool file_check(const string &name)
{
    ifstream file;
    file.open(name.c_str());
    return file.good();
}
bool options_class::check() const
{
    if (!file_check(map_file_))
    {
        cerr << "Error: no map file " << map_file_ << endl;
        return false;
    }
    if (!file_check(parameter_file_))
    {
        cerr << "Error: no parameter file " << parameter_file_ << endl;
        return false;
    }
    if (!file_check(font_file_))
    {
        cerr << "Error: no font file " << font_file_ << endl;
        return false;
    }

    return true;
}
