#include "render/LogicalDevice.hpp"

LogicalDevice::LogicalDevice(const std::shared_ptr<PhysicalDevice> & physicalDevice) :
    m_physicalDevice{physicalDevice}
{
    QueueFamilyIndices indices = m_physicalDevice->getQueueFamilies();

    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    std::set<uint32_t> uniqueQueueFamilies = {indices.graphicsFamily.value(), indices.presentFamily.value()};

    float queuePriority = 1.0f;
    for (uint32_t queueFamily : uniqueQueueFamilies) {
        VkDeviceQueueCreateInfo queueCreateInfo{};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = queueFamily;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;
        queueCreateInfos.push_back(queueCreateInfo);
    }

    VkPhysicalDeviceFeatures deviceFeatures{};
    deviceFeatures.sampleRateShading = VK_TRUE;

    VkDeviceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
    createInfo.pQueueCreateInfos = queueCreateInfos.data();

    createInfo.pEnabledFeatures = &deviceFeatures;

    // extensions
    createInfo.enabledExtensionCount = static_cast<uint32_t>(m_physicalDevice->getDeviceExtensions().size());
    createInfo.ppEnabledExtensionNames = m_physicalDevice->getDeviceExtensions().data();

    if (vkCreateDevice(m_physicalDevice->raw(), &createInfo, nullptr, &m_device) != VK_SUCCESS) 
    {
        throw std::runtime_error("failed to create logical device!");
    }

    // queues are created along with the logical device, we just need to query for them
    vkGetDeviceQueue(m_device, indices.graphicsFamily.value(), 0, &(m_graphicsQueue.raw()) );
    vkGetDeviceQueue(m_device, indices.presentFamily .value(), 0, &(m_presentQueue .raw()) );
}

LogicalDevice::~LogicalDevice()
{
    vkDestroyDevice(m_device, nullptr);
}

VkResult LogicalDevice::waitIdle()
{
    return vkDeviceWaitIdle(m_device);
}