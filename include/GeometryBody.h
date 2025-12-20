#include "Vec3.h"
#include "Line.h"
#include "GeometryType.h"

class GeometryBody {
public:
    virtual ~GeometryBody() = default;
    virtual Vec3* lineIntersect(const Line& line) const = 0;
    virtual bool containsPoint(const Vec3& point) const = 0;
    virtual GeometryType getType() const = 0;
};
