#include "LootGenerator.hpp"
#include "rpg/RpgGame.hpp"
#include "rpg/states/RpgStates.hpp"
#include "Equipment.hpp"

#include <cstdlib>


#define SWORD_ATTACK_SWORD_SWING \
	{ \
		"Sword Swing", AttackType::melee, AttackTargetType::single, (int)(0.5 * level + 4), 0, 0.85, 0.3, 1.5, 6, \
		    AttackEffect::none \
	}
#define SWORD_ATTACK_POWER_STRIKE \
	{ \
		"Power Strike", AttackType::melee, AttackTargetType::single, (int)(1.2 * level + 3), 0, 0.75, 0.1, 2.0, 12, \
		    AttackEffect::slow \
	}
#define SWORD_ATTACK_QUICK_STAB \
	{ \
		"Quick Stab", AttackType::melee, AttackTargetType::single, (int)(0.3 * level + 3), 0, 0.85, 0.0, 0.0, 3, \
		    AttackEffect::armor_breaking \
	}
#define SWORD_ATTACK_RAGE_BLADE \
	{ \
		"Rageblade", AttackType::melee, AttackTargetType::multi, (int)(0.2 * level + 3), 0, 0.6, 0.3, 2.0, 9, \
		    AttackEffect::none \
	}
#define SWORD_ATTACK_RISKY_STRIKE \
	{ \
		"Risky Strike", AttackType::melee, AttackTargetType::single, (int)(1.3 * level + 2), 0, 0.3, 0.8, 3.0, 15, \
		    AttackEffect::none \
	}

Equipment GenerateSword(int level, EquipmentMaterial material)
{
	Equipment item;
	int r_factor = std::rand() % 12;
	int agility, dexterity;

	switch (material) {
	case EquipmentMaterial::wood:
		agility = (r_factor < 4) ? (std::rand() % level) + level / 2 : 0;
		dexterity = (r_factor >= 4 && r_factor < 8) ? (std::rand() % level) + level / 2 : 0;
		item = Equipment("Wooden Sword", EquipmentType::sword, EquipmentMaterial::wood, level, agility, 0, 0, dexterity,
		                 0, 0);
		item.AddAttack(SWORD_ATTACK_SWORD_SWING);
		if (r_factor >= 8 && r_factor < 10) {
			item.AddAttack(SWORD_ATTACK_POWER_STRIKE);
		} else if (r_factor >= 8) {
			item.AddAttack(SWORD_ATTACK_QUICK_STAB);
		}
		break;

	case EquipmentMaterial::metal:
		agility = (r_factor < 4) ? 2 * ((std::rand() % level) + level / 2) : (std::rand() % level) + level / 2;
		dexterity = (r_factor >= 4 && r_factor < 8) ? 2 * ((std::rand() % level) + level / 2)
		                                            : (std::rand() % level) + level / 2;
		item = Equipment("Metal Sword", EquipmentType::sword, EquipmentMaterial::metal, level, agility, 0, 0, dexterity,
		                 0, 0);
		item.AddAttack(SWORD_ATTACK_SWORD_SWING);
		if (r_factor % 2) {
			item.AddAttack(SWORD_ATTACK_POWER_STRIKE);
		} else {
			item.AddAttack(SWORD_ATTACK_QUICK_STAB);
		}
		if (r_factor >= 8 && r_factor < 10) {
			item.AddAttack(SWORD_ATTACK_RAGE_BLADE);
		} else if (r_factor >= 8) {
			item.AddAttack(SWORD_ATTACK_RISKY_STRIKE);
		}
		break;
	case EquipmentMaterial::gold:
		agility = (r_factor < 6) ? 3 * ((std::rand() % level) + level / 2) : 2 * ((std::rand() % level) + level / 2);
		dexterity = (r_factor >= 6) ? 3 * ((std::rand() % level) + level / 2) : 2 * ((std::rand() % level) + level / 2);
		item = Equipment("Golden Sword", EquipmentType::sword, EquipmentMaterial::gold, level, agility, 0, 0, dexterity,
		                 0, 0);
		item.AddAttack(SWORD_ATTACK_QUICK_STAB);
		item.AddAttack(SWORD_ATTACK_POWER_STRIKE);
		if (r_factor % 2) {
			item.AddAttack(SWORD_ATTACK_RAGE_BLADE);
		} else {
			item.AddAttack(SWORD_ATTACK_RISKY_STRIKE);
		}
		break;
	default:
		item = Equipment("Unknown Sword", EquipmentType::sword, EquipmentMaterial::wood, level, 0, 0, 0, 0, 0, 0);
		item.AddAttack(SWORD_ATTACK_SWORD_SWING);
		break;
	}
	return item;
}

