#include "Renderer.h"


Renderer::Renderer() {

}

Renderer::~Renderer() {
}

void Renderer::add_obj(Obj& o) {
    objs.push_back(o);
}

void Renderer::set_light(Point<float> light_vec) {
    this->light_vec = light_vec;
}

void Renderer::render(Framebuffer& f, const Transform& t) {
    int width = f.width();
    int height = f.height();

    for (auto obj = objs.begin(); obj != objs.end(); ++obj) {
        auto& faces = obj->get_faces();
        for (int i = 0; i < faces.size(); i++) {
            auto face = faces[i];
            Point<float>& v0 = face.v[0];
            Point<float>& v1 = face.v[1];
            Point<float>& v2 = face.v[2];
            Point<float>& t0 = face.vt[0];
            Point<float>& t1 = face.vt[1];
            Point<float>& t2 = face.vt[2];
            
            Point<float> p0 = v0*t;
            Point<float> p1 = v1*t;
            Point<float> p2 = v2*t;

            Point<float> a = p2-p0; 
            Point<float> b = p1-p0;

            float nx = a.y*b.z - a.z*b.y; 
            float ny = a.z*b.x - a.x*b.z; 
            float nz = a.x*b.y - a.y*b.x; 

            const Point<float> n(nx,ny,nz);
            const Point<float> normalized = n.normalize();
            float intensity = normalized*light_vec;

            if (intensity > 0) {
                TextureShader s{p0, p1, p2, t0, t1, t2, intensity, face.texture};
                //FlatShader s{p0, p1, p2, Color::White*intensity};
                f.draw_triangle(p0, p1, p2, s); 
            }

        }
    }
}
