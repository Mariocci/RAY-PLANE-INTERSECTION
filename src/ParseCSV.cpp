#include "parseCSV.h"

#include "Sphere.h"
#include "Cylinder.h"
#include "Vec3.h"

#include <fstream>
#include <sstream>
#include <iostream>

static Vec3 normalizeOrDefault(const Vec3& v) {
    float n = v.norm();
    if (n < 1e-6f)
        return Vec3(0.0f, 0.0f, 1.0f);
    return v / n;
}

std::vector<std::unique_ptr<GeometryBody>>
parseCSV(const std::string& filename) {

    std::vector<std::unique_ptr<GeometryBody>> bodies;
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Failed to open CSV: " << filename << "\n";
        return bodies;
    }

    std::string line;
    std::getline(file, line); // preskoci header

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string col;
        std::vector<std::string> cols;

        while (std::getline(ss, col, ',')) {
            cols.push_back(col);
        }

        if (cols.size() < 16) continue;

        const std::string& type = cols[1];
        if (type == "Cube")
            continue;

        float px = std::stof(cols[3]);
        float py = std::stof(cols[4]);
        float pz = std::stof(cols[5]);

        float rx = std::stof(cols[9]);
        float ry = std::stof(cols[10]);
        float rz = std::stof(cols[11]);

        float radius  = std::stof(cols[12]);
        float radiusX = std::stof(cols[13]);
        float height  = std::stof(cols[15]);

        Vec3 center(px, py, pz);

        // 🔵 SPHERE
        if (type == "Sphere") {
            if (radius <= 0.0f) continue;

            bodies.push_back(
                std::make_unique<Sphere>(center, radius)
            );
        }

        // 🟢 CYLINDER
        else if (type == "Cylinder") {
            if (height <= 0.0f) continue;

            float r = (radiusX > 0.0f) ? radiusX : radius;
            if (r <= 0.0f) continue;

            Vec3 axis = normalizeOrDefault(Vec3(rx, ry, rz));

            Vec3 lower = center - axis * (height * 0.5f);
            Vec3 upper = center + axis * (height * 0.5f);

            bodies.push_back(
                std::make_unique<Cylinder>(lower, upper, r, height)
            );
        }
    }

    return bodies;
}
