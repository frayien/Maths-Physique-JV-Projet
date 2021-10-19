#include "render/GraphicsEngine.hpp"

GraphicsEngine::GraphicsEngine(const std::shared_ptr<IApplication> & application) :
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
    initSwapchain();
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

    initImGui();
    initSyncObjects();

    m_needRecord.resize(m_swapchainImageViews.size(), false);
}

GraphicsEngine::~GraphicsEngine()
{
}

bool GraphicsEngine::windowShouldClose() const
{
    return m_window->shouldClose();
}

void GraphicsEngine::windowPollEvents()
{
    Window::pollEvents();
}

void GraphicsEngine::clear()
{

}
void GraphicsEngine::draw()
{

}
void GraphicsEngine::display()
{
    drawFrame();
}

void GraphicsEngine::end()
{
    m_device->waitIdle();
}

void GraphicsEngine::run()
{
    while (!m_window->shouldClose()) 
    {
        Window::pollEvents();

        drawFrame();
    }

    m_device->waitIdle();
}

void GraphicsEngine::update(uint32_t currentImage)
{
    static auto previousTime = std::chrono::high_resolution_clock::now();

    auto currentTime = std::chrono::high_resolution_clock::now();
    float deltaTime = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - previousTime).count();
    previousTime = currentTime;

    m_application->update(*m_world, deltaTime);

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
        rerecordCommandBuffer(currentImage, *m_world);
        m_needRecord[currentImage] = false;
    }
    {
        UniformBufferObjectCamera ubo{};
        ubo.model = glm::mat4(1.0f);
        ubo.view = glm::lookAt(m_world->getCamera().getPosition(), m_world->getCamera().getPosition() + m_world->getCamera().getDirection(), glm::vec3(0.0f, 0.0f, 1.0f));
        ubo.proj = glm::perspective(glm::radians(45.0f), static_cast<float>(m_swapchainExtent.width) / static_cast<float>(m_swapchainExtent.height), 0.1f, m_world->getCamera().getViewDistance());
        ubo.proj[1][1] *= -1;
        ubo.lightPos = m_world->getLightSource().getPosition();
        ubo.lightColor = m_world->getLightSource().getColor();
        ubo.ambientLightStrength = m_world->getLightSource().getAmbient();

        const auto & uniformBufferMemory = m_uniformBufferMemories[currentImage];

        void* data_dst = uniformBufferMemory->mapMemory(0, sizeof(UniformBufferObjectCamera));
            memcpy(data_dst, &ubo, sizeof(UniformBufferObjectCamera));
        uniformBufferMemory->unmapMemory();        
    }
    {
        const auto & entities = m_world->getShapes();
        std::vector<UniformBufferObjectTransform> ubos(entities.size());
        size_t i = 0;
        for(const auto & entity : entities)
        {
            ubos[i].transform = entity->getTransform();
    
            ++i;
        }

        const auto & uniformBufferDynamicMemory = m_uniformBufferDynamicMemories[currentImage];

        void* data_dst = uniformBufferDynamicMemory->mapMemory(0, entities.size() * sizeof(UniformBufferObjectTransform));
            memcpy(data_dst, ubos.data(), entities.size() * sizeof(UniformBufferObjectTransform));
        uniformBufferDynamicMemory->unmapMemory();  
    }

    
}

