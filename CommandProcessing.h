#pragma once
#include <iostream>
#include <string>
#include <memory>
#include <unordered_map>
#include <functional>
#include <sstream>
#include <fstream>
#include "GameEngine.h"  // for IState, GameEngine
#include "LoggingObserver.h"

class GameEngine;
class ICommand;

// ================== CommandFactory ==================
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

// ================== CommandRegistrar ==================
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

// ================== CommandProcessor ==================
class CommandProcessor: public Subject, public ILoggable {
public:
	std::vector<CommandLog> commandHistory;

    explicit CommandProcessor(GameEngine* engine);

    void processInput(const std::string& userInput);

    void SaveCommand(const std::string& cmdName) {
        commandHistory.push_back({ cmdName, "" });
        Notify(this);
    }

    void SaveEffect(const std::string& effect) {
        if (!commandHistory.empty()) {
            commandHistory.back().effect = effect;
            Notify(this);
        }
    }

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

    bool validate(const std::string& commandName) {
        return CommandFactory::Instance().CreateCommand(commandName, "") != nullptr;
	}

private:
    std::unique_ptr<ICommand> readCommand(const std::string& userInput);
    GameEngine* gameEngine;
};

class ConsoleCommandProcessor : public CommandProcessor {
public:
    ConsoleCommandProcessor(GameEngine* engine);
    void proccessConsoleCommands(GameEngine& engine);
    // Continuously read commands from console
};


// ================== FileCommandProcessorAdapter ==================
class FileCommandProcessorAdapter : public CommandProcessor {
private:
    std::string filePath;

public:
    FileCommandProcessorAdapter(GameEngine* engine, const std::string& file);
    void ReadLinesFromFile(GameEngine& engine);
};

