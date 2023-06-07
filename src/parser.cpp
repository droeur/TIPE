#include "parser.hpp"

#include <yaml-cpp/yaml.h>

void settings_parse(){
    YAML::Node config = YAML::LoadFile("config.yaml");
    if (config["player1"]) {
        for(YAML::const_iterator u_yaml=config.begin();u_yaml != config.end();++u_yaml){
            
        }
    }
}