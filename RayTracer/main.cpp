#include <vector>
#include <iostream>
#include "types.h"
#include "basic_objects.h"
#include "time.h"
#include <GL/glut.h>

namespace ray = raytracer;

int main(int argc, char** argv) {
    //init glut in case user wants a render.
    std::vector<ray::Sphere> objects;

    ray::Transform transform (ray::Vector3(5, 0, -16), ray::Vector3(0));
    ray::Material material(ray::Vector3(.2, .1, 1), ray::Vector3(.2), 1, 0);
    ray::Sphere sphere (transform, 2.0f, material);
    ray::Sphere sphere1 (transform, 20, material);
    sphere1.transform.position = ray::Vector3(2, -3, -20);
    sphere1.material.color = ray::Vector3(.1);
    sphere1.material.emissionColor = ray::Vector3(.2);

    ray::Sphere bg (transform, 2, material);
    bg.material.color = ray::Vector3(.4, .4, .4);
    bg.material.emissionColor = ray::Vector3(3);
    bg.material.transparency = 2;
    bg.transform.position = ray::Vector3(0,0,-10);
    objects.push_back(bg);
    objects.push_back(sphere);
    objects.push_back(sphere1);
    sphere.material.color = ray::Vector3(1);
    sphere.material.emissionColor = ray::Vector3(255, 255, 1);
    sphere.material.transparency = 1;
    sphere.transform.position = ray::Vector3(-1, -3,-18);
    objects.push_back(sphere);

    ray::ImageToFile imgToFile("Test", "ppm");

    ray::RayTracer rayTracer(imgToFile);

    rayTracer.render(objects);
}