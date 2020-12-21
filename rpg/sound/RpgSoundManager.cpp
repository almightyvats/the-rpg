#include "RpgSoundManager.hpp"
#include <iostream>

RpgSoundManager::~RpgSoundManager() {}

void RpgSoundManager::init()
{
	int audio_rate = 22050;
	Uint16 audio_format = MIX_DEFAULT_FORMAT;
	int audio_channels = 2;
	int audio_buffers = 4096;

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
		std::cout << "Couldn't init audio: " << Mix_GetError();
	} else {
		std::cout << "Sound initialised" << std::endl;
	}
}

void RpgSoundManager::addMusic(const char *musicPath, const std::string &musicId)
{
	auto itr = m_MusicStore.find(musicId);
	if (itr == m_MusicStore.end()) {
		auto mix_music = Mix_LoadMUS(musicPath);
		if (mix_music != nullptr) {
			m_MusicStore.insert({musicId, mix_music});
		} else {
			std::cout << "Problem Loading " << musicId << " : " << Mix_GetError() << std::endl;
		}
	}
}

void RpgSoundManager::addSoundEffect(const char *effectPath, const std::string &effectId)
{
	auto itr = m_ChunkStore.find(effectId);
	if (itr == m_ChunkStore.end()) {
		auto mix_chuck = Mix_LoadWAV(effectPath);
		if (mix_chuck != nullptr) {
			m_ChunkStore.insert({effectId, mix_chuck});
		} else {
			std::cout << "Problem Loading " << effectId << " : " << Mix_GetError() << std::endl;
		}
	}
}

void RpgSoundManager::playMusic(const std::string &musicId)
{
	auto itr = m_MusicStore.find(musicId);
	if (itr != m_MusicStore.end()) {
		std::cout << "Music Found!\n";
		Mix_Music *music = itr->second;
		if (m_isMusicPlaying) {
			Mix_HaltMusic();
		}
		Mix_PlayMusic(music, -1);
	}
	m_lastPlayedId = musicId;
	m_isMusicPlaying = true;
}

void RpgSoundManager::playEffect(const std::string &effectId)
{
	auto itr = m_ChunkStore.find(effectId);
	if (itr != m_ChunkStore.end()) {
		Mix_PlayChannel(-1, itr->second, 0);
	}
}

void RpgSoundManager::pauseMusic()
{
	if (m_isMusicPlaying) {
		Mix_PauseMusic();
	}
	m_isMusicPlaying = false;
}

void RpgSoundManager::resumeMusic(const std::string &musicId)
{
	if (m_lastPlayedId != musicId) {
		Mix_HaltMusic();
		playMusic(musicId);
	} else if (!m_isMusicPlaying) {
		Mix_ResumeMusic();
	}

	m_lastPlayedId = musicId;
	m_isMusicPlaying = true;
}
