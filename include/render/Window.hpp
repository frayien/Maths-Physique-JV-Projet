#ifndef MPJVP_WINDOW
#define MPJVP_WINDOW

#include <vulkan/vulkan_raii.hpp>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <tuple>
#include <stdexcept>

class Window
{
private:
    const uint32_t WIDTH = 800;
    const uint32_t HEIGHT = 600;

    bool m_framebufferResized = false;

    GLFWwindow* m_window;
    
public:
    Window();
    virtual ~Window();

    /*
     * @return std::tuple<int, int> (width, height)
     */
    std::tuple<int, int> getFramebufferSize();

    bool isKeyPressed(int key) const;
    bool shouldClose();

    inline bool isFramebufferResized() const { return m_framebufferResized; }
    inline void setFramebufferResized(bool val) { m_framebufferResized = val; }

    static std::tuple<uint32_t, const char**> getRequiredInstanceExtensions();
    inline static void waitEvents() { glfwWaitEvents(); }
    inline static void pollEvents() { glfwPollEvents(); }

    inline GLFWwindow* & raw() { return m_window; }

private:
    static void framebufferResizeCallback(GLFWwindow* window, int width, int height);
};

#endif // MPJVP_WINDOW
