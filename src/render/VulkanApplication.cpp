#include "render/VulkanApplication.hpp"

VulkanApplication::VulkanApplication(const std::shared_ptr<IApplication> & application) :
    m_application{application}
{
    initWindow();
    initContext();
    initInstance();
    initSurface();
    initPhysicalDevice();
    initDevice();
    initCommandPool();
    initWorld();
    initDescriptorSetLayout();
    initRenderPass();
    initGraphicsPipeline();
    initFramebuffers();
    initDescriptorPool();
    initDescriptorSets();
    initCommandBuffers();

    for(size_t i = 0; i < m_commandBuffers->size(); ++i)
    {
        recordCommandBufferForTheFirstTime(i, *m_world);
    }

    //m_application->init(*m_world);

    //m_swapChain      = std::make_shared<SwapChain>     (m_window, m_surface, m_physicalDevice, m_logicalDevice, m_commandPool, m_world);

    //m_imGuiVulkan    = std::make_shared<ImGuiVulkan>   (m_application, m_window, m_instance, m_physicalDevice, m_logicalDevice, m_swapChain);

    //m_imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    //m_renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    //m_inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);
    //m_imagesInFlight.resize(m_swapChain->size(), VK_NULL_HANDLE);

    //VkSemaphoreCreateInfo semaphoreInfo{};
    //semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    //VkFenceCreateInfo fenceInfo{};
    //fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    //fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    //for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
    //{
    //    if (vkCreateSemaphore(m_logicalDevice->raw(), &semaphoreInfo, nullptr, &m_imageAvailableSemaphores[i]) != VK_SUCCESS ||
    //        vkCreateSemaphore(m_logicalDevice->raw(), &semaphoreInfo, nullptr, &m_renderFinishedSemaphores[i]) != VK_SUCCESS ||
    //        vkCreateFence(m_logicalDevice->raw(), &fenceInfo, nullptr, &m_inFlightFences[i]) != VK_SUCCESS)
    //    {
    //        
    //        throw std::runtime_error("failed to create synchronization objects for a frame!");
    //    }
    //}

    //m_needRecord.resize(m_swapChain->size(), false);
}

VulkanApplication::~VulkanApplication()
{
    //for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
    //{
    //    vkDestroySemaphore(m_logicalDevice->raw(), m_renderFinishedSemaphores[i], nullptr);
    //    vkDestroySemaphore(m_logicalDevice->raw(), m_imageAvailableSemaphores[i], nullptr);
    //    vkDestroyFence(m_logicalDevice->raw(), m_inFlightFences[i], nullptr);
    //}
}

void VulkanApplication::run()
{
    while (!m_window->shouldClose()) 
    {
        Window::pollEvents();

        drawFrame();
    }

    //m_logicalDevice->waitIdle();
}

void VulkanApplication::update(uint32_t currentImage)
{
    /*
    static auto previousTime = std::chrono::high_resolution_clock::now();

    auto currentTime = std::chrono::high_resolution_clock::now();
    float deltaTime = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - previousTime).count();
    previousTime = currentTime;

    //m_application->update(*m_world, deltaTime);

    if(m_world->hasChanged())
    {
        for(size_t i = 0; i<m_needRecord.size(); ++i)
        {
            m_needRecord[i] = true;
        }
        m_world->setChanged(false);
    }
    if(m_needRecord[currentImage])
    {
        //m_swapChain->recordCommandBuffer(currentImage);
        m_needRecord[currentImage] = false;
    }
    //{
    //    UniformBufferObjectCamera ubo{};
    //    ubo.model = glm::mat4(1.0f);
    //    ubo.view = glm::lookAt(m_world->getCamera().getPosition(), m_world->getCamera().getPosition() + m_world->getCamera().getDirection(), glm::vec3(0.0f, 0.0f, 1.0f));
    //    ubo.proj = glm::perspective(glm::radians(45.0f), m_swapChain->getExtent().width / (float) m_swapChain->getExtent().height, 0.1f, m_world->getCamera().getViewDistance());
    //    ubo.proj[1][1] *= -1;
    //    ubo.lightPos = m_world->getLightSource().getPosition();
    //    ubo.lightColor = m_world->getLightSource().getColor();
    //    ubo.ambientLightStrength = m_world->getLightSource().getAmbient();
    //
    //    Buffer & uniformBuffer = *(m_swapChain->getCameraUniformBuffer(currentImage));
    //
    //    uniformBuffer.loadData(&ubo, sizeof(UniformBufferObjectCamera));
    //}
    //{
    //    const auto & entities = m_world->getShapes();
    //    std::vector<UniformBufferObjectTransform> ubos(entities.size());
    //    size_t i = 0;
    //    for(const auto & entity : entities)
    //    {
    //        ubos[i].transform = entity->getTransform();
    //
    //        ++i;
    //    }
    //
    //    Buffer & uniformBuffer = *(m_swapChain->getTransformsUniformBuffer(currentImage));
    //
    //    uniformBuffer.loadData(ubos.data(), entities.size() * sizeof(UniformBufferObjectTransform));
    //}

    */
}

