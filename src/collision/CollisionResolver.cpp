#include "collision/CollisionResolver.hpp"

void CollisionResolver::resolveCollisions(std::vector<RigidBodyContact> collisions)
{
    const int maxIterations = collisions.size() * 2.0f;

    for (int i = 0 ; i < collisions.size(); ++i)
    {
        collisions[i].calculateInternals();
    }

    int i = 0;
    while (i < maxIterations && i < collisions.size())
    {
        // Find contact with the biggest penetration
        int indexMaxPenetration = -1;
        float maxPenetration = 0;
        for (int j = 0; j < collisions.size(); ++j)
        {
            if (collisions[j].getPenetration() > maxPenetration)
            {
                maxPenetration = collisions[j].getPenetration();
                indexMaxPenetration = j;
            }
        }

        if (indexMaxPenetration == -1) break;

        collisions[indexMaxPenetration].applyPositionChange();
        // updatePenetrations(collisions, indexMaxPenetration);
        collisions[indexMaxPenetration].applyVelocityChange();
        i++;

        // TODO : remove next line when updatePenetrations is fixed
        collisions.erase(collisions.begin() + indexMaxPenetration);
    }
}

void CollisionResolver::updatePenetrations(std::vector<RigidBodyContact> collisions, int indexMaxPenetration)
{
    Vector3f cp;

    for (int i = 0; i < collisions.size(); ++i)
    {
        if (collisions[i].getRigidbodies()[0])
        {
            if (collisions[i].getRigidbodies()[0] == collisions[indexMaxPenetration].getRigidbodies()[0])
            {
                cp = collisions[indexMaxPenetration].getRotationAmount()[0].crossProduct(collisions[i].getRelativeContactPosition()[0]);

                cp += collisions[indexMaxPenetration].getLinearMove()[0] * collisions[indexMaxPenetration].getNormal();

                collisions[i].setPenetration(collisions[i].getPenetration() - cp.dotProduct(collisions[i].getNormal()));
            }
            else if (collisions[i].getRigidbodies()[0] == collisions[indexMaxPenetration].getRigidbodies()[1])
            {
                cp = collisions[indexMaxPenetration].getRotationAmount()[1].crossProduct(collisions[i].getRelativeContactPosition()[0]);

                cp += collisions[indexMaxPenetration].getLinearMove()[1] * collisions[indexMaxPenetration].getNormal();

                collisions[i].setPenetration(collisions[i].getPenetration() - cp.dotProduct(collisions[i].getNormal()));
            }
        }
        if (collisions[i].getRigidbodies()[1])
        {
            if (collisions[i].getRigidbodies()[1] == collisions[indexMaxPenetration].getRigidbodies()[0])
            {
                cp = collisions[indexMaxPenetration].getRotationAmount()[0].crossProduct(collisions[i].getRelativeContactPosition()[1]);

                cp += collisions[indexMaxPenetration].getLinearMove()[0] * collisions[indexMaxPenetration].getNormal();

                collisions[i].setPenetration(collisions[i].getPenetration() - cp.dotProduct(collisions[i].getNormal()));
            }
            else if (collisions[i].getRigidbodies()[1] == collisions[indexMaxPenetration].getRigidbodies()[1])
            {
                cp = collisions[indexMaxPenetration].getRotationAmount()[1].crossProduct(collisions[i].getRelativeContactPosition()[1]);

                cp += collisions[indexMaxPenetration].getLinearMove()[1] * collisions[indexMaxPenetration].getNormal();

                collisions[i].setPenetration(collisions[i].getPenetration() - cp.dotProduct(collisions[i].getNormal()));
            }
        }
    }
}