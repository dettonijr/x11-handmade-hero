#include "Obj.h"

class Renderer {
public:
    Renderer();
    ~Renderer();

    void add_obj(Obj& o);
    void set_light(Point<float> light_vec);
    void render(Framebuffer& f, const Transform& t);

private:
    Point<float> light_vec;
    std::vector<Obj> objs; 
};
