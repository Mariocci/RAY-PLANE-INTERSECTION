#include "Sphere.h"

const GeometryType Sphere::type = GeometryType::Sphere;

Sphere::Sphere(const Vec3& center, const float radius) : center(center), radius(radius) {}

Vec3* Sphere::lineIntersect(const Line& line) const {
	// Stub: not implemented yet
	return nullptr;
}

bool Sphere::containsPoint(const Vec3& point) const {
	return center.distance(point) <= radius;
}

GeometryType Sphere::getType() const {
	return Sphere::type;
}
