//
// evoshooter
// Copyright (c) 2014 
// Borislav Stanimirov, Filip Chorbadzhiev, Nikolay Dimitrov
// Assen Kanev, Jem Kerim, Stefan Ivanov
//
//This game and all content in this file is licensed under  
//the Attribution-Noncommercial-Share Alike 3.0 version of the Creative Commons License.
//For reference the license is given below and can also be found at http://creativecommons.org/licenses/by-nc-sa/3.0/
//

#pragma once

#include "ImplicitSingleton.h"

class Texture;
typedef std::shared_ptr<Texture> TexturePtr;

class Sprite;
typedef std::shared_ptr<Sprite> SpritePtr;

class Effect;

class ResourceManager : public ImplicitSingleton<ResourceManager>
{
public:
    ResourceManager();

	TexturePtr getTexture(const std::string& file);
	void releaseTexture(TexturePtr& texture); // forget'about'it

    SpritePtr createSpriteFromSingleFrameTexture(const std::string& file); // one frame, no animation, no shit
    SpritePtr createSpriteFromSingleAnimationTexture(const std::string& file, Uint32 rows, Uint32 cols, int duration); //single looping animation in one texture

    Effect* getSpriteEffect() { return m_SpriteEffect; }
    Effect* getSimpleEffect() { return m_SimpleEffect; }
private:
	typedef std::map<std::string, TexturePtr> TextureMap;
	TextureMap m_Textures;
    Effect* m_SpriteEffect;
    Effect* m_SimpleEffect;

	TexturePtr findTexture(const std::string& file) const;
};
