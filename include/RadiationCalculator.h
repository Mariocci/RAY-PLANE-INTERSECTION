#pragma once
#include <vector>
#include <memory>

#include "GeometryBody.h"
#include "Line.h"
#include "Vec3.h"
#include "Material.h"

struct Intersection {
    const GeometryBody* body;
    Vec3 point;
    double distanceFromSensor;
    Material material;
    double distanceFromSource;
};

class RadiationCalculator {
public:
    static double compute(
        const Vec3& sensor,
        const Vec3& sourcePoint,
        const std::vector<std::unique_ptr<GeometryBody>>& bodies
    );
};
