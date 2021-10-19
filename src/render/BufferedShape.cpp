#include "render/BufferedShape.hpp"

#include "render/GraphicsEngine.hpp"

BufferedShape::BufferedShape(const GraphicsEngine & vulkanApplication, const std::vector<Vertex> & vertices, const std::vector<uint32_t> & indices) :
    m_indexSize{indices.size()},
    m_position{0.0f, 0.0f, 0.0f},
    m_rotation{1.0f},
    m_scale{1.0f, 1.0f, 1.0f}
{
    {
        vk::DeviceSize bufferSize = sizeof(Vertex) * vertices.size();

        // Create staging buffer
        std::shared_ptr<vk::raii::Buffer> staging = vulkanApplication.makeBuffer(bufferSize, vk::BufferUsageFlagBits::eTransferSrc);
        std::shared_ptr<vk::raii::DeviceMemory> stagingMemory = vulkanApplication.makeDeviceMemory(staging->getMemoryRequirements(), vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent);
        staging->bindMemory(**stagingMemory, 0);

        // Copy vertex data into staging buffer
        void* data_dst = stagingMemory->mapMemory(0, bufferSize);
            memcpy(data_dst, vertices.data(), bufferSize);
        stagingMemory->unmapMemory();

        // Create vertex buffer
        m_vertexBuffer = vulkanApplication.makeBuffer(bufferSize, vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eVertexBuffer);
        m_vertexDeviceMemory = vulkanApplication.makeDeviceMemory(m_vertexBuffer->getMemoryRequirements(), vk::MemoryPropertyFlagBits::eDeviceLocal);
        m_vertexBuffer->bindMemory(**m_vertexDeviceMemory, 0);

        // Copy staging buffer data into vertex buffer
        vulkanApplication.copyBuffer(*staging, *m_vertexBuffer, bufferSize);
    }
    {
        vk::DeviceSize bufferSize = sizeof(uint32_t) * indices.size();

        // Create staging buffer
        std::shared_ptr<vk::raii::Buffer> staging = vulkanApplication.makeBuffer(bufferSize, vk::BufferUsageFlagBits::eTransferSrc);
        std::shared_ptr<vk::raii::DeviceMemory> stagingMemory = vulkanApplication.makeDeviceMemory(staging->getMemoryRequirements(), vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent);
        staging->bindMemory(**stagingMemory, 0);
        
        // Copy index data into staging buffer
        void* data_dst = stagingMemory->mapMemory(0, bufferSize);
            memcpy(data_dst, indices.data(), bufferSize);
        stagingMemory->unmapMemory();

        // Create index buffer
        m_indexBuffer = vulkanApplication.makeBuffer(bufferSize, vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eIndexBuffer);
        m_indexDeviceMemory = vulkanApplication.makeDeviceMemory(m_indexBuffer->getMemoryRequirements(), vk::MemoryPropertyFlagBits::eDeviceLocal);
        m_indexBuffer->bindMemory(**m_indexDeviceMemory, 0);

        // Copy staging buffer data into index buffer
        vulkanApplication.copyBuffer(*staging, *m_indexBuffer, bufferSize);
    }
}

BufferedShape::~BufferedShape()
{
}

void BufferedShape::translate(const glm::vec3 & vect)
{
    m_position += vect;
}

void BufferedShape::rotate(float angle, const glm::vec3 & axis)
{
    m_rotation = glm::rotate(m_rotation, angle, glm::normalize(axis));
}

void BufferedShape::scale(const glm::vec3 & vect)
{
    m_scale.x *= vect.x;
    m_scale.y *= vect.y;
    m_scale.z *= vect.z;
}

void BufferedShape::scale(float factor)
{
    m_scale *= factor;
}

void BufferedShape::setScale(const glm::vec3 & vect)
{
    m_scale = vect;
}

void BufferedShape::setScale(float factor)
{
    m_scale = glm::vec3(1.f) * factor;
}

glm::mat4 BufferedShape::getTransform() const
{
    glm::mat4 transform{1.0f};
    transform = glm::translate(transform, m_position);
    transform *= m_rotation;
    transform = glm::scale(transform, m_scale);
    return transform;
}