//
// gd-proto
// Copyright (c) 2014 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
// main application class definition
#include "EvoShooter.pch.h"
#include "SoundManager.h"
#include <SDL_mixer.h>
#include <iostream>

SoundManager::SoundManager()
{
    initialize();
}
SoundManager::~SoundManager()
{}

void SoundManager::initialize()
{
    int audio_rate = 44100;
    //Uint16 audio_format = AUDIO_S16;
    Uint16 audio_format = AUDIO_F32LSB;
    int audio_channels = 2;
    int audio_buffers = 4096;
    //int audio_buffers = 8192;
    /*
    not sure if this does anything at all.. screw it.
    if (Mix_QuerySpec(&audio_rate, &audio_format, &audio_channels))
    {
        std::cout << "ERROR : Could not initialize audio!" << std::endl;
        return;
    }
    */

    if (Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers)) {
        std::cout << "ERROR : Could not initialize audio!" << std::endl;
        return;
    }
    //music
    Mix_Music* audioTrack = Mix_LoadMUS("sound/Track1.ogg");
    m_tracks.push_back(audioTrack);
    //sounds
    Mix_Chunk* chunk = Mix_LoadWAV("sound/PlayerDeath.wav");
    m_sounds.push_back(chunk);

    chunk = Mix_LoadWAV("sound/Shoot.wav");
    m_sounds.push_back(chunk);

    chunk = Mix_LoadWAV("sound/EnemyDeath.wav");
    m_sounds.push_back(chunk);

    chunk = Mix_LoadWAV("sound/EnemyOnHit.wav");
    m_sounds.push_back(chunk);

    m_currentTrack = NULL;
    m_isMusicPaused = false;
}
void SoundManager::playSound(ESounds sound, int loopCount)
{
    Mix_PlayChannel(-1, m_sounds[sound], loopCount);
}
void SoundManager::playTrack(unsigned int id, bool shouldLoop)
{
    if (m_currentTrack == NULL && id >= 0 && id < m_tracks.size())
    {
        int loopCount = shouldLoop ? -1 : 1;
        Mix_PlayMusic(m_tracks[id],loopCount);
        m_currentTrack = m_tracks[id];
    }
}
bool SoundManager::isMusicPlaying()
{
    return (m_currentTrack != NULL);
}
void SoundManager::pauseMusic()
{
    if (isMusicPlaying() && !m_isMusicPaused)
    {
        Mix_PauseMusic();
        m_isMusicPaused = true;
    }
     
}
void SoundManager::resumeMusic()
{
    if (m_isMusicPaused && m_currentTrack)
    {
        Mix_ResumeMusic();
        m_isMusicPaused = false;
    }
}
void SoundManager::stopMusic()
{
    if (m_currentTrack)
    {
        Mix_HaltMusic();
        m_currentTrack = NULL;
    }
}