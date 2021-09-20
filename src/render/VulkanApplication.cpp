#include "render/VulkanApplication.hpp"


void VulkanApplication::run()
{
    window         = std::make_shared<Window>        ();
    instance       = std::make_shared<Instance>      ();
    surface        = std::make_shared<Surface>       (window, instance);
    physicalDevice = std::make_shared<PhysicalDevice>(instance, surface);
    logicalDevice  = std::make_shared<LogicalDevice> (physicalDevice);
    swapChain      = std::make_shared<SwapChain>     (window, surface, physicalDevice, logicalDevice);


    initVulkan();
    mainLoop();
    cleanup();
}

void VulkanApplication::createSyncObjects()
{
    imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);
    imagesInFlight.resize(swapChain->size(), VK_NULL_HANDLE);

    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
    {
        if (vkCreateSemaphore(logicalDevice->raw(), &semaphoreInfo, nullptr, &imageAvailableSemaphores[i]) != VK_SUCCESS ||
            vkCreateSemaphore(logicalDevice->raw(), &semaphoreInfo, nullptr, &renderFinishedSemaphores[i]) != VK_SUCCESS ||
            vkCreateFence(logicalDevice->raw(), &fenceInfo, nullptr, &inFlightFences[i]) != VK_SUCCESS)
        {

            throw std::runtime_error("failed to create synchronization objects for a frame!");
        }
    }
}


void VulkanApplication::initVulkan() 
{
    createSyncObjects();
}

void VulkanApplication::updateUniformBuffer(uint32_t currentImage)
{
    static auto startTime = std::chrono::high_resolution_clock::now();
    static auto previousTime = std::chrono::high_resolution_clock::now();
    static float rotationAngle = 0.0f;

    const float rotationSpeed = 1.0f * glm::pi<float>(); // rad/s

    auto currentTime = std::chrono::high_resolution_clock::now();
    float totalTime = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();
    float deltaTime = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - previousTime).count();
    previousTime = currentTime;

    if(window->isKeyPressed(GLFW_KEY_LEFT))
    {
        rotationAngle += deltaTime * rotationSpeed;
    }
    else if(window->isKeyPressed(GLFW_KEY_RIGHT))
    {
        rotationAngle -= deltaTime * rotationSpeed;
    }
    else if(window->isKeyPressed(GLFW_KEY_UP))
    {
        rotationAngle = 0;
    }

    UniformBufferObject ubo{};
    ubo.model = glm::rotate(glm::mat4(1.0f), rotationAngle, glm::vec3(0.0f, 0.0f, 1.0f));
    ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    ubo.proj = glm::perspective(glm::radians(45.0f), swapChain->getExtent().width / (float) swapChain->getExtent().height, 0.1f, 10.0f);
    ubo.proj[1][1] *= -1;

    Buffer & uniformBuffer = *(swapChain->getUniformBuffer(currentImage));

    uniformBuffer.loadData(&ubo, sizeof(ubo));

     // TODO
     /*
    vertices[0].pos.z = glm::cos(totalTime);
    VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();

    // Create staging buffer
    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

    // Copy vertices data into staging buffer
    data = nullptr;
    vkMapMemory(device, stagingBufferMemory, 0, bufferSize, 0, &data);
        memcpy(data, vertices.data(), (size_t) bufferSize);
    vkUnmapMemory(device, stagingBufferMemory);

    // Copy staging buffer data into vertex buffer
    copyBuffer(stagingBuffer, vertexBuffer, bufferSize);

    // Clean staging buffer
    vkDestroyBuffer(device, stagingBuffer, nullptr);
    vkFreeMemory(device, stagingBufferMemory, nullptr);

    */
}

void VulkanApplication::drawFrame()
{
    /// ////////////////// acquire image ////////////////// ///

    vkWaitForFences(logicalDevice->raw(), 1, &inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);

    uint32_t imageIndex;
    VkResult result = vkAcquireNextImageKHR(logicalDevice->raw(), swapChain->raw(), UINT64_MAX, imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &imageIndex);
    if (result == VK_ERROR_OUT_OF_DATE_KHR)
    {
        swapChain->recreate();
        return;
    } 
    else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
    {
        throw std::runtime_error("failed to acquire swap chain image!");
    }

    // Check if a previous frame is using this image (i.e. there is its fence to wait on)
    if (imagesInFlight[imageIndex] != VK_NULL_HANDLE)
    {
        vkWaitForFences(logicalDevice->raw(), 1, &imagesInFlight[imageIndex], VK_TRUE, UINT64_MAX);
    }
    // Mark the image as now being in use by this frame
    imagesInFlight[imageIndex] = inFlightFences[currentFrame];

    /// ////////////////// execute the command buffer ////////////////// ///

    updateUniformBuffer(imageIndex);

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore waitSemaphores[] = {imageAvailableSemaphores[currentFrame]};
    VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &(swapChain->getCommandBuffer(imageIndex).raw());

    VkSemaphore signalSemaphores[] = {renderFinishedSemaphores[currentFrame]};
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    vkResetFences(logicalDevice->raw(), 1, &inFlightFences[currentFrame]);

    if (vkQueueSubmit(logicalDevice->getGraphicsQueue().raw(), 1, &submitInfo, inFlightFences[currentFrame]) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to submit draw command buffer!");
    }

    /// ////////////////// return image for presentation ////////////////// ///

    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;

    VkSwapchainKHR swapChains[] = {swapChain->raw()};
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;
    presentInfo.pImageIndices = &imageIndex;
    presentInfo.pResults = nullptr;
    
    result = vkQueuePresentKHR(logicalDevice->getPresentQueue().raw(), &presentInfo);

    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || window->isFramebufferResized())
    {
        window->setFramebufferResized(false);
        swapChain->recreate();
    }
    else if (result != VK_SUCCESS)
    {
        throw std::runtime_error("failed to present swap chain image!");
    }

    currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}

void VulkanApplication::mainLoop() 
{
    while (!window->shouldClose()) 
    {
        Window::pollEvents();
        drawFrame();
    }

    logicalDevice->waitIdle();
}

void VulkanApplication::cleanup()
{
    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
    {
        vkDestroySemaphore(logicalDevice->raw(), renderFinishedSemaphores[i], nullptr);
        vkDestroySemaphore(logicalDevice->raw(), imageAvailableSemaphores[i], nullptr);
        vkDestroyFence(logicalDevice->raw(), inFlightFences[i], nullptr);
    }
}