#include <iostream>

#include "Combatant.hpp"
#include "PlayerCombatant.hpp"
#include "EnemyCombatant.hpp"
#include "Equipment.hpp"

int main(int argc, char* argv[]) {
    std::cout << "start" << "\n";
    PlayerCombatant test1 = PlayerCombatant("test1", 5);
    Equipment sword1 = Equipment("Sword1", EquipmentType::sword, 2,0,0,0,0,0,0);
    Attack sword1_attack1 = {"Sword Swing", AttackType::melee, AttackTargetType::single, 50, 0.7, 0.2, 2.0, 4, AttackEffect::none};
    sword1.AddAttack(sword1_attack1);
    test1.AddEquipment(sword1);
    std::cout << test1.level() << "\n";

    EnemyCombatant test2 = EnemyCombatant("test1", 3, 500, {1,2,3,4,5,6}, 10);
    test2.AddAttack(sword1_attack1);

    std::vector<Combatant*> targets;
    targets.push_back(&test2);
    std::cout << test2.hp() << "\n";
    test1.PerformAttack(sword1_attack1, targets);
    std::cout << test2.hp() << "\n";
}