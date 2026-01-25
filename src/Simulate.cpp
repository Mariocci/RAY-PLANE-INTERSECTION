#include <iostream>
#include <iomanip>
#include <random>
#include "../include/Sphere.h"
#include "../include/Cylinder.h"
#include "../include/Line.h"

static void printVec(const Vec3& v) {
    std::cout << std::fixed << std::setprecision(6)
              << "(" << v.x << ", " << v.y << ", " << v.z << ")";
}

int main() {
    std::mt19937 gen(12345);

    Sphere sph(Vec3(0.0f, 0.0f, 0.0f), 1.0f);
    Cylinder cyl(Vec3(2.0f, 0.0f, 0.0f), Vec3(2.0f, 0.0f, 2.0f), 0.5f, 2.0f);

    std::uniform_real_distribution<float> dist01(0.0f, 1.0f);

    auto sample_direction = [&](Vec3& out) {
        float u = dist01(gen);
        float v = dist01(gen);
        double PI = std::acos(-1.0);
        float theta = std::acos(1.0f - 2.0f * u);
        float phi = 2.0f * (float)PI * v;
        out.x = std::sin(theta) * std::cos(phi);
        out.y = std::sin(theta) * std::sin(phi);
        out.z = std::cos(theta);
    };

    const int N = 4;
    Vec3 externalPoint(5.0f, 0.0f, 1.0f);

    std::cout << "--- Parameters (fixed for testing) ---\n";
    std::cout << "Sphere:\n";
    std::cout << "  center: "; printVec(Vec3(0.0f,0.0f,0.0f)); std::cout << "\n";
    std::cout << "  radius: 1.0\n";
    std::cout << "Cylinder:\n";
    std::cout << "  base_center_lower: "; printVec(Vec3(2.0f,0.0f,0.0f)); std::cout << "\n";
    std::cout << "  base_center_upper: "; printVec(Vec3(2.0f,0.0f,2.0f)); std::cout << "\n";
    std::cout << "  radius: 0.5\n";
    std::cout << "  height: 2.0\n";
    std::cout << "External point: "; printVec(externalPoint); std::cout << "\n\n";

    std::cout << "Sphere samples and intersections:\n";
    for (int i = 0; i < N; ++i) {
        Vec3 p = sph.randomPointInside();
        Line ray(externalPoint, p);

        std::cout << "Sphere sample " << (i+1) << ": "; printVec(p); std::cout << "\n";
        Vec3* isect_s = sph.lineIntersect(ray);
        if (isect_s) {
            std::cout << "  Intersection with Sphere at "; printVec(*isect_s);
            std::cout << " (distance from external: " << std::fixed << std::setprecision(6) << externalPoint.distance(*isect_s) << ")\n";
            delete isect_s;
        } else {
            std::cout << "  No intersection with Sphere\n";
        }

        Vec3* isect_c = cyl.lineIntersect(ray);
        if (isect_c) {
            std::cout << "  Intersection with Cylinder at "; printVec(*isect_c);
            std::cout << " (distance from external: " << std::fixed << std::setprecision(6) << externalPoint.distance(*isect_c) << ")\n";
            delete isect_c;
        } else {
            std::cout << "  No intersection with Cylinder\n";
        }
        std::cout << "\n";
    }

    std::cout << "Cylinder samples and intersections:\n";
    for (int i = 0; i < N; ++i) {
        Vec3 p = cyl.randomPointInside();
        Line ray(externalPoint, p);

        std::cout << "Cylinder sample " << (i+1) << ": "; printVec(p); std::cout << "\n";
        Vec3* isect_c = cyl.lineIntersect(ray);
        if (isect_c) {
            std::cout << "  Intersection with Cylinder at "; printVec(*isect_c);
            std::cout << " (distance from external: " << std::fixed << std::setprecision(6) << externalPoint.distance(*isect_c) << ")\n";
            delete isect_c;
        } else {
            std::cout << "  No intersection with Cylinder\n";
        }

        Vec3* isect_s = sph.lineIntersect(ray);
        if (isect_s) {
            std::cout << "  Intersection with Sphere at "; printVec(*isect_s);
            std::cout << " (distance from external: " << std::fixed << std::setprecision(6) << externalPoint.distance(*isect_s) << ")\n";
            delete isect_s;
        } else {
            std::cout << "  No intersection with Sphere\n";
        }
        std::cout << "\n";
    }
    std::cout << "\n--- Edge case tests (Cylinder boundary) ---\n";

    Vec3 edgePoints[] = {
        Vec3(2.5f, 0.0f, 1.0f),   // side surface
        Vec3(2.0f, 0.5f, 0.0f),   // bottom rim
        Vec3(2.0f, -0.5f, 2.0f),  // top rim
        Vec3(2.5f, 0.0f, 2.0f)    // corner (side + top)
    };

    for (int i = 0; i < 4; ++i) {
        Line ray(externalPoint, edgePoints[i]);

        std::cout << "Edge test " << (i+1) << ": target ";
        printVec(edgePoints[i]);
        std::cout << "\n";

        Vec3* hit = cyl.lineIntersect(ray);
        if (hit) {
            std::cout << "  Intersection at ";
            printVec(*hit);
            std::cout << " (distance: " << externalPoint.distance(*hit) << ")\n";
            delete hit;
        } else {
            std::cout << "  No intersection (ERROR)\n";
        }
    }

    return 0;
}
