#ifndef MPJVP_WORLD
#define MPJVP_WORLD

#include <vulkan/vulkan_raii.hpp>

#include <vector>
#include <list>
#include <limits>

#include "render/Camera.hpp"
#include "render/LightSource.hpp"
#include "render/BufferedShape.hpp"

#include "render/Window.hpp"

class VulkanApplication;

class World
{
private:
    std::shared_ptr<Window> m_window;

    const VulkanApplication & m_vulkanApplication;

    std::list<std::shared_ptr<BufferedShape> > m_shapes;
    bool m_changed = false;

    Camera m_camera;
    LightSource m_lightSource;

public:
    World(const std::shared_ptr<Window> & window, const VulkanApplication & vulkanApplication);
    virtual ~World();

    std::shared_ptr<BufferedShape> makeShape(const std::vector<Vertex> & vertices, const std::vector<uint32_t> & indices);
    void addShape(const std::shared_ptr<BufferedShape> & entity);
    void removeShape(const std::shared_ptr<BufferedShape> & entity);

    inline const Window & getWindow() const { return *m_window; }
    inline const std::list<std::shared_ptr<BufferedShape> > & getShapes() const { return m_shapes; }
    inline bool hasChanged() const { return m_changed; }
    inline void setChanged(bool val) { m_changed = val; }
    inline Camera & getCamera() { return m_camera; }
    inline LightSource & getLightSource() { return m_lightSource; }
    
    std::shared_ptr<BufferedShape> makeSquare(glm::vec3 color = {0.5f, 0.5f, 0.5f});
    std::shared_ptr<BufferedShape> makeDisk(glm::vec3 color = {0.5f, 0.5f, 0.5f});

    std::shared_ptr<BufferedShape> makeCube(glm::vec3 color = {0.5f, 0.5f, 0.5f});
    std::shared_ptr<BufferedShape> makeSphere(glm::vec3 color = {0.5f, 0.5f, 0.5f});
};

#endif // MPJVP_WORLD
