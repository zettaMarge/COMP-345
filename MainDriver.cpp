#include "GameEngine.h"
#include "Map/MapDriver.cpp"
#include "OrdersDriver.cpp"
#include "PlayerDriver.cpp"
#include "CardsDriver.cpp"

int main() {
	TestMaps();
	testPlayers();
	TestCards();
	testOrdersLists();
    TestGameEngine();
	return 0;
}

