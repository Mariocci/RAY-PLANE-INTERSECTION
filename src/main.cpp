#include <iostream>
#include <memory>
#include <string>
#include <limits>

#include "../include/GeometryBody.h"
#include "../include/Sphere.h"
#include "../include/Cylinder.h"

using namespace std;

int main() {
    cout << "Number of random points per body: ";
    int n;
    cin >> n;

    cout << "\nInput format:\n";
    cout << "  sphere   cx cy cz r\n";
    cout << "  cylinder cx cy cz ax ay az r h\n";
    cout << "Type 'exit' to quit.\n\n";

    string type;
    while (true) {
        cout << "> ";
        cin >> type;

        if (!cin || type == "exit") {
            cout << "Exiting.\n";
            break;
        }

        unique_ptr<GeometryBody> body;

        if (type == "sphere") {
            Vec3 center;
            double radius;

            cout << "Enter: cx cy cz r: ";
            cin >> center.x >> center.y >> center.z >> radius;

            body = make_unique<Sphere>(center, radius);
        }
        else if (type == "cylinder") {
            Vec3 center, axis;
            double radius, height;

            cout << "Enter: cx cy cz ax ay az r h: ";
            cin >> center.x >> center.y >> center.z
                >> axis.x >> axis.y >> axis.z
                >> radius >> height;

            body = make_unique<Cylinder>(center, axis, radius, height);
        }
        else {
            cerr << "Unknown body type.\n";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        cout << "Random points:\n";
        for (int i = 0; i < n; ++i) {
            Vec3 p = body->randomPointInside();
            cout << "  " << p.x << " " << p.y << " " << p.z << "\n";
        }
    }

    return 0;
}
