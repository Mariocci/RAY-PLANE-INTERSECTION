#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <limits>
#include <cmath>

#include "ParseCSV.h"
#include "GeometryBody.h"
#include "Line.h"
#include "RadiationCalculator.h"

int main() {

    int n;
    std::cout << "Number of random points per selected source: ";
    std::cin >> n;

    Scene scene = parseCSV("geometry.csv");

    const Vec3& sensor = scene.sensor;
    const auto& bodies = scene.bodies;
    std::vector<const GeometryBody*> sources;
    for (const auto& b : bodies) {
        if (b->material == Material::Radioactive) sources.push_back(b.get());
    }

    if (sources.empty()) {
        std::cerr << "Warning: no radioactive source bodies found in scene.\n";
        return 1;
    }

    if (sources.size() > 1) {
        std::cout << "Warning: multiple radioactive sources found, using the first one.\n";
    }
    const GeometryBody* sourceBody = sources.front();

    double sumRadiation = 0.0;
    double minRadiation = std::numeric_limits<double>::infinity();
    double maxRadiation = -std::numeric_limits<double>::infinity();
    double sumsq = 0.0;

    for (int i = 0; i < n; ++i) {
        Vec3 sourcePoint = sourceBody->randomPointInside();
        double radiation = RadiationCalculator::compute(sensor, sourcePoint, bodies);
        sumRadiation += radiation;
        sumsq += radiation * radiation;
        if (radiation < minRadiation) minRadiation = radiation;
        if (radiation > maxRadiation) maxRadiation = radiation;
    }

    double avg = (n > 0) ? (sumRadiation / n) : 0.0;
    double variance = (n > 1) ? (sumsq - (sumRadiation * sumRadiation) / n) / (n - 1) : 0.0;
    double stddev = (variance > 0.0) ? std::sqrt(variance) : 0.0;

    std::cout << "Samples: " << n << "\n";
    std::cout << "Average radiation: " << avg << " (photons/s)\n";
    std::cout << "Min radiation: " << minRadiation << " (photons/s)\n";
    std::cout << "Max radiation: " << maxRadiation << " (photons/s)\n";
    std::cout << "Stddev: " << stddev << " (photons/s)\n";

    double se = (n > 0) ? (stddev / std::sqrt((double)n)) : 0.0;
    double ci95 = 1.96 * se;
    std::cout << "StdErr: " << se << " (photons/s)\n";
    std::cout << "95% CI for mean: [" << (avg - ci95) << ", " << (avg + ci95) << "] (photons/s)\n";

    return 0;
}
