#include "render/ImGuiVulkan.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_vulkan.h"

#include <iomanip>
#include <sstream>

ImGuiVulkan::ImGuiVulkan(const std::shared_ptr<Window> & window, const std::shared_ptr<Instance> & instance, const std::shared_ptr<PhysicalDevice> & physicalDevice, const std::shared_ptr<LogicalDevice> & logicalDevice, const std::shared_ptr<SwapChain> & swapChain) :
    m_logicalDevice{logicalDevice},
    m_swapChain{swapChain}
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
    VkDescriptorPoolSize poolSizes[] =
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
    VkDescriptorPoolCreateInfo poolInfo = {};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
    poolInfo.maxSets = 1000 * IM_ARRAYSIZE(poolSizes);
    poolInfo.poolSizeCount = (uint32_t)IM_ARRAYSIZE(poolSizes);
    poolInfo.pPoolSizes = poolSizes;
    vkCreateDescriptorPool(m_logicalDevice->raw(), &poolInfo, nullptr, &m_imguiDescriptorPool);

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

    ImGui_ImplVulkan_InitInfo initInfo = {};
    initInfo.Instance = instance->raw();
    initInfo.PhysicalDevice = physicalDevice->raw();
    initInfo.Device = m_logicalDevice->raw();
    initInfo.QueueFamily = physicalDevice->getQueueFamilies().graphicsFamily.value();
    initInfo.Queue = m_logicalDevice->getGraphicsQueue().raw();
    initInfo.PipelineCache = VK_NULL_HANDLE;
    initInfo.DescriptorPool = m_imguiDescriptorPool;
    initInfo.Allocator = nullptr;
    initInfo.MinImageCount = m_swapChain->size();
    initInfo.ImageCount = m_swapChain->size();
    initInfo.CheckVkResultFn = nullptr;
    ImGui_ImplVulkan_Init(&initInfo, m_imGuiRenderPass);

    // Create command pools and command buffers
    createImGuiCommandPool(&m_imGuiCommandPool, VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);
    m_imGuiCommandBuffers.resize(m_swapChain->size());
    createImGuiCommandBuffers(m_imGuiCommandBuffers.data(), static_cast<uint32_t>(m_imGuiCommandBuffers.size()), m_imGuiCommandPool);

    // Upload fonts to the GPU
    VkCommandBuffer commandBuffer = beginSingleTimeCommands();
    ImGui_ImplVulkan_CreateFontsTexture(commandBuffer);
    endSingleTimeCommands(commandBuffer);

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

ImGuiVulkan::~ImGuiVulkan()
{
    cleanup();

    ImGui_ImplVulkan_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    vkDestroyDescriptorPool(m_logicalDevice->raw(), m_imguiDescriptorPool, nullptr);
}

