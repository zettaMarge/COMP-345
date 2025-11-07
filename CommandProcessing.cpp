#include "GameEngine.h"
#include "CommandProcessing.h"
#include <iostream>
#include <fstream>
#include <functional>


//self-registration of commands, every command class is registered in the registrar as a 'valid' command
//this makes it easier to check if a command exists without hardcoding each command name somewhere
CommandRegistrar<LoadMapCommand> LoadMapCommand::registrar("loadmap");
CommandRegistrar<ValidateMapCommand> ValidateMapCommand::registrar("validatemap");
CommandRegistrar<AddPlayerCommand> AddPlayerCommand::registrar("addplayer");
CommandRegistrar<GameStartCommand> GameStartCommand::registrar("gamestart");
CommandRegistrar<ReplayCommand> ReplayCommand::registrar("replay");
CommandRegistrar<QuitCommand> QuitCommand::registrar("quit");

//command processor parent class
CommandProcessor::CommandProcessor(GameEngine* engine)  
    : gameEngine(engine) {
}

//splits input line into a command and argument, creates the command using CommandFactory
//if the command actually exists it'll return it, if not it'll print error and return nullptr
std::unique_ptr<ICommand> CommandProcessor::readCommand(const std::string& userInput) {
    std::istringstream iss(userInput);
    std::string commandName;
    std::string argument;

    iss >> commandName;
    std::getline(iss, argument);
    if (!argument.empty() && argument[0] == ' ')
        argument.erase(0, 1);

    // Lookup in CommandFactory
    auto cmd = CommandFactory::Instance().CreateCommand(commandName, argument);
    if (!cmd) {
        std::cout << "Invalid command: " << commandName << "\n" << "Available commands: \n";
        for (auto& cmd : GameEngine::instance->currentState->availableCommands) {
            std::cout << " - " << cmd->name << "\n";
        }
        return nullptr;
    }
    return cmd;
}

//takes user input, sends it to readcommand for processing
//if a valid command is returned, saves it to history and executes it
void CommandProcessor::processInput(const std::string& userInput) {
    if(userInput == "history") {
        printCommandHistory();
        return;
	}
    auto cmd = readCommand(userInput);
    if (!cmd)
        return;

    SaveCommand(cmd->name);
	SaveEffect(cmd->Execute());
}

//console command procesor
ConsoleCommandProcessor::ConsoleCommandProcessor(GameEngine* engine)
    : CommandProcessor(engine) {}

void ConsoleCommandProcessor::proccessConsoleCommands(GameEngine& engine) {
    cout << "Enter commands (type 'quit' to finish):\n";
    string line;
    while (true) {
        cout << "> ";
        getline(cin, line);
		processInput(line);
    }
}
// file command processor, 
FileCommandProcessorAdapter::FileCommandProcessorAdapter(GameEngine* engine, const std::string& file)
    : CommandProcessor(engine), filePath(file) {
}

 void FileCommandProcessorAdapter::ReadLinesFromFile(GameEngine& engine) {
    std::ifstream file(filePath);
    if (!file) {
        std::cerr << "Error: Could not open file '" << filePath << "'\n";
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        processInput(line);
    }

    std::cout << "Finished processing file '" << filePath << "'\n";
}