#ifndef MPJVP_SHAPE
#define MPJVP_SHAPE

#include <vector>

#include "render/Vertex.hpp"

struct Shape
{   
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    glm::mat4 transform;
};


#endif // MPJVP_SHAPE
