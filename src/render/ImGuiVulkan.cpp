#include "render/ImGuiVulkan.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_vulkan.h"

#include <iomanip>
#include <sstream>

ImGuiVulkan::ImGuiVulkan(
    const std::unique_ptr<Window> & window,
    const std::unique_ptr<vk::raii::Instance> & instance,
    const std::unique_ptr<vk::raii::PhysicalDevice> & physicalDevice,
    const std::unique_ptr<vk::raii::Device> & device,
    const std::unique_ptr<vk::raii::Queue> & graphicsQueue,
    const std::vector<std::unique_ptr<vk::raii::ImageView> > & swapchainImageViews,
    vk::Extent2D swapchainExtent,
    vk::Format imageFormat,
    uint32_t graphicsFamily) :
    m_device{device.get()},
    m_graphicsFamily{graphicsFamily},
    m_swapchainExtent{swapchainExtent},
    m_swapchainSize{swapchainImageViews.size()}
{
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForVulkan(window->raw(), true);

    // Create Descriptor Pool for Imgui
    std::array<vk::DescriptorPoolSize, 11> poolSizes =
    {
        vk::DescriptorPoolSize{ vk::DescriptorType::eSampler, 1000 },
        vk::DescriptorPoolSize{ vk::DescriptorType::eCombinedImageSampler, 1000 },
        vk::DescriptorPoolSize{ vk::DescriptorType::eSampledImage, 1000 },
        vk::DescriptorPoolSize{ vk::DescriptorType::eStorageImage, 1000 },
        vk::DescriptorPoolSize{ vk::DescriptorType::eUniformTexelBuffer, 1000 },
        vk::DescriptorPoolSize{ vk::DescriptorType::eStorageTexelBuffer, 1000 },
        vk::DescriptorPoolSize{ vk::DescriptorType::eUniformBuffer, 1000 },
        vk::DescriptorPoolSize{ vk::DescriptorType::eStorageBuffer, 1000 },
        vk::DescriptorPoolSize{ vk::DescriptorType::eUniformBufferDynamic, 1000 },
        vk::DescriptorPoolSize{ vk::DescriptorType::eStorageBufferDynamic, 1000 },
        vk::DescriptorPoolSize{ vk::DescriptorType::eInputAttachment, 1000 }
    };

    vk::DescriptorPoolCreateInfo poolInfo{};
    poolInfo.flags = vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet;
    poolInfo.maxSets = static_cast<uint32_t>(1000 * poolSizes.size());
    poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
    poolInfo.pPoolSizes = poolSizes.data();

    m_imguiDescriptorPool = std::make_unique<vk::raii::DescriptorPool>(*m_device, poolInfo);

    // Create Render Pass for Imgui
    vk::AttachmentDescription attachmentDescription{};
    attachmentDescription.format = imageFormat;
    attachmentDescription.samples = vk::SampleCountFlagBits::e1;
    attachmentDescription.loadOp = vk::AttachmentLoadOp::eLoad;
    attachmentDescription.storeOp = vk::AttachmentStoreOp::eStore;
    attachmentDescription.stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
    attachmentDescription.stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
    attachmentDescription.initialLayout = vk::ImageLayout::eColorAttachmentOptimal;
    attachmentDescription.finalLayout = vk::ImageLayout::ePresentSrcKHR;

    vk::AttachmentReference color_attachment{};
    color_attachment.attachment = 0;
    color_attachment.layout = vk::ImageLayout::eColorAttachmentOptimal;

    vk::SubpassDescription subpass{};
    subpass.pipelineBindPoint = vk::PipelineBindPoint::eGraphics;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &color_attachment;

    // Create Subpass Dependency to synchronize the render passes
    vk::SubpassDependency dependency{};
    dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass = 0;
    dependency.srcStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput;
    dependency.dstStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput;
    dependency.srcAccessMask = {}; // or vk::AccessFlagBits::eColorAttachmentWrite;
    dependency.dstAccessMask = vk::AccessFlagBits::eColorAttachmentWrite;

    vk::RenderPassCreateInfo renderPassCreateInfo{};
    renderPassCreateInfo.attachmentCount = 1;
    renderPassCreateInfo.pAttachments = &attachmentDescription;
    renderPassCreateInfo.subpassCount = 1;
    renderPassCreateInfo.pSubpasses = &subpass;
    renderPassCreateInfo.dependencyCount = 1;
    renderPassCreateInfo.pDependencies = &dependency;

    m_imGuiRenderPass = std::make_unique<vk::raii::RenderPass>(*m_device, renderPassCreateInfo);

    ImGui_ImplVulkan_InitInfo initInfo{};
    initInfo.Instance = **instance;
    initInfo.PhysicalDevice = **physicalDevice;
    initInfo.Device = **m_device;
    initInfo.QueueFamily = graphicsFamily;
    initInfo.Queue = **graphicsQueue;
    initInfo.PipelineCache = VK_NULL_HANDLE;
    initInfo.DescriptorPool = **m_imguiDescriptorPool;
    initInfo.Allocator = nullptr;
    initInfo.MinImageCount = static_cast<uint32_t>(m_swapchainSize);
    initInfo.ImageCount = static_cast<uint32_t>(m_swapchainSize);
    initInfo.CheckVkResultFn = nullptr;
    ImGui_ImplVulkan_Init(&initInfo, **m_imGuiRenderPass);

    // Create command pools and command buffers
    createImGuiCommandPool();
    createImGuiCommandBuffers();

    // Upload fonts to the GPU
    auto commandBuffer = beginSingleTimeCommands();
    ImGui_ImplVulkan_CreateFontsTexture(*(*commandBuffer)[0]);
    endSingleTimeCommands(commandBuffer, graphicsQueue);

    // Create frame buffers
    m_imGuiFrameBuffers.resize(m_swapchainSize);

    for (uint32_t i = 0; i < swapchainImageViews.size(); i++)
    {
        std::array<vk::ImageView, 1> attachment = {**swapchainImageViews[i]};

        vk::FramebufferCreateInfo info = {};
        info.renderPass = **m_imGuiRenderPass;
        info.attachmentCount = 1;
        info.pAttachments = attachment.data();
        info.width = swapchainExtent.width;
        info.height = swapchainExtent.height;
        info.layers = 1;

        m_imGuiFrameBuffers[i] = std::make_unique<vk::raii::Framebuffer>(*m_device, info);
    }
}

