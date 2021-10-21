#ifndef MPJVP_BUFFEREDSHAPES
#define MPJVP_BUFFEREDSHAPES

#include <vulkan/vulkan_raii.hpp>

#include "render/Shape.hpp"

class GraphicsEngine;

class BufferedShapes
{
private:
    std::unique_ptr<vk::raii::Buffer> m_vertexBuffer;
    std::unique_ptr<vk::raii::DeviceMemory> m_vertexDeviceMemory;
    std::unique_ptr<vk::raii::Buffer> m_indexBuffer;
    std::unique_ptr<vk::raii::DeviceMemory> m_indexDeviceMemory;

    std::size_t m_indexSize;

public:
    BufferedShapes(const GraphicsEngine & graphicsEngine, const std::vector<Shape> & shapes);
    virtual ~BufferedShapes();

    inline const vk::raii::Buffer & getVertexBuffer() const { return *m_vertexBuffer; }
    inline const vk::raii::Buffer & getIndexBuffer() const  { return *m_indexBuffer;  }
    inline size_t getIndexBufferSize() const { return m_indexSize;  }
};

#endif // MPJVP_BUFFEREDSHAPES