void GraphicsEngine::drawFrame()
{
    constexpr uint64_t TIMEOUT = std::numeric_limits<uint64_t>().max();
    /// ////////////////// acquire image ////////////////// ///
    static_cast<void>( m_device->waitForFences(**m_inFlightFences[m_currentFrame], true, TIMEOUT) );

    vk::Result result;
    uint32_t imageIndex;
    try
    {
        std::tie(result, imageIndex) = m_swapchain->acquireNextImage(TIMEOUT, **m_imageAvailableSemaphores[m_currentFrame]);
    }
    catch(const vk::OutOfDateKHRError & error)
    {
        result = vk::Result::eErrorOutOfDateKHR;
    }

    if (result == vk::Result::eErrorOutOfDateKHR)
    {
        recreateSwapchain();
        m_imGuiVulkan->recreate(m_swapchainImageViews, m_swapchainExtent, m_swapchainImageFormat);
        return;
    }
    else if (result != vk::Result::eSuccess && result != vk::Result::eSuboptimalKHR)
    {
        throw std::runtime_error("failed to acquire swap chain image!");
    }

    // Check if a previous frame is using this image (i.e. there is its fence to wait on)
    if (m_imagesInFlight[imageIndex] != nullptr)
    {
        static_cast<void>( m_device->waitForFences(**m_imagesInFlight[imageIndex], true, TIMEOUT) );
    }
    // Mark the image as now being in use by this frame
    m_imagesInFlight[imageIndex] = m_inFlightFences[m_currentFrame];

    /// ////////////////// execute the command buffer ////////////////// ///

    update(imageIndex);

    m_imGuiVulkan->createFrame();
    m_imGuiVulkan->render(imageIndex);

    vk::SubmitInfo submitInfo{};

    std::array<vk::CommandBuffer, 2> submitCommandBuffers = { *(*m_commandBuffers)[imageIndex], *m_imGuiVulkan->getCommandBuffer(imageIndex) };
    std::array<vk::Semaphore, 1> waitSemaphores = { **m_imageAvailableSemaphores[m_currentFrame] };
    std::array<vk::PipelineStageFlags, 1> waitStages = { vk::PipelineStageFlagBits::eColorAttachmentOutput };

    submitInfo.waitSemaphoreCount = static_cast<uint32_t>(waitSemaphores.size());
    submitInfo.pWaitSemaphores = waitSemaphores.data();
    submitInfo.pWaitDstStageMask = waitStages.data();
    submitInfo.commandBufferCount = static_cast<uint32_t>(submitCommandBuffers.size());;
    submitInfo.pCommandBuffers = submitCommandBuffers.data();

    std::array<vk::Semaphore, 1> signalSemaphores = {**m_renderFinishedSemaphores[m_currentFrame]};
    submitInfo.signalSemaphoreCount = static_cast<uint32_t>(signalSemaphores.size());
    submitInfo.pSignalSemaphores = signalSemaphores.data();

    m_device->resetFences(**m_inFlightFences[m_currentFrame]);

    m_graphicsQueue->submit(submitInfo, **m_inFlightFences[m_currentFrame]);

    /// ////////////////// return image for presentation ////////////////// ///

    vk::PresentInfoKHR presentInfo{};

    presentInfo.waitSemaphoreCount = static_cast<uint32_t>(signalSemaphores.size());
    presentInfo.pWaitSemaphores = signalSemaphores.data();

    std::array<vk::SwapchainKHR, 1> swapchains = {**m_swapchain};
    presentInfo.swapchainCount = static_cast<uint32_t>(swapchains.size());
    presentInfo.pSwapchains = swapchains.data();
    presentInfo.pImageIndices = &imageIndex;
    presentInfo.pResults = nullptr;

    try
    {
        result = m_presentQueue->presentKHR(presentInfo);
    }
    catch(const vk::OutOfDateKHRError & error)
    {
        result = vk::Result::eErrorOutOfDateKHR;
    }
    
    if (result == vk::Result::eErrorOutOfDateKHR || result == vk::Result::eSuboptimalKHR || m_window->isFramebufferResized())
    {
        m_window->setFramebufferResized(false);
        recreateSwapchain();
        m_imGuiVulkan->recreate(m_swapchainImageViews, m_swapchainExtent, m_swapchainImageFormat);
    }
    else if (result != vk::Result::eSuccess)
    {
        throw std::runtime_error("failed to present swap chain image!");
    }

    m_currentFrame = (m_currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}

void GraphicsEngine::initWindow()
{
    m_window = std::make_shared<Window>();
}

void GraphicsEngine::initContext()
{
    m_context = std::make_shared<vk::raii::Context>();
}

bool GraphicsEngine::checkValidationLayerSupport() 
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

void GraphicsEngine::initInstance()
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

void GraphicsEngine::initSurface()
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

QueueFamilyIndices GraphicsEngine::findQueueFamilies(const vk::raii::PhysicalDevice & device) 
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

bool GraphicsEngine::checkDeviceExtensionSupport(const vk::raii::PhysicalDevice & device)
{
    auto availableExtensions = device.enumerateDeviceExtensionProperties();

    std::set<std::string> requiredExtensions(m_deviceExtensions.begin(), m_deviceExtensions.end());

    for (const auto& extension : availableExtensions)
    {
        requiredExtensions.erase(extension.extensionName);
    }

    return requiredExtensions.empty();
}

bool GraphicsEngine::isDeviceSuitable(const vk::raii::PhysicalDevice & device)
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

vk::SampleCountFlagBits GraphicsEngine::getMaxUsableSampleCount(const vk::raii::PhysicalDevice & device)
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

void GraphicsEngine::initPhysicalDevice()
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

vk::Format GraphicsEngine::findSupportedFormat(const std::vector<vk::Format>& candidates, vk::ImageTiling tiling, vk::FormatFeatureFlags features) const
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

vk::Format GraphicsEngine::findDepthFormat() const
{
    return findSupportedFormat(
        {vk::Format::eD32Sfloat, vk::Format::eD32SfloatS8Uint, vk::Format::eD24UnormS8Uint},
        vk::ImageTiling::eOptimal,
        vk::FormatFeatureFlagBits::eDepthStencilAttachment
    );
}

uint32_t GraphicsEngine::findMemoryType(uint32_t typeFilter, vk::MemoryPropertyFlags properties) const
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

void GraphicsEngine::initDevice()
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

    m_device = std::make_shared<vk::raii::Device>(*m_physicalDevice, createInfo);

    // queues are created along with the logical device, we just need to query for them
    m_graphicsQueue = std::make_shared<vk::raii::Queue>(*m_device, indices.graphicsFamily.value(), 0);
    m_presentQueue  = std::make_shared<vk::raii::Queue>(*m_device, indices.presentFamily .value(), 0);
}

