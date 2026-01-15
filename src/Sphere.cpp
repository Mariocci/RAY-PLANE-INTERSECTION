#include "Sphere.h"
#include "../include/Line.h"
#include <cmath>
#include <algorithm>

#include <random>

const GeometryType Sphere::type = GeometryType::Sphere;

Sphere::Sphere(const Vec3& center, const float radius) : center(center), radius(radius) {}

Vec3* Sphere::lineIntersect(const Line& line) const {
    Vec3 a = line.getA();
    Vec3 d = line.getB() - a;
	double x[3];
	double y[3];
	double z[3];

	x[0] = std::pow((a[0] - this->center[0]) , 2);
	x[1] = 2 * (d[0]) * (a[0] - this->center[0]);
	x[2] = std::pow((d)[0] , 2);

	y[0] = std::pow((a[1] - this->center[1]) , 2);
	y[1] = 2 * (d[1]) * (a[1] - this->center[1]);
	y[2] = std::pow((d)[1] , 2);

	z[0] = std::pow((a[2] - this->center[2]) , 2);
	z[1] = 2 * (d[2]) * (a[2] - this->center[2]);
	z[2] = std::pow((d)[2] , 2); 	

	double sum[3] = { x[0] + y[0] + z[0],
				  x[1] + y[1] + z[1],
				  x[2] + y[2] + z[2] };
	double discriminant = sum[1] * sum[1] - 4 * sum[0] * (sum[2] - std::pow( this->radius, 2));

    if (discriminant < 0.0) {
        // No intersection
        return nullptr;
    } else if (discriminant == 0.0) {
 		// One intersection (tangent)
 		double t = -sum[1] / (2 * sum[2]);
 		Vec3 intersection = a + d * t;
		return new Vec3(intersection);
    } else {
        // Two intersections
		double t1 = (-sum[1] + std::sqrt(discriminant)) / (2 * sum[2]);
		double t2 = (-sum[1] - std::sqrt(discriminant)) / (2 * sum[2]);
 		Vec3 intersection1 = a + d * t1;
 		Vec3 intersection2 = a + d * t2;
 		return std::min(new Vec3(intersection1), new Vec3(intersection2), 
					[&line](const Vec3* a, const Vec3* b) {
 						double distA = line.getA().distance(*a);
 						double distB = line.getA().distance(*b);
 						return distA < distB;
 					});
    }
}

bool Sphere::containsPoint(const Vec3& point) const {
	return center.distance(point) <= radius;
}

GeometryType Sphere::getType() const {
	return Sphere::type;
}

Vec3 Sphere::randomPointInside() const {
	static thread_local std::mt19937 gen(std::random_device{}());
	std::uniform_real_distribution<float> dist01(0.0f, 1.0f);

	float u = dist01(gen);
	float v = dist01(gen);
	float w = dist01(gen);

	double PI = std::acos(-1.0);

	float r = radius * std::cbrt(u);
	float cosTheta = 1.0f - 2.0f * v;
	float sinTheta = std::sqrt(std::max(0.0f, 1.0f - cosTheta * cosTheta));
	float phi = 2.0f * (float)PI * w;

	float x = r * sinTheta * std::cos(phi);
	float y = r * sinTheta * std::sin(phi);
	float z = r * cosTheta;

	return center + Vec3(x, y, z);
}
