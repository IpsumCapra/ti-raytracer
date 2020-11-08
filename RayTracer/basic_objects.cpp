#include "basic_objects.h"
#include <iostream>
#include <cmath>

using namespace raytracer;

//OBJECT
Object::Object(Transform transform, Material material) : transform(transform), material(material){
}

//SPHERE
Sphere::Sphere(Transform transform, float radius, Material material) : Object(transform, material), radius(radius), radius2(radius*radius) {
}

bool Sphere::intersect(const Vector3 &rayOrigin, const Vector3 &rayDirection, float &t0, float &t1) {
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

//POLYGON
Polygon::Polygon(Transform transform, Vector3 p0, Vector3 p1, Vector3 p2, Material material) : Object(transform, material), p0(p0), p1(p1), p2(p2) {
}

bool Polygon::intersect(const Vector3 &rayOrigin, const Vector3 &rayDirection, float &t0, float &t1)
{
    Vector3 l = transform.position - rayOrigin;

    Vector3 v0v1 = p1 - p0;
    Vector3 v0v2 = p2 - p0;
    Vector3 pvec = rayDirection.cross(v0v2);
    float det = v0v1.dot(pvec);

    // ray and triangle are parallel if det is close to 0
    const float kEpsilon = 1e-8;
    if (fabs(det) < kEpsilon) return false;
    //printf("Test1\n");

    float invDet = 1 / det;

    Vector3 tvec = l - p0;
    float u = tvec.dot(pvec) * invDet;
    if (u < 0 || u > 1) return false;

    Vector3 qvec = tvec.cross(v0v1);
    float v = rayDirection.dot(qvec) * invDet;
    if (v < 0 || u + v > 1) return false;

    float t = v0v2.dot(qvec) * invDet;

    //t:tNear, u:triIndex, v:uv
    t0 = t;
    //t1 = ; //?

    return true;
}