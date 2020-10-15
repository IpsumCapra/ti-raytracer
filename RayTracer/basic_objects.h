#include "types.h"
#ifndef BASIC_OBJECTS_H
#define BASIC_OBJECTS_H


namespace raytracer {
    class Object {
    public:
        Object(Transform transform, Material material);

        Transform transform;
        Material material;
    };

    class Cube : public Object {

    };

    class Sphere : public Object {

    };

    class Surface: public Object {
    };

    class Light: public Object {
    };
}

#endif BASIC_OBJECTS_H