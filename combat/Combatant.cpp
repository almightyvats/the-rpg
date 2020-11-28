#include "Combatant.hpp"

#include <cstdlib>
#include <ctime>

#define TOKENS_PER_LEVEL_UP 2

Combatant::Combatant(const std::string& name, int level) {
    m_name = name;

    m_level = 1;
    m_exp = 0;

    std::srand(std::time(nullptr));

    m_max_hp = (std::rand() % 200) + 100;
    m_hp = m_max_hp;
    m_state = 0;

    m_agility = (std::rand() % 10) + 1;
    m_strength = (std::rand() % 10) + 1;
    m_defense = (std::rand() % 10) + 1;
    m_dexterity = (std::rand() % 10) + 1;
    m_perception = (std::rand() % 10) + 1;
    m_luck = (std::rand() % 10) + 1;

    while (m_level < level) {
        level_up(TOKENS_PER_LEVEL_UP, 0);
    }
}

Combatant::~Combatant() {
    // for image in frontend
}

void Combatant::level_up(int tokens, int new_exp) {
    
    std::srand(std::time(nullptr));

    m_level++;
    m_exp = new_exp;
    int tokens_left = tokens;

    while (tokens_left > 0) {
        switch (rand() % 6) {
            case 0:
                m_agility++;
                break;
            case 1:
                m_strength++;
                break;
            case 2:
                m_defense++;
                break;            
            case 3:
                m_dexterity++;
                break;
            case 4:
                m_perception++;
                break;
            case 5:
                m_luck++;
                break;
        }
    }
}