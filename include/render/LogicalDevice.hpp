#ifndef FNES_LOGICALDEVICE
#define FNES_LOGICALDEVICE

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <stdexcept>
#include <vector>
#include <set>
#include <memory>

#include "render/PhysicalDevice.hpp"
#include "render/GraphicsQueue.hpp"
#include "render/PresentQueue.hpp"

class LogicalDevice
{
private:
    std::shared_ptr<PhysicalDevice> m_physicalDevice;

    VkDevice m_device;

    GraphicsQueue m_graphicsQueue;
    PresentQueue  m_presentQueue;
    
public:
    LogicalDevice(const std::shared_ptr<PhysicalDevice> & physicalDevice);
    virtual ~LogicalDevice();

    VkResult waitIdle();

    inline GraphicsQueue & getGraphicsQueue() { return m_graphicsQueue; }
    inline PresentQueue &  getPresentQueue()  { return m_presentQueue;  }

    inline std::shared_ptr<PhysicalDevice> & getPhysicalDevice() { return m_physicalDevice; }

    inline VkDevice & raw() { return m_device; }
};

#endif // FNES_LOGICALDEVICE
