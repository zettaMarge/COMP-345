#include "GameEngine.h"

// ===== SimpleState implementation =====
SimpleState::SimpleState(const std::string& n,
    const std::string& enterMsg,
    const std::string& exitMsg)
    : IState(n), enterMessage(enterMsg), exitMessage(exitMsg) {
}

SimpleState::SimpleState(const SimpleState& other)
    : IState(other.name),
    enterMessage(other.enterMessage),
    exitMessage(other.exitMessage) {
    for (const auto& cmd : other.availableCommands) {
        availableCommands.emplace_back(std::make_unique<SimpleCommand>(cmd->name, cmd->nextState));
    }
}

SimpleState& SimpleState::operator=(const SimpleState& other) {
    if (this != &other) {
        name = other.name;
        enterMessage = other.enterMessage;
        exitMessage = other.exitMessage;
        availableCommands.clear();
        for (const auto& cmd : other.availableCommands) {
            availableCommands.emplace_back(std::make_unique<SimpleCommand>(cmd->name, cmd->nextState));
        }
    }
    return *this;
}

std::ostream& operator<<(std::ostream& os, const SimpleState& s) {
    os << "State: " << s.name;
    return os;
}

void SimpleState::OnEnter() { std::cout << enterMessage << "\n"; }
void SimpleState::OnExit() { std::cout << exitMessage << "\n"; }

// ===== Concrete States =====

MainMenuState::MainMenuState()
    : SimpleState("MainMenu", "Welcome to the game!",
        "Leaving Main Menu...") {}

MapLoadedState::MapLoadedState()
    : SimpleState("MapLoaded", "Entering map loading state",
        "Leaving map loading state") {}

MapValidatedState::MapValidatedState()
    : SimpleState("MapValidated", "Map validated successfully",
		"Leaving map validated state") {}

PlayersAddedState::PlayersAddedState()
    : SimpleState("PlayersAdded", "Players added successfully",
        "Leaving players added state") {}

AssignReinforcementState::AssignReinforcementState()
    : SimpleState("AssignReinforcement", "Assigning reinforcements",
        "Leaving assign reinforcement state") {}

IssueOrdersState::IssueOrdersState()
    : SimpleState("IssueOrders", "Issuing orders",
        "Leaving issue orders state") {}

ExecuteOrdersState::ExecuteOrdersState()
    : SimpleState("ExecuteOrders", "Executing orders",
        "Leaving execute orders state") {}

WinGameState::WinGameState()
    : SimpleState("WinGame", "You won the game!",
        "Exiting win state") {}


// ===== GameEngine Implementation =====
GameEngine* GameEngine::instance = nullptr;

GameEngine::GameEngine() {
    // Create all states
    mainMenuState = std::make_unique<MainMenuState>();
    mapLoadedState = std::make_unique<MapLoadedState>();
    mapValidatedState = std::make_unique<MapValidatedState>();
    playersAddedState = std::make_unique<PlayersAddedState>();
    assignReinforcementState = std::make_unique<AssignReinforcementState>();
    issueOrdersState = std::make_unique<IssueOrdersState>();
    executeOrdersState = std::make_unique<ExecuteOrdersState>();
    winState = std::make_unique<WinGameState>();

    // Hook commands with automatic transitions
    mainMenuState->availableCommands.emplace_back(
        std::make_unique<SimpleCommand>("loadmap", mapLoadedState.get()));

    mapLoadedState->availableCommands.emplace_back(
        std::make_unique<SimpleCommand>("validatemap", mapValidatedState.get()));

    mapLoadedState->availableCommands.emplace_back(
        std::make_unique<SimpleCommand>("loadmap", nullptr));

    mapValidatedState->availableCommands.emplace_back(
        std::make_unique<SimpleCommand>("addplayer", playersAddedState.get()));

    playersAddedState->availableCommands.emplace_back(
        std::make_unique<SimpleCommand>("addplayer", nullptr));

    playersAddedState->availableCommands.emplace_back(
        std::make_unique<SimpleCommand>("assigncountries", assignReinforcementState.get()));

    assignReinforcementState->availableCommands.emplace_back(
        std::make_unique<SimpleCommand>("issueorder", issueOrdersState.get()));

    issueOrdersState->availableCommands.emplace_back(
        std::make_unique<SimpleCommand>("issueorder", nullptr));

    issueOrdersState->availableCommands.emplace_back(
        std::make_unique<SimpleCommand>("endissueorders", executeOrdersState.get()));

    executeOrdersState->availableCommands.emplace_back(
        std::make_unique<SimpleCommand>("execorder", nullptr));

    executeOrdersState->availableCommands.emplace_back(
        std::make_unique<SimpleCommand>("endexecorder", assignReinforcementState.get()));

    executeOrdersState->availableCommands.emplace_back(
        std::make_unique<SimpleCommand>("win", winState.get()));

    winState->availableCommands.emplace_back(
        std::make_unique<SimpleCommand>("play", mainMenuState.get()));

    winState->availableCommands.emplace_back(
        std::make_unique<SimpleCommand>("end", nullptr)); // exit game

    // Start the game
    currentState = mainMenuState.get();
    currentState->OnEnter();
}

GameEngine::~GameEngine() = default;

GameEngine::GameEngine(const GameEngine& other) {
    if (other.mainMenuState) mainMenuState = other.mainMenuState->Clone();
    if (other.executeOrdersState) executeOrdersState = other.executeOrdersState->Clone();

    currentState = (other.currentState == other.mainMenuState.get())
        ? mainMenuState.get()
        : executeOrdersState.get();
}

GameEngine& GameEngine::operator=(const GameEngine& other) {
    if (this != &other) {
        if (other.mainMenuState) mainMenuState = other.mainMenuState->Clone();
        if (other.executeOrdersState) executeOrdersState = other.executeOrdersState->Clone();

        currentState = (other.currentState == other.mainMenuState.get())
            ? mainMenuState.get()
            : executeOrdersState.get();
    }
    return *this;
}


int GameEngine::Run() {
    std::string input;
    while (true) {
        std::cout << "> ";
        if (!std::getline(std::cin, input)) break;

        // If ProcessInput returns false, break out of the loop (quit game).
        if (!ProcessInput(input)) {
            break;
        }
    }
    return 0;
}

bool GameEngine::ProcessInput(const std::string& input) {
    for (auto& cmd : currentState->availableCommands) {
        if (cmd->name == input) {
            cmd->Execute();
            if (cmd->name == "end") {
                std::cout << "Game Over.\n";
                return false;   // signal quit
            }
            if (cmd->nextState) {
                currentState->OnExit();
                currentState = cmd->nextState;
                currentState->OnEnter();
            }
            return true;  // valid command
        }
    }

    std::cout << "Invalid command. Available commands:\n";
    for (auto& cmd : currentState->availableCommands) {
        std::cout << " - " << cmd->name << "\n";
    }

    return true; // return true so the loop continues
}

// ===== Entry point =====
int main() {
    GameEngine::instance = new GameEngine();
    GameEngine::instance->Run();

    std::cout << "Press Enter to exit...";
    std::cin.get();

    delete GameEngine::instance;   
    GameEngine::instance = nullptr;
    return 0;
}