#define AXE_ATTACK_AXE_SWING \
	{ \
		"Axe Swing", AttackType::melee, AttackTargetType::single, (int)(0.6 * level + 3), 0, 0.9, 0.2, 2.0, 8, \
		    AttackEffect::none \
	}
#define AXE_ATTACK_HEAD_CRUSHER \
	{ \
		"Head Crusher", AttackType::melee, AttackTargetType::single, (int)(1.3 * level + 5), 0, 0.8, 0.0, 1.0, 13, \
		    AttackEffect::armor_breaking \
	}
#define AXE_ATTACK_ANKLE_BREAKER \
	{ \
		"Ankle Breaker", AttackType::melee, AttackTargetType::single, (int)(0.4 * level + 3), 0, 0.9, 0.0, 1.0, 10, \
		    AttackEffect::slow \
	}
#define AXE_ATTACK_SPINNING_AXE \
	{ \
		"Spinning Axe", AttackType::melee, AttackTargetType::multi, (int)(0.3 * level + 5), 0, 0.6, 0.2, 2.0, 8, \
		    AttackEffect::none \
	}
#define AXE_ATTACK_AXE_TOSS \
	{ \
		"Axe Toss", AttackType::ranged, AttackTargetType::single, (int)(1.8 * level + 5), (3 * level), 0.7, 0.1, 3.0, \
		    15, AttackEffect::none \
	}

Equipment GenerateAxe(int level, EquipmentMaterial material)
{
	Equipment item;
	int r_factor = std::rand() % 12;
	int strength, dexterity, agility, penetration;

	switch (material) {
	case EquipmentMaterial::wood:
		strength = (r_factor < 6) ? (std::rand() % level) + level / 2 : 0;
		dexterity = 0;
		item = Equipment("Wooden Axe", EquipmentType::axe, EquipmentMaterial::wood, level, 0, strength, 0, dexterity, 0,
		                 0);
		item.AddAttack(AXE_ATTACK_AXE_SWING);
		if (r_factor >= 6) {
			item.AddAttack(AXE_ATTACK_HEAD_CRUSHER);
		}
		break;

	case EquipmentMaterial::metal:
		strength = (r_factor < 6) ? 2 * ((std::rand() % level) + level / 2) : (std::rand() % level) + level / 2;
		dexterity = (r_factor >= 6) ? ((std::rand() % level) + level / 2) : 0;
		item = Equipment("Metal Axe", EquipmentType::axe, EquipmentMaterial::metal, level, 0, strength, 0, dexterity, 0,
		                 0);
		item.AddAttack(AXE_ATTACK_AXE_SWING);
		if (r_factor % 2) {
			item.AddAttack(AXE_ATTACK_HEAD_CRUSHER);
		} else {
			item.AddAttack(AXE_ATTACK_ANKLE_BREAKER);
		}
		break;
	case EquipmentMaterial::gold:
		strength = (r_factor < 4) ? 3 * ((std::rand() % level) + level / 2) : 2 * ((std::rand() % level) + level / 2);
		dexterity = (r_factor >= 4 && r_factor < 8) ? 2 * ((std::rand() % level) + level / 2)
		                                            : (std::rand() % level) + level / 2;
		agility = (r_factor >= 8) ? ((std::rand() % level) + level / 2) : 0;
		item = Equipment("Golden Axe", EquipmentType::axe, EquipmentMaterial::gold, level, agility, strength, 0,
		                 dexterity, 0, 0);
		item.AddAttack(AXE_ATTACK_AXE_SWING);
		if (r_factor % 2) {
			item.AddAttack(AXE_ATTACK_HEAD_CRUSHER);
		} else {
			item.AddAttack(AXE_ATTACK_ANKLE_BREAKER);
		}
		if (r_factor % 4 < 2) {
			item.AddAttack(AXE_ATTACK_SPINNING_AXE);
		} else {
			item.AddAttack(AXE_ATTACK_AXE_TOSS);
		}
		break;
	default:
		item = Equipment("Unknown Axe", EquipmentType::axe, EquipmentMaterial::wood, level, 0, 0, 0, 0, 0, 0);
		item.AddAttack(AXE_ATTACK_AXE_SWING);
		break;
	}
	return item;
}

#define BOW_ATTACK_ARROW_SHOT \
	{ \
		"Arrow Shot", AttackType::ranged, AttackTargetType::single, (int)(0.7 * level + 8), penetration, 0.8, 0.05, \
		    3.0, 4, AttackEffect::none \
	}
#define BOW_ATTACK_PRECISION_SHOT \
	{ \
		"Precision Shot", AttackType::ranged, AttackTargetType::single, (int)(0.7 * level + 8), penetration, 0.8, 0.3, \
		    3.0, 8, AttackEffect::none \
	}
