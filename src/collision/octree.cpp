#include "collision/octree.hpp"

Octree::Octree(Vector3f center, float halfLength) :
    m_center{center},
    m_halfLength{halfLength}
{

}

void Octree::insert(BoundingVolumeSphere* object)
{
    int index = getOctant(object);

    if (index != -1)
    {
        if (m_children[index] == nullptr)
        {
            Vector3f centerOffset;
            float step = m_halfLength / 2.0f;
            centerOffset.setX((index & 1) ? step : -step);
            centerOffset.setY((index & 2) ? step : -step);
            centerOffset.setZ((index & 4) ? step : -step);
            m_children[index] = std::make_unique<Octree>(m_center + centerOffset, step);
        }

        m_children[index]->insert(object);
    }
    else
    {
        m_objects.push_back(object);
    }
}

int Octree::getOctant(BoundingVolumeSphere* object) const
{
    int index = 0;

    for (int i = 0; i < 3; i++)
    {
        float delta = object->getPosition()[i] - m_center[i];

        if (std::abs(delta) <= object->getRadius())
        {
            // Straddle
            return -1;
        }

        if (delta > 0.0f)
        {
            index |= (1 << i);
        }
    }

    return index;
}

void Octree::clear()
{
    for (int i = 0; i < 8; i++)
    {
        if (m_children[i] != nullptr)
        {
            m_children[i]->clear();
            m_children[i].reset();
        }
    }

    m_objects.clear();
}

std::vector<std::pair<BoundingVolumeSphere*, BoundingVolumeSphere*>> Octree::findAllPossibleCollisions() const
{
    std::vector<std::pair<BoundingVolumeSphere*, BoundingVolumeSphere*>> possibleCollisions;
    findAllIntersections(possibleCollisions);
    return possibleCollisions;
}

void Octree::findAllIntersections(std::vector<std::pair<BoundingVolumeSphere*, BoundingVolumeSphere*>>& intersections) const
{
    // Intersections on the same level
    for (int i = 0; i < m_objects.size(); ++i)
    {
        for (int j = 0; j < i; ++j)
        {
            if (m_objects[i]->intersects(m_objects[j]))
            {
                intersections.emplace_back(m_objects[i], m_objects[j]);
            }
        }
    }

    // Intersections with and in children
    for (int i = 0; i < 8; ++i)
    {
        if (m_children[i] != nullptr)
        {
            // Objects in this level can intersect with objects in children
            for (BoundingVolumeSphere* object : m_objects)
            {
                m_children[i]->findIntersectionsInChildren(object, intersections);
            }

            // Intersections between objects in children
            m_children[i]->findAllIntersections(intersections);
        }
    }
}

void Octree::findIntersectionsInChildren(BoundingVolumeSphere* object, std::vector<std::pair<BoundingVolumeSphere*, BoundingVolumeSphere*>>& intersections) const
{
    // Intersections with objects in this child
    for (BoundingVolumeSphere* childObject : m_objects)
    {
        if (object->intersects(childObject))
        {
            intersections.emplace_back(object, childObject);
        }
    }

    // Intersections with children of this child
    for (int i = 0; i < 8; ++i)
    {
        if (m_children[i] != nullptr)
        {
            m_children[i]->findIntersectionsInChildren(object, intersections);
        }
    }
}