#include "render/SwapChain.hpp"

SwapChain::SwapChain(const std::shared_ptr<Window> & window, const std::shared_ptr<Surface> & surface, const std::shared_ptr<PhysicalDevice> & physicalDevice, const std::shared_ptr<LogicalDevice> & logicalDevice) :
    m_window{window},
    m_surface{surface},
    m_physicalDevice{physicalDevice},
    m_logicalDevice{logicalDevice}
{
    create();
    m_renderPass = std::make_shared<RenderPass>(m_logicalDevice, getImageFormat(), m_physicalDevice->findDepthFormat(), m_physicalDevice->getMsaaSampleCount());
    m_descriptorSetLayout = std::make_shared<DescriptorSetLayout>(m_logicalDevice);
    m_graphicsPipeline = std::make_shared<GraphicsPipeline>(m_logicalDevice, m_renderPass, m_descriptorSetLayout, getExtent(), m_physicalDevice->getMsaaSampleCount());
    m_commandPool = std::make_shared<CommandPool>(m_logicalDevice, physicalDevice->getQueueFamilies());

    {
        VkFormat colorFormat = getImageFormat();
        m_colorImage = std::make_shared<Image>(m_logicalDevice, *m_physicalDevice, getExtent().width, getExtent().height, m_physicalDevice->getMsaaSampleCount(), colorFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
        m_colorImageView = std::make_shared<ImageView>(m_logicalDevice, m_colorImage->raw(), colorFormat, VK_IMAGE_ASPECT_COLOR_BIT);
    }
    {
        VkFormat depthFormat = m_physicalDevice->findDepthFormat();
        m_depthImage = std::make_shared<Image>(m_logicalDevice, *m_physicalDevice, getExtent().width, getExtent().height, m_physicalDevice->getMsaaSampleCount(), depthFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
        m_depthImageView = std::make_shared<ImageView>(m_logicalDevice, m_depthImage->raw(), depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT);
    }

    for (std::shared_ptr<ImageView> & imageView : m_imageViews)
    {
        m_frameBuffers.push_back(std::make_shared<FrameBuffer>(m_logicalDevice, m_renderPass, m_colorImageView, m_depthImageView, imageView, getExtent()));
    }

    m_entities.emplace_back(m_logicalDevice, m_commandPool, *m_physicalDevice, 
    std::vector<Vertex>({
        {{-0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}},
        {{0.5f , -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}},
        {{0.5f ,  0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}},
        {{-0.5f,  0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}},

        {{-0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
        {{0.5f , -0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}},
        {{0.5f ,  0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
        {{-0.5f,  0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
    }),
    std::vector<uint32_t>({
        0, 1, 2, 2, 3, 0,
        4, 5, 6, 6, 7, 4,
    }));

    {
        VkDeviceSize bufferSize = sizeof(UniformBufferObject);
        for (std::shared_ptr<ImageView> & imageView : m_imageViews)
        {
            m_uniformBuffers.push_back(std::make_shared<Buffer>(m_logicalDevice, m_commandPool, *m_physicalDevice, bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT));
        }
    }

    m_descriptorPool = std::make_shared<DescriptorPool>(m_logicalDevice, m_imageViews.size());

    std::vector<VkDescriptorSet> descriptorSets = m_descriptorPool->createDescriptorSets(m_uniformBuffers, m_descriptorSetLayout->raw());
    m_commandBuffers = std::make_shared<CommandBuffers>(m_logicalDevice, m_commandPool, m_imageViews.size());
    for(size_t i = 0; i < m_commandBuffers->size(); ++i)
    {
        CommandBuffer commandBuffer = (*m_commandBuffers)[i];
        commandBuffer.record(*m_renderPass, *(m_frameBuffers[i]), *m_graphicsPipeline, getExtent(), descriptorSets[i], m_entities);
    }
}

SwapChain::~SwapChain()
{
    cleanup();
}

void SwapChain::recreate()
{
    int width = 0, height = 0;
    while (width == 0 || height == 0)
    {
        auto [nwidth, nheight] = m_window->getFramebufferSize();
        width = nwidth; height = nheight;
        Window::waitEvents();
    }

    m_logicalDevice->waitIdle();

    cleanup();
    
    create();
    m_renderPass = std::make_shared<RenderPass>(m_logicalDevice, getImageFormat(), m_physicalDevice->findDepthFormat(), m_physicalDevice->getMsaaSampleCount());
    m_graphicsPipeline = std::make_shared<GraphicsPipeline>(m_logicalDevice, m_renderPass, m_descriptorSetLayout, getExtent(), m_physicalDevice->getMsaaSampleCount());

    {
        VkFormat colorFormat = getImageFormat();
        m_colorImage = std::make_shared<Image>(m_logicalDevice, *m_physicalDevice, getExtent().width, getExtent().height, m_physicalDevice->getMsaaSampleCount(), colorFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
        m_colorImageView = std::make_shared<ImageView>(m_logicalDevice, m_colorImage->raw(), colorFormat, VK_IMAGE_ASPECT_COLOR_BIT);
    }
    {
        VkFormat depthFormat = m_physicalDevice->findDepthFormat();
        m_depthImage = std::make_shared<Image>(m_logicalDevice, *m_physicalDevice, getExtent().width, getExtent().height, m_physicalDevice->getMsaaSampleCount(), depthFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
        m_depthImageView = std::make_shared<ImageView>(m_logicalDevice, m_depthImage->raw(), depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT);
    }

    for (std::shared_ptr<ImageView> & imageView : m_imageViews)
    {
        m_frameBuffers.push_back(std::make_shared<FrameBuffer>(m_logicalDevice, m_renderPass, m_colorImageView, m_depthImageView, imageView, getExtent()));
    }

    {
        VkDeviceSize bufferSize = sizeof(UniformBufferObject);
        for (std::shared_ptr<ImageView> & imageView : m_imageViews)
        {
            m_uniformBuffers.push_back(std::make_shared<Buffer>(m_logicalDevice, m_commandPool, *m_physicalDevice, bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT));
        }
    }

    m_descriptorPool = std::make_shared<DescriptorPool>(m_logicalDevice, m_imageViews.size());

    std::vector<VkDescriptorSet> descriptorSets = m_descriptorPool->createDescriptorSets(m_uniformBuffers, m_descriptorSetLayout->raw());
    m_commandBuffers = std::make_shared<CommandBuffers>(m_logicalDevice, m_commandPool, m_imageViews.size());
    for(size_t i = 0; i < m_commandBuffers->size(); ++i)
    {
        CommandBuffer commandBuffer = (*m_commandBuffers)[i];
        commandBuffer.record(*m_renderPass, *(m_frameBuffers[i]), *m_graphicsPipeline, getExtent(), descriptorSets[i], m_entities);
    }
}

void SwapChain::create()
{
    SwapChainSupportDetails swapChainSupport = m_physicalDevice->getSwapChainSupport();

    VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
    VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
    VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities);

    m_imageFormat = surfaceFormat.format;
    m_extent = extent;

    uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
    if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount)
    {
        imageCount = swapChainSupport.capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = m_surface->raw();

    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;


    QueueFamilyIndices indices = m_physicalDevice->getQueueFamilies();
    uint32_t queueFamilyIndices[] = {indices.graphicsFamily.value(), indices.presentFamily.value()};

    if (indices.graphicsFamily != indices.presentFamily)
    {
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueFamilyIndices;
    }
    else
    {
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        createInfo.queueFamilyIndexCount = 0;
        createInfo.pQueueFamilyIndices = nullptr;
    }

    createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.presentMode = presentMode;
    createInfo.clipped = VK_TRUE;
    createInfo.oldSwapchain = VK_NULL_HANDLE;

    if (vkCreateSwapchainKHR(m_logicalDevice->raw(), &createInfo, nullptr, &m_swapChain) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create swap chain!");
    }

    // retrive the created swap chain
    vkGetSwapchainImagesKHR(m_logicalDevice->raw(), m_swapChain, &imageCount, nullptr);
    std::vector<VkImage> images(imageCount);
    vkGetSwapchainImagesKHR(m_logicalDevice->raw(), m_swapChain, &imageCount, images.data());

    for(VkImage & img : images)
    {
        m_imageViews.push_back(std::make_shared<ImageView>(m_logicalDevice, img, getImageFormat(), VK_IMAGE_ASPECT_COLOR_BIT));
    }
}

void SwapChain::cleanup()
{
    m_descriptorPool.reset();

    m_uniformBuffers.clear();
    m_frameBuffers.clear();

    m_depthImageView.reset();
    m_depthImage.reset();

    m_colorImageView.reset();
    m_colorImage.reset();

    m_graphicsPipeline.reset();
    m_renderPass.reset();
    m_imageViews.clear();

    vkDestroySwapchainKHR(m_logicalDevice->raw(), m_swapChain, nullptr);
}

VkSurfaceFormatKHR SwapChain::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) 
{
    for (const auto& availableFormat : availableFormats)
    {
        if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
        {
            return availableFormat;
        }
    }

    return availableFormats[0];
}

VkPresentModeKHR SwapChain::chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes)
{
    for (const auto& availablePresentMode : availablePresentModes)
    {
        if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
        {
            return availablePresentMode;
        }
    }

    return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D SwapChain::chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities)
{
    if (capabilities.currentExtent.width != UINT32_MAX)
    {
        return capabilities.currentExtent;
    }
    else
    {
        auto [width, height] = m_window->getFramebufferSize();

        VkExtent2D actualExtent =
        {
            static_cast<uint32_t>(width),
            static_cast<uint32_t>(height)
        };

        actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
        actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

        return actualExtent;
    }
}