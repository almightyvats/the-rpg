#ifndef RPG_RPGGAMESTATE_HPP
#define RPG_RPGGAMESTATE_HPP

#include "rpg/RpgGame.hpp"

class RpgGameState {
  public:
	virtual void Pause() = 0;
	virtual void Resume() = 0;

	virtual void HandleEvents(RpgGame *) = 0;
	virtual void Update(RpgGame *) = 0;
	virtual void Render(RpgGame *) = 0;

	void changeState(RpgGame *game, RpgGameState &state) { game->changeState(state); }

	enum groupLabels : std::size_t {
		groupMap,
		groupPlayers,
		groupColliders,
		groupProjectiles,
		groupNpcs,
		groupEnemies,
		groupItems
	};	
};

#endif // RPG_RPGGAMESTATE_HPP
