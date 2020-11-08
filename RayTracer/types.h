#include <vector>
#include <cstddef>
#include <string>

#ifndef TYPES_H
#define TYPES_H

namespace raytracer {
    class Object;
    class Sphere;

    class Vector3 {
    public:
        Vector3(float x, float y, float z);
        Vector3(float v);

        float magnitude() const;
        void normalize();
        float dot(const Vector3&) const;

        float x;
        float y;
        float z;

        //Custom operators
        Vector3 operator+(const Vector3 &vector) const { return Vector3(x + vector.x, y + vector.y, z + vector.z); }

        Vector3 operator-(const Vector3 &vector) const { return Vector3(x - vector.x, y - vector.y, z - vector.z); }

        Vector3 operator-() const { return Vector3(-x, -y, -z); }

        Vector3 operator*(const float &factor) const { return Vector3(x * factor, y * factor, z * factor); }

        Vector3 operator*(const Vector3 &vector) const { return Vector3(x * vector.x, y * vector.y, z * vector.z); }

        Vector3 operator/(const float &factor) const { return Vector3(x / factor, y / factor, z / factor); }

        Vector3 operator/(const Vector3 &vector) const { return Vector3(x / vector.x, y / vector.y, z / vector.z); }

        Vector3 operator+=(const Vector3 &vector) {
            x += vector.x, y += vector.y, z += vector.z;
            return *this;
        }

        Vector3 operator-=(const Vector3 &vector) {
            x -= vector.x, y -= vector.y, z -= vector.z;
            return *this;
        }

        Vector3 operator*=(const Vector3 &vector) {
            x *= vector.x, y *= vector.y, z *= vector.z;
            return *this;
        }

        Vector3 operator/=(const Vector3 &vector) {
            x /= vector.x, y /= vector.y, z /= vector.z;
            return *this;
        }

    };

    class Transform {
    public:
        Transform(Vector3 position, Vector3 rotation);

        Vector3 position;
        Vector3 rotation;
    };

    class Material {
    public:
        Material(Vector3 color, Vector3 emissionColor, float transparency, float albedo);

        Vector3 color;
        Vector3 emissionColor;
        float transparency;
        float albedo;
    };

    class Image {
    public:
        Image(unsigned int width, unsigned int height);

        bool setPixel(int x, int y);

        bool valid();

        std::vector<Vector3> image;
        //kan niet negatief zijn.
        unsigned int width;
        unsigned int height;
    };

    class ImageOutput {
    public:
        ImageOutput(Image image, std::string name);
        virtual void render() {};

        Image image;
        std::string name;
    };

    class ImageToOpenGL : public ImageOutput {
    public:
        ImageToOpenGL(Image image, std::string name);

        void render();

    private:
        void display();
    };

    class ImageToFile : public ImageOutput {
    public:
        ImageToFile(Image image, std::string name, std::string type);
        ImageToFile(std::string name, std::string type);
        std::string type;

        void render();
    };

    class RayTracer {
    public:
        RayTracer(ImageOutput &imageOutput, unsigned int width, unsigned int height);

        ImageOutput imageOutput;
        Image image;
        unsigned int width;
        unsigned int height;
        Vector3 trace(const Vector3 &rayOrigin, const Vector3 &rayDirection, std::vector<Object*> &objects, const int &depth);
        float mix(const float &a, const float &b, const float &mix);
        void render(std::vector<Object*> &objects);
    };
}

#endif TYPES_H