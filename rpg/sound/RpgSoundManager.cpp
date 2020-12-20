#include "RpgSoundManager.hpp"
#include <iostream>

RpgSoundManager::~RpgSoundManager() {}

void RpgSoundManager::init() {
    int audio_rate = 22050;
    Uint16 audio_format = MIX_DEFAULT_FORMAT;
    int audio_channels = 2;
    int audio_buffers = 4096;

    if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, audio_buffers) != 0)
    {
		std::cout << "Couldn't init audio: " << Mix_GetError();
    }
}

void RpgSoundManager::addMusic(const char *musicPath, const std::string& musicId) {
	auto itr = m_MusicStore.find(musicId);
	if (itr == m_MusicStore.end()) {
		auto mix_music = Mix_LoadMUS(musicPath);
		m_MusicStore.insert({musicId, mix_music});
	}
}

void RpgSoundManager::addSoundEffect(const char *effectPath,const std::string& effectId) {
    auto itr = m_ChunkStore.find(effectId);
    if (itr == m_ChunkStore.end()) {
		auto mix_music = Mix_LoadWAV(effectPath);
		m_ChunkStore.insert({effectId, mix_music});
	}
}

void RpgSoundManager::playMusic(const std::string &musicId) {
    auto itr = m_MusicStore.find(musicId);
	if (itr != m_MusicStore.end()) {
        Mix_PlayMusic(itr->second, -1);
	}

    isMusicPlaying = true;
}

void RpgSoundManager::playEffect(const std::string &effectId) {
    auto itr = m_ChunkStore.find(effectId);
    if (itr != m_ChunkStore.end()) {
        Mix_PlayChannel(-1, itr->second, 0);
    }
}
void RpgSoundManager::pauseMusic() {

    Mix_PauseMusic();
    isMusicPlaying = false;
}
void RpgSoundManager::ResumeMusic() {

	Mix_ResumeMusic();
	isMusicPlaying = true;
}