void GraphicsEngine::initCommandPool()
{
    QueueFamilyIndices indices = findQueueFamilies(*m_physicalDevice);

    vk::CommandPoolCreateInfo poolInfo{};
    poolInfo.queueFamilyIndex = indices.graphicsFamily.value();
    poolInfo.flags = vk::CommandPoolCreateFlagBits::eResetCommandBuffer;

    m_commandPool = std::make_shared<vk::raii::CommandPool>(*m_device, poolInfo);
}

void GraphicsEngine::copyBuffer(vk::raii::Buffer & src, vk::raii::Buffer & dest, vk::DeviceSize size) const
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

std::shared_ptr<vk::raii::Buffer> GraphicsEngine::makeBuffer(vk::DeviceSize size, vk::BufferUsageFlags usage) const
{
    vk::BufferCreateInfo bufferInfo{};
    bufferInfo.size = size;
    bufferInfo.usage = usage;
    bufferInfo.sharingMode = vk::SharingMode::eExclusive;

    return std::make_shared<vk::raii::Buffer>(*m_device, bufferInfo);
}

void GraphicsEngine::initWorld()
{
    m_world = std::make_shared<World>(m_window, *this);

    m_application->init(*m_world);
}

vk::SurfaceFormatKHR GraphicsEngine::chooseSwapSurfaceFormat()
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

vk::PresentModeKHR GraphicsEngine::chooseSwapPresentMode()
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

vk::Extent2D GraphicsEngine::chooseSwapExtent(const vk::SurfaceCapabilitiesKHR & capabilities)
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

uint32_t GraphicsEngine::chooseSwapImageCount(const vk::SurfaceCapabilitiesKHR & capabilities)
{
    uint32_t imageCount = capabilities.minImageCount + 1;
    if (capabilities.maxImageCount > 0 && imageCount > capabilities.maxImageCount)
    {
        imageCount = capabilities.maxImageCount;
    }

    return imageCount;
}

