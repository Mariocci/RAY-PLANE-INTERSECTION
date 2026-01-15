#include "Cylinder.h"
#include "../include/Line.h"
#include <cmath>
#include <algorithm>
#include <vector>
#include <random>

const GeometryType Cylinder::type = GeometryType::Cylinder;

Cylinder::Cylinder(Vec3 base_center_lower, 
				   Vec3 base_center_upper,
				   float radius,
				   float height
				  ) : base_center_lower(base_center_lower), base_center_upper(base_center_upper), radius(radius), height(height) {}

Vec3* Cylinder::lineIntersect(const Line& line) const {
	Vec3 a = line.getA();
	Vec3 d = line.getB() - a;

	Vec3 c1 = base_center_lower;
	Vec3 c2 = base_center_upper;
	Vec3 v = c2 - c1;
	float h = v.norm();
	const float EPS = 1e-6f;
	if (h < EPS) return nullptr;
	Vec3 v_hat = v / h;

	Vec3 d_par = v_hat * d.dot(v_hat);
	Vec3 m = d - d_par; 

	Vec3 a_minus_c1 = a - c1;
	Vec3 a_par = v_hat * a_minus_c1.dot(v_hat);
	Vec3 w = a_minus_c1 - a_par; 

	double A = m.dot(m);
	double B = 2.0 * m.dot(w);
	double C = w.dot(w) - (double)radius * (double)radius;

	if (std::abs(A) < EPS) {
		return nullptr;
	}

	double disc = B * B - 4 * A * C;
	if (disc < 0.0) return nullptr;

	std::vector<std::pair<double, Vec3>> candidates;
	auto test_t = [&](double t) {
		Vec3 p = a + d * (float)t;
		double proj = (p - c1).dot(v_hat);
		if (proj >= 0.0 && proj <= h) {
			candidates.emplace_back(t, p);
		}
	};

	if (disc == 0.0) {
		double t = -B / (2 * A);
		test_t(t);
	} else {
		double sqrt_d = std::sqrt(disc);
		double t1 = (-B + sqrt_d) / (2 * A);
		double t2 = (-B - sqrt_d) / (2 * A);
		test_t(t1);
		test_t(t2);
	}

	if (candidates.empty()) return nullptr;

	auto cmp = [](const std::pair<double, Vec3>& p1, const std::pair<double, Vec3>& p2){
		return std::abs(p1.first) < std::abs(p2.first);
	};

	auto best = std::min_element(candidates.begin(), candidates.end(), cmp);
	return new Vec3(best->second);
}

bool Cylinder::containsPoint(const Vec3& point) const {
	Vec3 v = base_center_upper - base_center_lower;
	float h = v.norm();
	const float EPS = 1e-6f;
	if (h < EPS) return false;
	Vec3 v_hat = v / h;
	Vec3 rel = point - base_center_lower;
	float proj = rel.dot(v_hat);
	if (proj < 0.0f || proj > h) return false;
	Vec3 perp = rel - v_hat * proj;
	return perp.norm() <= radius;
}

GeometryType Cylinder::getType() const {
	return Cylinder::type;
}

Vec3 Cylinder::randomPointInside() const {
	static thread_local std::mt19937 gen(std::random_device{}());
	std::uniform_real_distribution<float> dist01(0.0f, 1.0f);

	float u = dist01(gen);
	float phi = 2.0f * (float)std::acos(-1.0) * dist01(gen);
	float r = radius * std::sqrt(u);
	float z = dist01(gen) * height;

	Vec3 v = base_center_upper - base_center_lower;
	float h = v.norm();
	const float EPS = 1e-6f;
	if (h < EPS) return base_center_lower;
	Vec3 v_hat = v / h;

	Vec3 arbitrary = (std::abs(v_hat.x) < 0.9f) ? Vec3(1.0f, 0.0f, 0.0f) : Vec3(0.0f, 1.0f, 0.0f);
	Vec3 uvec = v_hat.cross(arbitrary);
	float unorm = uvec.norm();
	if (unorm < EPS) return base_center_lower + v_hat * z;
	uvec = uvec / unorm;
	Vec3 wvec = v_hat.cross(uvec);

	Vec3 point_on_axis = base_center_lower + v_hat * z;
	Vec3 radial = uvec * (r * std::cos(phi)) + wvec * (r * std::sin(phi));
	return point_on_axis + radial;
}

