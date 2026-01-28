#include <iostream>
#include <memory>
#include <string>
#include <limits>

#include "../include/GeometryBody.h"
#include "../include/Sphere.h"
#include "../include/Cylinder.h"
#include "ParseCSV.h"

using namespace std;

string geometryTypeToString(GeometryType type) {
    switch (type) {
    case GeometryType::Sphere: return "Sphere";
    case GeometryType::Cylinder: return "Cylinder";
    default: return "Unknown";
    }
}

int main() {
    int n;
    cout << "Number of random points per body: ";
    cin >> n;

    auto bodies = parseCSV("geometry.csv");

    for (size_t i = 0; i < bodies.size(); ++i) {
        auto& body = bodies[i];
        cout << "Body #" << i << " (" << geometryTypeToString(body->getType()) << "):\n";
        for (int j = 0; j < n; ++j) {
            Vec3 p = body->randomPointInside();
            cout << "  " << p.x << " " << p.y << " " << p.z << "\n";
        }
        cout << "\n";
    }

    return 0;
}
