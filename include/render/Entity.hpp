#ifndef FNES_ENTITY
#define FNES_ENTITY

#include "render/PhysicalDevice.hpp"
#include "render/LogicalDevice.hpp"
#include "render/CommandPool.hpp"

#include "render/Buffer.hpp"
#include "render/Vertex.hpp"

class Entity
{
private:
    std::unique_ptr<Buffer> m_vertexBuffer;
    std::unique_ptr<Buffer> m_indexBuffer;

    size_t m_indexSize;
    
public:
    Entity(const std::shared_ptr<LogicalDevice> & logicalDevice, const std::shared_ptr<CommandPool> & commandPool, PhysicalDevice & physicalDevice, const std::vector<Vertex> & vertices, const std::vector<uint32_t> & indices);
    virtual ~Entity();

    Entity(Entity&&) = default;

    inline std::unique_ptr<Buffer> & getVertexBuffer() { return m_vertexBuffer; }
    inline std::unique_ptr<Buffer> & getIndexBuffer()  { return m_indexBuffer;  }
    inline size_t getIndexBufferSize()  { return m_indexSize;  }
};

#endif // FNES_ENTITY