#define BOW_ATTACK_TRIP_ARROW \
	{ \
		"Trip Arrow", AttackType::ranged, AttackTargetType::single, (int)(0.7 * level + 8), penetration, 0.95, 0.0, \
		    0.0, 4, AttackEffect::slow \
	}
#define BOW_ATTACK_HAIL_OF_ARROWS \
	{ \
		"Hail of Arrows", AttackType::ranged, AttackTargetType::multi, (int)(0.7 * level + 8), \
		    (int)(0.5 * penetration), 0.4, 0.05, 3.0, 10, AttackEffect::none \
	}

Equipment GenerateBow(int level, EquipmentMaterial material)
{
	Equipment item;
	int r_factor = std::rand() % 12;
	int agility, perception, penetration;

	switch (material) {
	case EquipmentMaterial::wood:
		agility = 0;
		perception = (r_factor < 4) ? (std::rand() % level) + level / 2 : 0;
		penetration = (r_factor >= 4 && r_factor < 8) ? 2 * level : (int)(1.5 * level);
		item = Equipment("Wooden Bow", EquipmentType::bow, EquipmentMaterial::wood, level, agility, 0, 0, 0, perception,
		                 0);
		item.AddAttack(BOW_ATTACK_ARROW_SHOT);
		if (r_factor >= 8) {
			item.AddAttack(BOW_ATTACK_PRECISION_SHOT);
		}
		break;

	case EquipmentMaterial::metal:
		agility = (r_factor < 4) ? ((std::rand() % level) + level / 2) : 0;
		perception = (r_factor >= 4 && r_factor < 8) ? (int)(2.5 * level) : 2 * level;
		penetration = (std::rand() % level) + level / 2;
		item = Equipment("Metal Bow", EquipmentType::bow, EquipmentMaterial::metal, level, agility, 0, 0, 0, perception,
		                 0);
		item.AddAttack(BOW_ATTACK_PRECISION_SHOT);
		if (r_factor >= 8) {
			item.AddAttack(BOW_ATTACK_TRIP_ARROW);
		} else {
			item.AddAttack(BOW_ATTACK_ARROW_SHOT);
		}
		break;
	case EquipmentMaterial::gold:
		agility = (r_factor < 4) ? 2 * ((std::rand() % level) + level / 2) : (std::rand() % level) + level / 2;
		perception = (r_factor >= 4 && r_factor < 8) ? 3 * level : (int)(2.5 * level);
		penetration = (r_factor >= 8) ? 2 * ((std::rand() % level) + level / 2) : (std::rand() % level) + level / 2;
		item = Equipment("Golden Bow", EquipmentType::bow, EquipmentMaterial::gold, level, agility, 0, 0, 0, perception,
		                 0);
		item.AddAttack(BOW_ATTACK_PRECISION_SHOT);
		item.AddAttack(BOW_ATTACK_TRIP_ARROW);
		item.AddAttack(BOW_ATTACK_HAIL_OF_ARROWS);
		break;
	default:
		item = Equipment("Unknown Bow", EquipmentType::bow, EquipmentMaterial::wood, level, 0, 0, 0, 0, 0, 0);
		item.AddAttack(BOW_ATTACK_ARROW_SHOT);
		break;
	}
	return item;
}

#define SHIELD_ATTACK_SHIELD_BASH \
	{ \
		"Shield Bash", AttackType::melee, AttackTargetType::single, (int)(0.5 * level + 1), 0, 0.85, 0.0, 1.0, 4, \
		    AttackEffect::slow \
	}

Equipment GenerateShield(int level, EquipmentMaterial material)
{
	Equipment item;
	int r_factor = std::rand() % 12;
	int agility, defense;

	switch (material) {
	case EquipmentMaterial::wood:
		agility = -0.5 * ((std::rand() % level) + level / 2);
		defense = 0.5 * ((std::rand() % level) + level / 2);
		item = Equipment("Wooden Shield", EquipmentType::shield, EquipmentMaterial::wood, level, agility, 0, defense, 0,
		                 0, 0);
		break;

	case EquipmentMaterial::metal:
		agility = -1 * ((std::rand() % level) + level / 2);
		defense = 1.5 * ((std::rand() % level) + level / 2);
		item = Equipment("Metal Shield", EquipmentType::shield, EquipmentMaterial::metal, level, agility, 0, defense, 0,
		                 0, 0);
		break;
	case EquipmentMaterial::gold:
		agility = (r_factor < 4) ? 0 : 0.5 * ((std::rand() % level) + level / 2);
		defense = (r_factor >= 4 && r_factor < 8) ? 3 * ((std::rand() % level) + level / 2)
		                                          : 2 * ((std::rand() % level) + level / 2);
		item = Equipment("Golden Shield", EquipmentType::shield, EquipmentMaterial::gold, level, agility, 0, defense, 0,
		                 0, 0);
		if (r_factor >= 8) {
			item.AddAttack(SHIELD_ATTACK_SHIELD_BASH);
		}
		break;
	default:
		item = Equipment("Useless Shield", EquipmentType::shield, EquipmentMaterial::wood, level, -level, 0, level, 0,
		                 0, 0);
		break;
	}
	return item;
}

