#include "GameEngine.h"
#include "CommandProcessing.h"
#include <iostream>
#include <fstream>
#include <functional>


//self-registration of commands, every command class is registered in the registrar as a 'valid' command
//this makes it easier to check if a command exists without hardcoding each command name somewhere
CommandRegistrar<TournamentCommand> TournamentCommand::registrar("tournament");
CommandRegistrar<LoadMapCommand> LoadMapCommand::registrar("loadmap");
CommandRegistrar<ValidateMapCommand> ValidateMapCommand::registrar("validatemap");
CommandRegistrar<AddPlayerCommand> AddPlayerCommand::registrar("addplayer");
CommandRegistrar<GameStartCommand> GameStartCommand::registrar("gamestart");
CommandRegistrar<ReplayCommand> ReplayCommand::registrar("replay");
CommandRegistrar<QuitCommand> QuitCommand::registrar("quit");
CommandRegistrar<NewGameCommand> NewGameCommand::registrar("newgame");

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

// trim helper
static std::string trim(const std::string& s) {
    size_t a = s.find_first_not_of(" \t\n\r\"");
    if (a == std::string::npos) return "";
    size_t b = s.find_last_not_of(" \t\n\r\"");
    return s.substr(a, b - a + 1);
}

// split by comma and trim; ignores empty tokens
static std::vector<std::string> splitByCommaAndTrim(const std::string& s) {
    std::vector<std::string> out;
    std::string cur;
    for (char c : s) {
        if (c == ',') {
            std::string t = trim(cur);
            if (!t.empty()) out.push_back(t);
            cur.clear();
        }
        else cur.push_back(c);
    }
    std::string t = trim(cur);
    if (!t.empty()) out.push_back(t);
    return out;
}

// parse substring into tokens allowing:
//  - comma-separated (preferred if commas exist),
//  - or quoted items: "multi word name"
//  - or fallback to whitespace-separated tokens
static std::vector<std::string> parseListAllowingSpaces(const std::string& s) {
    // If it contains a comma -> use comma splitting
    if (s.find(',') != std::string::npos) {
        return splitByCommaAndTrim(s);
    }

    std::vector<std::string> out;
    size_t i = 0, n = s.size();
    while (i < n) {
        // skip whitespace
        while (i < n && std::isspace((unsigned char)s[i])) ++i;
        if (i >= n) break;

        if (s[i] == '"') {
            // quoted token
            ++i; // skip opening "
            std::string token;
            while (i < n && s[i] != '"') { token.push_back(s[i]); ++i; }
            if (i < n && s[i] == '"') ++i; // skip closing "
            std::string t = trim(token);
            if (!t.empty()) out.push_back(t);
        }
        else {
            // unquoted token: read until whitespace
            std::string token;
            while (i < n && !std::isspace((unsigned char)s[i])) {
                token.push_back(s[i]); ++i;
            }
            std::string t = trim(token);
            if (!t.empty()) out.push_back(t);
        }
    }
    return out;
}

// find position of the earliest next flag (like " -M" or " -P" etc.) starting from 'pos'
static size_t findNextFlagPos(const std::string& input, size_t pos) {
    const std::vector<std::string> flags = { " -M", " -P", " -G", " -D", "\t-M", "\t-P", "\t-G", "\t-D" };
    size_t best = std::string::npos;
    for (auto& f : flags) {
        size_t p = input.find(f, pos);
        if (p != std::string::npos && (best == std::string::npos || p < best)) best = p;
    }
    return best;
}

// extract substring contents after a flag position (pos points to '-' of flag)
static std::string extractBetweenFlags(const std::string& input, size_t flagPos) {
    if (flagPos == std::string::npos) return "";
    // move past the flag token (skip until whitespace after '-X')
    size_t i = flagPos;
    // skip until first whitespace after flag e.g. "-M" -> skip "-M"
    while (i < input.size() && !std::isspace((unsigned char)input[i])) ++i;
    // skip whitespace
    while (i < input.size() && std::isspace((unsigned char)input[i])) ++i;

    size_t next = findNextFlagPos(input, i);
    if (next == std::string::npos) return input.substr(i);
    else return input.substr(i, next - i);
}

// The main parser function
TournamentParameters CommandProcessor::ParseTournamentCommand(const std::string& input) {
    TournamentParameters params;

    size_t posM = input.find("-M");
    size_t posP = input.find("-P");
    size_t posG = input.find("-G");
    size_t posD = input.find("-D");

    // extract lists and parse them
    if (posM != std::string::npos) {
        std::string mapsPart = extractBetweenFlags(input, posM);
        params.maps = parseListAllowingSpaces(mapsPart);
    }
    if (posP != std::string::npos) {
        std::string playersPart = extractBetweenFlags(input, posP);
        params.strategies = parseListAllowingSpaces(playersPart);
    }
    if (posG != std::string::npos) {
        std::string gPart = extractBetweenFlags(input, posG);
        std::istringstream iss(gPart);
        iss >> params.games;
    }
    if (posD != std::string::npos) {
        std::string dPart = extractBetweenFlags(input, posD);
        std::istringstream iss(dPart);
        iss >> params.maxTurns;
    }

    // validation
    if (params.maps.size() < 1 || params.maps.size() > 5)
        throw std::runtime_error("M must contain 1–5 maps (use quotes or commas for multi-word names).");
    if (params.strategies.size() < 2 || params.strategies.size() > 4)
        throw std::runtime_error("P must contain 2–4 player strategies (use quotes or commas for multi-word names).");
    if (params.games < 1 || params.games > 5)
        throw std::runtime_error("G must be between 1 and 5.");
    if (params.maxTurns < 10 || params.maxTurns > 50)
        throw std::runtime_error("D must be between 10 and 50.");

    return params;
}


//takes user input, sends it to readcommand for processing
//if a valid command is returned, saves it to history and executes it
void CommandProcessor::processInput(const std::string& userInput) {
    if(userInput == "history") {
        PrintCommandHistory();
        return;
	}
    auto cmd = readCommand(userInput);
    if (!cmd)
        return;

    SaveCommand(cmd->name);
    if(cmd->name == "tournament") {
        try {
            TournamentParameters params = ParseTournamentCommand(userInput);
            cmd->argument = ""; // Clear argument to avoid confusion
			TournamentCommand* tournamentCmd = dynamic_cast<TournamentCommand*>(cmd.get());
            tournamentCmd->mapFiles = params.maps;
            tournamentCmd->playerStrategies = params.strategies;
            tournamentCmd->numberOfGames = params.games;
			tournamentCmd->maxTurns = params.maxTurns;
            SaveEffect(cmd->Execute());
            return;
        }
        catch (const std::runtime_error& e) {
            cmd->effect = std::string("Error parsing tournament command: ") + e.what();
            std::cout << cmd->effect << "\n";
            SaveEffect(cmd->effect);
            return;
        }
	}
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