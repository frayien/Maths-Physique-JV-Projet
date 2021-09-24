#ifndef MPJVP_ENTITY
#define MPJVP_ENTITY

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "render/LogicalDevice.hpp"
#include "render/CommandPool.hpp"

#include "render/Buffer.hpp"
#include "render/Vertex.hpp"

class Entity
{
private:
    std::unique_ptr<Buffer> m_vertexBuffer;
    std::unique_ptr<Buffer> m_indexBuffer;

    glm::mat4 m_transform;

    size_t m_indexSize;
    
public:
    Entity(const std::shared_ptr<LogicalDevice> & logicalDevice, const std::shared_ptr<CommandPool> & commandPool, const std::vector<Vertex> & vertices, const std::vector<uint32_t> & indices);
    virtual ~Entity();

    void translate(const glm::vec3 & vect);
    void rotate(float angle, const glm::vec3 & axis);
    void scale(const glm::vec3 & vect);
    void scale(float factor);

    inline const std::unique_ptr<Buffer> & getVertexBuffer() const { return m_vertexBuffer; }
    inline const std::unique_ptr<Buffer> & getIndexBuffer() const { return m_indexBuffer;  }
    inline size_t getIndexBufferSize() const { return m_indexSize;  }
    inline const glm::mat4 & getTransform() const { return m_transform; }
};

#endif // MPJVP_ENTITY
