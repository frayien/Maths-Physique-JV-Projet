#ifndef MPJVP_COMMANDBUFFERS
#define MPJVP_COMMANDBUFFERS

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "render/CommandBuffer.hpp"

#include "render/LogicalDevice.hpp"
#include "render/CommandPool.hpp"

class CommandBuffers
{
private:
    std::shared_ptr<LogicalDevice> m_logicalDevice;
    std::shared_ptr<CommandPool> m_commandPool;

    std::vector<VkCommandBuffer> m_commandBuffers;
    
public:
    CommandBuffers(const std::shared_ptr<LogicalDevice> & logicalDevice, const std::shared_ptr<CommandPool> & commandPool, size_t size);
    virtual ~CommandBuffers();

    void recreate(size_t size);

    inline CommandBuffer operator[](size_t i) { return CommandBuffer(m_commandBuffers[i]); }
    inline size_t size() const { return m_commandBuffers.size(); }

private:
    void cleanup();
    void create(size_t size);
};

#endif // MPJVP_COMMANDBUFFERS
