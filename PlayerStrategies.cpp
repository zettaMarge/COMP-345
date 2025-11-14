#include "PlayerStrategies.h"

void PlayerStrategies::SetPlayer(Player* p) {
    player = p;
}

PlayerStrategies::~PlayerStrategies() {
    player = NULL;
}


// ----- Human -----
vector<Territory*> HumanPlayerStrategy::ToAttack() const {

}

vector<Territory*> HumanPlayerStrategy::ToDefend() const {
    
}

void HumanPlayerStrategy::IssueOrder() {

}
// ----- Human -----


// ----- Aggressive -----
vector<Territory*> AggressivePlayerStrategy::ToAttack() const {

}

vector<Territory*> AggressivePlayerStrategy::ToDefend() const {
    
}

void AggressivePlayerStrategy::IssueOrder() {
    
}
// ----- Aggressive -----


// ----- Benevolent -----
vector<Territory*> BenevolentPlayerStrategy::ToAttack() const {

}

vector<Territory*> BenevolentPlayerStrategy::ToDefend() const {
    
}

void BenevolentPlayerStrategy::IssueOrder() {
    
}
// ----- Benevolent -----


// ----- Neutral -----
vector<Territory*> NeutralPlayerStrategy::ToAttack() const {

}

vector<Territory*> NeutralPlayerStrategy::ToDefend() const {
    
}

void NeutralPlayerStrategy::IssueOrder() {
    
}
// ----- Neutral -----


// ----- Cheater -----
vector<Territory*> CheaterPlayerStrategy::ToAttack() const {

}

vector<Territory*> CheaterPlayerStrategy::ToDefend() const {
    
}

void CheaterPlayerStrategy::IssueOrder() {
    
}
// ----- Cheater -----