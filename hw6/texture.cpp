// texture.cpp - procedural texture generation
#include "texture.hpp"
#include <vector>
#include <cstdlib>
#include <cmath>

static GLuint makeTextureRGBA(int w,int h,const std::vector<unsigned char>& rgba)
{
    GLuint id=0;
    glGenTextures(1,&id);
    glBindTexture(GL_TEXTURE_2D,id);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,w,h,0,GL_RGBA,GL_UNSIGNED_BYTE,rgba.data());
    
    // Modern OpenGL mipmap generation (GLU-free)
    glGenerateMipmap(GL_TEXTURE_2D);
    return id;
}

// Generate black/white checker pattern
GLuint makeCheckerTexture(int size, int check)
{
    std::vector<unsigned char> img(size*size*4);
    for(int y=0;y<size;++y)
    for(int x=0;x<size;++x){
        int c = (((x/check)+(y/check))&1)? 230 : 40;
        img[4*(y*size+x)+0]= (unsigned char)c;
        img[4*(y*size+x)+1]= (unsigned char)c;
        img[4*(y*size+x)+2]= (unsigned char)c;
        img[4*(y*size+x)+3]= 255;
    }
    return makeTextureRGBA(size,size,img);
}

// Generate colored stripe pattern
GLuint makeStripeTexture(int size, int period)
{
    std::vector<unsigned char> img(size*size*4);
    for(int y=0;y<size;++y)
    for(int x=0;x<size;++x){
        int s = ((x/period)&1)? 220 : 80;
        int t = ((y/period)&1)? 220 : 80;
        img[4*(y*size+x)+0]= (unsigned char)((s*0.7 + t*0.3));
        img[4*(y*size+x)+1]= (unsigned char)((s*0.5 + t*0.5));
        img[4*(y*size+x)+2]= (unsigned char)((s*0.2 + t*0.8));
        img[4*(y*size+x)+3]= 255;
    }
    return makeTextureRGBA(size,size,img);
}
