#ifndef FNES_WINDOW
#define FNES_WINDOW

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <tuple>
#include <stdexcept>
#include <string>

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

    void createWindowSurface(VkInstance instance, VkSurfaceKHR* surface);

    /*
     * @return std::tuple<int, int> (width, height)
     */
    std::tuple<int, int> getFramebufferSize();

    bool isKeyPressed(int key);
    bool shouldClose();

    inline bool isFramebufferResized() const { return m_framebufferResized; }
    inline void setFramebufferResized(bool val) { m_framebufferResized = val; }

    static std::tuple<uint32_t, const char**> getRequiredInstanceExtensions();
    inline static void waitEvents() { glfwWaitEvents(); }
    inline static void pollEvents() { glfwPollEvents(); }

private:
    static void framebufferResizeCallback(GLFWwindow* window, int width, int height);
};

#endif // FNES_WINDOW
