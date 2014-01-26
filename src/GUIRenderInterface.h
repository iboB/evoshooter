//
// gd-proto
// Copyright (c) 2014 Borislav Stanimirov
//
//This game and all content in this file is licensed under  
//the Attribution-Noncommercial-Share Alike 3.0 version of the Creative Commons License.
//For reference the license is given below and can also be found at http://creativecommons.org/licenses/by-nc-sa/3.0/
//
// render interface for libRocket
#pragma once

#include <Rocket/Core/RenderInterface.h>

class GUIRenderInterface : public Rocket::Core::RenderInterface
{
public:

    GUIRenderInterface();

    /// Called by Rocket when it wants to render geometry that it does not wish to optimise.
    void RenderGeometry(Rocket::Core::Vertex* vertices, int numVertices,
        int* indices, int numIndices, Rocket::Core::TextureHandle texture,
        const Rocket::Core::Vector2f& translation) override;

    /// Called by Rocket when it wants to compile geometry it believes will be static for the forseeable future.
    Rocket::Core::CompiledGeometryHandle CompileGeometry(Rocket::Core::Vertex* vertices, int numVertices,
        int* indices, int numIndices, Rocket::Core::TextureHandle texture) override;

    /// Called by Rocket when it wants to render application-compiled geometry.
    void RenderCompiledGeometry(Rocket::Core::CompiledGeometryHandle geometry,
        const Rocket::Core::Vector2f& translation) override;

    /// Called by Rocket when it wants to release application-compiled geometry.
    void ReleaseCompiledGeometry(Rocket::Core::CompiledGeometryHandle geometry) override;

    /// Called by Rocket when it wants to enable or disable scissoring to clip content.
    void EnableScissorRegion(bool enable) override;
    /// Called by Rocket when it wants to change the scissor region.
    void SetScissorRegion(int x, int y, int width, int height) override;

    /// Called by Rocket when a texture is required by the library.
    bool LoadTexture(Rocket::Core::TextureHandle& outTextureHandle,
        Rocket::Core::Vector2i& outTextureDimensions, const Rocket::Core::String& resourcePath) override;
    /// Called by Rocket when a texture is required to be built from an internally-generated sequence of pixels.
    bool GenerateTexture(Rocket::Core::TextureHandle& outTextureHandle, const Rocket::Core::byte* data,
        const Rocket::Core::Vector2i& sourceDimensions) override;
    /// Called by Rocket when a loaded texture is no longer required.
    void ReleaseTexture(Rocket::Core::TextureHandle textureHandle) override;

private:
    GLuint m_fragmentTextureShaderId;
    GLuint m_fragmentColorShaderId;
    GLuint m_vertexShaderId;

    GLuint m_textureProgramId;
    GLuint m_colorProgramId;

    // shader parameter uniform locations
    GLuint m_ulTexture;
    GLuint m_ulTextureProjection;
    GLuint m_ulTextureTranslation;
    GLuint m_ulColorProjection;
    GLuint m_ulColorTranslation;
};