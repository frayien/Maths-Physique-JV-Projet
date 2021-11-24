#ifndef MPJVP_OCTREE
#define MPJVP_OCTREE

#include "collision/BoundingVolumeSphere.hpp"

#include <array>
#include <vector>

class Octree
{
private:
    std::array<std::unique_ptr<Octree>, 8> m_children;
    std::vector<BoundingVolumeSphere*> m_objects;
    Vector3f m_center;
    float m_halfLength;

public:
    Octree() = default;
    Octree(Vector3f center, float halfLength);
    virtual ~Octree() = default;

    void insert(BoundingVolumeSphere* object);
    void clear();
    std::vector<std::pair<BoundingVolumeSphere*, BoundingVolumeSphere*>> findAllPossibleCollisions() const;

private:
    int getOctant(BoundingVolumeSphere* object) const;
    void findAllIntersections(std::vector<std::pair<BoundingVolumeSphere*, BoundingVolumeSphere*>>& intersections) const;
    void findIntersectionsInChildren(BoundingVolumeSphere* object, std::vector<std::pair<BoundingVolumeSphere*, BoundingVolumeSphere*>>& intersections) const;

};

#endif // MPJVP_OCTREE
