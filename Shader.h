#include "Color.h"
#include "Point.h"
#include "TGAFile.hpp"

#define I2F(x) ((float)x+0.5)
#define F2I(x) ((int)x)

float edgeFunction(const Point<float>& v0, const Point<float>& v1, const Point<float>& p);

struct FragmentShader {
    virtual ~FragmentShader() {};
    virtual Color fragment(int x, int y) = 0;
};

struct TextureShader : FragmentShader {
    Point<float>& v0;        
    Point<float>& v1;        
    Point<float>& v2;        
    Point<float>& t0;        
    Point<float>& t1;        
    Point<float>& t2;        
    float intensity;
    TGAFile& texture;     

    TextureShader(
        Point<float>& _v0,        
        Point<float>& _v1,        
        Point<float>& _v2,        
        Point<float>& _t0,        
        Point<float>& _t1,        
        Point<float>& _t2,        
        float _intensity,
        TGAFile& _texture) :
        v0(_v0), 
        v1(_v1),        
        v2(_v2),       
        t0(_t0),        
        t1(_t1),        
        t2(_t2),
        intensity(_intensity),
        texture(_texture) 
    {
    }

    virtual ~TextureShader() {

    }

    virtual Color fragment(int x, int y) {
        float w0 = edgeFunction(v1,v2,Point<float>(I2F(x),I2F(y),0));
        float w1 = edgeFunction(v2,v0,Point<float>(I2F(x),I2F(y),0));
        float w2 = edgeFunction(v0,v1,Point<float>(I2F(x),I2F(y),0));
        float area = edgeFunction(v0,v1,v2);
        w0 /= area;
        w1 /= area;
        w2 /= area;
         
        float tx = t0.x * w0 + t1.x*w1 + t2.x*w2;
        float ty = t0.y * w0 + t1.y*w1 + t2.y*w2;
 
        return texture.get_color(tx,ty)*intensity;
    }
};

struct FlatShader : FragmentShader {
    Color& c;     

    FlatShader(Color& c): c(c)
    {
    }

    virtual ~FlatShader() {

    }

    virtual Color fragment(int x, int y) {
        return c;
    }
};
