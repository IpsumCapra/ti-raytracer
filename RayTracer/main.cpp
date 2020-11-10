#include <vector>
#include <iostream>
#include "types.h"
#include "basic_objects.h"
#include "time.h"

namespace ray = raytracer;

int main(int argc, char** argv) {
    std::vector<ray::Object*> objects;
    std::vector<ray::Light*> lights;


    ray::Transform transform (ray::Vector3(0, 0, -18), ray::Vector3(0));
    ray::Material material(ray::Vector3(.2, .1, 1), ray::Vector3(0), 1, 1);
    ray::Sphere sphere (transform, 2.0f, material);

    ray::Sphere sphere1 (transform, 30, material);

    sphere1.transform.position = ray::Vector3(0, -3, -80);
    sphere1.material.color = ray::Vector3(.2);
    sphere1.material.emissionColor = ray::Vector3(.4);

    ray::Sphere sphere2(transform, 2, material);

    sphere2.material.color = ray::Vector3(.4, .4, .4);
    sphere2.material.emissionColor = ray::Vector3(3);
    sphere2.material.transparency = 0;
    sphere2.transform.position = ray::Vector3(0, 0, -18);

    ray::Sphere sphere3(transform, 3, material);

    sphere3.material.color = ray::Vector3(.7, .4, .4);
    sphere3.material.emissionColor = ray::Vector3(1);
    sphere3.material.transparency = 1;
    sphere3.transform.position = ray::Vector3(0, -2, -18);

    objects.push_back(&sphere);
    objects.push_back(&sphere1);
    //objects.push_back(&sphere2);
    //objects.push_back(&sphere3);


    // POLYGON
    auto p0 = ray::Vector3(0, -2, 0);
    auto p1 = ray::Vector3(-3, -2, 0);
    auto p2 = ray::Vector3(-3, 5, 0);
    ray::Polygon polygon1(transform, p0, p1, p2, material);
    polygon1.transform.position = ray::Vector3(0, 0, 18);
    polygon1.material.color = ray::Vector3(.6, 1, 0);

    auto p3 = ray::Vector3(0, -2, 0);
    auto p4 = ray::Vector3(0, 5, 0);
    auto p5 = ray::Vector3(-3, 5, 0);
    ray::Polygon polygon2(transform, p3, p4, p5, material);
    polygon2.transform.position = ray::Vector3(0, 0, 18);
    polygon2.material.color = ray::Vector3(.7, .7, 0);

    //objects.push_back(&polygon1);
    //objects.push_back(&polygon2);


    // CUBE
    ray::Cube cube1(transform, 3, material);
    cube1.transform.position = ray::Vector3(2, 0, 18);
    cube1.material.color = ray::Vector3(0, .4, 0);
    cube1.material.emissionColor = ray::Vector3(.8,.1,.8);
    cube1.material.transparency = 0;

    ray::Cube cube2(transform, 4, material);
    cube2.transform.position = ray::Vector3(-4, 0, 18);
    cube2.material.color = ray::Vector3(.7, .4, .4);
    //cube2.material.albedo = 0.5;
    
    objects.push_back(&cube1);
    objects.push_back(&cube2);


    //LIGHT
    ray::Light light1(transform, 1);
    light1.transform.position = ray::Vector3(4, 3, -15);

    lights.push_back(&light1);


    ray::ImageToFile imgToFile("Test.ppm", "ppm");

    ray::RayTracer rayTracer(imgToFile, 640, 480);

    rayTracer.render(objects, lights);
}