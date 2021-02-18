#pragma once

#include "Components.hpp"
#include <string>

struct NameComponent : public Component {

  public:
	std::string name;

	NameComponent() = default;
	NameComponent(std::string name) { this->name = name; }
	~NameComponent() {}
};