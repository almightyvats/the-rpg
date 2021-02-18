#pragma once

#include "../RpgLabel.hpp"
#include "../combat/Equipment.hpp"
#include "RpgGameState.hpp"
#include <memory>

class RpgInventoryState : public RpgGameState {

	RpgInventoryState();
	~RpgInventoryState();

  public:
	RpgInventoryState(const RpgInventoryState &) = delete;
	RpgInventoryState &operator=(const RpgInventoryState &) = delete;

	static RpgInventoryState &Instance()
	{
		static RpgInventoryState m_is;
		return m_is;
	}

	void Pause() override;
	void Resume() override;

	void HandleEvents(RpgGame *rpgGame) override;
	void Update(RpgGame *rpgGame) override;
	void Render(RpgGame *rpgGame) override;

	static SDL_Event m_event;
};