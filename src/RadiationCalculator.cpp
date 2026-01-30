#include "RadiationCalculator.h"
#include "RadiationConstants.h"
#include <algorithm>
#include <cmath>

double RadiationCalculator::compute(
    const Vec3& sensor,
    const Vec3& sourcePoint,
    const std::vector<std::unique_ptr<GeometryBody>>& bodies
) {
    Line ray(sourcePoint, sensor);
    std::vector<Intersection> intersections;

    for (const auto& body : bodies) {
        if (Vec3* hit = body->lineIntersect(ray)) {
            intersections.push_back({ body.get(), *hit, sensor.distance(*hit), body->material, sourcePoint.distance(*hit) });
            delete hit;
        }
    }

    std::sort(intersections.begin(), intersections.end(),
              [](const Intersection& a, const Intersection& b) {
                  return a.distanceFromSource < b.distanceFromSource;
              });

    double transmission = 1.0;
    Vec3 prev = sourcePoint;

    for (const auto& inter : intersections) {
        double segLen = prev.distance(inter.point);
        double mu = 0.0;
        switch (inter.material) {
            case Material::Air: mu = RadiationConstants::ABSORPTION_AIR; break;
            case Material::Lead: mu = RadiationConstants::ABSORPTION_LEAD; break;
            case Material::Concrete: mu = RadiationConstants::ABSORPTION_CONCRETE; break;
            case Material::Radioactive: mu = 0.0; break;
            default: mu = 0.0; break;
        }
        transmission *= std::exp(-mu * segLen);
        prev = inter.point;
    }

    double finalLen = prev.distance(sensor);
    transmission *= std::exp(-RadiationConstants::ABSORPTION_AIR * finalLen);

    // Convert transmission coefficient into received radiation at the sensor.
    // Use simple isotropic source model: received = SOURCE_STRENGTH * transmission * (A_sensor / (4*pi*r^2)) * efficiency
    double r = sourcePoint.distance(sensor);
    if (r < RadiationConstants::EPSILON) r = RadiationConstants::EPSILON;
    double geometricFactor = RadiationConstants::SENSOR_AREA / (4.0 * RadiationConstants::PI * r * r);
    double received = RadiationConstants::SOURCE_STRENGTH * transmission * geometricFactor * RadiationConstants::SENSOR_EFFICIENCY;

    return received;
}
