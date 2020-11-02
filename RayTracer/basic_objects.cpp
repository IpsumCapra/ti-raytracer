#include "basic_objects.h"
#include <iostream>
#include <cmath>

using namespace raytracer;

//OBJECT
Object::Object(Transform transform, Material material) : transform(Vector3(0),Vector3(0)), material(Vector3(0),Vector3(0),0,0){
    Object::transform = transform;
    Object::material = material;
}

//SPHERE
Sphere::Sphere(Transform transform, float radius, Material material) : Object(transform, material) {
    Sphere::radius = radius;
    Sphere::radius2 = radius * radius;
}

bool Sphere::intersect(const Vector3 &rayOrigin, const Vector3 &rayDirection, float &t0, float &t1) const {
    Vector3 l = transform.position - rayOrigin;

    float tca = l.dot(rayDirection);
    if (tca < 0) return false;

    float d2 = l.dot(l) - tca * tca;
    if (d2 > radius2) return false;

    float thc = std::sqrt((radius2) - d2);
    t0 = tca - thc;
    t1 = tca + thc;

    return true;
}