#include "types.h"
#include "basic_objects.h"
#include <cmath>
#include <fstream>
#include <iostream>
#include <GL/glut.h>

#define MAX_RAY_DEPTH 5

using namespace raytracer;

//VECTOR 3
Vector3::Vector3(float x, float y, float z) {
    Vector3::x = x;
    Vector3::y = y;
    Vector3::z = z;
}

Vector3::Vector3(float v) {
    Vector3::x = v;
    Vector3::y = v;
    Vector3::z = v;
}

float Vector3::magnitude() const {
    return sqrt((pow(x, 2) + pow(y, 2) + pow(z, 2)));
}

float Vector3::dot(const Vector3 &vector) const {
    return x * vector.x + y * vector.y + z * vector.z;
}

void Vector3::normalize() {
    auto m = magnitude();
    if (m > 0) {
        x /= m;
        y /= m;
        z /= m;
    }
}

//IMAGE
Image::Image(unsigned int width, unsigned int height) {
    Image::width = width;
    Image::height = height;
}


bool Image::valid() {
    return width * height == image.size();
}

//IMAGE OUTPUT
ImageOutput::ImageOutput(Image image, std::string name) : image(1, 1) {
    ImageOutput::image = image;
    ImageOutput::name = name;
}

//IMAGE TO FILE
ImageToFile::ImageToFile(Image image, std::string name, std::string type) : ImageOutput(image, name) {
    ImageToFile::type = type;
}

ImageToFile::ImageToFile(std::string name, std::string type) : ImageOutput(Image(0, 0), name) {
    ImageToFile::type = type;
}

ImageToOpenGL::ImageToOpenGL(Image image, std::string name) : ImageOutput(image, name) {}


//EXPORT IMAGE
void ImageToFile::render() {
    //maybe add more types in the future...
    std::string supported = std::string("ppm");

    if (supported.compare(type) != 0) {
        std::cout <<"Unsupported file type, supported type is ppm." << std::endl;
        return;
    }

    if (image.valid()) {
        std::ofstream outputFile(name);

        //add header.
        outputFile << "P3 " << image.width << " " << image.height << " " << 255 << std::endl;

        for (int i = 0; i < image.height; i++) {
            int pointer = i * image.width;
            for (; pointer < (i + 1) * image.width; pointer++) {
                outputFile << (int) (std::min(float(1), image.image[pointer].x) * 255) << " " <<
                           (int) (std::min(float(1), image.image[pointer].y) * 255) << " " <<
                           (int) (std::min(float(1), image.image[pointer].z) * 255) << " ";
            }
            outputFile << std::endl;
        }

        outputFile.close();
    } else {
        std::cout <<"Invalid image!" << std::endl;
    }
}

//IMAGE TO OPENGL
void ImageToOpenGL::display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, image.width, image.height, 0.0);


    std::cout << "Starting render... ";

    glBegin(GL_POINTS);

    for (auto y = 0; y < image.height; y++) {
        for (auto x = 0; y < image.width; x++) {

            glColor3f(10, 10, 10);
            glVertex2i(x, y);

            glFlush();
        }
    }

    glEnd();

    std::cout << "Done!\n";
}

void ImageToOpenGL::render() {
    glutInitWindowPosition(80, 80);
    glutInitWindowSize(image.width, image.height);
    glutCreateWindow("OpenGL Render");
    display();
    while (true) {}
}

//TRANSFORM
Transform::Transform(Vector3 position, Vector3 rotation) : position(0), rotation(0) {
    Transform::position = position;
    Transform::rotation = rotation;
}

//MATERIAL
Material::Material(Vector3 color, Vector3 emissionColor, float transparency, float albedo) : color(0),
                                                                                             emissionColor(0) {
    Material::color = color;
    Material::emissionColor = emissionColor;
    Material::transparency = transparency;
    Material::albedo = albedo;
}

//RAYTRACER
RayTracer::RayTracer(ImageOutput &imageOutput) : image(0, 0), imageOutput(Image(0, 0), "") {
    RayTracer::imageOutput = imageOutput;
}

float RayTracer::mix(const float &a, const float &b, const float &mix) {
    return b * mix + a * (1 - mix);
}

