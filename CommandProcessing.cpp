#include "GameEngine.h"
#include "CommandProcessing.h"
#include <iostream>
#include <fstream>
#include <functional>



CommandRegistrar<LoadMapCommand> LoadMapCommand::registrar("loadmap");
CommandRegistrar<ValidateMapCommand> ValidateMapCommand::registrar("validatemap");
CommandRegistrar<AddPlayerCommand> AddPlayerCommand::registrar("addplayer");
CommandRegistrar<GameStartCommand> GameStartCommand::registrar("gamestart");
CommandRegistrar<ReplayCommand> ReplayCommand::registrar("replay");
CommandRegistrar<QuitCommand> QuitCommand::registrar("quit");

// ----------------------
// CommandProcessor base
// ----------------------

CommandProcessor::CommandProcessor(GameEngine* engine)  
    : gameEngine(engine) {
}


std::unique_ptr<ICommand> CommandProcessor::parseCommand(const std::string& userInput) {
    // Split input: "loadmap world.map" into ["loadmap", "world.map"]
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
        std::cout << "Invalid command: " << commandName << "\n";
        return nullptr;
    }

    return cmd;
}


void CommandProcessor::processInput(const std::string& userInput) {
    auto cmd = parseCommand(userInput);
    if (!cmd)
        return;

    // Execute the command
    this->SaveCommand(*cmd);
    cmd->Execute();
}

void CommandProcessor::SaveCommand(const ICommand& cmd) {
    std::string commandDescription = "Executed " + cmd.name + " effect was " + cmd.effect;
    commandHistory.push_back(std::make_unique<std::string>(commandDescription));
}

// ================== ConsoleCommandProcessor ==================
ConsoleCommandProcessor::ConsoleCommandProcessor(GameEngine* engine)
    : CommandProcessor(engine) {}

void ConsoleCommandProcessor::readCommands(GameEngine& engine) {
    cout << "Enter commands (type 'quit' to finish):\n";
    string line;
    while (true) {
        cout << "> ";
        getline(cin, line);
   
        // Split command name and argument
        istringstream iss(line);
        string commandName, argument;
        iss >> commandName;
        getline(iss, argument);
        if (!argument.empty() && argument[0] == ' ') argument.erase(0, 1);

        auto cmd = CommandFactory::Instance().CreateCommand(commandName, argument);
        if (cmd) {
			this->SaveCommand(*cmd);
            engine.ExecuteCommand(std::move(cmd));
        }
        else {
            //engine.ProcessInput(commandName);
            std::cout << "Invalid command. Available commands:\n";
                for (auto& cmd : GameEngine::instance->currentState->availableCommands) {
                    std::cout << " - " << cmd->name << "\n";
                }
        }
    }
}
// ================== FileCommandProcessorAdapter ==================
FileCommandProcessorAdapter::FileCommandProcessorAdapter(GameEngine* engine, const std::string& file)
    : CommandProcessor(engine), filePath(file) {
}

 void FileCommandProcessorAdapter::readCommands(GameEngine& engine) {
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