#include "GameEngine.h"
#include "CommandProcessing.h"
#include "PlayerStrategies.h"
#include "PlayerStrategiesDriver.h"

void TestCommandProcessing();
void TestLoggingObserver();
void testOrderExecution();
void testPlayerStrategies();

int main() {
	testPlayerStrategies();
	//Assignement 2 tests
	//TestCommandProcessing();
	//GameEngine* game = new GameEngine();
	//GameEngine::instance = game;
	//game->TestStartupPhase();
	//testOrderExecution();
	//TestGameEngine();
	//TestLoggingObserver();

	//Assignment 1 tests
	//	TestMaps();
	//	TestPlayers();
	//	TestCards();
	//	TestOrdersLists();
	//  TestGameEngine();
	return 0;
}

