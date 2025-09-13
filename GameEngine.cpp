#include <string> 
#include <iostream>
using namespace std;

class GameEngine
{
	IState currentState;

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
		for (ICommand command : currentState.AvailableCommands)
		{
			if (command.name == commandName) {
				command.Execute();
				return true;
			}
		}
		return false;
	}

	bool TryTransitionToState(string newState) {
		if (currentState.CanTransitionTo(newState)) {
			// Logic to change the current state
			currentState = newState; // This line is illustrative; actual implementation may vary
			return true;
		}
		return false;
	}

}

class ICommand
{
	string name;
	virtual void Execute() = 0;
}

class IState
{
	string name;
	virtual IState AvailableStateTransitions[] = 0;
	virtual ICommand AvailableCommands[] = 0;

	virtual void OnEnter() = 0;
	virtual void OnExit() = 0;

	bool CanTransitionTo(string stateName) {
		for (IState availableState : AvailableStateTransitions)
		{
			if (availableState.name == stateName) {
				return true;
			}
		}
		return false;
	}
}

class StartState : IState
{
	string name = "StartState";
	IState AvailableStateTransitions[] = { MapLoadedState };
	ICommand AvailableCommands[] = { new HelpCommand() };
	void OnEnter() override {
		cout << "Welcome to the game! Type 'help' for a list of commands." << endl;
	}
	void OnExit() override {
		cout << "Exiting Start State." << endl;
	}
}

class MapLoadedState : IState
{
	string name = "loadmap";
	IState AvailableStateTransitions[] = { MapLoadedState, MapValidatedState };
	ICommand AvailableCommands[] = { new LookCommand(), new MoveCommand() };
	void OnEnter() override {
		cout << "Loading map..." << endl;
	}
	void OnExit() override {
		cout << "Exiting Map Loaded State." << endl;
	}
}

class MapValidatedState : IState
{
	string name = "validatemap";
	IState AvailableStateTransitions[] = { new InventoryState() };
	ICommand AvailableCommands[] = { new LookCommand(), new MoveCommand(), new ValidateMapCommand() };
	void OnEnter() override {
		cout << "Validating map..." << endl;
	}
	void OnExit() override {
		cout << "Exiting Map Validated State." << endl;
	}
}

class PlayersAddedState : IState
{
	string name = "addplayer";
	IState AvailableStateTransitions[] = { PlayersAddedState };
	ICommand AvailableCommands[] = { new LookCommand(), new MoveCommand(), new AddPlayerCommand() };
	void OnEnter() override {
		cout << "Adding players..." << endl;
	}
	void OnExit() override {
		cout << "Exiting Players Added State." << endl;
	}
}

class AssignReinforcementState : IState
{
	string name = "assigncountries";
	IState AvailableStateTransitions[] = { IssueOrdersState };
	ICommand AvailableCommands[] = { new LookCommand(), new MoveCommand(), new AssignReinforcementCommand() };
	void OnEnter() override {
		cout << "Assigning reinforcements..." << endl;
	}
	void OnExit() override {
		cout << "Exiting Assign Reinforcement State." << endl;
	}
}

class IssueOrdersState : IState
{
	string name = "issueorder";
	IState AvailableStateTransitions[] = { ExecuteOrdersState, IssueOrdersState };
	ICommand AvailableCommands[] = { new LookCommand(), new MoveCommand(), new IssueOrderCommand() };
	void OnEnter() override {
		cout << "Issuing orders..." << endl;
	}
	void OnExit() override {
		cout << "Exiting Issue Orders State." << endl;
	}
}

class ExecuteOrdersState : IState
{
	string name = "endissueorders";
	IState AvailableStateTransitions[] = { AssignReinforcementState, ExecuteOrdersState, WinState };
	ICommand AvailableCommands[] = { new LookCommand(), new MoveCommand(), new ExecuteOrderCommand() };
	void OnEnter() override {
		cout << "Executing orders..." << endl;
	}
	void OnExit() override {
		cout << "Exiting Execute Orders State." << endl;
	}
}

class WinState : IState
{
	string name = "win";
	IState AvailableStateTransitions[] = { StartState };
	ICommand AvailableCommands[] = { new LookCommand(), new MoveCommand(), new WinCommand() };
	void OnEnter() override {
		cout << "Congratulations! You've won the game!" << endl;
	}
	void OnExit() override {
		cout << "Exiting Win State." << endl;
	}
}


