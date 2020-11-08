#include "types.h"
#include <iostream>
#ifndef BASIC_OBJECTS_H
#define BASIC_OBJECTS_H


namespace raytracer {
    class Object {
    public:
        Object(Transform transform, Material material);

        virtual bool intersect(const Vector3 &rayOrigin,const Vector3 &rayDirection, float &t0, float &t1) = 0;

        Transform transform;
        Material material;
    };

    class Polygon: public Object {
    public:
        Polygon(Transform transform, Vector3 p0, Vector3 p1, Vector3 p2, Material material);

        Vector3 p0, p1, p2;

        bool intersect(const Vector3 &rayOrigin,const Vector3 &rayDirection, float &t0, float &t1) override;
    };

    class Cube : public Object {
    public:
        Cube(Transform transform, float width, Material material);

        float width;
        std::vector<Polygon> polygons;

        bool intersect(const Vector3 &rayOrigin,const Vector3 &rayDirection, float &t0, float &t1) override;
    };

    class Sphere : public Object {
    public:
        Sphere(Transform transform, float radius, Material material);

        //radius and radius^2 of sphere.
        float radius, radius2;

        bool intersect(const Vector3 &rayOrigin,const Vector3 &rayDirection, float &t0, float &t1) override;
    };

    class Surface: public Object {
    };

    class Light: public Object {

    };
}

#endif BASIC_OBJECTS_H