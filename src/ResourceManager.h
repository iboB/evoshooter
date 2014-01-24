//
// evoshooter
// Copyright (c) 2014 
// Borislav Stanimirov, Filip Chorbadzhiev, Nikolay Dimitrov
// Assen Kanev, Jem Kerim, Stefan Ivanov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//

#pragma once

#include "ImplicitSingleton.h"

class Texture;
typedef std::shared_ptr<Texture> TexturePtr;

class Sprite;
typedef std::shared_ptr<Sprite> SpritePtr;

class ResourceManager : public ImplicitSingleton<ResourceManager>
{
public:
	TexturePtr getTexture(const std::string& file);
	void releaseTexture(TexturePtr& texture); // forget'about'it

    SpritePtr createSpriteFromSingleFrameTexture(const std::string& file); // one frame, no animation, no shit
    SpritePtr createSpriteFromSingleAnimationTexture(const std::string& file, Uint32 rows, Uint32 cols); //single looping animation in one texture

private:
	typedef std::map<std::string, TexturePtr> TextureMap;
	TextureMap m_Textures;

	TexturePtr findTexture(const std::string& file) const;
};
