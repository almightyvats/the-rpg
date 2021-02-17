#ifndef RPG_SOUNDMANAGER_HPP
#define RPG_SOUNDMANAGER_HPP
#include <SDL_mixer.h>
#include <map>
#include <memory>
#include <string>

class RpgSoundManager {
  private:
	inline static std::map<std::string, Mix_Chunk *> m_ChunkStore;
	inline static std::map<std::string, Mix_Music *> m_MusicStore;

	inline static bool m_isMusicPlaying;
	inline static std::string m_lastPlayedId;

	inline static int m_lastVolume;
	inline static bool m_isMuted = false;

  public:
	RpgSoundManager() = default;
	~RpgSoundManager();

	static void init();

	static void addMusic(const char *musicPath, const std::string &musicId);
	static void addSoundEffect(const char *effectPath, const std::string &effectId);

	static void playMusic(const std::string &musicId);
	static void playEffect(const std::string &effectId);

	static void pauseMusic();
	static void resumeMusic(const std::string &musicId);

	static void setMusicVolume(int volume);
	static int getMusicVolume();
	static void toggleMuteVolume();
	static bool isMusicMuted();
	static int getLastSetMusicVolume();

	static void cleanupSoundFiles();
};

#endif