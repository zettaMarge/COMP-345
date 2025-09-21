#include "Map.h"
#include <iostream>
#include <fstream>
#include <algorithm>

bool MapLoader::loadMapFile(const std::string& filename){
    loadFile(filename);
    if(content.empty()) return false;
    if(validateMap() == false) return false;
    // createMap();

    return true;
}

void MapLoader::loadFile(const std::string& filename){
    std::ifstream file(filename);
    if(!file.is_open()){
        std::cerr << "Could not open file: " << filename;
        return;
    }

    std::string line;
    while(std::getline(file, line)){
        if(!line.empty() && !std::all_of(line.begin(), line.end(), isspace)){
            content.push_back(line);
        }
    }

    file.close();
}

bool MapLoader::validateMap(){
    bool hasMetaData = false, hasContinents = false, hasTerritories = false;
    std::string checking = "None";
    for(int i = 0; i < content.size(); i++){
        if(content[i] == "[Map]"){
            hasMetaData = true;
            checking = "MetaData";
            continue;
        }else if(content[i] == "[Continents]"){
            hasContinents = true;
            checking = "Continents";
            continue;
        }else if(content[i] == "[Territories]"){
            hasTerritories = true;
            checking = "Territories";
            continue;
        }

        if(checking == "MetaData"){
            if(content[i].find("=") == std::string::npos) return false;
        }else if(checking == "Continents"){
            size_t valueAssign = content[i].find("=");
            if(valueAssign == std::string::npos) return false;

            try{
                std::stoi(content[i].substr(valueAssign + 1));
            }catch(...){
                return false;
            }
        }else if(checking == "Territories"){
            if(std::count(content[i].begin(), content[i].end(), ',') < 3) return false;
        }
    }

    return hasMetaData && hasContinents && hasTerritories;
}

void MapLoader::createMap(){
    std::string checking = "None";
    std::vector<std::pair<std::string, int>> continents;

    for(int i = 0; i < content.size(); i++){
        if(content[i] == "[Continents]"){
            checking = "Continents";
            continue;
        }else if(content[i] == "[Territories]"){
            checking = "Territories";
            continue;
        }

        if(checking == "Continents"){
            size_t valueAssign = content[i].find("=");
            std::string key = content[i].substr(0, valueAssign);
            std::string value = content[i].substr(valueAssign + 1);
            continents.push_back({key, std::stoi(value)});
        }else if(checking == "Territories"){

        }
    }
}

int main() {
    MapLoader loader;

    if (loader.loadMapFile("../Maps/_11_ Against All Odds.map")) {
        std::cout << "Success";
    } else {
        std::cout << "Fail";
    }

    return 0;
}