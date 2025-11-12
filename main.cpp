#include <iostream>
#include <string>
#include <vector>
#include "include/GeometryObject.h"
#include "include/Sphere.h"
#include "include/Cylinder.h"

using namespace std;


int main() {
    while (true) {
        string type;
        cin >> type; 
        
        GeometryObject* obj = nullptr;
        
        if (type == "sphere") {
            Vec3 center; double radius;
            cin >> center.x >> center.y >> center.z >> radius;
            obj = new Sphere(center, radius);
        } else if (type == "cylinder") {
            Vec3 center, axis; double radius, height;
            cin >> center.x >> center.y >> center.z >> axis.x >> axis.y >> axis.z >> radius >> height;
            obj = new Cylinder(center, axis, radius, height);
        }

        if (obj) {
            auto patches = obj->generateSubPatches(n);
            for (auto& p : patches) cout << p.centroid << "\n";
        }
    }
    return 0;
}