void VulkanApplication::drawFrame()
{
    /// ////////////////// acquire image ////////////////// ///
/*
    vkWaitForFences(m_logicalDevice->raw(), 1, &m_inFlightFences[m_currentFrame], VK_TRUE, UINT64_MAX);

    uint32_t imageIndex;
    VkResult result = vkAcquireNextImageKHR(m_logicalDevice->raw(), m_swapChain->raw(), UINT64_MAX, m_imageAvailableSemaphores[m_currentFrame], VK_NULL_HANDLE, &imageIndex);
    if (result == VK_ERROR_OUT_OF_DATE_KHR)
    {
        m_swapChain->recreate();
        m_imGuiVulkan->recreate();
        return;
    } 
    else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
    {
        throw std::runtime_error("failed to acquire swap chain image!");
    }

    // Check if a previous frame is using this image (i.e. there is its fence to wait on)
    if (m_imagesInFlight[imageIndex] != VK_NULL_HANDLE)
    {
        vkWaitForFences(m_logicalDevice->raw(), 1, &m_imagesInFlight[imageIndex], VK_TRUE, UINT64_MAX);
    }
    // Mark the image as now being in use by this frame
    m_imagesInFlight[imageIndex] = m_inFlightFences[m_currentFrame];

    /// ////////////////// execute the command buffer ////////////////// ///

    update(imageIndex);

    m_imGuiVulkan->createFrame();
    m_imGuiVulkan->render(imageIndex);

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    std::array<VkCommandBuffer, 2> submitCommandBuffers = { m_swapChain->getCommandBuffer(imageIndex)->raw(), m_imGuiVulkan->getCommandBuffer(imageIndex) };

    VkSemaphore waitSemaphores[] = {m_imageAvailableSemaphores[m_currentFrame]};
    VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;
    submitInfo.commandBufferCount = static_cast<uint32_t>(submitCommandBuffers.size());;
    submitInfo.pCommandBuffers = submitCommandBuffers.data();

    VkSemaphore signalSemaphores[] = {m_renderFinishedSemaphores[m_currentFrame]};
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    vkResetFences(m_logicalDevice->raw(), 1, &m_inFlightFences[m_currentFrame]);

    if (vkQueueSubmit(m_logicalDevice->getGraphicsQueue().raw(), 1, &submitInfo, m_inFlightFences[m_currentFrame]) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to submit draw command buffer!");
    }

    /// ////////////////// return image for presentation ////////////////// ///

    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;

    VkSwapchainKHR swapChains[] = {m_swapChain->raw()};
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;
    presentInfo.pImageIndices = &imageIndex;
    presentInfo.pResults = nullptr;
    
    result = vkQueuePresentKHR(m_logicalDevice->getPresentQueue().raw(), &presentInfo);

    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || m_window->isFramebufferResized())
    {
        m_window->setFramebufferResized(false);
        m_swapChain->recreate();
        m_imGuiVulkan->recreate();
    }
    else if (result != VK_SUCCESS)
    {
        throw std::runtime_error("failed to present swap chain image!");
    }

    m_currentFrame = (m_currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
    */
}

void VulkanApplication::initWindow()
{
    m_window = std::make_shared<Window>();
}

void VulkanApplication::initContext()
{
    m_context = std::make_shared<vk::raii::Context>();
}

bool VulkanApplication::checkValidationLayerSupport() 
{
    std::vector<vk::LayerProperties> availableLayers = vk::enumerateInstanceLayerProperties();

    // check if all wanted validation layers are available
    for (const char* layerName : m_validationLayers)
    {
        bool layerFound = false;

        for (const auto & layerProperties : availableLayers)
        {
            if (strcmp(layerName, layerProperties.layerName) == 0)
            {
                layerFound = true;
                break;
            }
        }

        if (!layerFound)
        {
            return false;
        }
    }

    return true;
}

void VulkanApplication::initInstance()
{
    if (m_enableValidationLayers && !checkValidationLayerSupport()) 
    {
        throw std::runtime_error("validation layers requested, but not available!");
    }

    vk::ApplicationInfo appInfo;

    appInfo.pApplicationName = "Vulkan";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    vk::InstanceCreateInfo createInfo;
    
    createInfo.pApplicationInfo = &appInfo;

    // get vulkan extensions from glfw
    auto [glfwExtensionCount, glfwExtensions] = Window::getRequiredInstanceExtensions();
    
    createInfo.enabledExtensionCount = glfwExtensionCount;
    createInfo.ppEnabledExtensionNames = glfwExtensions;

    // parameter validation layers
    if (m_enableValidationLayers) 
    {
        createInfo.enabledLayerCount = static_cast<uint32_t>(m_validationLayers.size());
        createInfo.ppEnabledLayerNames = m_validationLayers.data();
    }
    else 
    {
        createInfo.enabledLayerCount = 0;
    }

    m_instance = std::make_shared<vk::raii::Instance>(*m_context, createInfo);
}

void VulkanApplication::initSurface()
{
    VkSurfaceKHR surface;
    VkResult result = glfwCreateWindowSurface(static_cast<VkInstance>(**m_instance), m_window->raw(), nullptr, &surface);
    if (result != VK_SUCCESS) 
    {
        const char* description;
        glfwGetError(&description);
        throw std::runtime_error("failed to create window surface! " + std::to_string(result) + " " + description);
    }

    m_surface = std::make_shared<vk::raii::SurfaceKHR>(*m_instance, surface);
}

QueueFamilyIndices VulkanApplication::findQueueFamilies(const vk::raii::PhysicalDevice & device) 
{
    QueueFamilyIndices indices;

    std::vector<vk::QueueFamilyProperties> queueFamilies = device.getQueueFamilyProperties();

    int i = 0;
    for (const auto& queueFamily : queueFamilies) 
    {
        // find graphicsFamily
        if (queueFamily.queueFlags & vk::QueueFlagBits::eGraphics) 
        {
            indices.graphicsFamily = i;
        }

        // find presentFamily
        if (device.getSurfaceSupportKHR(i, **m_surface)) 
        {
            indices.presentFamily = i;
        }

        if (indices.isComplete())
        {
            break;
        }

        i++;
    }

    return indices;
}

bool VulkanApplication::checkDeviceExtensionSupport(const vk::raii::PhysicalDevice & device)
{
    auto availableExtensions = device.enumerateDeviceExtensionProperties();

    std::set<std::string> requiredExtensions(m_deviceExtensions.begin(), m_deviceExtensions.end());

    for (const auto& extension : availableExtensions)
    {
        requiredExtensions.erase(extension.extensionName);
    }

    return requiredExtensions.empty();
}

bool VulkanApplication::isDeviceSuitable(const vk::raii::PhysicalDevice & device)
{
    QueueFamilyIndices indices = findQueueFamilies(device);

    bool extensionsSupported = checkDeviceExtensionSupport(device);

    bool swapChainAdequate = false;
    if (extensionsSupported) 
    {
        auto formats = device.getSurfaceFormatsKHR(**m_surface);
        auto presentModes = device.getSurfacePresentModesKHR(**m_surface);
        swapChainAdequate = !formats.empty() && !presentModes.empty();
    }

    return indices.isComplete() && extensionsSupported && swapChainAdequate;
}

