#ifndef MPJVP_PHYSICALDEVICE
#define MPJVP_PHYSICALDEVICE

#include <vulkan/vulkan_raii.hpp>

#include <vector>
#include <memory>
#include <set>
#include <string>
#include <optional>

class PhysicalDevice
{
public:
{
    
    VkDeviceSize getMinUniformBufferOffsetAlignment() const;

    inline VkPhysicalDevice & raw() { return m_physicalDevice; }

private:
};



#endif // MPJVP_PHYSICALDEVICE
