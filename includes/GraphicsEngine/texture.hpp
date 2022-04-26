#pragma once
#include <stb/stb_image.h>

namespace GE
{
    /**
     * @brief Represent a texture, abstracting and dealing with texture loading and configuration
     *
     */
    class Texture
    {
    public:
        Texture(const char *imgPath, int format);
        void bind();
        void bind(int slot);
        void setWrap(int s, int t);
        void setWrapS(int v);
        void setWrapT(int v);
        void setFilter(int mag, int min);
        void setFilterMag(int v);
        void setFilterMin(int v);
        unsigned int getID();
        int getWidth();
        int getHeight();
        int getNrChannels();

    private:
        unsigned int ID;
        int width, height, nrChannels;
    };
}