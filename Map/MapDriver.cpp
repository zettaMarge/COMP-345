#include "Map.h"

#include <iostream>
#include <string>

std::vector<Map*> TestLoadMaps(const std::vector<std::string>& mapFiles){
    std::vector<Map*> validMaps;

    for(int i = 0; i < mapFiles.size(); i++){
        std::cout << "Checking map file: " << mapFiles[i] << std::endl;
        MapLoader loader;

        if(loader.LoadMapFile(mapFiles[i])){
            Map* gameMap = new Map(loader.CreateMap());

            if(gameMap->Validate()){
                std::cout << "Map " << mapFiles[i] << " is valid\n" << std::endl;
                validMaps.push_back(gameMap);
            }else{
                std::cerr << "Map " << mapFiles[i] << " is NOT valid\n" << std::endl;
                delete gameMap;
            }
        }else{
            std::cerr << "The map file " << mapFiles[i] << " could NOT be loaded, file is not a valid map\n" << std::endl;
        }
    }

    return validMaps;
}

int TestMaps(){
    std::vector<std::string> mapFiles = {
        "../Maps/_11_ Against All Odds.map",
        "../Maps/001_I72_Ghtroc 720.map",
        "../Maps/005_I72_V-22.map",
        "../Maps/READ FIRST.txt",
        "../Maps/_64_ BIG BLUE.map"
    };

    std::vector<Map*> validMaps = TestLoadMaps(mapFiles);
    std::cout << "Successfully loaded " << validMaps.size() << " valid maps" << std::endl;

    for(Map* m : validMaps){
        delete m;
    }

    return 0;
}
