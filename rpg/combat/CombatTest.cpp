#include <iostream>
#include <string>

#include "Combat.hpp"
#include "Combatant.hpp"
#include "PlayerCombatant.hpp"
#include "EnemyCombatant.hpp"
#include "Equipment.hpp"

#define RAT_STATS {(int)(1.5*lvl), 2*lvl, (int)(0.5*lvl + 1), 2*lvl, (int)(0.5*lvl + 1), lvl}
#define RAT_MAX_HP 20 + 5*lvl
#define RAT_ATTACK_TACKLE {"Tackle", AttackType::melee, AttackTargetType::single, 4, 0, 0.95, 0.1, 2.0, 6, AttackEffect::none}
#define RAT_ATTACK_BITE {"Bite", AttackType::melee, AttackTargetType::single, 6, 0, 0.8, 0.2, 2.5, 4, AttackEffect::none}

#define EQ_SWORD_ATTACK_SWORD_SWING {"Sword Swing", AttackType::melee, AttackTargetType::single, 9, 0, 0.85, 0.3, 1.5, 6, AttackEffect::none}
#define EQ_BOW_ATTACK_PRECISION_SHOT {"Precision Shot", AttackType::ranged, AttackTargetType::single, 10, (int)(1.5*item_level+1), 0.8, 0.2, 3.0, 7, AttackEffect::none}
#define EQ_BOW_ATTACK_HAIL_OF_ARROWS {"Hail of Arrows", AttackType::ranged, AttackTargetType::multi, 4, (int)(1.5*item_level+1), 0.5, 0.05, 3.0, 9, AttackEffect::none}

EnemyCombatant GenerateEnemyRat(int lvl, int number)
{
    std::string name = std::string("Rat ") + std::to_string(number);
    EnemyCombatant rat = EnemyCombatant(name, lvl, RAT_MAX_HP, RAT_STATS, 0);
    rat.AddAttack(RAT_ATTACK_TACKLE);
    rat.AddAttack(RAT_ATTACK_BITE);
    return rat;
}

Equipment GenerateSimpleSword(std::string name, int item_level)
{
    Equipment sword = Equipment(name, EquipmentType::sword, item_level,-(int)(0.5*item_level), (int)(0.5*item_level), 0,0,0,0);
    sword.AddAttack(EQ_SWORD_ATTACK_SWORD_SWING);
    return sword;
}

Equipment GenerateSimpleArmor(std::string name, int item_level)
{
    Equipment armor = Equipment(name, EquipmentType::armor, item_level,-(int)(0.5*item_level),0,item_level,0,0,0);
    return armor;
}

Equipment GenerateSimpleBow(std::string name, int item_level)
{
    Equipment bow = Equipment(name, EquipmentType::bow, item_level,(int)(0.5*item_level),0,0,0,0,0);
    bow.AddAttack(EQ_BOW_ATTACK_PRECISION_SHOT);
    bow.AddAttack(EQ_BOW_ATTACK_HAIL_OF_ARROWS);
    return bow;
}

int main(int argc, char* argv[])
{

    PlayerCombatant pc_hiei = PlayerCombatant("Hiei", 10, 80, 15, 13, 8, 12, 2, 9);
    pc_hiei.AddEquipment(GenerateSimpleSword("Metal Sword", 7));
    pc_hiei.AddEquipment(GenerateSimpleArmor("Plot Armor", 5));
    PlayerCombatant pc_pokkle = PlayerCombatant("Pokkle", 7, 60, 10, 4, 11, 4, 17, 4);
    pc_pokkle.AddEquipment(GenerateSimpleBow("Wooden Bow", 7));
    pc_hiei.AddEquipment(GenerateSimpleArmor("Ten", 7));

    EnemyCombatant rat1 = EnemyCombatant(GenerateEnemyRat(5, 1));
    EnemyCombatant rat2 = EnemyCombatant(GenerateEnemyRat(10, 2));
    EnemyCombatant rat3 = EnemyCombatant(GenerateEnemyRat(7, 3));

    std::vector<Combatant*> player_combatants;
    player_combatants.push_back(&pc_hiei);
    player_combatants.push_back(&pc_pokkle);

    std::vector<Combatant*> enemy_combatants;
    enemy_combatants.push_back(&rat1);
    enemy_combatants.push_back(&rat2);
    enemy_combatants.push_back(&rat3);

    Combat combat = Combat(player_combatants, enemy_combatants);

    combat.Initiate();
}