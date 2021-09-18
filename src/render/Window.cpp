#include "render/Window.hpp"

Window::Window()
{
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    m_window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
    glfwSetWindowUserPointer(m_window, this);

    glfwSetFramebufferSizeCallback(m_window, framebufferResizeCallback);
    glfwSetInputMode(m_window, GLFW_STICKY_KEYS, GLFW_TRUE);
}

Window::~Window()
{
    glfwDestroyWindow(m_window);
    glfwTerminate();
}

void Window::createWindowSurface(VkInstance instance, VkSurfaceKHR* surface)
{
    VkResult result = glfwCreateWindowSurface(instance, m_window, nullptr, surface);
    if (result != VK_SUCCESS) 
    {
        const char* description;
        glfwGetError(&description);
        throw std::runtime_error("failed to create window surface! " + std::to_string(result) + " " + description);
    }
}

std::tuple<int, int> Window::getFramebufferSize()
{
    int width, height;
    glfwGetFramebufferSize(m_window, &width, &height);
    return {width, height};
}

bool Window::isKeyPressed(int key)
{
    return glfwGetKey(m_window, key) == GLFW_PRESS;
}

bool Window::shouldClose()
{
    return glfwWindowShouldClose(m_window);
}

std::tuple<uint32_t, const char**> Window::getRequiredInstanceExtensions()
{
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;

    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    return {glfwExtensionCount, glfwExtensions};
}

void Window::framebufferResizeCallback(GLFWwindow* window, int width, int height)
{
    auto app = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
    app->m_framebufferResized = true;
}