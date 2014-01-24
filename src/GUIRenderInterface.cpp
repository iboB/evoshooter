//
// gd-proto
// Copyright (c) 2014 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
// render interface for libRocket
#include "GDProto.pch.h"
#include "GUIRenderInterface.h"

#include "Application.h"
#include "Texture.h"

#include "GUIAttribs.h"

#include <Rocket/Core/Context.h>

using namespace std;
using namespace mathgp;

static const char* FragmentTextureShaderSource =
"\
    precision mediump float; \
    varying vec2 v_texCoord; \
    varying vec4 v_color; \
    uniform sampler2D u_texture; \
    \
    void main() \
    { \
        gl_FragColor = texture2D( u_texture, v_texCoord ) * v_color; \
    } \
";

static const char* FragmentColorShaderSource =
"\
    precision mediump float; \
    varying vec2 v_texCoord; \
    varying vec4 v_color; \
    \
    void main() \
    { \
        gl_FragColor = v_color; \
    } \
";

static const char* VertexShaderSource =
"\
    attribute vec2 a_position; \
    attribute vec2 a_texCoord; \
    attribute vec4 a_color; \
    varying vec2 v_texCoord; \
    varying vec4 v_color; \
    varying vec2 v_position; \
    uniform mat4 u_projection; \
    uniform vec2 u_translation; \
    \
    void main() \
    { \
        v_position = a_position + u_translation; \
        gl_Position = u_projection * vec4(v_position, 0.0, 1.0); \
        v_texCoord = a_texCoord; \
        v_color = a_color; \
    }\
";


static GLuint CreateSimpleShader(GLenum type, const char* source)
{
    const char* glSources[] = { source };
    const GLint glLengths[] = { GLint(strlen(source)) };

    GLuint shaderId = glCreateShader(type);
    glShaderSource(shaderId, 1, glSources, glLengths);
    glCompileShader(shaderId);

    GLint compilationSuccess = GL_FALSE;
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &compilationSuccess);

    GLenum glError = glGetError();
    if (glError != GL_NO_ERROR || !compilationSuccess)
    {
        GLint infoLen = 0;
        glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &infoLen);

        if (infoLen)
        {
            char* infoLog = new char[infoLen];
            glGetShaderInfoLog(shaderId, infoLen, nullptr, infoLog);

            assert(false);
            cout << "error compiling shader:\n"
                << infoLog << endl;

            delete[] infoLog;
        }
        else
        {
            assert(false);
            cout << "unknown error compiling shader" << endl;
        }

        return 0;
    }

    return shaderId;
}

static GLuint CreateProgram(GLuint vertexShader, GLuint fragmentShader)
{
    GLuint programId = glCreateProgram();
    glAttachShader(programId, vertexShader);
    glAttachShader(programId, fragmentShader);

    // bind custom attribs
    glBindAttribLocation(programId, Attrib_Position, "a_position");
    glBindAttribLocation(programId, Attrib_TexCoord, "a_texCoord");
    glBindAttribLocation(programId, Attrib_Color, "a_color");

    glLinkProgram(programId);

    GLint linkSuccess;
    glGetProgramiv(programId, GL_LINK_STATUS, &linkSuccess);

    GLenum glError = glGetError();
    if (glError != GL_NO_ERROR || !linkSuccess)
    {
        GLint infoLen = 0;
        glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &infoLen);

        if (infoLen)
        {
            char* infoLog = new char[infoLen];
            glGetProgramInfoLog(programId, infoLen, nullptr, infoLog);

            assert(false);
            cout << "error linking program:\n" << infoLog << endl;

            delete[] infoLog;
        }
        else
        {
            assert(false);
            cout << "unknown error linking program" << endl;
        }

        return 0;
    }

    return programId;
}

