#include <cstdint>

struct Color {
    uint8_t r, g, b;

    Color(uint8_t r0, float g0, float b0) : r(r0), g(g0), b(b0) {
    }

    ~Color() { 
    }

    static Color White;
    static Color Black;
    static Color Red;
    static Color Green;
    static Color Blue;
};
