#include "render/BufferedShapes.hpp"

#include "render/GraphicsEngine.hpp"

BufferedShapes::BufferedShapes(const GraphicsEngine & graphicsEngine, const std::vector<Shape> & shapes)
{
    std::size_t verticeN = 0;
    std::size_t indexN = 0;
    for(const Shape & shape : shapes)
    {
        verticeN += shape.vertices.size();
        indexN += shape.indices.size();
    }

    {
        vk::DeviceSize bufferSize = sizeof(Vertex) * verticeN;

        // Create staging buffer
        std::unique_ptr<vk::raii::Buffer> staging = graphicsEngine.makeBuffer(bufferSize, vk::BufferUsageFlagBits::eTransferSrc);
        std::shared_ptr<vk::raii::DeviceMemory> stagingMemory = graphicsEngine.makeDeviceMemory(staging->getMemoryRequirements(), vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent);
        staging->bindMemory(**stagingMemory, 0);

        // Copy vertex data into staging buffer
        void* data_dst = stagingMemory->mapMemory(0, bufferSize);
            std::size_t vertexOff = 0;
            for(std::size_t i = 0; i < shapes.size(); ++i)
            {
                memcpy(static_cast<Vertex*>(data_dst) + vertexOff, shapes[i].vertices.data(), shapes[i].vertices.size() * sizeof(Vertex));
                vertexOff += shapes[i].vertices.size();
            }
        stagingMemory->unmapMemory();

        // Create vertex buffer
        m_vertexBuffer = graphicsEngine.makeBuffer(bufferSize, vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eVertexBuffer);
        m_vertexDeviceMemory = graphicsEngine.makeDeviceMemory(m_vertexBuffer->getMemoryRequirements(), vk::MemoryPropertyFlagBits::eDeviceLocal);
        m_vertexBuffer->bindMemory(**m_vertexDeviceMemory, 0);

        // Copy staging buffer data into vertex buffer
        graphicsEngine.copyBuffer(*staging, *m_vertexBuffer, bufferSize);
    }
    {
        vk::DeviceSize bufferSize = sizeof(uint32_t) * indexN;

        // Create staging buffer
        std::shared_ptr<vk::raii::Buffer> staging = graphicsEngine.makeBuffer(bufferSize, vk::BufferUsageFlagBits::eTransferSrc);
        std::shared_ptr<vk::raii::DeviceMemory> stagingMemory = graphicsEngine.makeDeviceMemory(staging->getMemoryRequirements(), vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent);
        staging->bindMemory(**stagingMemory, 0);
        
        // Copy index data into staging buffer
        void* data_dst = stagingMemory->mapMemory(0, bufferSize);
            std::size_t indexOff = 0;
            for(std::size_t i = 0; i < shapes.size(); ++i)
            {
                memcpy(static_cast<uint32_t*>(data_dst) + indexOff, shapes[i].indices.data(), shapes[i].indices.size() * sizeof(uint32_t));
                indexOff += shapes[i].indices.size();
            }
        stagingMemory->unmapMemory();

        // Create index buffer
        m_indexBuffer = graphicsEngine.makeBuffer(bufferSize, vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eIndexBuffer);
        m_indexDeviceMemory = graphicsEngine.makeDeviceMemory(m_indexBuffer->getMemoryRequirements(), vk::MemoryPropertyFlagBits::eDeviceLocal);
        m_indexBuffer->bindMemory(**m_indexDeviceMemory, 0);

        // Copy staging buffer data into index buffer
        graphicsEngine.copyBuffer(*staging, *m_indexBuffer, bufferSize);
    }
}

BufferedShapes::~BufferedShapes()
{
}