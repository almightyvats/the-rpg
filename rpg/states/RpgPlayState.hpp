#ifndef RPG_RPGPLAYSTATE_HPP
#define RPG_RPGPLAYSTATE_HPP

#include "RpgGameState.hpp"
#include "rpg/RpgGame.hpp"

class RpgPlayState : public RpgGameState {
	RpgPlayState();
	~RpgPlayState();

  public:
	RpgPlayState(const RpgPlayState &) = delete;
	RpgPlayState &operator=(const RpgPlayState &) = delete;

	static RpgPlayState &Instance(bool loadSaveGame = false)
	{
		static RpgPlayState m_ps;

		if (loadSaveGame) {
			m_ps.LoadPlayerPos();
		}
		return m_ps;
	}

	void Pause() override;
	void Resume() override;

	void HandleEvents(RpgGame *rpgGame) override;
	void Update(RpgGame *rpgGame) override;
	void Render(RpgGame *rpgGame) override;

	void LoadPlayerPos();

	static SDL_Event event;
};

#endif // RPG_RPGPLAYSTATE_HPP