vk::SampleCountFlagBits VulkanApplication::getMaxUsableSampleCount(const vk::raii::PhysicalDevice & device)
{
    auto physicalDeviceProperties = device.getProperties();

    vk::SampleCountFlags counts = physicalDeviceProperties.limits.framebufferColorSampleCounts & physicalDeviceProperties.limits.framebufferDepthSampleCounts;
    if (counts & vk::SampleCountFlagBits::e64) { return vk::SampleCountFlagBits::e64; }
    if (counts & vk::SampleCountFlagBits::e32) { return vk::SampleCountFlagBits::e32; }
    if (counts & vk::SampleCountFlagBits::e16) { return vk::SampleCountFlagBits::e16; }
    if (counts & vk::SampleCountFlagBits::e8)  { return vk::SampleCountFlagBits::e8;  }
    if (counts & vk::SampleCountFlagBits::e4)  { return vk::SampleCountFlagBits::e4;  }
    if (counts & vk::SampleCountFlagBits::e2)  { return vk::SampleCountFlagBits::e2;  }

    return vk::SampleCountFlagBits::e1;
}

void VulkanApplication::initPhysicalDevice()
{
    vk::raii::PhysicalDevices physicalDevices(*m_instance);

    for (auto& device : physicalDevices)
    {
        if (isDeviceSuitable(device))
        {
            m_msaaSampleCount = getMaxUsableSampleCount(device);
            m_physicalDevice = std::make_shared<vk::raii::PhysicalDevice>(std::move(device));
            break;
        }
    }

    if (m_physicalDevice == nullptr)
    {
        throw std::runtime_error("failed to find a suitable GPU!");
    }
}

vk::Format VulkanApplication::findSupportedFormat(const std::vector<vk::Format>& candidates, vk::ImageTiling tiling, vk::FormatFeatureFlags features) const
{
    for (vk::Format format : candidates)
    {
        vk::FormatProperties props = m_physicalDevice->getFormatProperties(format);

        if (tiling == vk::ImageTiling::eLinear && (props.linearTilingFeatures & features) == features)
        {
            return format;
        }
        else if (tiling == vk::ImageTiling::eOptimal && (props.optimalTilingFeatures & features) == features)
        {
            return format;
        }
    }

    throw std::runtime_error("failed to find supported format!");
}

vk::Format VulkanApplication::findDepthFormat() const
{
    return findSupportedFormat(
        {vk::Format::eD32Sfloat, vk::Format::eD32SfloatS8Uint, vk::Format::eD24UnormS8Uint},
        vk::ImageTiling::eOptimal,
        vk::FormatFeatureFlagBits::eDepthStencilAttachment
    );
}

uint32_t VulkanApplication::findMemoryType(uint32_t typeFilter, vk::MemoryPropertyFlags properties) const
{
    vk::PhysicalDeviceMemoryProperties memProperties = m_physicalDevice->getMemoryProperties();

    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
    {
        if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
        {
            return i;
        }
    }

    throw std::runtime_error("failed to find suitable memory type!");
}

void VulkanApplication::initDevice()
{
    QueueFamilyIndices indices = findQueueFamilies(*m_physicalDevice);

    std::vector<vk::DeviceQueueCreateInfo> queueCreateInfos;
    std::set<uint32_t> uniqueQueueFamilies = {indices.graphicsFamily.value(), indices.presentFamily.value()};

    float queuePriority = 1.0f;
    for (uint32_t queueFamily : uniqueQueueFamilies)
    {
        vk::DeviceQueueCreateInfo queueCreateInfo{};
        queueCreateInfo.queueFamilyIndex = queueFamily;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;
        queueCreateInfos.push_back(queueCreateInfo);
    }

    vk::PhysicalDeviceFeatures deviceFeatures{};
    deviceFeatures.sampleRateShading = VK_TRUE;

    vk::DeviceCreateInfo createInfo{};
    createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
    createInfo.pQueueCreateInfos = queueCreateInfos.data();

    createInfo.pEnabledFeatures = &deviceFeatures;

    // extensions
    createInfo.enabledExtensionCount = static_cast<uint32_t>(m_deviceExtensions.size());
    createInfo.ppEnabledExtensionNames = m_deviceExtensions.data();

    m_device = std::make_shared<vk::raii::Device>(m_physicalDevice, createInfo);

    // queues are created along with the logical device, we just need to query for them
    m_graphicsQueue = std::make_shared<vk::raii::Queue>(*m_device, indices.graphicsFamily.value(), 0);
    m_presentQueue  = std::make_shared<vk::raii::Queue>(*m_device, indices.presentFamily .value(), 0);
}

void VulkanApplication::initCommandPool()
{
    QueueFamilyIndices indices = findQueueFamilies(*m_physicalDevice);

    vk::CommandPoolCreateInfo poolInfo{};
    poolInfo.queueFamilyIndex = indices.graphicsFamily.value();
    poolInfo.flags = vk::CommandPoolCreateFlagBits::eResetCommandBuffer;

    m_commandPool = std::make_shared<vk::raii::CommandPool>(*m_device, poolInfo);
}

void VulkanApplication::copyBuffer(vk::raii::Buffer & src, vk::raii::Buffer & dest, vk::DeviceSize size) const
{
    // Allocate a new command buffer
    vk::CommandBufferAllocateInfo allocInfo{};
    allocInfo.level = vk::CommandBufferLevel::ePrimary;
    allocInfo.commandPool = **m_commandPool;
    allocInfo.commandBufferCount = 1;

    vk::raii::CommandBuffers commandBuffers(*m_device, allocInfo);
    vk::raii::CommandBuffer & commandBuffer = commandBuffers[0];

    // Begin the copy command
    vk::CommandBufferBeginInfo beginInfo{};
    beginInfo.flags = vk::CommandBufferUsageFlagBits::eOneTimeSubmit;

    commandBuffer.begin(beginInfo);

    vk::BufferCopy copyRegion{};
    copyRegion.srcOffset = 0; // Optional
    copyRegion.dstOffset = 0; // Optional
    copyRegion.size = size;
    commandBuffer.copyBuffer(*src, *dest, copyRegion);

    commandBuffer.end();

    // Submit the command to the queue
    vk::SubmitInfo submitInfo{};
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &*commandBuffer;

    m_graphicsQueue->submit(submitInfo);
    m_graphicsQueue->waitIdle();
}

