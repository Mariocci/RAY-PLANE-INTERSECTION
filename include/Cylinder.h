#include "GeometryBody.h"
#include "Vec3.h"

class Cylinder : public GeometryBody {
    private:
        Vec3 base_center_lower;
        Vec3 base_center_upper;
        float radius;
        float height;
        static const GeometryType type;
    public:
        Cylinder(Vec3 base_center_lower, 
                Vec3 base_center_upper,
                float radius,
                float height
            );
        ~Cylinder() override {}

        Vec3* lineIntersect(const Line& line) const override;
        bool containsPoint(const Vec3& point) const override;
        GeometryType getType() const override;
};