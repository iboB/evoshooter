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

class Texture;
typedef std::shared_ptr<Texture> TexturePtr;

class Sprite
{
public:
	Sprite();
	~Sprite();

    void init(const std::string& file, Uint32 startX, Uint32 startY, Uint32 width, Uint32 height, Uint32 rows, Uint32 cols, Uint32 duration, bool loop = true);
    void init(TexturePtr texture, Uint32 startX, Uint32 startY, Uint32 width, Uint32 height, Uint32 rows, Uint32 cols, Uint32 duration, bool loop = true);

	void startAnimation();
	void restartAnimation();
	void stopAnimation();

    void setLoop(bool loop) { m_Loop = loop; }

	void update();
    void render();

private:
    void getCurrentFrame(Uint32& x, Uint32& y);

	Uint32 m_StartX;
	Uint32 m_StartY;
    Uint32 m_Width;
    Uint32 m_Height;

	Uint32 m_Rows;
	Uint32 m_Cols;
	Uint32 m_Duration;
	Uint32 m_StartTime;

	TexturePtr m_Texture;
	Uint32 m_CurrentFrame;

    Uint32 m_CellWidth;
    Uint32 m_CellHeight;

	bool m_Loop;

    bool m_IsRunning;
};

typedef std::shared_ptr<Sprite> SpritePtr;