std::shared_ptr<vk::raii::Buffer> VulkanApplication::makeBuffer(vk::DeviceSize size, vk::BufferUsageFlags usage) const
{
    vk::BufferCreateInfo bufferInfo{};
    bufferInfo.size = size;
    bufferInfo.usage = usage;
    bufferInfo.sharingMode = vk::SharingMode::eExclusive;

    return std::make_shared<vk::raii::Buffer>(*m_device, bufferInfo);
}

void VulkanApplication::initWorld()
{
    m_world = std::make_shared<World>(m_window, *this);
}

vk::SurfaceFormatKHR VulkanApplication::chooseSwapSurfaceFormat()
{
    std::vector<vk::SurfaceFormatKHR> availableFormats = m_physicalDevice->getSurfaceFormatsKHR(**m_surface);

    for (const auto& availableFormat : availableFormats)
    {
        if (availableFormat.format == vk::Format::eB8G8R8A8Srgb && availableFormat.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear)
        {
            return availableFormat;
        }
    }

    return availableFormats[0];
}

vk::PresentModeKHR VulkanApplication::chooseSwapPresentMode()
{
    // uncomment to enable triple buffering if available
    /*
    std::vector<vk::PresentModeKHR> availablePresentModes = m_physicalDevice->getSurfacePresentModesKHR(**m_surface);

    for (const auto& availablePresentMode : availablePresentModes)
    {
        if (availablePresentMode == vk::PresentModeKHR::eMailbox)
        {
            return availablePresentMode;
        }
    }
    */

    // V-sync
    return vk::PresentModeKHR::eFifo;
}

vk::Extent2D VulkanApplication::chooseSwapExtent(const vk::SurfaceCapabilitiesKHR & capabilities)
{
    if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>().max())
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

uint32_t VulkanApplication::chooseSwapImageCount(const vk::SurfaceCapabilitiesKHR & capabilities)
{
    uint32_t imageCount = capabilities.minImageCount + 1;
    if (capabilities.maxImageCount > 0 && imageCount > capabilities.maxImageCount)
    {
        imageCount = capabilities.maxImageCount;
    }

    return imageCount;
}

void VulkanApplication::initSwapchain()
{
    vk::SurfaceCapabilitiesKHR surfaceCapabilities = m_physicalDevice->getSurfaceCapabilitiesKHR(**m_surface);

    vk::SurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat();
    vk::PresentModeKHR presentMode = chooseSwapPresentMode();
    vk::Extent2D extent = chooseSwapExtent(surfaceCapabilities);
    uint32_t imageCount = chooseSwapImageCount(surfaceCapabilities);

    m_swapchainImageFormat = surfaceFormat.format;

    m_swapchainExtent = extent;


    vk::SwapchainCreateInfoKHR createInfo{};
    createInfo.surface = **m_surface;

    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = vk::ImageUsageFlagBits::eColorAttachment;

    QueueFamilyIndices indices = findQueueFamilies(*m_physicalDevice);
    uint32_t queueFamilyIndices[] = {indices.graphicsFamily.value(), indices.presentFamily.value()};

    if (indices.graphicsFamily != indices.presentFamily)
    {
        createInfo.imageSharingMode = vk::SharingMode::eConcurrent;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueFamilyIndices;
    }
    else
    {
        createInfo.imageSharingMode = vk::SharingMode::eExclusive;
        createInfo.queueFamilyIndexCount = 0;
        createInfo.pQueueFamilyIndices = nullptr;
    }

    createInfo.preTransform = surfaceCapabilities.currentTransform;
    createInfo.compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque;
    createInfo.presentMode = presentMode;
    createInfo.clipped = true;
    createInfo.oldSwapchain = nullptr;

    m_swapchain = std::make_shared<vk::raii::SwapchainKHR>(*m_device, createInfo);

    // retrive the created swap chain
    std::vector<VkImage> images = m_swapchain->getImages();

    // construct imageviews
    for(VkImage & img : images)
    {
        vk::ImageViewCreateInfo viewInfo{};
        viewInfo.image = vk::Image(img);
        viewInfo.viewType = vk::ImageViewType::e2D;
        viewInfo.format = m_swapchainImageFormat;
        viewInfo.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
        viewInfo.subresourceRange.baseMipLevel = 0;
        viewInfo.subresourceRange.levelCount = 1;
        viewInfo.subresourceRange.baseArrayLayer = 0;
        viewInfo.subresourceRange.layerCount = 1;

        m_swapchainImageViews.push_back(std::make_shared<vk::raii::ImageView>(*m_device, viewInfo));
    }
}

void VulkanApplication::recreateSwapchain()
{
    // wait for the size to be non null (happen when the window is minimized)
    int width = 0, height = 0;
    while (width == 0 || height == 0)
    {
        auto [nwidth, nheight] = m_window->getFramebufferSize();
        width = nwidth; height = nheight;
        Window::waitEvents();
    }

    m_device->waitIdle();

    m_swapchainImageViews.clear();
    m_frameBuffers.clear();
    m_uniformBufferMemories.clear();
    m_uniformBuffers.clear();
    m_uniformBufferDynamicMemories.clear();
    m_uniformBufferDynamics.clear();
    m_commandBufferInUseShapeLists.clear();

    initSwapchain();
    initRenderPass();
    initGraphicsPipeline();
    initFramebuffers();
    initDescriptorPool();
    initDescriptorSets();
    initCommandBuffers();

    for(size_t i = 0; i < m_commandBuffers->size(); ++i)
    {
        recordCommandBufferForTheFirstTime(i, *m_world);
    }
}

