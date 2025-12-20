#include "Line.h"
#include "GeometryType.h"

Line::Line(const Vec3& a, const Vec3& b) : point_a(a), point_b(b) {}

Vec3* Line::lineIntersect(const Line& line) const {
    Vec3 p1 = point_a;
    Vec3 d1 = point_b - point_a;
    Vec3 p2 = line.point_a;
    Vec3 d2 = line.point_b - line.point_a;

    Vec3 cross_d = d1.cross(d2);
    float denom = cross_d.norm();
    const float EPS = 1e-6f;
    if (denom < EPS) {
        return nullptr;
    }

    Vec3 p21 = p1 - p2;
    float t = (p21.cross(d2)).dot(cross_d) / (denom * denom);
    float s = -(p21.cross(d1)).dot(cross_d) / (denom * denom);

    Vec3 intersection1 = p1 + d1 * t;
    Vec3 intersection2 = p2 + d2 * s;

    if ((intersection1 - intersection2).norm() < EPS) {
        return new Vec3((intersection1 + intersection2) * 0.5f);
    } else {
        return nullptr;
    }
}

bool Line::containsPoint(const Vec3& point) const {
    Vec3 AB = point_b - point_a;
    Vec3 AP = point - point_a;
    
    Vec3 cross_prod = AB.cross(AP);
    const float EPS = 1e-6f;
    return cross_prod.norm() < EPS;
}

GeometryType Line::getType() const {
    return GeometryType::Line;
}