ImGuiVulkan::~ImGuiVulkan()
{
    ImGui_ImplVulkan_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void ImGuiVulkan::render(uint32_t imageIndex)
{
    ImGui::Render();

    const auto & commandBuffer = (*m_imGuiCommandBuffers)[imageIndex];

    vk::CommandBufferBeginInfo commandBufferInfo{};
    commandBufferInfo.flags |= vk::CommandBufferUsageFlagBits::eOneTimeSubmit;

    commandBuffer.begin(commandBufferInfo);

    vk::RenderPassBeginInfo renderPassInfo{};
    renderPassInfo.renderPass = **m_imGuiRenderPass;
    renderPassInfo.framebuffer = **m_imGuiFrameBuffers[imageIndex];
    renderPassInfo.renderArea.extent = m_swapchainExtent;
    renderPassInfo.clearValueCount = 1;

    vk::ClearValue clearValue{};
    clearValue.color = std::array<float, 4>{0.0f, 0.0f, 0.0f, 1.0f};

    renderPassInfo.pClearValues = &clearValue;

    commandBuffer.beginRenderPass(renderPassInfo, vk::SubpassContents::eInline);

    // Record Imgui Draw Data and draw funcs into command buffer
    ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), *commandBuffer);

    // Submit command buffer
    commandBuffer.endRenderPass();
    commandBuffer.end();
}

