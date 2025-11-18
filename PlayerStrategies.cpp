#include "PlayerStrategies.h"
#include <vector>

void PlayerStrategies::SetPlayer(Player* p) {
    player = p;
}

PlayerStrategies::~PlayerStrategies() {
    player = NULL;
}


// ----- Human -----
std::vector<Territory*> HumanPlayerStrategy::ToAttack() const {

}

std::vector<Territory*> HumanPlayerStrategy::ToDefend() const {
    
}

void HumanPlayerStrategy::IssueOrder() {

}
// ----- Human -----


// ----- Aggressive -----
std::vector<Territory*> AggressivePlayerStrategy::ToAttack() const {

}

std::vector<Territory*> AggressivePlayerStrategy::ToDefend() const {
    
}

void AggressivePlayerStrategy::IssueOrder() {
    
}
// ----- Aggressive -----


// ----- Benevolent -----
std::vector<Territory*> BenevolentPlayerStrategy::ToAttack() const {

}

std::vector<Territory*> BenevolentPlayerStrategy::ToDefend() const {
    
}

void BenevolentPlayerStrategy::IssueOrder() {
    
}
// ----- Benevolent -----


// ----- Neutral -----
std::vector<Territory*> NeutralPlayerStrategy::ToAttack() const {

}

std::vector<Territory*> NeutralPlayerStrategy::ToDefend() const {
    
}

void NeutralPlayerStrategy::IssueOrder() {
    
}
// ----- Neutral -----


// ----- Cheater -----
std::vector<Territory*> CheaterPlayerStrategy::ToAttack() const {

}

std::vector<Territory*> CheaterPlayerStrategy::ToDefend() const {
    
}

void CheaterPlayerStrategy::IssueOrder() {
    
}
// ----- Cheater -----