void ImGuiVulkan::render(uint32_t m_imageIndex)
{
    ImGui::Render();

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

void ImGuiVulkan::cleanup()
{
    for (auto framebuffer : m_imGuiFrameBuffers)
    {
        vkDestroyFramebuffer(m_logicalDevice->raw(), framebuffer, nullptr);
    }
    vkDestroyRenderPass(m_logicalDevice->raw(), m_imGuiRenderPass, nullptr);
    vkFreeCommandBuffers(m_logicalDevice->raw(), m_imGuiCommandPool, static_cast<uint32_t>(m_imGuiCommandBuffers.size()), m_imGuiCommandBuffers.data());
    vkDestroyCommandPool(m_logicalDevice->raw(), m_imGuiCommandPool, nullptr);
}

void ImGuiVulkan::recreate()
{
    cleanup();

    ImGui_ImplVulkan_SetMinImageCount(m_swapChain->size());

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

    createImGuiCommandPool(&m_imGuiCommandPool, VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);
    m_imGuiCommandBuffers.resize(m_swapChain->size());
    createImGuiCommandBuffers(m_imGuiCommandBuffers.data(), static_cast<uint32_t>(m_imGuiCommandBuffers.size()), m_imGuiCommandPool);

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

void ImGuiVulkan::createFrame()
{
    ImGui_ImplVulkan_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // Frame to select the projectile, see its properties and set initial position, initial acceleration and damping
    ImGui::Begin("Projectile selection");

    // Default size
    float imGuiWidth = 330.0f;
    float imGuiHeight = 380.0f;
    ImGui::SetWindowSize(ImVec2(imGuiWidth, imGuiHeight));

    ImGui::Text("Choose the projectile :");

    // Add a combo box to select the projectile
    if (ImGui::BeginCombo("##combo", currentProjectile))
    {
        for (int n = 0; n < IM_ARRAYSIZE(projectiles); n++)
        {
            bool isSelected = (currentProjectile == projectiles[n]);
            if (ImGui::Selectable(projectiles[n], isSelected))
            {
                currentProjectile = projectiles[n];
                currentIndex = n;
            }

            if (isSelected)
            {
                ImGui::SetItemDefaultFocus();
            }

        }
        ImGui::EndCombo();
    }

    ImGui::NewLine();

    ImGui::Text("Projectile's properties : ");

    // Add a table to display projectile's properties (initial velocity and mass)
    if (ImGui::BeginTable("informationTable", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg))
    {
        std::stringstream ss;
        for (int row = 0; row < 2; row++)
        {
            ImGui::TableNextRow();
            for (int column = 0; column < 2; column++)
            {
                ImGui::TableSetColumnIndex(column);

                if (column == 0)
                {
                    // First column : display properties' name
                    if (row == 0)
                    {
                        // First row : initial velocity
                        ImGui::Text("Initial velocity");
                    }
                    else
                    {
                        // Second row : mass
                        ImGui::Text("Mass");
                    }
                }
                else
                {
                    // Second column : display properties' value
                    if (row == 0)
                    {
                        // First row : initial velocity
                        std::stringstream().swap(ss); // clear ss
                        ss << "{";
                        ss << std::fixed << std::setprecision(2) << projectilesInitialVelocity[currentIndex][0];
                        ss << " ; ";
                        ss << std::fixed << std::setprecision(2) << projectilesInitialVelocity[currentIndex][1];
                        ss << " ; ";
                        ss << std::fixed << std::setprecision(2) << projectilesInitialVelocity[currentIndex][2];
                        ss << "}";
                        ImGui::Text(ss.str().c_str());
                    }
                    else
                    {
                        // Second row : mass
                        std::stringstream().swap(ss); // clear ss
                        ss << std::fixed << std::setprecision(2) << projectilesMass[currentIndex];
                        ImGui::Text(ss.str().c_str());
                    }
                }
            }
        }
        ImGui::EndTable();
    }

    ImGui::NewLine();

    ImGui::Text("Adjustable settings :");

    ImGui::Indent(10.0f);

    // Input for the initial position
    ImGui::Text("Initial position (X ; Y ; Z) :");
    ImGui::InputFloat3("", initialPosition.data());

    ImGui::NewLine();

    // Input for the initial acceleration
    ImGui::Text("Initial acceleration (X ; Y ; Z) :");
    ImGui::InputFloat3("", initialAcceleration.data());

    ImGui::NewLine();

    // Input for the damping
    ImGui::Text("Damping : ");
    ImGui::InputFloat("", &damping);

    ImGui::Unindent();

    // Button to confirm selection
    ImGui::NewLine();
    ImGui::SetCursorPosX((ImGui::GetWindowSize().x) * 0.4f);
    if (ImGui::Button("Select"))
    {
        // TODO: change particule
    }

    ImGui::End();
}

VkCommandBuffer ImGuiVulkan::beginSingleTimeCommands()
{
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = m_imGuiCommandPool;
    allocInfo.commandBufferCount = 1;

    VkCommandBuffer commandBuffer;
    vkAllocateCommandBuffers(m_logicalDevice->raw(), &allocInfo, &commandBuffer);

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer(commandBuffer, &beginInfo);

    return commandBuffer;
}

void ImGuiVulkan::endSingleTimeCommands(VkCommandBuffer commandBuffer)
{
    vkEndCommandBuffer(commandBuffer);

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;

    vkQueueSubmit(m_logicalDevice->getGraphicsQueue().raw(), 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(m_logicalDevice->getGraphicsQueue().raw());

    vkFreeCommandBuffers(m_logicalDevice->raw(), m_imGuiCommandPool, 1, &commandBuffer);
}

void ImGuiVulkan::createImGuiCommandPool(VkCommandPool* commandPool, VkCommandPoolCreateFlags flags)
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

void ImGuiVulkan::createImGuiCommandBuffers(VkCommandBuffer* commandBuffer, uint32_t commandBufferCount, VkCommandPool &commandPool)
{
    VkCommandBufferAllocateInfo commandBufferAllocateInfo = {};
    commandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    commandBufferAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    commandBufferAllocateInfo.commandPool = commandPool;
    commandBufferAllocateInfo.commandBufferCount = commandBufferCount;
    vkAllocateCommandBuffers(m_logicalDevice->raw(), &commandBufferAllocateInfo, commandBuffer);
}