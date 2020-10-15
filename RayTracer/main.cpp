#include <iostream>
#include "types.h"

namespace ray = raytracer;

int main() {
    ray::Vector3 v3(10, 23, 2);

    std::cout << v3.x << std::endl;
    std::cout << v3.y << std::endl;
    std::cout << v3.z << std::endl;
    std::cout << v3.magnitude() << std::endl;
    return 0;
}