void ImGuiVulkan::recreate(const std::vector<std::unique_ptr<vk::raii::ImageView> > & swapchainImageViews, vk::Extent2D swapchainExtent, vk::Format imageFormat)
{
    m_swapchainExtent = swapchainExtent;
    m_swapchainSize = swapchainImageViews.size();

    m_imGuiCommandBuffers.reset();
    m_imGuiCommandPool.reset();

    m_imGuiFrameBuffers.clear();

    ImGui_ImplVulkan_SetMinImageCount(m_swapchainSize);

    vk::AttachmentDescription attachmentDescription{};
    attachmentDescription.format = imageFormat;
    attachmentDescription.samples = vk::SampleCountFlagBits::e1;
    attachmentDescription.loadOp = vk::AttachmentLoadOp::eLoad;
    attachmentDescription.storeOp = vk::AttachmentStoreOp::eStore;
    attachmentDescription.stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
    attachmentDescription.stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
    attachmentDescription.initialLayout = vk::ImageLayout::eColorAttachmentOptimal;
    attachmentDescription.finalLayout = vk::ImageLayout::ePresentSrcKHR;

    vk::AttachmentReference color_attachment{};
    color_attachment.attachment = 0;
    color_attachment.layout = vk::ImageLayout::eColorAttachmentOptimal;

    vk::SubpassDescription subpass{};
    subpass.pipelineBindPoint = vk::PipelineBindPoint::eGraphics;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &color_attachment;

    // Create Subpass Dependency to synchronize the render passes
    vk::SubpassDependency dependency{};
    dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass = 0;
    dependency.srcStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput;
    dependency.dstStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput;
    dependency.srcAccessMask = {};  // or vk::AccessFlagBits::eColorAttachmentWrite;
    dependency.dstAccessMask = vk::AccessFlagBits::eColorAttachmentWrite;

    vk::RenderPassCreateInfo renderPassCreateInfo{};
    renderPassCreateInfo.attachmentCount = 1;
    renderPassCreateInfo.pAttachments = &attachmentDescription;
    renderPassCreateInfo.subpassCount = 1;
    renderPassCreateInfo.pSubpasses = &subpass;
    renderPassCreateInfo.dependencyCount = 1;
    renderPassCreateInfo.pDependencies = &dependency;

    m_imGuiRenderPass = std::make_unique<vk::raii::RenderPass>(*m_device, renderPassCreateInfo);

    createImGuiCommandPool();
    createImGuiCommandBuffers();

    m_imGuiFrameBuffers.resize(m_swapchainSize);

    for (uint32_t i = 0; i < swapchainImageViews.size(); i++)
    {
        std::array<vk::ImageView, 1> attachment = {**swapchainImageViews[i]};

        vk::FramebufferCreateInfo info = {};
        info.renderPass = **m_imGuiRenderPass;
        info.attachmentCount = 1;
        info.pAttachments = attachment.data();
        info.width = swapchainExtent.width;
        info.height = swapchainExtent.height;
        info.layers = 1;

        m_imGuiFrameBuffers[i] = std::make_unique<vk::raii::Framebuffer>(*m_device, info);
    }
}

void ImGuiVulkan::createFrame(IImGuiFrameGenerator* generator)
{
    ImGui_ImplVulkan_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // Default size
    float imGuiWidth = 330.0f;
    float imGuiHeight = 400.0f;
    ImGui::SetWindowSize(ImVec2(imGuiWidth, imGuiHeight));

    generator->createFrame();
}

std::unique_ptr<vk::raii::CommandBuffers> ImGuiVulkan::beginSingleTimeCommands()
{
    vk::CommandBufferAllocateInfo allocInfo{};
    allocInfo.level = vk::CommandBufferLevel::ePrimary;
    allocInfo.commandPool = **m_imGuiCommandPool;
    allocInfo.commandBufferCount = 1;

    auto commandBuffers = std::make_unique<vk::raii::CommandBuffers>(*m_device, allocInfo);

    vk::CommandBufferBeginInfo beginInfo{};
    beginInfo.flags = vk::CommandBufferUsageFlagBits::eOneTimeSubmit;

    (*commandBuffers)[0].begin(beginInfo);

    return commandBuffers;
}

void ImGuiVulkan::endSingleTimeCommands(const std::unique_ptr<vk::raii::CommandBuffers> & commandBuffers, const std::unique_ptr<vk::raii::Queue> & graphicsQueue)
{
    (*commandBuffers)[0].end();

    vk::SubmitInfo submitInfo{};
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &*(*commandBuffers)[0];

    graphicsQueue->submit(submitInfo);
    graphicsQueue->waitIdle();
}

void ImGuiVulkan::createImGuiCommandPool()
{
    vk::CommandPoolCreateInfo commandPoolCreateInfo{};
    commandPoolCreateInfo.queueFamilyIndex = m_graphicsFamily;
    commandPoolCreateInfo.flags = vk::CommandPoolCreateFlagBits::eResetCommandBuffer;

    m_imGuiCommandPool = std::make_unique<vk::raii::CommandPool>(*m_device, commandPoolCreateInfo);
}

void ImGuiVulkan::createImGuiCommandBuffers()
{
    vk::CommandBufferAllocateInfo commandBufferAllocateInfo{};
    commandBufferAllocateInfo.level = vk::CommandBufferLevel::ePrimary;
    commandBufferAllocateInfo.commandPool = **m_imGuiCommandPool;
    commandBufferAllocateInfo.commandBufferCount = m_swapchainSize;
    m_imGuiCommandBuffers = std::make_unique<vk::raii::CommandBuffers>(*m_device, commandBufferAllocateInfo);
}