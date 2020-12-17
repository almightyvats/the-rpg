#include <iostream>
#include <string>

#include "Combat.hpp"
#include "Combatant.hpp"
#include "PlayerCombatant.hpp"
#include "EnemyCombatant.hpp"
#include "Equipment.hpp"

#define RAT_STATS {2*lvl, lvl, (int)(0.5*lvl + 1), 2*lvl, (int)(0.5*lvl + 1), lvl}
#define RAT_MAX_HP 20 + 5*lvl
#define RAT_ATTACK_TACKLE {"Tackle", AttackType::melee, AttackTargetType::single, 4, 0.95, 0.1, 2.0, 6, AttackEffect::none}
#define RAT_ATTACK_BITE {"Bite", AttackType::melee, AttackTargetType::single, 6, 0.8, 0.2, 2.5, 4, AttackEffect::none}

#define EQ_SWORD_ATTACK_SWORD_SWING {"Sword Swing", AttackType::melee, AttackTargetType::single, 9, 0.85, 0.2, 1.5, 6, AttackEffect::none}
#define EQ_BOW_ATTACK_PRECISION_SHOT {"Precision Shot", AttackType::ranged, AttackTargetType::single, 12, 0.8, 0.2, 3.0, 9, AttackEffect::none}
#define EQ_BOW_ATTACK_HAIL_OF_ARROWS {"Hail of Arrows", AttackType::ranged, AttackTargetType::multi, 6, 0.5, 0.05, 3.0, 12, AttackEffect::none}

EnemyCombatant GenerateEnemyRat(int lvl, int number) {

    std::string name = std::string("Rat ") + std::to_string(number);
    EnemyCombatant rat = EnemyCombatant(name, lvl, RAT_MAX_HP, RAT_STATS, 0);
    rat.AddAttack(RAT_ATTACK_TACKLE);
    rat.AddAttack(RAT_ATTACK_BITE);
    return rat;
}

Equipment GenerateSimpleSword(std::string name, int item_level) {
    Equipment sword = Equipment(name, EquipmentType::sword, item_level, 0, item_level, 0,0,0,0);
    sword.AddAttack(EQ_SWORD_ATTACK_SWORD_SWING);
    return sword;
}

Equipment GenerateSimpleBow(std::string name, int item_level) {
    Equipment bow = Equipment(name, EquipmentType::bow, item_level,0,0,0,0,0,0);
    bow.AddAttack(EQ_BOW_ATTACK_PRECISION_SHOT);
    bow.AddAttack(EQ_BOW_ATTACK_HAIL_OF_ARROWS);
    return bow;
}

int main(int argc, char* argv[])
{

    PlayerCombatant test1 = PlayerCombatant("Arthur", 5);
    Equipment sword1 = Equipment("Sword1", EquipmentType::sword, 2,0,0,0,0,0,0);
    Attack sword1_attack1 = {"Sword Swing", AttackType::melee, AttackTargetType::single, 50, 0.7, 0.2, 2.0, 4, AttackEffect::none};
    sword1.AddAttack(sword1_attack1);
    test1.AddEquipment(sword1);
    std::cout << test1.level() << "\n";

    EnemyCombatant test2 = EnemyCombatant("test2", 3, 500, {1,2,3,4,5,6}, 10);
    test2.AddAttack(sword1_attack1);

    std::vector<Combatant*> targets;
    targets.push_back(&test2);
    std::cout << test2.hp() << "\n";
    test1.PerformAttack(sword1_attack1, targets);
    std::cout << test2.hp() << "\n";
    test1.ChooseAndPerformAction(std::vector<Combatant*>(), targets);

    std::vector<Combatant*> player_combatants;
    player_combatants.push_back(&test1);

    std::cout << "cooldowns: " << test1.cooldown_ << " " << test2.cooldown_ << "\n";

    Combat combat = Combat(player_combatants, targets);

    test2.cooldown_ = 3;

    combat.Initiate();

    std::cout << "cooldowns: " << test1.cooldown_ << " " << test2.cooldown_ << "\n";
}