Vector3 RayTracer::trace(const Vector3 &rayOrigin, const Vector3 &rayDirection, const std::vector<Sphere> &objects,
                         const int &depth) {
    float tNear = INFINITY;

    const Object *object = NULL;

    //std::cout << objects.size() << std::endl;

    for (unsigned i = 0; i < objects.size(); ++i) {
        float t0 = INFINITY, t1 = INFINITY;
        if (objects[i].intersect(rayOrigin, rayDirection, t0, t1)) {
            if (t0 < 0) t0 = t1;
            if (t0 < tNear) {
                tNear = t0;
                object = &objects[i];
            }
        }
    }

    if (!object) return Vector3(3);
    Vector3 surfaceColor(0); // color of the ray/surfaceof the object intersected by the ray
    Vector3 phit = rayOrigin + rayDirection * tNear; // point of intersection
    Vector3 nhit = phit - object->transform.position; // normal at the intersection point
    nhit.normalize(); // normalize normal direction
    // If the normal and the view direction are not opposite to each other
    // reverse the normal direction. That also means we are inside the sphere so set
    // the inside bool to true. Finally reverse the sign of IdotN which we want
    // positive.
    float bias = 1e-4; // add some bias to the point from which we will be tracing
    bool inside = false;

    if (rayDirection.dot(nhit) > 0) nhit = -nhit, inside = true;
    if ((object->material.transparency > 0 || object->material.albedo > 0) && depth < MAX_RAY_DEPTH) {
        float facingratio = -rayDirection.dot(nhit);
        // change the mix value to tweak the effect
        float fresneleffect = mix(pow(1 - facingratio, 3), 1, 0.1);
        // compute reflection direction (not need to normalize because all vectors
        // are already normalized)
        Vector3 reflectionDirection = rayDirection - nhit * 2 * rayDirection.dot(nhit);
        reflectionDirection.normalize();
        Vector3 reflection = trace(phit + nhit * bias, reflectionDirection, objects, depth + 1);
        Vector3 refraction(0);
        // if the sphere is also transparent compute refraction ray (transmission)
        if (object->material.transparency) {
            float ior = 1.1, eta = (inside) ? ior : 1 / ior; // are we inside or outside the surface?
            float cosi = -nhit.dot(rayDirection);
            float k = 1 - eta * eta * (1 - cosi * cosi);
            Vector3 refractionDirection = rayDirection * eta + nhit * (eta * cosi - sqrt(k));
            refractionDirection.normalize();
            refraction = trace(phit - nhit * bias, refractionDirection, objects, depth + 1);
        }
        // the result is a mix of reflection and refraction (if the sphere is transparent)
        surfaceColor = (
                               reflection * fresneleffect +
                               refraction * (1 - fresneleffect) * object->material.transparency) *
                       object->material.color;
    } else {
        // it's a diffuse object, no need to raytrace any further
        for (unsigned i = 0; i < objects.size(); ++i) {
            if (objects[i].material.emissionColor.magnitude() > 0) {
                // this is a light
                Vector3 transmission(1, 1, 1);
                Vector3 lightDirection = objects[i].transform.position - phit;
                lightDirection.normalize();
                for (unsigned j = 0; j < objects.size(); ++j) {
                    if (i != j) {
                        float t0, t1;
                        if (objects[j].intersect(phit + nhit * bias, lightDirection, t0, t1)) {
                            transmission = Vector3(0, 0, 0);
                            break;
                        }
                    }
                }
                surfaceColor += transmission * object->material.color *
                                std::max(float(0), nhit.dot(lightDirection)) * objects[i].material.emissionColor;
            }
        }
    }

    return surfaceColor + object->material.emissionColor;
}

void RayTracer::render(const std::vector<Sphere> &objects) {
    printf("Starting render. \n");
    image = Image(640, 480);

    auto width = image.width;
    auto height = image.height;

    float invWidth = 1 / float(width), invHeight = 1 / float(height);
    float fov = 30, aspectratio = width / float(height);
    float angle = tan(M_PI * 0.5 * fov / 180.);

    // Trace rays
    for (unsigned y = 0; y < height; ++y) {
        for (unsigned x = 0; x < width; ++x) {
            float xx = (2 * ((x + 0.5) * invWidth) - 1) * angle * aspectratio;
            float yy = (1 - 2 * ((y + 0.5) * invHeight)) * angle;
            Vector3 rayDirection(xx, yy, -1);
            rayDirection.normalize();
            Vector3 result = trace(Vector3(0), rayDirection, objects, 0);
            image.image.push_back(result);
        }
    }
    printf("You still with me?\n");

    ImageToFile imgtofile("meep.ppm", "ppm");

    imgtofile.image = image;
    imgtofile.render();
}
