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

void VulkanApplication::makeBuffer(std::unique_ptr<vk::raii::Buffer> & buffer, std::unique_ptr<vk::raii::DeviceMemory> & deviceMemory, vk::DeviceSize size, vk::BufferUsageFlags usage, vk::MemoryPropertyFlags properties) const
{
    vk::BufferCreateInfo bufferInfo{};
    bufferInfo.size = size;
    bufferInfo.usage = usage;
    bufferInfo.sharingMode = vk::SharingMode::eExclusive;

    buffer = std::make_unique<vk::raii::Buffer>(*m_device, bufferInfo);

    vk::MemoryRequirements memRequirements = buffer->getMemoryRequirements();

    vk::MemoryAllocateInfo allocInfo{};
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);

    deviceMemory = std::make_unique<vk::raii::DeviceMemory>(*m_device, allocInfo);

    buffer->bindMemory(**deviceMemory, 0);
}

void VulkanApplication::initWorld()
{
    m_world = std::make_shared<World>(m_window, *this);
}