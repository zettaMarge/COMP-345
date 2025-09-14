#include <string> 
#include <iostream>
#include <vector>

using namespace std;

class ICommand
{
public:
    string name;
    ICommand(const std::string& n) : name(n) {}
    virtual void Execute() {}
    virtual ~ICommand() = default;
};

class IState
{
public:
    string name;
    virtual void OnEnter() = 0;
    virtual void OnExit() = 0;
    virtual vector<ICommand*> GetAvailableCommands() const = 0;
    virtual vector<IState*> GetAvailableStateTransitions() const = 0;

    IState* CanTransitionTo(string stateName) {
        for (IState* availableState : GetAvailableStateTransitions())
        {
            if (availableState->name == stateName) {
                return availableState;
            }
        }
        return nullptr;
    }
};

class GameEngine
{
public: static GameEngine* instance;
	IState* currentState;

	public: 
	int main() {
		string input;
		while (true) {
			cout << "> ";
			getline(cin, input);
			if (input == "exit") break;
			if (!ProcessInput(input)) {
				cout << "Invalid command or state transition." << endl;
			}
		}
		return 0;
	}

	bool ProcessInput(string input) {
		// Process the input and determine if it matches any command or state transition
		if (TryCommand(input)) {
			return true;
		}
		else if (TryTransitionToState(input)) {
			return true;
		}
		return false;
	}

	bool TryCommand(string commandName) const {
		for (ICommand* command : currentState->GetAvailableCommands())
		{
			if (command->name == commandName) {
				command->Execute();
				return true;
			}
		}
		return false;
	}

	bool TryTransitionToState(string newStateName) {
		if (IState* newState = currentState->CanTransitionTo(newStateName)) {
			// Logic to change the current state
			currentState = newState; // This line is illustrative; actual implementation may vary
			return true;
		}
		return false;
	}

};

GameEngine* GameEngine::instance = nullptr;

class StartState : public IState
{
public:
    std::string name = "StartState";
    std::vector<IState*> availableStateTransitions;
    std::vector<ICommand*> availableCommands;

    StartState() {
        // Populate availableStateTransitions and availableCommands as needed
    }

    void OnEnter() override {
        std::cout << "Welcome to the game! Type 'help' for a list of commands." << std::endl;
    }
    void OnExit() override {
        std::cout << "Exiting Start State." << std::endl;
    }
    std::vector<IState*> GetAvailableStateTransitions() const override {
        return availableStateTransitions;
    }
    std::vector<ICommand*> GetAvailableCommands() const override {
        return availableCommands;
    }
};

class MapLoadedState : public IState
{
public:
	std::string name = "loadmap";
	std::vector<IState*> availableStateTransitions;
	std::vector<ICommand*> availableCommands;

	MapLoadedState() {
		// Populate availableStateTransitions and availableCommands as needed
	}

	void OnEnter() override {
		cout << "Loading map..." << endl;
	}
	void OnExit() override {
		cout << "Exiting Map Loaded State." << endl;
	}
	std::vector<IState*> GetAvailableStateTransitions() const override {
		return availableStateTransitions;
	}

	std::vector<ICommand*> GetAvailableCommands() const override {
		return availableCommands;
	}
};

class MapValidatedState : public IState
{
public:
	std::string name = "validatemap";
	std::vector<IState*> availableStateTransitions;
	std::vector<ICommand*> availableCommands;

	MapValidatedState() {
		// Populate availableStateTransitions and availableCommands as needed
	}

	void OnEnter() override {
		cout << "Validating map..." << endl;
	}
	void OnExit() override {
		cout << "Exiting Map Validated State." << endl;
	}
	std::vector<IState*> GetAvailableStateTransitions() const override {
		return availableStateTransitions;
	}

	std::vector<ICommand*> GetAvailableCommands() const override {
		return availableCommands;
	}
};

class PlayersAddedState : public IState
{
public:
	std::string name = "addplayer";
	std::vector<IState*> availableStateTransitions;
	std::vector<ICommand*> availableCommands;

	PlayersAddedState() {
		// Populate availableStateTransitions and availableCommands as needed
	}

	void OnEnter() override {
		cout << "Adding players..." << endl;
	}
	void OnExit() override {
		cout << "Exiting Players Added State." << endl;
	}
	std::vector<IState*> GetAvailableStateTransitions() const override {
		return availableStateTransitions;
	}

	std::vector<ICommand*> GetAvailableCommands() const override {
		return availableCommands;
	}
};

class AssignReinforcementState : public IState
{
public:
	std::string name = "assigncountries";
	std::vector<IState*> availableStateTransitions;
	std::vector<ICommand*> availableCommands;

	AssignReinforcementState() {
		// Populate availableStateTransitions and availableCommands as needed
	}

	void OnEnter() override {
		cout << "Assigning reinforcements..." << endl;
	}
	void OnExit() override {
		cout << "Exiting Assign Reinforcement State." << endl;
	}
	std::vector<IState*> GetAvailableStateTransitions() const override {
		return availableStateTransitions;
	}

	std::vector<ICommand*> GetAvailableCommands() const override {
		return availableCommands;
	}
};

class IssueOrdersState : public IState
{
public:
	std::string name = "issueorder";
	std::vector<IState*> availableStateTransitions;
	std::vector<ICommand*> availableCommands;

	IssueOrdersState() {
		// Populate availableStateTransitions and availableCommands as needed
	}

	void OnEnter() override {
		cout << "Issuing orders..." << endl;
	}
	void OnExit() override {
		cout << "Exiting Issue Orders State." << endl;
	}
	std::vector<IState*> GetAvailableStateTransitions() const override {
		return availableStateTransitions;
	}

	std::vector<ICommand*> GetAvailableCommands() const override {
		return availableCommands;
	}
};

class ExecuteOrdersState : public IState
{
public:
	std::string name = "endissueorders";
	std::vector<IState*> availableStateTransitions;
	std::vector<ICommand*> availableCommands;

	ExecuteOrdersState() {
		// Populate availableStateTransitions and availableCommands as needed
	}

	void OnEnter() override {
		cout << "Executing orders..." << endl;
	}
	void OnExit() override {
		cout << "Exiting Execute Orders State." << endl;
	}
	std::vector<IState*> GetAvailableStateTransitions() const override {
		return availableStateTransitions;
	}

	std::vector<ICommand*> GetAvailableCommands() const override {
		return availableCommands;
	}
};

int main() {
    GameEngine::instance = new GameEngine();
    return GameEngine::instance->main();
}

