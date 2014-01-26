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

#include "EvoShooter.pch.h"

#include "Texture.h"
#include "Sprite.h"
#include "Effect.h"

#include "ResourceManager.h"

ResourceManager::ResourceManager()
{
    m_SpriteEffect = new Effect;

    m_SpriteEffect->loadVertexShaderFromFile("shaders/sprite.vert");
    m_SpriteEffect->loadPixelShaderFromFile("shaders/sprite.frag");

    m_SpriteEffect->link();

    m_SimpleEffect = new Effect;

    m_SimpleEffect->loadVertexShaderFromFile("shaders/simple.vert");
    m_SimpleEffect->loadPixelShaderFromFile("shaders/simple.frag");

    m_SimpleEffect->link();
}

TexturePtr ResourceManager::getTexture(const std::string& file)
{
	TexturePtr texture = findTexture(file);

	if(texture)
	{
		return texture;
	}

	texture.reset(new Texture());

	try
	{
		texture->loadFromFile(file.c_str());
	}
	catch(...)
	{
		return TexturePtr();
	}

	m_Textures[file] = texture;

	return texture;
}

void ResourceManager::releaseTexture(TexturePtr& texture)
{
	TexturePtr cachedTexture = findTexture(texture->name());

	texture.reset((Texture*)0);

	if(!cachedTexture)
	{
		return;
	}

	if(cachedTexture.use_count() == 2)
	{
		m_Textures.erase(cachedTexture->name());
	}
}

SpritePtr ResourceManager::createSpriteFromSingleFrameTexture(const std::string& file)
{
    TexturePtr texture = getTexture(file);

    if (!texture)
    {
        return SpritePtr(0);
    }

    SpritePtr sprite(new Sprite());

    sprite->init(file, 0, 0, texture->width(), texture->height(), 1, 1, 0, false);

    return sprite;
}

SpritePtr ResourceManager::createSpriteFromSingleAnimationTexture(const std::string& file, Uint32 rows, Uint32 cols, int duration)
{
    TexturePtr texture = getTexture(file);

    if (!texture)
    {
        return SpritePtr(0);
    }

    SpritePtr sprite(new Sprite());

    sprite->init(file, 0, 0, texture->width(), texture->height(), rows, cols, duration, true);

    return sprite;
}

TexturePtr ResourceManager::findTexture(const std::string& file) const
{
	TextureMap::const_iterator it = m_Textures.find(file);

	if (it != m_Textures.end())
	{
		return it->second;
	}

	return TexturePtr();
}
