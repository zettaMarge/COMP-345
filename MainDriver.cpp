#include "GameEngine.h"
#include "MapDriver.cpp"
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

