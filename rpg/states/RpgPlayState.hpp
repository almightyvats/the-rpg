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

	static RpgPlayState &Instance()
	{
		static RpgPlayState m_ps;
		return m_ps;
	}

	void Pause();
	void Resume();

	void HandleEvents(RpgGame *rpgGame);
	void Update(RpgGame *rpgGame);
	void Render(RpgGame *rpgGame);

	static SDL_Event event;
	static AssetManager *assets;
	enum groupLabels : std::size_t { groupMap, groupPlayers, groupColliders, groupProjectiles };
};

#endif // RPG_RPGPLAYSTATE_HPP
