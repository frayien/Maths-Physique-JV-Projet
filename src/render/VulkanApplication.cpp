#include "render/VulkanApplication.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_vulkan.h"

#include <chrono>
#include <thread>

VulkanApplication::VulkanApplication(const std::shared_ptr<IApplication> & application) :
    m_application{application}
{
    m_window         = std::make_shared<Window>        ();
    m_instance       = std::make_shared<Instance>      ();
    m_surface        = std::make_shared<Surface>       (m_window, m_instance);
    m_physicalDevice = std::make_shared<PhysicalDevice>(m_instance, m_surface);
    m_logicalDevice  = std::make_shared<LogicalDevice> (m_physicalDevice);
    m_commandPool    = std::make_shared<CommandPool>   (m_logicalDevice);
    m_world          = std::make_shared<World>         (m_window, m_physicalDevice, m_logicalDevice, m_commandPool);

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

    initImGui();
}

VulkanApplication::~VulkanApplication()
{
    imGuiCleanUp();

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
        using namespace std::chrono;
        high_resolution_clock::time_point frameStart = high_resolution_clock::now();

        Window::pollEvents();

        if (m_swapChainRebuild)
        {
            m_swapChainRebuild = false;
            ImGui_ImplVulkan_SetMinImageCount(m_swapChain->size()); // Not sure
        }

        ImGui_ImplVulkan_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::ShowDemoWindow();
        ImGui::Render();

        drawFrame();

        high_resolution_clock::time_point frameEnd = high_resolution_clock::now();
        duration<double> frameTime = duration_cast<duration<double>>(frameEnd - frameStart);

        double waitTime = 0.016667 - frameTime.count();
        if (waitTime > 0)
        {
            std::this_thread::sleep_for(std::chrono::duration<double>(waitTime));
        }
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

    VkResult result = vkAcquireNextImageKHR(m_logicalDevice->raw(), m_swapChain->raw(), UINT64_MAX, m_imageAvailableSemaphores[m_currentFrame], VK_NULL_HANDLE, &m_imageIndex);
    if (result == VK_ERROR_OUT_OF_DATE_KHR)
    {
        m_swapChain->recreate();
        m_swapChainRebuild = true;
        return;
    } 
    else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
    {
        throw std::runtime_error("failed to acquire swap chain image!");
    }

    // Check if a previous frame is using this image (i.e. there is its fence to wait on)
    if (m_imagesInFlight[m_imageIndex] != VK_NULL_HANDLE)
    {
        vkWaitForFences(m_logicalDevice->raw(), 1, &m_imagesInFlight[m_imageIndex], VK_TRUE, UINT64_MAX);
    }
    // Mark the image as now being in use by this frame
    m_imagesInFlight[m_imageIndex] = m_inFlightFences[m_currentFrame];

    /// ////////////////// execute the command buffer ////////////////// ///

    update(m_imageIndex);

    imGuiRender();

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    std::array<VkCommandBuffer, 2> submitCommandBuffers = { m_swapChain->getCommandBuffer(m_imageIndex).raw(), m_imGuiCommandBuffers[m_imageIndex] };

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
    presentInfo.pImageIndices = &m_imageIndex;
    presentInfo.pResults = nullptr;
    
    result = vkQueuePresentKHR(m_logicalDevice->getPresentQueue().raw(), &presentInfo);

    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || m_window->isFramebufferResized())
    {
        m_window->setFramebufferResized(false);
        m_swapChain->recreate();
        m_swapChainRebuild = true;
    }
    else if (result != VK_SUCCESS)
    {
        throw std::runtime_error("failed to present swap chain image!");
    }

    m_currentFrame = (m_currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}

VkCommandBuffer VulkanApplication::beginSingleTimeCommands()
{
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = m_commandPool->raw();
    allocInfo.commandBufferCount = 1;

    VkCommandBuffer commandBuffer;
    vkAllocateCommandBuffers(m_logicalDevice->raw(), &allocInfo, &commandBuffer);

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer(commandBuffer, &beginInfo);

    return commandBuffer;
}

