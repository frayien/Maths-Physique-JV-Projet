#ifndef MPJVP_BUFFEREDSHAPE
#define MPJVP_BUFFEREDSHAPE

#include <vulkan/vulkan_raii.hpp>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "render/Vertex.hpp"

class VulkanApplication;

class BufferedShape
{
private:
    std::shared_ptr<vk::raii::Buffer> m_vertexBuffer;
    std::shared_ptr<vk::raii::DeviceMemory> m_vertexDeviceMemory;
    std::shared_ptr<vk::raii::Buffer> m_indexBuffer;
    std::shared_ptr<vk::raii::DeviceMemory> m_indexDeviceMemory;

    glm::vec3 m_position;
    glm::mat4 m_rotation;
    glm::vec3 m_scale;

    size_t m_indexSize;
    
public:
    BufferedShape(const VulkanApplication & vulkanApplication, const std::vector<Vertex> & vertices, const std::vector<uint32_t> & indices);
    virtual ~BufferedShape();

    inline void setPosition(const glm::vec3 & vect) { m_position = vect; }
    void translate(const glm::vec3 & vect);
    void rotate(float angle, const glm::vec3 & axis);
    void scale(const glm::vec3 & vect);
    void scale(float factor);

    inline const std::shared_ptr<vk::raii::Buffer> & getVertexBuffer() const { return m_vertexBuffer; }
    inline const std::shared_ptr<vk::raii::Buffer> & getIndexBuffer() const { return m_indexBuffer;  }
    inline size_t getIndexBufferSize() const { return m_indexSize;  }
    glm::mat4 getTransform() const;
};

#endif // MPJVP_BUFFEREDSHAPE
