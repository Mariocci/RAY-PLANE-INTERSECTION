#include "Sphere.h"
#include <cmath>

const GeometryType Sphere::type = GeometryType::Sphere;

Sphere::Sphere(const Vec3& center, const float radius) : center(center), radius(radius) {}

Vec3* Sphere::lineIntersect(const Line& line) const {
	double x[3];
	double y[3];
	double z[3];

	x[0] = std::pow((line.getA()[0] - this->center[0]) , 2);
	x[1] = 2 * (line.getD1()[0]) * (line.getA()[0] - this->center[0]);
	x[2] = std::pow((line.getD1())[0] , 2);

	y[0] = std::pow((line.getA()[1] - this->center[1]) , 2);
	y[1] = 2 * (line.getD1()[1]) * (line.getA()[1] - this->center[1]);
	y[2] = std::pow((line.getD1())[1] , 2);

	z[0] = std::pow((line.getA()[2] - this->center[2]) , 2);
	z[1] = 2 * (line.getD1()[2]) * (line.getA()[2] - this->center[2]);
	z[2] = std::pow((line.getD1())[2] , 2);	

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
		Vec3 intersection = line.getA() + line.getD1() * t;
		return new Vec3(intersection);
    } else {
        // Two intersections
		double t1 = (-sum[1] + std::sqrt(discriminant)) / (2 * sum[2]);
		double t2 = (-sum[1] - std::sqrt(discriminant)) / (2 * sum[2]);
		Vec3 intersection1 = line.getA() + line.getD1() * t1;
		Vec3 intersection2 = line.getA() + line.getD1() * t2;
		return std::min(new Vec3(intersection1), new Vec3(intersection2), 
						[line](const Vec3* a, const Vec3* b) {
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
