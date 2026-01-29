#pragma once
#include <vector>
#include <memory>
#include <string>

#include "Vec3.h"
#include "Material.h"

class GeometryBody;

struct Scene {
    Vec3 sensor;
    std::vector<std::unique_ptr<GeometryBody>> bodies;
};

Scene parseCSV(const std::string& filename);
