#include <engine/gl/Texture.hpp>

#include <stb_image.h>
#include <OpenGL/gl3.h>
#include <utils/Logger.hpp>

Texture Texture::CreateTexture(const char* path)
{
    GLuint id;
    GLint width, height, noc;

    GLubyte* data = stbi_load(path, &width, &height, &noc, 0);
    GLenum format = (noc == 3) ? GL_RGB : (noc == 4) ? GL_RGBA : GL_RED;

    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);

    LOG("Texture created:", path);

    return { .id = id, .path = path };
}

void Texture::DestroyTexture(const Texture& texture)
{
    glDeleteTextures(1, &texture.id);
    LOG("Texture destroyed:", texture.path);
}