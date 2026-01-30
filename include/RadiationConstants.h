#pragma once

namespace RadiationConstants {

    constexpr double PI = 3.141592653589793;
    constexpr double ABSORPTION_AIR = 0.02;
    constexpr double ABSORPTION_LEAD = 0.90;
    constexpr double ABSORPTION_CONCRETE = 0.35;
    constexpr double SENSOR_EFFICIENCY = 1.0;

    // Source & sensor defaults (units: photons / s and m^2 respectively)
    constexpr double SOURCE_STRENGTH = 1e6; // default source emission (photons per second)
    constexpr double SENSOR_AREA = 1e-4;    // default sensor effective area (1 cm^2)

    constexpr int DEFAULT_SAMPLE_COUNT = 10000;

    constexpr double EPSILON = 1e-6;
}