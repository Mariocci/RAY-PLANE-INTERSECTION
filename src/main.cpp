#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include <limits>
#include <cmath>

#include "ParseCSV.h"
#include "GeometryBody.h"
#include "Line.h"
#include "RadiationCalculator.h"

int main(int argc, char** argv) {

    std::string csvPath;
    int n = -1;

    auto printUsage = [&](bool full=false) {
        std::cerr << "Usage: " << argv[0] << " -p <path-to-csv> [-n <num-samples>]\n";
        if (full) {
            std::cerr << "  -p, --path   Path to CSV scene file (required)\n";
            std::cerr << "  -n, --num    Number of random samples per source (optional)\n";
            std::cerr << "  -h, --help   Show this help message\n";
        }
    };

    for (int i = 1; i < argc; ++i) {
        std::string a = argv[i];
        if (a == "-p" || a == "--path") {
            if (i + 1 < argc) {
                csvPath = argv[++i];
            } else {
                std::cerr << "Error: missing value for " << a << "\n";
                printUsage();
                return 1;
            }
        } else if (a == "-n" || a == "--num") {
            if (i + 1 < argc) {
                try {
                    n = std::stoi(argv[++i]);
                    if (n <= 0) { std::cerr << "Invalid sample count: must be > 0\n"; return 1; }
                } catch (...) {
                    std::cerr << "Failed to parse sample count argument\n";
                    return 1;
                }
            } else {
                std::cerr << "Error: missing value for " << a << "\n";
                printUsage();
                return 1;
            }
        } else if (a == "-h" || a == "--help") {
            printUsage(true);
            return 0;
        } else {
            std::cerr << "Unknown argument: " << a << "\n";
            printUsage();
            return 1;
        }
    }

    if (csvPath.empty()) {
        std::cerr << "Error: CSV path is required.\n";
        printUsage();
        return 1;
    }

    std::cout << "Using CSV: " << csvPath << "\n";

    if (n == -1) {
        std::cout << "Number of random points per selected source: ";
        std::cin >> n;
        if (n <= 0) { std::cerr << "Invalid sample count: must be > 0\n"; return 1; }
    } else {
        std::cout << "Number of random points per selected source (from arg): " << n << "\n";
    }

    Scene scene = parseCSV(csvPath);

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
