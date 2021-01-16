#pragma once
#include <algorithm>
#include <array>
#include <bitset>
#include <iostream>
#include <memory>
#include <vector>

class Component;
class Entity;
class Manager;

using ComponentID = std::size_t;
using Group = std::size_t;
using State = std::size_t;

inline ComponentID getNewComponentTypeID()
{
	static ComponentID lastID = 0u;
	return lastID++;
}

template <typename T>
inline ComponentID getComponentTypeID() noexcept
{
	static_assert(std::is_base_of<Component, T>::value, "");
	static ComponentID typeID = getNewComponentTypeID();
	return typeID;
}

constexpr std::size_t maxComponents = 32;
constexpr std::size_t maxGroups = 32;

using ComponentBitSet = std::bitset<maxComponents>;
using GroupBitset = std::bitset<maxGroups>;

using ComponentArray = std::array<Component *, maxComponents>;

class Component {
  public:
	Entity *entity;

	virtual void init() {}
	virtual void update() {}
	virtual void draw(int alpha) {}
	virtual ~Component() {}
};

class Entity {
  private:
	Manager &manager;
	const State m_state;
	bool active = true;
	std::vector<std::unique_ptr<Component>> components;

	ComponentArray componentArray;
	ComponentBitSet componentBitset;
	GroupBitset groupBitset;

  public:
	Entity(Manager &mManager, State state) : manager(mManager), m_state(state) {}

	void update(State state)
	{
		if (m_state == state) {
			for (auto &c : components)
				c->update();
		}
	}
	void draw(int alpha, State state)
	{
		if (m_state == state) {
			for (auto &c : components)
				c->draw(alpha);
		}
	}

	State getState() { return m_state; };
	bool isActive() const { return active; }
	void destroy() { active = false; }

	bool hasGroup(Group mGroup) { return groupBitset[mGroup]; }

	void addGroup(Group mGroup);
	void delGroup(Group mGroup) { groupBitset[mGroup] = false; }

	template <typename T>
	bool hasComponent() const
	{
		return componentBitset[getComponentTypeID<T>()];
	}

	template <typename T, typename... TArgs>
	T &addComponent(TArgs &&... mArgs)
	{
		T *c(new T(std::forward<TArgs>(mArgs)...));
		c->entity = this;
		std::unique_ptr<Component> uPtr{c};
		components.emplace_back(std::move(uPtr));

		componentArray[getComponentTypeID<T>()] = c;
		componentBitset[getComponentTypeID<T>()] = true;

		c->init();
		return *c;
	}

	template <typename T>
	T &getComponent() const
	{
		auto ptr(componentArray[getComponentTypeID<T>()]);
		return *static_cast<T *>(ptr);
	}
};

class Manager {
  private:
	std::vector<std::unique_ptr<Entity>> entities;
	std::array<std::vector<Entity *>, maxGroups> groupedEntities;

  public:
	void update(State state)
	{
		for (auto &e : entities)
			e->update(state);
	}
	void draw(int alpha, State state)
	{
		for (auto &e : entities)
			e->draw(alpha, state);
	}
	void refresh(State state)
	{
		for (auto i(0u); i < maxGroups; i++) {
			auto &v(groupedEntities[i]);
			v.erase(std::remove_if(std::begin(v), std::end(v),
			                       [i, state](Entity *mEntity) {
				                       return ((!mEntity->isActive() || !mEntity->hasGroup(i))
				                               && (mEntity->getState() == state));
			                       }),
			        std::end(v));
		}

		entities.erase(std::remove_if(std::begin(entities), std::end(entities),
		                              [state](const std::unique_ptr<Entity> &mEntity) {
			                              return (!mEntity->isActive() && (mEntity->getState() == state));
		                              }),
		               std::end(entities));
	}

	void AddToGroup(Entity *mEntity, Group mGroup) { groupedEntities[mGroup].emplace_back(mEntity); }

	std::vector<Entity *> &getGroup(Group mGroup) { return groupedEntities[mGroup]; }

	Entity &addEntity(State state)
	{
		Entity *e = new Entity(*this, state);
		std::unique_ptr<Entity> uPtr{e};
		entities.emplace_back(std::move(uPtr));
		return *e;
	}
};