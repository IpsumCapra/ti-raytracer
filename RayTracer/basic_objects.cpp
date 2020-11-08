#include "basic_objects.h"
#include <iostream>
#include <cmath>

using namespace raytracer;

//OBJECT
Object::Object(Transform transform, Material material) : transform(transform), material(material){
}

//CUBE
Cube::Cube(Transform transform, float width, Material material) : Object(transform, material), width(width) {
    auto p0 = Vector3(-.5*width, -.5*width, -.5*width);
    auto p1 = Vector3(.5*width, -.5*width, -.5*width);
    auto p2 = Vector3(.5*width, -.5*width, .5*width);
    auto p3 = Vector3(-.5*width, -.5*width, .5*width);
    auto p4 = Vector3(-.5*width, .5*width, -.5*width);
    auto p5 = Vector3(.5*width, .5*width, -.5*width);
    auto p6 = Vector3(.5*width, .5*width, .5*width);
    auto p7 = Vector3(-.5*width, .5*width, .5*width);
    Polygon polygon0(transform, p0, p1, p5, material);
    Polygon polygon1(transform, p0, p5, p4, material);
    Polygon polygon2(transform, p1, p2, p6, material);
    Polygon polygon3(transform, p1, p6, p5, material);
    Polygon polygon4(transform, p2, p6, p7, material);
    Polygon polygon5(transform, p2, p7, p3, material);
    Polygon polygon6(transform, p3, p0, p4, material);
    Polygon polygon7(transform, p3, p4, p7, material);
    Polygon polygon8(transform, p1, p0, p3, material);
    Polygon polygon9(transform, p1, p3, p2, material);
    Polygon polygon10(transform, p4, p5, p6, material);
    Polygon polygon11(transform, p4, p6, p7, material);
    polygon0.transform.position = transform.position;
    polygon1.transform.position = transform.position;
    polygon2.transform.position = transform.position;
    polygon3.transform.position = transform.position;
    polygon4.transform.position = transform.position;
    polygon5.transform.position = transform.position;
    polygon6.transform.position = transform.position;
    polygon7.transform.position = transform.position;
    polygon8.transform.position = transform.position;
    polygon9.transform.position = transform.position;
    polygon10.transform.position = transform.position;
    polygon11.transform.position = transform.position;
    //polygon11.transform.position = Vector3(0, 0, 18);
    polygons.push_back(polygon0);
    polygons.push_back(polygon1);
    polygons.push_back(polygon2);
    polygons.push_back(polygon3);
    polygons.push_back(polygon4);
    polygons.push_back(polygon5);
    polygons.push_back(polygon6);
    polygons.push_back(polygon7);
    polygons.push_back(polygon8);
    polygons.push_back(polygon9);
    polygons.push_back(polygon10);
    polygons.push_back(polygon11);
}

bool Cube::intersect(const Vector3 &rayOrigin, const Vector3 &rayDirection, float &t0, float &t1) {
    float t0temp = INFINITY, t1temp = INFINITY;
    /*
    for (Polygon polygon: polygons) {
        polygon.intersect(rayOrigin, rayDirection, t0, t1);
    }
     */

    float tNear = INFINITY;

    Polygon* object = NULL;

    for (Polygon polygon: polygons) {
        if (polygon.intersect(rayOrigin, rayDirection, t0temp, t1temp)) {
            if (t0temp < 0) t0temp = t1temp;
            if (t0temp < tNear) {
                tNear = t0temp;
                object = &polygon;
            }
        }
    }

    if(object) {
        t0 = t0temp;
        t1 = t1temp;
        return true;
    }
    return false;
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