void VulkanApplication::initDescriptorSetLayout()
{
    std::array<vk::DescriptorSetLayoutBinding, 2> uboLayoutBindings;

    uboLayoutBindings[0].binding = 0;
    uboLayoutBindings[0].descriptorType = vk::DescriptorType::eUniformBuffer; 
    uboLayoutBindings[0].descriptorCount = 1;
    uboLayoutBindings[0].stageFlags = vk::ShaderStageFlagBits::eVertex;
    uboLayoutBindings[0].pImmutableSamplers = nullptr;

    uboLayoutBindings[1].binding = 1;
    uboLayoutBindings[1].descriptorType = vk::DescriptorType::eUniformBufferDynamic; 
    uboLayoutBindings[1].descriptorCount = 1;
    uboLayoutBindings[1].stageFlags = vk::ShaderStageFlagBits::eVertex;
    uboLayoutBindings[1].pImmutableSamplers = nullptr;

    vk::DescriptorSetLayoutCreateInfo layoutInfo{};
    layoutInfo.bindingCount = uboLayoutBindings.size();
    layoutInfo.pBindings = uboLayoutBindings.data();

    m_descriptorSetLayout = std::make_shared<vk::raii::DescriptorSetLayout>(**m_device, layoutInfo);
}

void VulkanApplication::initRenderPass()
{
    vk::AttachmentDescription colorAttachment{};
    colorAttachment.format = m_swapchainImageFormat;
    colorAttachment.samples = m_msaaSampleCount;
    colorAttachment.loadOp = vk::AttachmentLoadOp::eClear;
    colorAttachment.storeOp = vk::AttachmentStoreOp::eStore;
    colorAttachment.stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
    colorAttachment.stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
    colorAttachment.initialLayout = vk::ImageLayout::eUndefined;
    colorAttachment.finalLayout = vk::ImageLayout::eColorAttachmentOptimal;

    vk::AttachmentReference colorAttachmentRef{};
    colorAttachmentRef.attachment = 0;
    colorAttachmentRef.layout = vk::ImageLayout::eColorAttachmentOptimal;

    vk::AttachmentDescription depthAttachment{};
    depthAttachment.format = findDepthFormat();
    depthAttachment.samples = m_msaaSampleCount;
    depthAttachment.loadOp = vk::AttachmentLoadOp::eClear;
    depthAttachment.storeOp = vk::AttachmentStoreOp::eDontCare;
    depthAttachment.stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
    depthAttachment.stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
    depthAttachment.initialLayout = vk::ImageLayout::eUndefined;
    depthAttachment.finalLayout = vk::ImageLayout::eDepthStencilAttachmentOptimal;

    vk::AttachmentReference depthAttachmentRef{};
    depthAttachmentRef.attachment = 1;
    depthAttachmentRef.layout = vk::ImageLayout::eDepthStencilAttachmentOptimal;

    vk::AttachmentDescription colorAttachmentResolve{};
    colorAttachmentResolve.format = m_swapchainImageFormat;
    colorAttachmentResolve.samples = vk::SampleCountFlagBits::e1;
    colorAttachmentResolve.loadOp = vk::AttachmentLoadOp::eDontCare;
    colorAttachmentResolve.storeOp = vk::AttachmentStoreOp::eStore;
    colorAttachmentResolve.stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
    colorAttachmentResolve.stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
    colorAttachmentResolve.initialLayout = vk::ImageLayout::eUndefined;
    colorAttachmentResolve.finalLayout = vk::ImageLayout::eColorAttachmentOptimal; // vk::ImageLayout::eColorAttachmentOptimal or vk::ImageLayout::ePresentSrcKHR ?

    vk::AttachmentReference colorAttachmentResolveRef{};
    colorAttachmentResolveRef.attachment = 2;
    colorAttachmentResolveRef.layout = vk::ImageLayout::eColorAttachmentOptimal;

    vk::SubpassDescription subpass{};
    subpass.pipelineBindPoint = vk::PipelineBindPoint::eGraphics;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorAttachmentRef;
    subpass.pDepthStencilAttachment = &depthAttachmentRef;
    subpass.pResolveAttachments = &colorAttachmentResolveRef;

    std::array<vk::AttachmentDescription, 3> attachments = {colorAttachment, depthAttachment, colorAttachmentResolve};
    vk::RenderPassCreateInfo renderPassInfo{};
    renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
    renderPassInfo.pAttachments = attachments.data();
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;

    vk::SubpassDependency dependency{};
    dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass = 0;
    dependency.srcAccessMask = {};
    dependency.srcStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput | vk::PipelineStageFlagBits::eEarlyFragmentTests;
    dependency.dstStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput | vk::PipelineStageFlagBits::eEarlyFragmentTests;
    dependency.dstAccessMask = vk::AccessFlagBits::eColorAttachmentWrite | vk::AccessFlagBits::eDepthStencilAttachmentWrite;
    
    renderPassInfo.dependencyCount = 1;
    renderPassInfo.pDependencies = &dependency;

    m_renderPass = std::make_shared<vk::raii::RenderPass>(*m_device, renderPassInfo);
}

std::vector<char> VulkanApplication::readFile(const std::string& filename)
{
    std::ifstream file(filename, std::ios::ate | std::ios::binary);

    if (!file.is_open())
    {
        throw std::runtime_error("failed to open file!");
    }

    size_t fileSize = (size_t) file.tellg();
    std::vector<char> buffer(fileSize);

    file.seekg(0);
    file.read(buffer.data(), fileSize);

    file.close();
    return buffer;
}

vk::raii::ShaderModule VulkanApplication::createShaderModule(const std::vector<char>& code)
{
    vk::ShaderModuleCreateInfo createInfo{};
    createInfo.codeSize = code.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data()); // data alignement is handled by std::vector

    vk::raii::ShaderModule shaderModule(*m_device, createInfo);
    return shaderModule;
}

