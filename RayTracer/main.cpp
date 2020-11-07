#include <vector>
#include <iostream>
#include "types.h"
#include "basic_objects.h"
#include "time.h"

namespace ray = raytracer;

int main(int argc, char** argv) {
    std::vector<ray::Object*> objects;

    ray::Transform transform (ray::Vector3(5, 0, -18), ray::Vector3(0));
    ray::Material material(ray::Vector3(.2, .1, 1), ray::Vector3(0), 1, 1);
    ray::Sphere sphere (transform, 2.0f, material);

    ray::Sphere sphere1 (transform, 20, material);

    sphere1.transform.position = ray::Vector3(2, -3, -20);
    sphere1.material.color = ray::Vector3(.2);
    sphere1.material.emissionColor = ray::Vector3(0);

    ray::Sphere sphere2(transform, 2, material);

    sphere2.material.color = ray::Vector3(.4, .4, .4);
    sphere2.material.emissionColor = ray::Vector3(3);
    sphere2.material.transparency = 0;
    sphere2.transform.position = ray::Vector3(0, 0, -10);

    ray::Sphere sphere3(transform, 3, material);

    sphere3.material.color = ray::Vector3(.7, .4, .4);
    sphere3.material.emissionColor = ray::Vector3(1);
    sphere3.material.transparency = 1;
    sphere3.transform.position = ray::Vector3(0, -2, -18);

    objects.push_back(&sphere);
    objects.push_back(&sphere1);
    objects.push_back(&sphere2);
    objects.push_back(&sphere3);

    ray::ImageToFile imgToFile("Test", "ppm");

    ray::RayTracer rayTracer(imgToFile);

    rayTracer.render(objects);
}