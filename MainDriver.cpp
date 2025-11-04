#include "GameEngine.h"
#include "CommandProcessing.h"
void TestCommandProcessing();
int TestMaps();
int TestPlayers();
int TestCards();
void TestOrdersLists();

int main() {
	TestCommandProcessing();
	TestMaps();
	TestPlayers();
	TestCards();
	TestOrdersLists();
    TestGameEngine();
	return 0;
}

