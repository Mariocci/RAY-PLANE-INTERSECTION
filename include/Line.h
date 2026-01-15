#pragma once

#include "GeometryBody.h"
#include "Vec3.h"

class Line : public GeometryBody {
    private:
        Vec3 point_a;
        Vec3 point_b;
        static const GeometryType type;
    public:
        Line(const Vec3& a, const Vec3& b);
        ~Line() override {}

        Vec3* lineIntersect(const Line& line) const override;
        bool containsPoint(const Vec3& point) const override;
        Vec3 randomPointInside() const override;
        GeometryType getType() const override;
            Vec3 getA() const;
            Vec3 getB() const;
};
