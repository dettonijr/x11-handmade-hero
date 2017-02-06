#include <cstdint>
#include <random>

struct Color {
    uint8_t r, g, b;

    Color(uint8_t r0, uint8_t g0, uint8_t b0) : r(r0), g(g0), b(b0) {
    }

    ~Color() { 
    }

    static Color random() {
        return Color(std::rand(), std::rand(), std::rand()); 
    }

    static Color White;
    static Color Black;
    static Color Red;
    static Color Green;
    static Color Blue;
};
