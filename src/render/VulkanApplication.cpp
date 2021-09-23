#include "render/VulkanApplication.hpp"

VulkanApplication::VulkanApplication(const std::shared_ptr<IApplication> & application) :
    m_application{application}
{
    m_window         = std::make_shared<Window>        ();
    m_instance       = std::make_shared<Instance>      ();
    m_surface        = std::make_shared<Surface>       (m_window, m_instance);
    m_physicalDevice = std::make_shared<PhysicalDevice>(m_instance, m_surface);
    m_logicalDevice  = std::make_shared<LogicalDevice> (m_physicalDevice);
    m_commandPool    = std::make_shared<CommandPool>   (m_logicalDevice);
    m_world          = std::make_shared<World>         (m_window, m_logicalDevice, m_commandPool);

    m_application->init(*m_world);

    m_swapChain      = std::make_shared<SwapChain>     (m_window, m_surface, m_physicalDevice, m_logicalDevice, m_commandPool, m_world);

    m_imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    m_renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    m_inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);
    m_imagesInFlight.resize(m_swapChain->size(), VK_NULL_HANDLE);

    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
    {
        if (vkCreateSemaphore(m_logicalDevice->raw(), &semaphoreInfo, nullptr, &m_imageAvailableSemaphores[i]) != VK_SUCCESS ||
            vkCreateSemaphore(m_logicalDevice->raw(), &semaphoreInfo, nullptr, &m_renderFinishedSemaphores[i]) != VK_SUCCESS ||
            vkCreateFence(m_logicalDevice->raw(), &fenceInfo, nullptr, &m_inFlightFences[i]) != VK_SUCCESS)
        {

            throw std::runtime_error("failed to create synchronization objects for a frame!");
        }
    }

    m_needRecord.resize(m_swapChain->size(), false);
}

VulkanApplication::~VulkanApplication()
{
    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
    {
        vkDestroySemaphore(m_logicalDevice->raw(), m_renderFinishedSemaphores[i], nullptr);
        vkDestroySemaphore(m_logicalDevice->raw(), m_imageAvailableSemaphores[i], nullptr);
        vkDestroyFence(m_logicalDevice->raw(), m_inFlightFences[i], nullptr);
    }
}

void VulkanApplication::run()
{
    while (!m_window->shouldClose()) 
    {
        Window::pollEvents();
        drawFrame();
    }

    m_logicalDevice->waitIdle();
}

void VulkanApplication::update(uint32_t currentImage)
{
    static auto startTime = std::chrono::high_resolution_clock::now();
    static auto previousTime = std::chrono::high_resolution_clock::now();

    auto currentTime = std::chrono::high_resolution_clock::now();
    float totalTime = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();
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
        m_swapChain->recordCommandBuffer(currentImage);
        m_needRecord[currentImage] = false;
    }

    UniformBufferObject ubo{};
    ubo.model = glm::mat4(1.0f);
    ubo.view = glm::lookAt(m_world->getCamera().getPosition(), m_world->getCamera().getPosition() + m_world->getCamera().getDirection(), glm::vec3(0.0f, 0.0f, 1.0f));
    ubo.proj = glm::perspective(glm::radians(45.0f), m_swapChain->getExtent().width / (float) m_swapChain->getExtent().height, 0.1f, 10.0f);
    ubo.proj[1][1] *= -1;

    Buffer & uniformBuffer = *(m_swapChain->getUniformBuffer(currentImage));

    uniformBuffer.loadData(&ubo, sizeof(ubo));
}

void VulkanApplication::drawFrame()
{
    /// ////////////////// acquire image ////////////////// ///

    vkWaitForFences(m_logicalDevice->raw(), 1, &m_inFlightFences[m_currentFrame], VK_TRUE, UINT64_MAX);

    uint32_t imageIndex;
    VkResult result = vkAcquireNextImageKHR(m_logicalDevice->raw(), m_swapChain->raw(), UINT64_MAX, m_imageAvailableSemaphores[m_currentFrame], VK_NULL_HANDLE, &imageIndex);
    if (result == VK_ERROR_OUT_OF_DATE_KHR)
    {
        m_swapChain->recreate();
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

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore waitSemaphores[] = {m_imageAvailableSemaphores[m_currentFrame]};
    VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &(m_swapChain->getCommandBuffer(imageIndex).raw());

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
    }
    else if (result != VK_SUCCESS)
    {
        throw std::runtime_error("failed to present swap chain image!");
    }

    m_currentFrame = (m_currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}