void VulkanApplication::initGraphicsPipeline()
{
    auto vertShaderCode = readFile("shaders/vert.spv");
    auto fragShaderCode = readFile("shaders/frag.spv");

    vk::raii::ShaderModule vertShaderModule = createShaderModule(vertShaderCode);
    vk::raii::ShaderModule fragShaderModule = createShaderModule(fragShaderCode);

    // vert
    vk::PipelineShaderStageCreateInfo vertShaderStageInfo{};
    vertShaderStageInfo.stage = vk::ShaderStageFlagBits::eVertex;
    vertShaderStageInfo.module = *vertShaderModule;
    vertShaderStageInfo.pName = "main";

    // frag
    vk::PipelineShaderStageCreateInfo fragShaderStageInfo{};
    fragShaderStageInfo.stage = vk::ShaderStageFlagBits::eFragment;
    fragShaderStageInfo.module = *fragShaderModule;
    fragShaderStageInfo.pName = "main";

    std::array<vk::PipelineShaderStageCreateInfo, 2> shaderStages = {vertShaderStageInfo, fragShaderStageInfo};

    vk::PipelineVertexInputStateCreateInfo vertexInputInfo{};

    auto bindingDescription = Vertex::getBindingDescription();
    auto attributeDescriptions = Vertex::getAttributeDescriptions();

    vertexInputInfo.vertexBindingDescriptionCount = 1;
    vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
    vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
    vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

    vk::PipelineInputAssemblyStateCreateInfo inputAssembly{};
    inputAssembly.topology = vk::PrimitiveTopology::eTriangleList;
    inputAssembly.primitiveRestartEnable = false;

    vk::Viewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = (float) m_swapchainExtent.width;
    viewport.height = (float) m_swapchainExtent.height;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;

    vk::Rect2D scissor{};
    scissor.offset = vk::Offset2D{0, 0};
    scissor.extent = m_swapchainExtent;

    vk::PipelineViewportStateCreateInfo viewportState{};
    viewportState.viewportCount = 1;
    viewportState.pViewports = &viewport;
    viewportState.scissorCount = 1;
    viewportState.pScissors = &scissor;

    vk::PipelineRasterizationStateCreateInfo rasterizer{};
    rasterizer.depthClampEnable = false;
    rasterizer.rasterizerDiscardEnable = false;
    rasterizer.polygonMode = vk::PolygonMode::eFill;
    rasterizer.lineWidth = 1.0f;
    rasterizer.cullMode = vk::CullModeFlagBits::eBack;
    rasterizer.frontFace = vk::FrontFace::eCounterClockwise;
    rasterizer.depthBiasEnable = false;
    rasterizer.depthBiasConstantFactor = 0.0f;
    rasterizer.depthBiasClamp = 0.0f;
    rasterizer.depthBiasSlopeFactor = 0.0f;

    vk::PipelineMultisampleStateCreateInfo multisampling{};
    multisampling.sampleShadingEnable = true;
    multisampling.minSampleShading = 0.2f;
    multisampling.rasterizationSamples = m_msaaSampleCount;
    multisampling.pSampleMask = nullptr;
    multisampling.alphaToCoverageEnable = false;
    multisampling.alphaToOneEnable = false;

    vk::PipelineColorBlendAttachmentState colorBlendAttachment{};
    colorBlendAttachment.colorWriteMask = vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA;
    colorBlendAttachment.blendEnable = true;
    colorBlendAttachment.srcColorBlendFactor = vk::BlendFactor::eSrcAlpha;
    colorBlendAttachment.dstColorBlendFactor = vk::BlendFactor::eOneMinusSrcAlpha;
    colorBlendAttachment.colorBlendOp = vk::BlendOp::eAdd;
    colorBlendAttachment.srcAlphaBlendFactor = vk::BlendFactor::eOne;
    colorBlendAttachment.dstAlphaBlendFactor = vk::BlendFactor::eZero;
    colorBlendAttachment.alphaBlendOp = vk::BlendOp::eAdd;

    vk::PipelineColorBlendStateCreateInfo colorBlending{};
    colorBlending.logicOpEnable = false;
    colorBlending.logicOp = vk::LogicOp::eCopy; // Optional
    colorBlending.attachmentCount = 1;
    colorBlending.pAttachments = &colorBlendAttachment;
    colorBlending.blendConstants[0] = 0.0f; // Optional
    colorBlending.blendConstants[1] = 0.0f; // Optional
    colorBlending.blendConstants[2] = 0.0f; // Optional
    colorBlending.blendConstants[3] = 0.0f; // Optional

    vk::PipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.setLayoutCount = 1;
    pipelineLayoutInfo.pSetLayouts = &**m_descriptorSetLayout;
    pipelineLayoutInfo.pushConstantRangeCount = 0;
    pipelineLayoutInfo.pPushConstantRanges = nullptr;

    m_graphicsPipelineLayout = std::make_shared<vk::raii::PipelineLayout>(**m_device, pipelineLayoutInfo);

    vk::PipelineDepthStencilStateCreateInfo depthStencil{};
    depthStencil.depthTestEnable = true;
    depthStencil.depthWriteEnable = true;
    depthStencil.depthCompareOp = vk::CompareOp::eLess;
    depthStencil.depthBoundsTestEnable = false;
    depthStencil.minDepthBounds = 0.0f; // Optional
    depthStencil.maxDepthBounds = 1.0f; // Optional
    depthStencil.stencilTestEnable = false;
    depthStencil.front = vk::StencilOpState{}; // Optional
    depthStencil.back = vk::StencilOpState{}; // Optional

    vk::GraphicsPipelineCreateInfo pipelineInfo{};
    pipelineInfo.stageCount = 2;

    pipelineInfo.pStages = shaderStages.data();
    pipelineInfo.pVertexInputState = &vertexInputInfo;
    pipelineInfo.pInputAssemblyState = &inputAssembly;
    pipelineInfo.pViewportState = &viewportState;
    pipelineInfo.pRasterizationState = &rasterizer;
    pipelineInfo.pMultisampleState = &multisampling;
    pipelineInfo.pDepthStencilState = &depthStencil;
    pipelineInfo.pColorBlendState = &colorBlending;
    pipelineInfo.pDynamicState = nullptr;

    pipelineInfo.layout = **m_graphicsPipelineLayout;
    pipelineInfo.renderPass = **m_renderPass;
    pipelineInfo.subpass = 0;
    pipelineInfo.basePipelineHandle = nullptr;
    pipelineInfo.basePipelineIndex = -1;

    m_graphicsPipeline = std::make_shared<vk::raii::Pipeline>(m_device, nullptr, pipelineInfo);
}

