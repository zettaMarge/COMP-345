#include "GameEngine.h"
#include "CommandProcessing.h"
void TestCommandProcessing();
int TestMaps();
int TestPlayers();
int TestCards();
void TestOrdersLists();

int main() {
	TestCommandProcessing();
	//how i am running the code:  g++ -std=c++17 -lstdc++fs -o game MainDriver.cpp GameEngine.cpp Map.cpp Player.cpp  Cards.cpp Orders.cpp 
	GameEngine engine;
	engine.TestStartupPhase();
	/*
	TestMaps();
	TestPlayers();
	TestCards();
	TestOrdersLists();
    TestGameEngine();
	*/
	return 0;
}

