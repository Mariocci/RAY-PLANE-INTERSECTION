#include "GeometryBody.h"
#include "Vec3.h"
#include "GeometryType.h"

class Sphere : public GeometryBody {
    private:
        Vec3 center;
        float radius;
        static const GeometryType type;
    public:
        Sphere(const Vec3& center, const float radius);
        ~Sphere() override {}

        Vec3* lineIntersect(const Line& line) const override;
        bool containsPoint(const Vec3& point) const override;
        GeometryType getType() const override;
};
