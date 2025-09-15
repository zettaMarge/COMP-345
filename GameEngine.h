#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <memory>

// ===== ICommand =====
class ICommand {
public:
    std::string name;
    ICommand(const std::string& n) : name(n) {}
    virtual void Execute() = 0;
    virtual ~ICommand() = default;
};

// Concrete commands
class LoadMapCommand : public ICommand {
public: LoadMapCommand() : ICommand("loadmap") {}
      void Execute() override { std::cout << "Executing LoadMapCommand\n"; }
};
class ValidateMapCommand : public ICommand {
public: ValidateMapCommand() : ICommand("validatemap") {}
      void Execute() override { std::cout << "Executing ValidateMapCommand\n"; }
};
class AddPlayerCommand : public ICommand {
public: AddPlayerCommand() : ICommand("addplayer") {}
      void Execute() override { std::cout << "Executing AddPlayerCommand\n"; }
};
class AssignCountriesCommand : public ICommand {
public: AssignCountriesCommand() : ICommand("assigncountries") {}
      void Execute() override { std::cout << "Executing AssignCountriesCommand\n"; }
};
class IssueOrderCommand : public ICommand {
public: IssueOrderCommand() : ICommand("issueorder") {}
      void Execute() override { std::cout << "Executing IssueOrderCommand\n"; }
};
class EndIssueOrdersCommand : public ICommand {
public: EndIssueOrdersCommand() : ICommand("endissueorders") {}
      void Execute() override { std::cout << "Executing EndIssueOrdersCommand\n"; }
};
class ExecuteOrdersCommand : public ICommand {
public: ExecuteOrdersCommand() : ICommand("executeorders") {}
      void Execute() override { std::cout << "Executing ExecuteOrdersCommand\n"; }
};
class EndExecuteOrdersCommand : public ICommand {
public: EndExecuteOrdersCommand() : ICommand("endexecuteorders") {}
      void Execute() override { std::cout << "Executing EndExecuteOrdersCommand\n"; }
};
class WinCommand : public ICommand {
public: WinCommand() : ICommand("win") {}
      void Execute() override { std::cout << "Executing WinCommand\n"; }
};
class EndCommand : public ICommand {
public: EndCommand() : ICommand("end") {}
      void Execute() override { std::cout << "Executing EndCommand\n"; }
};
class PlayCommand : public ICommand {
public: PlayCommand() : ICommand("play") {}
      void Execute() override { std::cout << "Executing PlayCommand\n"; }
};

// ===== IState =====
class IState {
public:
    std::string name;
    std::vector<ICommand*> availableCommands;           // non-owning
    std::vector<IState*> availableStateTransitions;     // non-owning

    IState(const std::string& n) : name(n) {}
    virtual void OnEnter() = 0;
    virtual void OnExit() = 0;

    IState* CanTransitionTo(const std::string& stateName) {
        for (auto* s : availableStateTransitions)
            if (s->name == stateName) return s;
        return nullptr;
    }
    virtual ~IState() = default;
};

// Concrete states
class StartState : public IState {
public: StartState() : IState("start") {}
      void OnEnter() override { std::cout << "Welcome to the game!\n"; }
      void OnExit() override { std::cout << "Exiting Start State.\n"; }
};
class MapLoadedState : public IState {
public: MapLoadedState() : IState("loadmap") {}
      void OnEnter() override { std::cout << "Loading map...\n"; }
      void OnExit() override { std::cout << "Exiting Map Loaded State.\n"; }
};
class MapValidatedState : public IState {
public: MapValidatedState() : IState("validatemap") {}
      void OnEnter() override { std::cout << "Validating map...\n"; }
      void OnExit() override { std::cout << "Exiting Map Validated State.\n"; }
};
class PlayersAddedState : public IState {
public: PlayersAddedState() : IState("addplayer") {}
      void OnEnter() override { std::cout << "Adding players...\n"; }
      void OnExit() override { std::cout << "Exiting Players Added State.\n"; }
};
class AssignReinforcementState : public IState {
public: AssignReinforcementState() : IState("assigncountries") {}
      void OnEnter() override { std::cout << "Assigning reinforcements...\n"; }
      void OnExit() override { std::cout << "Exiting Assign Reinforcement State.\n"; }
};
class IssueOrdersState : public IState {
public: IssueOrdersState() : IState("issueorder") {}
      void OnEnter() override { std::cout << "Issuing orders...\n"; }
      void OnExit() override { std::cout << "Exiting Issue Orders State.\n"; }
};
class ExecuteOrdersState : public IState {
public: ExecuteOrdersState() : IState("executeorders") {}
      void OnEnter() override { std::cout << "Executing orders...\n"; }
      void OnExit() override { std::cout << "Exiting Execute Orders State.\n"; }
};
class WinState : public IState {
public: WinState() : IState("win") {}
      void OnEnter() override { std::cout << "You win!\n"; }
      void OnExit() override { std::cout << "Exiting Win State.\n"; }
};

// ===== GameEngine =====
class GameEngine {
public:
    static GameEngine* instance;

    // Own all states
    std::unique_ptr<StartState> startState;
    std::unique_ptr<MapLoadedState> mapLoadedState;
    std::unique_ptr<MapValidatedState> mapValidatedState;
    std::unique_ptr<PlayersAddedState> playersAddedState;
    std::unique_ptr<AssignReinforcementState> assignReinforcementState;
    std::unique_ptr<IssueOrdersState> issueOrdersState;
    std::unique_ptr<ExecuteOrdersState> executeOrdersState;
    std::unique_ptr<WinState> winState;

    IState* currentState = nullptr;

    GameEngine();
    int main();
    bool ProcessInput(const std::string& input);
    bool TryCommand(const std::string& commandName);
    bool TryTransitionToState(const std::string& stateName);
};