std::shared_ptr<vk::raii::Image> VulkanApplication::makeImage(vk::Extent2D extent, vk::SampleCountFlagBits numSamples, vk::Format format, vk::ImageTiling tiling, vk::ImageUsageFlags usage) const
{
    vk::ImageCreateInfo imageInfo{};
    imageInfo.imageType = vk::ImageType::e2D;
    imageInfo.extent.width = extent.width;
    imageInfo.extent.height = extent.height;
    imageInfo.extent.depth = 1;
    imageInfo.mipLevels = 1;
    imageInfo.arrayLayers = 1;
    imageInfo.format = format;
    imageInfo.tiling = tiling;
    imageInfo.initialLayout = vk::ImageLayout::eUndefined;
    imageInfo.usage = usage;
    imageInfo.samples = numSamples;
    imageInfo.sharingMode = vk::SharingMode::eExclusive;

    return std::make_unique<vk::raii::Image>(*m_device, imageInfo);
}

std::shared_ptr<vk::raii::DeviceMemory> VulkanApplication::makeDeviceMemory(vk::MemoryRequirements memRequirements, vk::MemoryPropertyFlags memProperties) const
{
    vk::MemoryAllocateInfo allocInfo{};
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, memProperties);

    return std::make_shared<vk::raii::DeviceMemory>(*m_device, allocInfo);
}

std::shared_ptr<vk::raii::ImageView> VulkanApplication::makeImageView(vk::Image image, vk::Format format, vk::ImageAspectFlags aspectFlags) const
{
    vk::ImageViewCreateInfo viewInfo{};
    viewInfo.image = image;
    viewInfo.viewType = vk::ImageViewType::e2D;
    viewInfo.format = format;
    viewInfo.subresourceRange.aspectMask = aspectFlags;
    viewInfo.subresourceRange.baseMipLevel = 0;
    viewInfo.subresourceRange.levelCount = 1;
    viewInfo.subresourceRange.baseArrayLayer = 0;
    viewInfo.subresourceRange.layerCount = 1;

    return std::make_shared<vk::raii::ImageView>(*m_device, viewInfo);
}

void VulkanApplication::initFramebuffers()
{
    m_colorImage = makeImage(m_swapchainExtent, m_msaaSampleCount, m_swapchainImageFormat, vk::ImageTiling::eOptimal, vk::ImageUsageFlagBits::eTransientAttachment | vk::ImageUsageFlagBits::eColorAttachment);
    m_colorImageMemory = makeDeviceMemory(m_colorImage->getMemoryRequirements(), vk::MemoryPropertyFlagBits::eDeviceLocal);
    m_colorImage->bindMemory(**m_colorImageMemory, 0);
    m_colorImageView = makeImageView(**m_colorImage, m_swapchainImageFormat, vk::ImageAspectFlagBits::eColor);


    vk::Format depthFormat = findDepthFormat();
    m_depthImage = makeImage(m_swapchainExtent, m_msaaSampleCount, depthFormat, vk::ImageTiling::eOptimal, vk::ImageUsageFlagBits::eDepthStencilAttachment);
    m_depthImageMemory = makeDeviceMemory(m_depthImage->getMemoryRequirements(), vk::MemoryPropertyFlagBits::eDeviceLocal);
    m_depthImage->bindMemory(**m_depthImageMemory, 0);
    m_depthImageView = makeImageView(**m_depthImage, depthFormat, vk::ImageAspectFlagBits::eDepth);


    for (const auto & imageView : m_swapchainImageViews)
    {
        std::array<vk::ImageView, 3> attachments =
        {
            **m_colorImageView,
            **m_depthImageView,
            **imageView,
        };

        vk::FramebufferCreateInfo framebufferInfo{};
        framebufferInfo.renderPass = **m_renderPass;
        framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
        framebufferInfo.pAttachments = attachments.data();
        framebufferInfo.width = m_swapchainExtent.width;
        framebufferInfo.height = m_swapchainExtent.height;
        framebufferInfo.layers = 1;

        m_frameBuffers.push_back(std::make_shared<vk::raii::Framebuffer>(*m_device, framebufferInfo));
    }
}

void VulkanApplication::initDescriptorPool()
{
    size_t size = m_swapchainImageViews.size();

    std::array<vk::DescriptorPoolSize, 2> poolSizes = 
    {
        vk::DescriptorPoolSize{vk::DescriptorType::eUniformBuffer       , static_cast<uint32_t>(size)},
        vk::DescriptorPoolSize{vk::DescriptorType::eUniformBufferDynamic, static_cast<uint32_t>(size)},
    };

    vk::DescriptorPoolCreateInfo poolInfo{};
    poolInfo.poolSizeCount = poolSizes.size();
    poolInfo.pPoolSizes = poolSizes.data();
    poolInfo.maxSets = static_cast<uint32_t>(size * poolSizes.size());

    m_descriptorPool = std::make_shared<vk::raii::DescriptorPool>(*m_device, poolInfo);
}

void VulkanApplication::initDescriptorSets()
{
    size_t size = m_swapchainImageViews.size();

    std::vector<vk::DescriptorSetLayout> layouts(size, **m_descriptorSetLayout);
    vk::DescriptorSetAllocateInfo allocInfo{};
    allocInfo.descriptorPool = **m_descriptorPool;
    allocInfo.descriptorSetCount = static_cast<uint32_t>(size);
    allocInfo.pSetLayouts = layouts.data();

    m_descriptorSets = std::make_shared<vk::raii::DescriptorSets>(*m_device, allocInfo);

    m_uniformBuffers.resize(size);
    m_uniformBufferMemories.resize(size);
    m_uniformBufferDynamics.resize(size);
    m_uniformBufferDynamicMemories.resize(size);

    for (size_t i = 0; i < size; i++)
    {
        m_uniformBuffers[i] = makeBuffer(sizeof(UniformBufferObjectCamera), vk::BufferUsageFlagBits::eUniformBuffer);
        m_uniformBufferMemories[i] = makeDeviceMemory(m_uniformBuffers[i]->getMemoryRequirements(), vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent);
        m_uniformBuffers[i]->bindMemory(**m_uniformBufferMemories[i], 0);

        initDynamicBuffer(i, m_world->getShapes().size());
        updateDescriptorSet(i);
    }
}

