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

	void startRendering(Uint32 frameOffset = 0);
    void restartRendering(Uint32 frameOffset = 0);
	void stopRendering();

    void setLoop(bool loop) { m_Loop = loop; }

    void setScale(float scale);

    void update(const mathgp::vector3& position, const mathgp::vector3& camDir);
    void render(const mathgp::matrix4& viewProj);

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
    Uint32 m_FramesOffset;

    Uint32 m_CellWidth;
    Uint32 m_CellHeight;

    float m_Scale;
    float m_ScaledFrameWidth;
    float m_ScaledFrameHeight;

    mathgp::vector3 m_Position;

	bool m_Loop;

    bool m_IsRendering;

    struct SpriteVertex
    {
        mathgp::point3 position;
        mathgp::float2 texCoord;
    };

    enum SpriteVertexAttribs
    {
        Attr_Pos, Attr_TexCoord
    };

    SpriteVertex m_Vertices[4];
    static unsigned sm_Indices[6];
};

typedef std::shared_ptr<Sprite> SpritePtr;
