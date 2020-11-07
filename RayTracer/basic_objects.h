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

    class Cube : public Object {

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

    class Polygon: public Object {

        bool intersect(const Vector3 &rayOrigin,const Vector3 &rayDirection, float &t0, float &t1) override;
    };
}

#endif BASIC_OBJECTS_H