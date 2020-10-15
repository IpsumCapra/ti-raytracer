#include "types.h"
#include <cmath>

using namespace raytracer;

Vector3::Vector3(float x, float y, float z) {
    Vector3::x = x;
    Vector3::y = y;
    Vector3::z = z;
}

float Vector3::magnitude() {
    return sqrt((pow(x, 2) + pow(y, 2) + pow(z, 2)));
}