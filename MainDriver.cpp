#include "GameEngine.h"
#include "CommandProcessing.h"

void TestCommandProcessing();
void TestLoggingObserver();
void testOrderExecution();

int main() {
	TestCommandProcessing();
	GameEngine* game = new GameEngine();
	GameEngine::instance = game;
	game->TestStartupPhase();
	testOrderExecution();
	TestGameEngine();
	TestLoggingObserver();
	//command procceing i think is done in the command processing driver	

	//Assignment 1 tests
	//	TestMaps();
	//	TestPlayers();
	//	TestCards();
	//	TestOrdersLists();
	//  TestGameEngine();
	return 0;
}

