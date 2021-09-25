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

    glm::vec3 m_position;
    glm::mat4 m_rotation;
    glm::vec3 m_scale;

    size_t m_indexSize;
    
public:
    Entity(const std::shared_ptr<LogicalDevice> & logicalDevice, const std::shared_ptr<CommandPool> & commandPool, const std::vector<Vertex> & vertices, const std::vector<uint32_t> & indices);
    virtual ~Entity();

    inline void setPosition(const glm::vec3 & vect) { m_position = vect; }
    void translate(const glm::vec3 & vect);
    void rotate(float angle, const glm::vec3 & axis);
    void scale(const glm::vec3 & vect);
    void scale(float factor);

    inline const std::unique_ptr<Buffer> & getVertexBuffer() const { return m_vertexBuffer; }
    inline const std::unique_ptr<Buffer> & getIndexBuffer() const { return m_indexBuffer;  }
    inline size_t getIndexBufferSize() const { return m_indexSize;  }
    glm::mat4 getTransform() const;
};

#endif // MPJVP_ENTITY
