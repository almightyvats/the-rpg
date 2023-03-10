#pragma once

#include <cereal/types/vector.hpp>
#include <sstream>

#include "Combatant.hpp"

enum class EquipmentType {
	sword,
	axe,
	bow,
	shield,
	heal_item,
};

enum class EquipmentMaterial {
	wood,
	metal,
	gold,
	fire,
	ice,
	special,
};

class Equipment {
  public:
	Equipment(const std::string &name, EquipmentType eq_type, EquipmentMaterial eq_mat, int min_level, int agi, int str,
	          int def, int dex, int perc, int luck);
	Equipment() = default;
	~Equipment();

	void AddAttack(Attack attack);
	void AddAbility(Ability ability);

	std::string name() const { return name_; }
	EquipmentType type() const { return type_; }
	EquipmentMaterial material() const { return material_; }
	int min_level() const { return min_level_; }

	std::vector<Attack> attacks() const { return attacks_; }
	std::vector<Ability> abilities() const { return abilities_; }
	int bonus_agility() const { return bonus_agility_; }
	int bonus_strength() const { return bonus_strength_; }
	int bonus_defense() const { return bonus_defense_; }
	int bonus_dexterity() const { return bonus_dexterity_; }
	int bonus_perception() const { return bonus_perception_; }
	int bonus_luck() const { return bonus_luck_; }

	std::string item_info()
	{
		std::stringstream result;
		switch (type()) {
		case EquipmentType::sword:
			result << "Sword"
			       << "\n";
			break;
		case EquipmentType::axe:
			result << "Axe"
			       << "\n";
			break;
		case EquipmentType::bow:
			result << "Bow"
			       << "\n";
			break;
		case EquipmentType::shield:
			result << "Shield"
			       << "\n";
			break;
		case EquipmentType::heal_item:
			result << "Potion"
			       << "\n";
			break;
		default:
			break;
		}
		result << "min. lvl: " << min_level() << "\n";
		if (bonus_agility() > 0) {
			result << "agility:"
			       << "    " << bonus_agility() << "\n";
		}
		if (bonus_strength() > 0) {
			result << "strength:"
			       << "    " << bonus_strength() << "\n";
		}
		if (bonus_defense() > 0) {
			result << "defense:"
			       << "    " << bonus_defense() << "\n";
		}
		if (bonus_dexterity() > 0) {
			result << "dexterity:"
			       << "    " << bonus_dexterity() << "\n";
		}
		if (bonus_perception() > 0) {
			result << "perception:"
			       << "    " << bonus_perception() << "\n";
		}
		if (bonus_luck() > 0) {
			result << "luck:"
			       << "    " << bonus_luck() << "\n";
		}

		return result.str();
	}

	template <class Archive>
	void serialize(Archive &archive)
	{
		archive(CEREAL_NVP(name_), CEREAL_NVP(min_level_), CEREAL_NVP(bonus_agility_), CEREAL_NVP(bonus_strength_),
		        CEREAL_NVP(bonus_defense_), CEREAL_NVP(bonus_dexterity_), CEREAL_NVP(bonus_perception_),
		        CEREAL_NVP(bonus_luck_), CEREAL_NVP(attacks_), CEREAL_NVP(abilities_), CEREAL_NVP(type_),
		        CEREAL_NVP(material_)); // serialize things by passing them to the archive
	}

  protected:
	std::string name_;
	EquipmentType type_;
	EquipmentMaterial material_;

	int min_level_;

	std::vector<Attack> attacks_;
	std::vector<Ability> abilities_;

	int bonus_agility_;
	int bonus_strength_;
	int bonus_defense_;
	int bonus_dexterity_;
	int bonus_perception_;
	int bonus_luck_;
};