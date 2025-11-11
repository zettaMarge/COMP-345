#pragma once
#include <iostream>
#include <string>
#include <memory>
#include <unordered_map>
#include <functional>
#include <sstream>
#include <fstream>
#include "GameEngine.h"  
#include "LoggingObserver.h"

class GameEngine;
class ICommand;

// command factory pattern implementation
//creates command objects based on their string name
class CommandFactory {
public:
    using Creator = std::function<std::unique_ptr<ICommand>(const std::string&)>;

    static CommandFactory& Instance() {
        static CommandFactory instance;
        return instance;
    }

    void Register(const std::string& name, Creator createFunc) {
        creators[name] = std::move(createFunc);
    }

    std::unique_ptr<ICommand> CreateCommand(const std::string& name, const std::string& arg) {
        auto it = creators.find(name);
        if (it != creators.end())
            return it->second(arg);
        return nullptr;
    }

private:
    std::unordered_map<std::string, Creator> creators;
};

//Command registrar template for self-registration of commands
//each command class will use this to register itself with the factory
template <typename T>
class CommandRegistrar {
public:
    CommandRegistrar(const std::string& name) {
        CommandFactory::Instance().Register(name, [](const std::string& arg) {
            return std::make_unique<T>(arg);
            });
    }
};

struct CommandLog {
    std::string command;
    std::string effect;
};

// command processor class
// processes user input into commands, maintains command history
class CommandProcessor: public Subject, public ILoggable {
public:
	std::vector<CommandLog> commandHistory;

	//constructor using a gameengine object pointer
    explicit CommandProcessor(GameEngine* engine);

	// processes user input string into command and executes it
    void processInput(const std::string& userInput);

	//saves command name to history
    void SaveCommand(const std::string& cmdName) {
        commandHistory.push_back({ cmdName, "" });
        Notify(this);
    }

	//adds command effect to previous command in history
    void SaveEffect(const std::string& effect) {
        if (!commandHistory.empty()) {
            commandHistory.back().effect = effect;
            Notify(this);
        }
    }

	//prints the command history to console
    void PrintCommandHistory() const {
        std::cout << "Command History:\n";
        for (const auto& entry : commandHistory) {
            std::cout << entry.command;

            if (!entry.effect.empty()) {
                std::cout << " -> " << entry.effect;
            }

            std::cout << "\n";
        }
    }

    std::string StringToLog() override {
        if (commandHistory.empty()) {
            return "CommandProcessor: No commands yet";
        }

        const auto& lastCmd = commandHistory.back();

        if (lastCmd.effect.empty()) {
            return "Command: " + lastCmd.command;
        } else {
            return "Command effect: " + lastCmd.command + " -> " + lastCmd.effect;
        }
    }

	//validates if a command name exists in the factory
	//not to be confused with GameEngine::IsValidCommand which checks if a command is valid in the current state
    bool validate(const std::string& commandName) {
        return CommandFactory::Instance().CreateCommand(commandName, "") != nullptr;
	}

private:
	//reads user input string, splits into command and argument
    std::unique_ptr<ICommand> readCommand(const std::string& userInput);
    GameEngine* gameEngine;
};

class ConsoleCommandProcessor : public CommandProcessor {
public:
	//constructor
    ConsoleCommandProcessor(GameEngine* engine);

    // Continuously read commands from console
    void proccessConsoleCommands(GameEngine& engine);
};


//file command processor adapter
//uses adapter pattern to read commands from a file
class FileCommandProcessorAdapter : public CommandProcessor {
private:
    std::string filePath;

public:
    FileCommandProcessorAdapter(GameEngine* engine, const std::string& file);

	//read lines from file and use processInput to process them
    void ReadLinesFromFile(GameEngine& engine);
};

