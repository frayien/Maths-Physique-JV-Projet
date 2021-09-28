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

    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) const;
    VkFormat findDepthFormat() const;
    VkDeviceSize getMinUniformBufferOffsetAlignment() const;

    inline VkPhysicalDevice & raw() { return m_physicalDevice; }

private:
    VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features) const;
};



#endif // MPJVP_PHYSICALDEVICE
