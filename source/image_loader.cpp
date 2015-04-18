#include "image_loader.h"

#include <fstream>
#include <iostream>

using namespace std;

unsigned char* loadBMP(const char* fname,int *width, int *height)
{
    ifstream f(fname, ios::binary);

    if(f.good())
    {
        char* info = new char[54];
        f.read(info, 54);
        *width = *(int*)&info[18];
        *height = *(int*)&info[22];
        int pixelsize = *(short*)&info[28];
        int offset = *(int*)&info[10];

        int pixelbytes = pixelsize /8;
        int rowsize = ((pixelsize*(*width) + 31)/32)*4;
        char *pixels = new char[pixelbytes*(*width)*(*height)];
        char *rowtemp = new char[rowsize];

        char *pixeliter = pixels;
        char *rowtempiter;

        if(pixelbytes==9)
        {
            for(int i=0;i<3*(*width)*(*height);i++)
                pixels[i] = 126;
            return (unsigned char*)pixels;
        }


        f.seekg(offset, ios::beg);

        for(int i=0;i<(*height);i++)
        {
            f.read(rowtemp, rowsize);
            rowtempiter = rowtemp;
            if(pixelbytes==3)
            {
                for(int j=0;j<(*width)*3; j+=3)
                {
                    pixeliter[0] = rowtempiter[2];
                    pixeliter[1] = rowtempiter[1];
                    pixeliter[2] = rowtempiter[0];
                    pixeliter += 3;
                    rowtempiter += 3;
                }

            }
            else
            {
                for(int j=0;j<(*width)*pixelbytes; j+=pixelbytes)
                {
                    pixeliter[0] = rowtempiter[3];
                    pixeliter[1] = rowtempiter[2];
                    pixeliter[2] = rowtempiter[1];
                    pixeliter[3] = rowtempiter[0];

                    pixeliter += pixelbytes;
                    rowtempiter += pixelbytes;
                }
            }
        }

        return (unsigned char*)pixels;
    }
    else
        return NULL;
}

void unload_BMP(unsigned char* pixels)
{
    delete[] pixels;
}

