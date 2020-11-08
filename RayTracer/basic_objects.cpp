#include "basic_objects.h"
#include <iostream>
#include <cmath>

using namespace raytracer;

//OBJECT
Object::Object(Transform transform, Material material) : transform(transform), material(material){
    //Object::transform = transform;
    //Object::material = material;
}

//SPHERE
Sphere::Sphere(Transform transform, float radius, Material material) : Object(transform, material), radius(radius), radius2(radius*radius) {
    //Sphere::radius = radius;
    //Sphere::radius2 = radius * radius;
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
    //Polygon::p0 = p0;
    //Polygon::p1 = p1;
    //Polygon::p2 = p2;
}

bool Polygon::intersect(const Vector3 &rayOrigin, const Vector3 &rayDirection, float &t0, float &t1)
{
    if (false) {
        //printf("Test0\n");
        std::cout << "Test0\n";
    }

    Vector3 v0v1 = p1 - p0;
    Vector3 v0v2 = p2 - p0;
    Vector3 pvec = rayDirection.cross(v0v2);
    float det = v0v1.dot(pvec);

    // ray and triangle are parallel if det is close to 0
    const float kEpsilon = 1e-8;
    if (fabs(det) < kEpsilon) return false;
    //printf("Test1\n");

    float invDet = 1 / det;

    Vector3 tvec = rayOrigin - p0;
    float u = tvec.dot(pvec) * invDet;
    if (u < 0 || u > 1) return false;
    //printf("Test2\n");

    Vector3 qvec = tvec.cross(v0v1);
    float v = rayDirection.dot(qvec) * invDet;
    if (v < 0 || u + v > 1) return false;
    //printf("Test3\n");

    float t = v0v2.dot(qvec) * invDet;

    //t:tNear, u:triIndex, v:uv
    t0 = t;
    //t1 = ;

    return true;
}

/*bool Polygon::intersect(const Vector3 &rayOrigin, const Vector3 &rayDirection, float &t0, float &t1) {
    // p0, p1, p2

    uint32_t j = 0;
    bool isect = false;
    for (uint32_t i = 0; i < numTris; ++i) {
        //const Vec3f &v0 = P[trisIndex[j]];
        //const Vec3f &v1 = P[trisIndex[j + 1]];
        //const Vec3f &v2 = P[trisIndex[j + 2]];
        float t = kInfinity, u, v;
        if (rayTriangleIntersect(rayOrigin, rayDirection, p0, p1, p2) && t < tNear) {
            triIndex = i;
            isect |= true;
        }
        j += 3;
    }

    return isect;
}

bool Polygon::rayTriangleIntersect(
        const Vector3 &orig, const Vector3 &dir,
        const Vector3 &v0, const Vector3 &v1, const Vector3 &v2)
{
    Vector3 v0v1 = v1 - v0;
    Vector3 v0v2 = v2 - v0;
    Vector3 pvec = dir.cross(v0v2);
    float det = v0v1.dot(pvec);

    // ray and triangle are parallel if det is close to 0
    const float kEpsilon = 1e-8;
    if (fabs(det) < kEpsilon) return false;

    float invDet = 1 / det;

    Vector3 tvec = orig - v0;
    float u = tvec.dot(pvec) * invDet;
    if (u < 0 || u > 1) return false;

    Vector3 qvec = tvec.cross(v0v1);
    float v = dir.dot(qvec) * invDet;
    if (v < 0 || u + v > 1) return false;

    float t = v0v2.dot(qvec) * invDet;

    return true;
}*/