GUIRenderInterface::GUIRenderInterface()
{
    glGetError(); // clear error state so as to not confuse opengl about the compilation errors

    // create shaders
    m_fragmentTextureShaderId = CreateSimpleShader(GL_FRAGMENT_SHADER, FragmentTextureShaderSource);
    m_fragmentColorShaderId = CreateSimpleShader(GL_FRAGMENT_SHADER, FragmentColorShaderSource);
    m_vertexShaderId = CreateSimpleShader(GL_VERTEX_SHADER, VertexShaderSource);

    // Create Programs
    m_textureProgramId = CreateProgram(m_vertexShaderId, m_fragmentTextureShaderId);
    m_ulTexture = glGetUniformLocation(m_textureProgramId, "u_texture");
    m_ulTextureProjection = glGetUniformLocation(m_textureProgramId, "u_projection");
    m_ulTextureTranslation = glGetUniformLocation(m_textureProgramId, "u_translation");

    m_colorProgramId = CreateProgram(m_vertexShaderId, m_fragmentColorShaderId);
    m_ulColorProjection = glGetUniformLocation(m_colorProgramId, "u_projection");
    m_ulColorTranslation = glGetUniformLocation(m_colorProgramId, "u_translation");
}


void GUIRenderInterface::RenderGeometry(Rocket::Core::Vertex* vertices, int numVertices,
    int* indices, int numIndices, Rocket::Core::TextureHandle textureHandle,
    const Rocket::Core::Vector2f& translation)
{
    auto size = GetContext()->GetDimensions();

    // custom ortho 2d matrix
    const matrix projection = matrix::rows(
        2.f/size.x, 0,          0,     -1,
        0,          -2.f/size.y,0,      1,
        0,          0,          0,      0,
        0,          0,          0,      1
    );

    if (textureHandle)
    {
        glUseProgram(m_textureProgramId);
        glUniformMatrix4fv(m_ulTextureProjection, 1, GL_FALSE, projection.as_array());
        glUniform2f(m_ulTextureTranslation, translation.x, translation.y);
    }
    else
    {
        glUseProgram(m_colorProgramId);
        glUniformMatrix4fv(m_ulColorProjection, 1, GL_FALSE, projection.as_array());
        glUniform2f(m_ulColorTranslation, translation.x, translation.y);
    }

    glVertexAttribPointer(Attrib_Position, 2, GL_FLOAT, GL_FALSE, sizeof(Rocket::Core::Vertex), &vertices->position);    
    glVertexAttribPointer(Attrib_Color, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Rocket::Core::Vertex), &vertices->colour);

    if (textureHandle)
    {
        auto tex = reinterpret_cast<Texture*>(textureHandle);
        glBindTexture(GL_TEXTURE_2D, tex->glHandle());
        glUniform1i(m_ulTexture, 0);
        glVertexAttribPointer(Attrib_TexCoord, 2, GL_FLOAT, GL_FALSE, sizeof(Rocket::Core::Vertex), &vertices->tex_coord);
    }

    glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, indices);
}

Rocket::Core::CompiledGeometryHandle GUIRenderInterface::CompileGeometry(Rocket::Core::Vertex*, int, int*, int, Rocket::Core::TextureHandle)
{
    // no obvious need to support this yet
    return 0;
}
void GUIRenderInterface::RenderCompiledGeometry(Rocket::Core::CompiledGeometryHandle, const Rocket::Core::Vector2f&) {}
void GUIRenderInterface::ReleaseCompiledGeometry(Rocket::Core::CompiledGeometryHandle) {}

void GUIRenderInterface::EnableScissorRegion(bool enable)
{
    if (enable)
    {
        glEnable(GL_SCISSOR_TEST);
    }
    else
    {
        glDisable(GL_SCISSOR_TEST);
    }
}

void GUIRenderInterface::SetScissorRegion(int x, int y, int width, int height)
{
    glScissor(x, -y, width, height);
}

bool GUIRenderInterface::LoadTexture(Rocket::Core::TextureHandle& outTextureHandle,
    Rocket::Core::Vector2i& outTextureDimensions, const Rocket::Core::String& resourcePath)
{
    auto tex = new Texture;

    tex->loadFromFile(resourcePath.CString());

    return !!tex;
}

bool GUIRenderInterface::GenerateTexture(Rocket::Core::TextureHandle& outTextureHandle, const Rocket::Core::byte* data,
    const Rocket::Core::Vector2i& sourceDimensions)
{
    auto tex = new Texture;
    tex->loadFromData(GL_RGBA, sourceDimensions.x, sourceDimensions.y, GL_RGBA, GL_UNSIGNED_BYTE, data);

    outTextureHandle = reinterpret_cast<Rocket::Core::TextureHandle>(tex);

    return !!tex;
}

void GUIRenderInterface::ReleaseTexture(Rocket::Core::TextureHandle textureHandle)
{
    auto tex = reinterpret_cast<Texture*>(textureHandle);
    delete tex;
}
