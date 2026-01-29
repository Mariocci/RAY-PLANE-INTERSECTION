#pragma once

#include "Vec3.h"
#include "GeometryType.h"
#include "Material.h"

class Line;

class GeometryBody {
public:
    virtual ~GeometryBody() = default;
    virtual Vec3* lineIntersect(const Line& line) const = 0;
    virtual bool containsPoint(const Vec3& point) const = 0;
    virtual GeometryType getType() const = 0;
    virtual Vec3 randomPointInside() const = 0;

    Material material = Material::Air;
};
