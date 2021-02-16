
#ifndef RPG_RPGPLAYERCONVOSTATE_HPP
#define RPG_RPGPLAYERCONVOSTATE_HPP

#include "RpgGameState.hpp"
#include "rpg/RpgGame.hpp"
#include "rpg/RpgLabel.hpp"
#include "rpg/RpgTimer.hpp"
#include <memory>

class RpgLabel;

using LabelItem = std::shared_ptr<RpgLabel>;
using LabelItemList = std::vector<LabelItem>;

class RpgPlayerConvoState : public RpgGameState {

	RpgPlayerConvoState();
	~RpgPlayerConvoState();

  public:
	RpgPlayerConvoState(const RpgPlayerConvoState &) = delete;
	RpgPlayerConvoState &operator=(const RpgPlayerConvoState &) = delete;

	static RpgPlayerConvoState &Instance(std::string npcName)
	{
		static RpgPlayerConvoState m_pcs;
		m_pcs.NewConversation(npcName);
		return m_pcs;
	}

	void Pause() override;
	void Resume() override;

	void HandleEvents(RpgGame *rpgGame) override;
	void Update(RpgGame *rpgGame) override;
	void Render(RpgGame *rpgGame) override;

	static bool isPlayerReadyToTalk();
	static void setPlayerReadyToTalk(bool talkOrNot);
	void NewConversation(std::string npcName);

  private:
	inline static SDL_Event m_event;
	inline static bool m_readyToTalk = true;
	bool m_isPlayerTalking;
	LabelItemList m_labels;
    LabelItem m_playerDialogueLabel;
	SDL_Rect m_dialogueBox1;
	SDL_Rect m_dialogueBox2;
	std::unique_ptr<RpgTimer> m_timer;
	std::string m_scriptPath;
	int m_totalLines;
};

#endif