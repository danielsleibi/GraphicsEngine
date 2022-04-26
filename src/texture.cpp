#include <GraphicsEngine/texture.hpp>
#include <glad/glad.h>
#include <iostream>

GE::Texture::Texture(const char *imgPath, int format)
{
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // set the texture wrapping/filtering options (on currently bound texture)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    // load and generate the texture
    stbi_set_flip_vertically_on_load(true);
    int width, height, nrChannels;
    unsigned char *data = stbi_load(imgPath, &width, &height,
                                    &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, format,
                     GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
    ID = texture;
}
void GE::Texture::bind() { glBindTexture(GL_TEXTURE_2D, ID); }
void GE::Texture::bind(int slot)
{
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, ID);
}

void GE::Texture::setWrap(int s, int t)
{
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, s);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, t);
}
void GE::Texture::setWrapS(int v) { glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, v); }
void GE::Texture::setWrapT(int v) { glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, v); }
void GE::Texture::setFilter(int mag, int min)
{
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag);
}
void GE::Texture::setFilterMag(int v) { glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, v); }
void GE::Texture::setFilterMin(int v) { glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, v); }
unsigned int GE::Texture::getID() { return ID; }
int GE::Texture::getWidth() { return width; }
int GE::Texture::getHeight() { return height; }
int GE::Texture::getNrChannels() { return nrChannels; }