void GraphicsEngine::initSwapchain()
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

void GraphicsEngine::recreateSwapchain()
{
    // wait for the size to be non null (happen when the window is minimized)
    int width = 0, height = 0;
    while (width == 0 || height == 0)
    {
        std::tie(width, height) = m_window->getFramebufferSize();
        Window::waitEvents();
    }

    m_device->waitIdle();

    m_commandBuffers.reset();
    m_commandBufferInUseShapeLists.clear();
    m_descriptorSets.reset();
    m_uniformBufferDynamicMemories.clear();
    m_uniformBufferDynamics.clear();
    m_uniformBufferMemories.clear();
    m_uniformBuffers.clear();
    m_descriptorPool.reset();
    m_frameBuffers.clear();
    m_graphicsPipeline.reset();
    m_renderPass.reset();
    m_swapchainImageViews.clear();
    m_swapchain.reset();

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

void GraphicsEngine::initDescriptorSetLayout()
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

    m_descriptorSetLayout = std::make_shared<vk::raii::DescriptorSetLayout>(*m_device, layoutInfo);
}

void GraphicsEngine::initRenderPass()
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

std::vector<char> GraphicsEngine::readFile(const std::string& filename)
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

vk::raii::ShaderModule GraphicsEngine::createShaderModule(const std::vector<char>& code)
{
    vk::ShaderModuleCreateInfo createInfo{};
    createInfo.codeSize = code.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data()); // data alignement is handled by std::vector

    vk::raii::ShaderModule shaderModule(*m_device, createInfo);
    return shaderModule;
}

void GraphicsEngine::initGraphicsPipeline()
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

    m_graphicsPipelineLayout = std::make_shared<vk::raii::PipelineLayout>(*m_device, pipelineLayoutInfo);

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

    m_graphicsPipeline = std::make_shared<vk::raii::Pipeline>(*m_device, nullptr, pipelineInfo);
}

std::shared_ptr<vk::raii::Image> GraphicsEngine::makeImage(vk::Extent2D extent, vk::SampleCountFlagBits numSamples, vk::Format format, vk::ImageTiling tiling, vk::ImageUsageFlags usage) const
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

std::shared_ptr<vk::raii::DeviceMemory> GraphicsEngine::makeDeviceMemory(vk::MemoryRequirements memRequirements, vk::MemoryPropertyFlags memProperties) const
{
    vk::MemoryAllocateInfo allocInfo{};
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, memProperties);

    return std::make_shared<vk::raii::DeviceMemory>(*m_device, allocInfo);
}

std::shared_ptr<vk::raii::ImageView> GraphicsEngine::makeImageView(vk::Image image, vk::Format format, vk::ImageAspectFlags aspectFlags) const
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

void GraphicsEngine::initFramebuffers()
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

void GraphicsEngine::initDescriptorPool()
{
    size_t size = m_swapchainImageViews.size();

    std::array<vk::DescriptorPoolSize, 2> poolSizes = 
    {
        vk::DescriptorPoolSize{vk::DescriptorType::eUniformBuffer       , static_cast<uint32_t>(size)},
        vk::DescriptorPoolSize{vk::DescriptorType::eUniformBufferDynamic, static_cast<uint32_t>(size)},
    };

    vk::DescriptorPoolCreateInfo poolInfo{};
    poolInfo.flags = vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet;
    poolInfo.poolSizeCount = poolSizes.size();
    poolInfo.pPoolSizes = poolSizes.data();
    poolInfo.maxSets = static_cast<uint32_t>(size * poolSizes.size());

    m_descriptorPool = std::make_shared<vk::raii::DescriptorPool>(*m_device, poolInfo);
}

void GraphicsEngine::initDescriptorSets()
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

