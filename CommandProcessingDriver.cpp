#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>
#include <string>
#include "GameEngine.h"
#include "CommandProcessing.h"

using namespace std;

void TestCommandProcessing() {
    GameEngine engine;

    std::unique_ptr<FileCommandProcessorAdapter> fileProcessor;
    ConsoleCommandProcessor consoleProcessor(&engine);

    std::cout << "Read commands from file? (y/n): ";
    char choice;
    std::cin >> choice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (choice == 'y' || choice == 'Y') {
        std::string filename;
        std::cout << "Enter filename: ";
        std::getline(std::cin, filename);

        fileProcessor = std::make_unique<FileCommandProcessorAdapter>(&engine, filename);
        fileProcessor->readCommands(engine); 
    }
   
    std::cout << "Starting console command processing...\n";
	
    consoleProcessor.readCommands(engine);

    std::cout << "Command processing complete.\n";
}
