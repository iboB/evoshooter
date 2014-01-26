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

    int currentFrame() { return m_CurrentFrame; }
    const mathgp::vector3 lastPosition() const { return m_Position; }

    void setLoop(bool loop) { m_Loop = loop; }

    void setScale(float scale);

    float scaledFrameWidth() const { return m_ScaledFrameWidth; }
    float scaledFrameHeight() const { return m_ScaledFrameHeight; }

    void setFlipX(bool flip) { m_FlipX = flip; }

    void setSortingYOffset(float offset);
    float getSortingY() const;

    void update(const mathgp::vector3& position, const mathgp::vector3& camDir);
    void render(const mathgp::matrix4& viewProj);

    bool isRendering() { return m_IsRendering; }

    bool isDone();
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

    bool m_FlipX;

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

    float m_SortingYOffset;
};

typedef std::shared_ptr<Sprite> SpritePtr;