void GraphicsEngine::initDynamicBuffer(size_t i, size_t dynamicBufferSize)
{
    m_uniformBufferDynamics[i] = makeBuffer(dynamicBufferSize * sizeof(UniformBufferObjectTransform), vk::BufferUsageFlagBits::eUniformBuffer);
    m_uniformBufferDynamicMemories[i] = makeDeviceMemory(m_uniformBufferDynamics[i]->getMemoryRequirements(), vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent);
    m_uniformBufferDynamics[i]->bindMemory(**m_uniformBufferDynamicMemories[i], 0);
}

void GraphicsEngine::updateDescriptorSet(size_t i)
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

void GraphicsEngine::initCommandBuffers()
{
    size_t size = m_swapchainImageViews.size();

    vk::CommandBufferAllocateInfo allocInfo{};
    allocInfo.commandPool = **m_commandPool;
    allocInfo.level = vk::CommandBufferLevel::ePrimary;
    allocInfo.commandBufferCount = static_cast<uint32_t>(size);

    m_commandBuffers = std::make_shared<vk::raii::CommandBuffers>(*m_device, allocInfo);
    m_commandBufferInUseShapeLists.resize(size);
}

void GraphicsEngine::recordCommandBufferForTheFirstTime(size_t i, const World & world)
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
    clearValues[0].color = std::array<float, 4>{0.0f, 0.0f, 0.0f, 1.0f};
    clearValues[1].depthStencil = vk::ClearDepthStencilValue{1.0f, 0};

    renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
    renderPassInfo.pClearValues = clearValues.data();

    commandBuffer.beginRenderPass(renderPassInfo, vk::SubpassContents::eInline);

    commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, **m_graphicsPipeline);

    size_t offset = 0;
    for(const auto & shape : m_commandBufferInUseShapeLists[i])
    {
        std::array<uint32_t, 1> dynamicOffsets = {sizeof(UniformBufferObjectTransform) * static_cast<uint32_t>(offset)};

        commandBuffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, **m_graphicsPipelineLayout, 0, *(*m_descriptorSets)[i], dynamicOffsets);

        std::array<vk::Buffer, 1> vertexBuffers = {**shape->getVertexBuffer()};
        std::array<vk::DeviceSize, 1> offsets = {0};

        commandBuffer.bindVertexBuffers(0, vertexBuffers, offsets);

        vk::Buffer indexBuffer = **shape->getIndexBuffer();

        commandBuffer.bindIndexBuffer(indexBuffer, 0, vk::IndexType::eUint32);

        commandBuffer.drawIndexed(static_cast<uint32_t>(shape->getIndexBufferSize()), 1, 0, 0, 0);

        ++offset;
    }

    commandBuffer.endRenderPass();
    commandBuffer.end();
}

void GraphicsEngine::rerecordCommandBuffer(size_t i, const World & world)
{
    initDynamicBuffer(i, world.getShapes().size());
    updateDescriptorSet(i);
    recordCommandBufferForTheFirstTime(i, world);
}

void GraphicsEngine::initImGui()
{
    m_imGuiVulkan = std::make_shared<ImGuiVulkan>(m_application, m_window, m_instance, m_physicalDevice, m_device, m_graphicsQueue, m_swapchainImageViews, m_swapchainExtent, m_swapchainImageFormat, findQueueFamilies(*m_physicalDevice).graphicsFamily.value());
}

void GraphicsEngine::initSyncObjects()
{
    m_imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    m_renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    m_inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);
    m_imagesInFlight.resize(m_swapchainImageViews.size(), nullptr);

    vk::SemaphoreCreateInfo semaphoreInfo{};

    vk::FenceCreateInfo fenceInfo{};
    fenceInfo.flags = vk::FenceCreateFlagBits::eSignaled;

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
    {
        m_imageAvailableSemaphores[i] = std::make_shared<vk::raii::Semaphore>(*m_device, semaphoreInfo);
        m_renderFinishedSemaphores[i] = std::make_shared<vk::raii::Semaphore>(*m_device, semaphoreInfo);
        m_inFlightFences[i] = std::make_shared<vk::raii::Fence>(*m_device, fenceInfo);
    }
}