#define HEAL_ABILITY_HEAL \
	{ \
		"Heal", AbilityTargetType::team_single, 3 * level, 0, 1, 8, AbilityEffect::none \
	}
#define HEAL_ABILITY_TEAM_HEAL \
	{ \
		"Team Heal", AbilityTargetType::team_multi, 2 * level, 0, 1, 8, AbilityEffect::none \
	}

Equipment GenerateHealItem(int level, EquipmentMaterial material)
{
	Equipment item;
	int agility, luck;

	switch (material) {
	case EquipmentMaterial::wood:
		item = Equipment("Selfrefilling Potion", EquipmentType::heal_item, EquipmentMaterial::wood, level, 0, 0, 0, 0,
		                 0, 0);
		item.AddAbility(HEAL_ABILITY_HEAL);
		break;
	case EquipmentMaterial::metal:
		item =
		    Equipment("Infinite Potion", EquipmentType::heal_item, EquipmentMaterial::metal, level, 0, 0, 0, 0, 0, 0);
		item.AddAbility(HEAL_ABILITY_HEAL);
		item.AddAbility(HEAL_ABILITY_TEAM_HEAL);
		break;
	case EquipmentMaterial::gold:
		agility = (std::rand() % level) + level / 2;
		luck = (std::rand() % level) + level / 2;
		item = Equipment("Heretic Potion", EquipmentType::heal_item, EquipmentMaterial::gold, level, agility, 0, 0, 0,
		                 0, luck);
		item.AddAbility(HEAL_ABILITY_HEAL);
		item.AddAbility(HEAL_ABILITY_TEAM_HEAL);
		break;
	default:
		item = Equipment("Unknown Potion", EquipmentType::heal_item, EquipmentMaterial::wood, level, 0, 0, 0, 0, 0, 0);
		break;
	}
	return item;
}

#define DROP_RATE_F_MAT 10
#define DROP_RATE_MAT_WOOD 6
#define DROP_RATE_MAT_METAL 3
#define DROP_RATE_MAT_GOLD 1

EquipmentType item_types[] = {EquipmentType::sword, EquipmentType::axe, EquipmentType::bow, EquipmentType::shield,
                              EquipmentType::heal_item};

Equipment GenerateLoot(int min_level, int max_level)
{
	EquipmentType item_type = item_types[std::rand() % (sizeof(item_types) / sizeof(*item_types))];
	int item_level = (std::rand() % (max_level - min_level + 1)) + min_level;
	int r_mat = std::rand() % DROP_RATE_F_MAT;
	EquipmentMaterial item_mat;

	if (r_mat < DROP_RATE_MAT_WOOD) {
		item_mat = EquipmentMaterial::wood;
	} else if (r_mat < DROP_RATE_MAT_METAL + DROP_RATE_MAT_WOOD) {
		item_mat = EquipmentMaterial::metal;
	} else {
		item_mat = EquipmentMaterial::gold;
	}

	Equipment item;
	switch (item_type) {
	case EquipmentType::sword:
		item = GenerateSword(item_level, item_mat);
		break;
	case EquipmentType::axe:
		item = GenerateAxe(item_level, item_mat);
		break;
	case EquipmentType::bow:
		item = GenerateBow(item_level, item_mat);
		break;
	case EquipmentType::shield:
		item = GenerateShield(item_level, item_mat);
		break;
	case EquipmentType::heal_item:
		item = GenerateHealItem(item_level, item_mat);
	}
	RpgGame::assets->CreateInventoryItem(item, RpgPlayState::groupItems);
	return item;
}

Equipment GenerateBaseLoot(int min_level, int max_level)
{
	EquipmentType item_type = item_types[std::rand() % (sizeof(item_types) / sizeof(*item_types))];
	int item_level = (std::rand() % (max_level - min_level + 1)) + min_level;
	EquipmentMaterial item_mat = EquipmentMaterial::wood;

	Equipment item;
	switch (item_type) {
	case EquipmentType::sword:
		item = GenerateSword(item_level, item_mat);
		break;
	case EquipmentType::axe:
		item = GenerateAxe(item_level, item_mat);
		break;
	case EquipmentType::bow:
		item = GenerateBow(item_level, item_mat);
		break;
	case EquipmentType::shield:
		item = GenerateShield(item_level, item_mat);
		break;
	case EquipmentType::heal_item:
		item = GenerateHealItem(item_level, item_mat);
	}
	RpgGame::assets->CreateInventoryItem(item, RpgPlayState::groupItems);
	return item;
}
