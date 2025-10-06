#include "GameEngine.h"
#include "Map/MapDriver.cpp"
#include "OrdersDriver.cpp"
#include "PlayerDriver.cpp"
#include "CardsDriver.cpp"

int main() {
	TestLoadMaps({
		"../Maps/_11_ Against All Odds.map",
		"../Maps/001_I72_Ghtroc 720.map",
		"../Maps/005_I72_V-22.map",
		"../Maps/READ FIRST.txt",
		"../Maps/_64_ BIG BLUE.map"
		});
	testPlayers();
	testOrdersLists();
    TestGameEngine();
	return 0;
}

