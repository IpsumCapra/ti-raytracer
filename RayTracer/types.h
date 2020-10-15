#ifndef TYPES_H
#define TYPES_H

namespace raytracer {
    class Vector3 {
    public:
        Vector3(float x, float y, float z);

        float magnitude();

        float x;
        float y;
        float z;
    };

    class Color {
    public:
        Color(int r, int g, int b);

        Color(int hex);

        float r;
        float g;
        float b;
    private:
        Vector3 color;
    };

    class Transform {
    public:
        Transform(Vector3 Position, Vector3 Rotation);

        Vector3 position;
        Vector3 rotation;
    };

    class Material {
    public:
        Material(Color color, float transparency, float albedo);

        Color color;
        float transparency;
        float albedo;
    };
}

#endif TYPES_H