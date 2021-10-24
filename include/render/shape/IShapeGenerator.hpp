#ifndef MPJVP_ISHAPEGENERATOR
#define MPJVP_ISHAPEGENERATOR

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include <memory>
 
#include "render/Shape.hpp"

class IShapeGenerator
{
public:
    IShapeGenerator() = default;
    virtual ~IShapeGenerator() = default;

    virtual void addShape(std::vector<Shape> & shapes) = 0;
};


#endif // MPJVP_ISHAPEGENERATOR
