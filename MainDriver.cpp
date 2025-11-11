#include "GameEngine.h"
#include "CommandProcessing.h"

int main() {
	GameEngine* game = new GameEngine();
	GameEngine::instance = game;
	game->TestStartupPhase();
	game->TestOrderExecution();
	TestGameEngine();

	//Assignment 1 tests
	//	TestMaps();
	//	TestPlayers();
	//	TestCards();
	//	TestOrdersLists();
	//  TestGameEngine();
	return 0;
}