void VulkanApplication::endSingleTimeCommands(VkCommandBuffer commandBuffer)
{
    vkEndCommandBuffer(commandBuffer);

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;

    vkQueueSubmit(m_logicalDevice->getGraphicsQueue().raw(), 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(m_logicalDevice->getGraphicsQueue().raw());

    vkFreeCommandBuffers(m_logicalDevice->raw(), m_commandPool->raw(), 1, &commandBuffer);
}

void VulkanApplication::initImGui()
{
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForVulkan(m_window->raw(), true);

    // Create Descriptor Pool for Imgui
    VkDescriptorPoolSize pool_sizes[] =
    {
        { VK_DESCRIPTOR_TYPE_SAMPLER, 1000 },
        { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
        { VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000 },
        { VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000 },
        { VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000 },
        { VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000 },
        { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000 },
        { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000 },
        { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
        { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
        { VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000 }
    };
    VkDescriptorPoolCreateInfo pool_info = {};
    pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
    pool_info.maxSets = 1000 * IM_ARRAYSIZE(pool_sizes);
    pool_info.poolSizeCount = (uint32_t)IM_ARRAYSIZE(pool_sizes);
    pool_info.pPoolSizes = pool_sizes;
    vkCreateDescriptorPool(m_logicalDevice->raw(), &pool_info, nullptr, &m_imguiDescriptorPool);

    // Create Render Pass for Imgui
    VkAttachmentDescription attachmentDescription = {};
    attachmentDescription.format = m_swapChain->getImageFormat();
    attachmentDescription.samples = VK_SAMPLE_COUNT_1_BIT;
    attachmentDescription.loadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
    attachmentDescription.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    attachmentDescription.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    attachmentDescription.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    attachmentDescription.initialLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    attachmentDescription.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentReference color_attachment = {};
    color_attachment.attachment = 0;
    color_attachment.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass = {};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &color_attachment;

    // Create Subpass Dependency to synchronize the render passes
    VkSubpassDependency dependency = {};
    dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass = 0;
    dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.srcAccessMask = 0;  // or VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

    VkRenderPassCreateInfo renderPassCreateInfo = {};
    renderPassCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassCreateInfo.attachmentCount = 1;
    renderPassCreateInfo.pAttachments = &attachmentDescription;
    renderPassCreateInfo.subpassCount = 1;
    renderPassCreateInfo.pSubpasses = &subpass;
    renderPassCreateInfo.dependencyCount = 1;
    renderPassCreateInfo.pDependencies = &dependency;
    if (vkCreateRenderPass(m_logicalDevice->raw(), &renderPassCreateInfo, nullptr, &m_imGuiRenderPass) != VK_SUCCESS)
    {
        throw std::runtime_error("Could not create Dear ImGui's render pass");
    }

    ImGui_ImplVulkan_InitInfo init_info = {};
    init_info.Instance = m_instance->raw();
    init_info.PhysicalDevice = m_physicalDevice->raw();
    init_info.Device = m_logicalDevice->raw();
    init_info.QueueFamily = m_physicalDevice->getQueueFamilies().graphicsFamily.value();
    init_info.Queue = m_logicalDevice->getGraphicsQueue().raw();
    init_info.PipelineCache = VK_NULL_HANDLE;
    init_info.DescriptorPool = m_imguiDescriptorPool;
    init_info.Allocator = nullptr;
    init_info.MinImageCount = m_swapChain->size(); // Not sure
    init_info.ImageCount = m_swapChain->size(); // Not sure
    init_info.CheckVkResultFn = nullptr;
    ImGui_ImplVulkan_Init(&init_info, m_imGuiRenderPass);

    // Upload fonts
    VkCommandBuffer command_buffer = beginSingleTimeCommands();
    ImGui_ImplVulkan_CreateFontsTexture(command_buffer);
    endSingleTimeCommands(command_buffer);

    // Create command pools and command buffers
    createImGuiCommandPool(&m_imGuiCommandPool, VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);
    m_imGuiCommandBuffers.resize(m_swapChain->size());
    createImGuiCommandBuffers(m_imGuiCommandBuffers.data(), static_cast<uint32_t>(m_imGuiCommandBuffers.size()), m_imGuiCommandPool);

    // Create frame buffers
    VkImageView attachment[1];
    VkFramebufferCreateInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    info.renderPass = m_imGuiRenderPass;
    info.attachmentCount = 1;
    info.pAttachments = attachment;
    info.width = m_swapChain->getExtent().width;
    info.height = m_swapChain->getExtent().height;
    info.layers = 1;
    m_imGuiFrameBuffers.resize(m_swapChain->size());
    for (uint32_t i = 0; i < m_swapChain->size(); i++)
    {
        attachment[0] = m_swapChain->getImageViews()[i]->raw();
        vkCreateFramebuffer(m_logicalDevice->raw(), &info, nullptr, &m_imGuiFrameBuffers[i]);
    }
}

void VulkanApplication::imGuiRender()
{
    vkResetCommandPool(m_logicalDevice->raw(), m_imGuiCommandPool, 0);
    VkCommandBufferBeginInfo commandBufferInfo = {};
    commandBufferInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    commandBufferInfo.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    vkBeginCommandBuffer(m_imGuiCommandBuffers[m_imageIndex], &commandBufferInfo);

    VkRenderPassBeginInfo renderPassInfo = {};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = m_imGuiRenderPass;
    renderPassInfo.framebuffer = m_imGuiFrameBuffers[m_imageIndex];
    renderPassInfo.renderArea.extent.width = m_swapChain->getExtent().width;
    renderPassInfo.renderArea.extent.height = m_swapChain->getExtent().height;
    renderPassInfo.clearValueCount = 1;

    VkClearValue clearValue;
    clearValue.color = {{0.0f, 0.0f, 0.0f, 1.0f}};

    renderPassInfo.pClearValues = &clearValue;
    vkCmdBeginRenderPass(m_imGuiCommandBuffers[m_imageIndex], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

    // Record Imgui Draw Data and draw funcs into command buffer
    ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), m_imGuiCommandBuffers[m_imageIndex]);

    // Submit command buffer
    vkCmdEndRenderPass(m_imGuiCommandBuffers[m_imageIndex]);
    vkEndCommandBuffer(m_imGuiCommandBuffers[m_imageIndex]);
}

void VulkanApplication::createImGuiCommandPool(VkCommandPool* commandPool, VkCommandPoolCreateFlags flags)
{
    QueueFamilyIndices queueFamilyIndices = m_logicalDevice->getPhysicalDevice()->getQueueFamilies();

    VkCommandPoolCreateInfo commandPoolCreateInfo = {};
    commandPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    commandPoolCreateInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();
    commandPoolCreateInfo.flags = flags;

    if (vkCreateCommandPool(m_logicalDevice->raw(), &commandPoolCreateInfo, nullptr, commandPool) != VK_SUCCESS)
    {
        throw std::runtime_error("Could not create graphics command pool (ImGui)");
    }
}

void VulkanApplication::createImGuiCommandBuffers(VkCommandBuffer* commandBuffer, uint32_t commandBufferCount, VkCommandPool &commandPool)
{
    VkCommandBufferAllocateInfo commandBufferAllocateInfo = {};
    commandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    commandBufferAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    commandBufferAllocateInfo.commandPool = commandPool;
    commandBufferAllocateInfo.commandBufferCount = commandBufferCount;
    vkAllocateCommandBuffers(m_logicalDevice->raw(), &commandBufferAllocateInfo, commandBuffer);
}

void VulkanApplication::imGuiCleanUp()
{
    for (auto framebuffer : m_imGuiFrameBuffers)
    {
        vkDestroyFramebuffer(m_logicalDevice->raw(), framebuffer, nullptr);
    }
    vkDestroyRenderPass(m_logicalDevice->raw(), m_imGuiRenderPass, nullptr);
    vkFreeCommandBuffers(m_logicalDevice->raw(), m_imGuiCommandPool, static_cast<uint32_t>(m_imGuiCommandBuffers.size()), m_imGuiCommandBuffers.data());
    vkDestroyCommandPool(m_logicalDevice->raw(), m_imGuiCommandPool, nullptr);

    ImGui_ImplVulkan_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    vkDestroyDescriptorPool(m_logicalDevice->raw(), m_imguiDescriptorPool, nullptr);
}