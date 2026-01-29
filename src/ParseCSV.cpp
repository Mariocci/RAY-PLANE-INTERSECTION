#include "ParseCSV.h"

#include "Sphere.h"
#include "Cylinder.h"
#include "Vec3.h"
#include "Material.h"

#include <algorithm>

#include <fstream>
#include <sstream>
#include <iostream>

static Vec3 normalizeOrDefault(const Vec3& v) {
    float n = v.norm();
    if (n < 1e-6f)
        return Vec3(0.0f, 0.0f, 1.0f);
    return v / n;
}

Scene parseCSV(const std::string& filename) {
    Scene scene;
    scene.sensor = Vec3(0, 0, 0);

    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open CSV\n";
        return scene;
    }

    std::string line;
    std::getline(file, line); // header

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string col;
        std::vector<std::string> cols;

        while (std::getline(ss, col, ',')) {
            cols.push_back(col);
        }

        if (cols.size() < 16) continue;

        std::string material = cols[2];
        std::transform(material.begin(), material.end(), material.begin(), ::toupper);
        float px = std::stof(cols[3]);
        float py = std::stof(cols[4]);
        float pz = std::stof(cols[5]);

        if (material == "SENSOR") {
            scene.sensor = Vec3(px, py, pz);
            continue;
        }

        const std::string& type = cols[1];
        Vec3 center(px, py, pz);

        Material mat = Material::Air;
        if (material == "LEAD") mat = Material::Lead;
        else if (material == "CONCRETE") mat = Material::Concrete;
        else if (material == "AIR") mat = Material::Air;
        else if (material == "RADIOACTIVE") mat = Material::Radioactive;

        if (type == "Sphere") {
            float radius = std::stof(cols[12]);
            if (radius > 0.0f) {
                scene.bodies.push_back(
                    std::make_unique<Sphere>(center, radius, mat)
                );
            }
        }

        else if (type == "Cylinder") {
            float height  = std::stof(cols[15]);
            float radiusX = std::stof(cols[13]);
            float radius  = std::stof(cols[12]);

            float r = (radiusX > 0.0f) ? radiusX : radius;
            if (r <= 0.0f || height <= 0.0f) continue;

            Vec3 axis = normalizeOrDefault(
                Vec3(std::stof(cols[9]),
                     std::stof(cols[10]),
                     std::stof(cols[11]))
            );

            Vec3 lower = center - axis * (height * 0.5f);
            Vec3 upper = center + axis * (height * 0.5f);

            scene.bodies.push_back(
                std::make_unique<Cylinder>(lower, upper, r, height, mat)
            );
        }
    }

    return scene;
}
