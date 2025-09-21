#pragma once
#include <vector>
#include <string>

class MapLoader{
    public:
        bool loadMapFile(const std::string& filename);
    private:
        std::vector<std::string> content;
        void loadFile(const std::string& filename);
        bool validateMap();
        void createMap();
};