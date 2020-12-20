#ifndef RPG_SOUNDMANAGER_HPP
#define RPG_SOUNDMANAGER_HPP
#include <SDL_mixer.h>
#include <map>
#include <string>
#include <memory>

class RpgSoundManager {
  private:
    static std::map<std::string, Mix_Chunk*> m_ChunkStore;
    static std::map<std::string, Mix_Music*> m_MusicStore;

	static bool isMusicPlaying;
  public:
    RpgSoundManager() = default;
	~RpgSoundManager();

	static void init();

	static void addMusic(const char *musicPath, const std::string& musicId);
	static void addSoundEffect(const char *effectPath, const std::string& effectId);

	static void playMusic(const std::string& musicId);
	static void playEffect(const std::string& effectId);

	static void pauseMusic();
	static void ResumeMusic();

};

#endif