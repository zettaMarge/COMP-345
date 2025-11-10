#include "GameEngine.h"
#include "CommandProcessing.h"

void testLoggingObserver();

int main() {
	GameEngine* game = new GameEngine();
	GameEngine::instance = game;
	game->TestStartupPhase();
	game->TestOrderExecution();
	TestGameEngine();
	// testLoggingObserver();
	//command procceing i think is done in the command processing driver	

	//Assignment 1 tests
	//	TestMaps();
	//	TestPlayers();
	//	TestCards();
	//	TestOrdersLists();
	//  TestGameEngine();
	return 0;
}

