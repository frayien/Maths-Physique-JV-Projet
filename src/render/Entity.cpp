#include "render/Entity.hpp"

Entity::Entity(const std::shared_ptr<LogicalDevice> & logicalDevice, const std::shared_ptr<CommandPool> & commandPool, PhysicalDevice & physicalDevice, const std::vector<Vertex> & vertices, const std::vector<uint32_t> & indices)
{
    {
        VkDeviceSize bufferSize = sizeof(Vertex) * vertices.size();

        // Create staging buffer
        Buffer staging{logicalDevice, commandPool, physicalDevice, bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT};

        // Copy vertices data into staging buffer
        staging.loadData(vertices.data(), (size_t) bufferSize);

        // Create vertex buffer
        m_vertexBuffer = std::make_unique<Buffer>(logicalDevice, commandPool, physicalDevice, bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

        // Copy staging buffer data into vertex buffer
        m_vertexBuffer->copyFrom(staging, bufferSize);
    }
    {
        VkDeviceSize bufferSize = sizeof(uint32_t) * indices.size();

        // Create staging buffer
        Buffer staging{logicalDevice, commandPool, physicalDevice, bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT};

        // Copy vertices data into staging buffer
        staging.loadData(indices.data(), (size_t) bufferSize);

        // Create index buffer
        m_indexBuffer = std::make_unique<Buffer>(logicalDevice, commandPool, physicalDevice, bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

        // Copy staging buffer data into index buffer
        m_indexBuffer->copyFrom(staging, bufferSize);
    }
}

Entity::~Entity()
{
}