void VulkanApplication::initDynamicBuffer(size_t i, size_t dynamicBufferSize)
{
    m_uniformBufferDynamics[i] = makeBuffer(dynamicBufferSize * sizeof(UniformBufferObjectTransform), vk::BufferUsageFlagBits::eUniformBuffer);
    m_uniformBufferDynamicMemories[i] = makeDeviceMemory(m_uniformBufferDynamics[i]->getMemoryRequirements(), vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent);
    m_uniformBufferDynamics[i]->bindMemory(**m_uniformBufferDynamicMemories[i], 0);
}

void VulkanApplication::updateDescriptorSet(size_t i)
{
    vk::DescriptorBufferInfo bufferInfo{};
    bufferInfo.buffer = **m_uniformBuffers[i];
    bufferInfo.offset = 0;
    bufferInfo.range = sizeof(UniformBufferObjectCamera);

    std::array<vk::WriteDescriptorSet, 2> descriptorWrites;
    descriptorWrites[0].dstSet = *(*m_descriptorSets)[i];
    descriptorWrites[0].dstBinding = 0;
    descriptorWrites[0].dstArrayElement = 0;
    descriptorWrites[0].descriptorType = vk::DescriptorType::eUniformBuffer;
    descriptorWrites[0].descriptorCount = 1;
    descriptorWrites[0].pBufferInfo = &bufferInfo;
    descriptorWrites[0].pNext = nullptr;

    vk::DescriptorBufferInfo bufferInfoDynamic{};
    bufferInfoDynamic.buffer = **m_uniformBufferDynamics[i];
    bufferInfoDynamic.offset = 0;
    bufferInfoDynamic.range = sizeof(UniformBufferObjectTransform);

    descriptorWrites[1].dstSet = *(*m_descriptorSets)[i];
    descriptorWrites[1].dstBinding = 1;
    descriptorWrites[1].dstArrayElement = 0;
    descriptorWrites[1].descriptorType = vk::DescriptorType::eUniformBufferDynamic;
    descriptorWrites[1].descriptorCount = 1;
    descriptorWrites[1].pBufferInfo = &bufferInfoDynamic;
    descriptorWrites[1].pNext = nullptr;

    m_device->updateDescriptorSets(descriptorWrites, nullptr);
}

void VulkanApplication::initCommandBuffers()
{
    size_t size = m_swapchainImageViews.size();

    vk::CommandBufferAllocateInfo allocInfo{};
    allocInfo.commandPool = **m_commandPool;
    allocInfo.level = vk::CommandBufferLevel::ePrimary;
    allocInfo.commandBufferCount = static_cast<uint32_t>(size);

    m_commandBuffers = std::make_shared<vk::raii::CommandBuffers>(*m_device, allocInfo);
    m_commandBufferInUseShapeLists.resize(size);
}

void VulkanApplication::recordCommandBufferForTheFirstTime(size_t i, const World & world)
{
    // store internaly a refenrence to the current entites.
    // This way, when an entity is removed from the world
    // it is not deleted before this buffer gets re-recorded.
    m_commandBufferInUseShapeLists[i] = world.getShapes();

    auto & commandBuffer = (*m_commandBuffers)[i];

    // starting command buffer recording
    vk::CommandBufferBeginInfo beginInfo{};
    beginInfo.flags = {};
    beginInfo.pInheritanceInfo = nullptr;

    commandBuffer.begin(beginInfo);

    // starting a render pass
    vk::RenderPassBeginInfo renderPassInfo{};
    renderPassInfo.renderPass = **m_renderPass;
    renderPassInfo.framebuffer = **m_frameBuffers[i];
    renderPassInfo.renderArea.offset = vk::Offset2D{0, 0};
    renderPassInfo.renderArea.extent = m_swapchainExtent;

    std::array<vk::ClearValue, 2> clearValues{};
    clearValues[0].color = {{0.0f, 0.0f, 0.0f, 1.0f}};
    clearValues[1].depthStencil = vk::ClearDepthStencilValue{1.0f, 0};

    renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
    renderPassInfo.pClearValues = clearValues.data();

    commandBuffer.beginRenderPass(renderPassInfo, vk::SubpassContents::eInline);

    commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, **m_graphicsPipeline);

    size_t i = 0;
    for(const auto & shape : m_commandBufferInUseShapeLists[i])
    {
        std::array<uint32_t, 1> dynamicOffsets = {sizeof(UniformBufferObjectTransform) * static_cast<uint32_t>(i)};

        commandBuffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, **m_graphicsPipelineLayout, 0, *(*m_descriptorSets)[i], dynamicOffsets);

        std::array<vk::Buffer, 1> vertexBuffers = {**shape->getVertexBuffer()};
        std::array<vk::DeviceSize, 1> offsets = {0};

        commandBuffer.bindVertexBuffers(0, vertexBuffers, offsets);

        vk::Buffer indexBuffer = **shape->getIndexBuffer();

        commandBuffer.bindIndexBuffer(indexBuffer, 0, vk::IndexType::eUint32);

        commandBuffer.drawIndexed(static_cast<uint32_t>(shape->getIndexBufferSize()), 1, 0, 0, 0);

        ++i;
    }

    commandBuffer.endRenderPass();
    commandBuffer.end();
}

void VulkanApplication::rerecordCommandBuffer(size_t i, const World & world)
{
    initDynamicBuffer(i, world.getShapes().size());
    updateDescriptorSet(i);
    recordCommandBufferForTheFirstTime(i, world);
}

