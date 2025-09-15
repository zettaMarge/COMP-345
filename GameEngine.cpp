#include "GameEngine.h"

GameEngine* GameEngine::instance = nullptr;

GameEngine::GameEngine() {
    // Create states
    startState = std::make_unique<StartState>();
    mapLoadedState = std::make_unique<MapLoadedState>();
    mapValidatedState = std::make_unique<MapValidatedState>();
    playersAddedState = std::make_unique<PlayersAddedState>();
    assignReinforcementState = std::make_unique<AssignReinforcementState>();
    issueOrdersState = std::make_unique<IssueOrdersState>();
    executeOrdersState = std::make_unique<ExecuteOrdersState>();
    winState = std::make_unique<WinState>();

    // Hook up transitions
    startState->availableStateTransitions.push_back(mapLoadedState.get());
    mapLoadedState->availableStateTransitions.push_back(mapValidatedState.get());
    mapValidatedState->availableStateTransitions.push_back(playersAddedState.get());
    playersAddedState->availableStateTransitions.push_back(assignReinforcementState.get());
    assignReinforcementState->availableStateTransitions.push_back(issueOrdersState.get());
    issueOrdersState->availableStateTransitions.push_back(executeOrdersState.get());
    executeOrdersState->availableStateTransitions.push_back(winState.get());
    winState->availableStateTransitions.push_back(startState.get()); // loop

    // Hook up commands
    startState->availableCommands.push_back(new LoadMapCommand());
    mapLoadedState->availableCommands.push_back(new ValidateMapCommand());
    mapValidatedState->availableCommands.push_back(new AddPlayerCommand());
    playersAddedState->availableCommands.push_back(new AssignCountriesCommand());
    assignReinforcementState->availableCommands.push_back(new IssueOrderCommand());
    issueOrdersState->availableCommands.push_back(new EndIssueOrdersCommand());
    executeOrdersState->availableCommands.push_back(new WinCommand());
    winState->availableCommands.push_back(new PlayCommand());
    winState->availableCommands.push_back(new EndCommand());

    currentState = startState.get();
    currentState->OnEnter();
}

int GameEngine::main() {
    std::string input;
    while (true) {
        std::cout << "> ";
        if (!std::getline(std::cin, input)) break;
        if (input == "exit") break;
        if (!ProcessInput(input)) {
            std::cout << "Invalid command or state transition.\n";
        }
    }
    return 0;
}

bool GameEngine::ProcessInput(const std::string& input) {
    if (TryCommand(input)) return true;
    if (TryTransitionToState(input)) return true;
    return false;
}

bool GameEngine::TryCommand(const std::string& commandName) {
    for (auto* command : currentState->availableCommands) {
        if (command->name == commandName) {
            command->Execute();
            return true;
        }
    }
    return false;
}

bool GameEngine::TryTransitionToState(const std::string& newStateName) {
    if (IState* newState = currentState->CanTransitionTo(newStateName)) {
        currentState->OnExit();
        currentState = newState;
        currentState->OnEnter();
        return true;
    }
    return false;
}

// === Entry point ===
int main() {
    GameEngine::instance = new GameEngine();
    int result = GameEngine::instance->main();

    std::cout << "Game ended. Press Enter to exit...";
    std::cin.get();
    return result;
}
