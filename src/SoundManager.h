//
// gd-proto
// Copyright (c) 2014 Borislav Stanimirov
//
//This game and all content in this file is licensed under  
//the Attribution-Noncommercial-Share Alike 3.0 version of the Creative Commons License.
//For reference the license is given below and can also be found at http://creativecommons.org/licenses/by-nc-sa/3.0/
//
// main application class declaration
#pragma once

#include "ImplicitSingleton.h"

/*
struct Mix_Music;
struct Mix_Chunk;
*/
#include <SDL_mixer.h>

enum ESounds{
    ESounds_PlayerDeath = 0,
    ESounds_Shoot,
    ESounds_EnemyDeath,
    ESounds_EnemyOnHit,
    ESounds_Count
};

class SoundManager : public ImplicitSingleton<SoundManager>
{
public:
    SoundManager();
    ~SoundManager();

    void initialize();

    void playSound(ESounds sound, int loopCount = 0);
    void playTrack(unsigned int id, bool shouldLoop);
    bool isMusicPlaying();
    void pauseMusic();
    void resumeMusic();
    void stopMusic();
private:
   
    std::vector<Mix_Chunk*> m_sounds;
    std::vector<Mix_Music*> m_tracks;
    Mix_Music* m_currentTrack;
    bool m_isMusicPaused;
};
