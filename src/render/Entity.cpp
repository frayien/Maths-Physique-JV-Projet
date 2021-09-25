#include "render/Entity.hpp"

Entity::Entity(const std::shared_ptr<LogicalDevice> & logicalDevice, const std::shared_ptr<CommandPool> & commandPool, const std::vector<Vertex> & vertices, const std::vector<uint32_t> & indices) :
    m_indexSize{indices.size()},
    m_position{0.0f, 0.0f, 0.0f},
    m_rotation{1.0f},
    m_scale{1.0f, 1.0f, 1.0f}
{
    {
        VkDeviceSize bufferSize = sizeof(Vertex) * vertices.size();

        // Create staging buffer
        Buffer staging{logicalDevice, bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT};

        // Copy vertices data into staging buffer
        staging.loadData(vertices.data(), (size_t) bufferSize);

        // Create vertex buffer
        m_vertexBuffer = std::make_unique<Buffer>(logicalDevice, bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

        // Copy staging buffer data into vertex buffer
        commandPool->copyBuffer(staging, *m_vertexBuffer, bufferSize);
    }
    {
        VkDeviceSize bufferSize = sizeof(uint32_t) * indices.size();

        // Create staging buffer
        Buffer staging{logicalDevice, bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT};

        // Copy vertices data into staging buffer
        staging.loadData(indices.data(), (size_t) bufferSize);

        // Create index buffer
        m_indexBuffer = std::make_unique<Buffer>(logicalDevice, bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

        // Copy staging buffer data into index buffer
        commandPool->copyBuffer(staging, *m_indexBuffer, bufferSize);
    }
}

Entity::~Entity()
{
}

void Entity::translate(const glm::vec3 & vect)
{
    m_position += vect;
}

void Entity::rotate(float angle, const glm::vec3 & axis)
{
    m_rotation = glm::rotate(m_rotation, angle, glm::normalize(axis));
}

void Entity::scale(const glm::vec3 & vect)
{
    m_scale.x *= vect.x;
    m_scale.y *= vect.y;
    m_scale.z *= vect.z;
}

void Entity::scale(float factor)
{
    m_scale *= factor;
}

glm::mat4 Entity::getTransform() const
{
    glm::mat4 transform{1.0f};
    transform = glm::translate(transform, m_position);
    transform *= m_rotation;
    transform = glm::scale(transform, m_scale);
    return transform;
}