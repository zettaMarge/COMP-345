#include "GameEngine.h"

// ===== Concrete States =====
class MainMenuState : public IState {
public:
    MainMenuState() : IState("MainMenu") {}
    void OnEnter() override { std::cout << "Welcome to the game!\n"; }
    void OnExit() override { std::cout << "Exiting Main Menu\n"; }
};

class MapLoaded : public IState {
public:
    MapLoaded() : IState("MapLoaded") {}
    void OnEnter() override { std::cout << "Loading map...\n"; }
    void OnExit() override { std::cout << "Exiting Map Loaded State\n"; }
};

class MapValidated : public IState {
public:
    MapValidated() : IState("MapValidated") {}
    void OnEnter() override { std::cout << "Validating map...\n"; }
    void OnExit() override { std::cout << "Exiting Map Validated State\n"; }
};

class PlayersAdded : public IState {
public:
    PlayersAdded() : IState("PlayersAdded") {}
    void OnEnter() override { std::cout << "Adding players...\n"; }
    void OnExit() override { std::cout << "Exiting Players Added State\n"; }
};

class AssignReinforcement : public IState {
public:
    AssignReinforcement() : IState("AssignReinforcement") {}
    void OnEnter() override { std::cout << "Assigning reinforcements...\n"; }
    void OnExit() override { std::cout << "Exiting Assign Reinforcement State\n"; }
};

class IssueOrders : public IState {
public:
    IssueOrders() : IState("IssueOrders") {}
    void OnEnter() override { std::cout << "Issuing orders...\n"; }
    void OnExit() override { std::cout << "Exiting Issue Orders State\n"; }
};

class ExecuteOrders : public IState {
public:
    ExecuteOrders() : IState("ExecuteOrders") {}
    void OnEnter() override { std::cout << "Executing orders...\n"; }
    void OnExit() override { std::cout << "Exiting Execute Orders State\n"; }
};

class WinGame : public IState {
public:
    WinGame() : IState("WinGame") {}
    void OnEnter() override { std::cout << "You win!\n"; }
    void OnExit() override { std::cout << "Exiting Win State\n"; }
};

// ===== GameEngine Implementation =====
GameEngine* GameEngine::instance = nullptr;

GameEngine::GameEngine() {
    // Create all states
    mainMenuState = std::make_unique<MainMenuState>();
    mapLoadedState = std::make_unique<MapLoaded>();
    mapValidatedState = std::make_unique<MapValidated>();
    playersAddedState = std::make_unique<PlayersAdded>();
    assignReinforcementState = std::make_unique<AssignReinforcement>();
    issueOrdersState = std::make_unique<IssueOrders>();
    executeOrdersState = std::make_unique<ExecuteOrders>();
    winState = std::make_unique<WinGame>();

    // Hook commands with automatic transitions
    mainMenuState->availableCommands.emplace_back(
        std::make_unique<SimpleCommand>("loadmap", mapLoadedState.get()));

    mapLoadedState->availableCommands.emplace_back(
        std::make_unique<SimpleCommand>("validatemap", mapValidatedState.get()));

    mapValidatedState->availableCommands.emplace_back(
        std::make_unique<SimpleCommand>("addplayer", playersAddedState.get()));

    playersAddedState->availableCommands.emplace_back(
        std::make_unique<SimpleCommand>("assigncountries", assignReinforcementState.get()));

    assignReinforcementState->availableCommands.emplace_back(
        std::make_unique<SimpleCommand>("issueorder", issueOrdersState.get()));

    issueOrdersState->availableCommands.emplace_back(
        std::make_unique<SimpleCommand>("endissueorders", executeOrdersState.get()));

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

int GameEngine::Run() {
    std::string input;
    while (true) {
        std::cout << "> ";
        if (!std::getline(std::cin, input)) break;
        if (!ProcessInput(input)) std::cout << "Invalid command\n";
    }
    return 0;
}

bool GameEngine::ProcessInput(const std::string& input) {
    for (auto& cmd : currentState->availableCommands) {
        if (cmd->name == input) {
            cmd->Execute();
            if (cmd->name == "end") {
                std::cout << "Game Over.\n";
                return false;
            }
            if (cmd->nextState) {
                currentState->OnExit();
                currentState = cmd->nextState;
                currentState->OnEnter();
            }
            return true;
        }
    }
    return false;
}

// ===== Entry point =====
int main() {
    GameEngine::instance = new GameEngine();
    GameEngine::instance->Run();

    std::cout << "Press Enter to exit...";
    std::cin.get();
    return 0;
}
