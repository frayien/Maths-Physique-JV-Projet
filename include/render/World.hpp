#ifndef MPJVP_WORLD
#define MPJVP_WORLD

#include <vector>
#include <list>

#include "render/Camera.hpp"
#include "render/LightSource.hpp"
#include "render/Entity.hpp"

#include "render/Window.hpp"
#include "render/LogicalDevice.hpp"
#include "render/CommandPool.hpp"

class World
{
private:
    std::shared_ptr<Window> m_window;
    std::shared_ptr<LogicalDevice> m_logicalDevice;
    std::shared_ptr<CommandPool> m_commandPool;

    std::list<std::shared_ptr<Entity> > m_entities;
    bool m_changed = false;

    Camera m_camera;
    LightSource m_lightSource;

public:
    World(const std::shared_ptr<Window> & window, const std::shared_ptr<LogicalDevice> & logicalDevice, const std::shared_ptr<CommandPool> & commandPool);
    virtual ~World();

    std::shared_ptr<Entity> makeEntity(const std::vector<Vertex> & vertices, const std::vector<uint32_t> & indices);
    void addEntity(const std::shared_ptr<Entity> & entity);
    void removeEntity(const std::shared_ptr<Entity> & entity);

    inline const Window & getWindow() const { return *m_window; }
    inline const std::list<std::shared_ptr<Entity> > & getEntities() const { return m_entities; }
    inline bool hasChanged() const { return m_changed; }
    inline void setChanged(bool val) { m_changed = val; }
    inline Camera & getCamera() { return m_camera; }
    inline LightSource & getLightSource() { return m_lightSource; }
    
    std::shared_ptr<Entity> makeSquare(glm::vec3 color = {0.5f, 0.5f, 0.5f});
    std::shared_ptr<Entity> makeDisk(glm::vec3 color = {0.5f, 0.5f, 0.5f});

    std::shared_ptr<Entity> makeCube(glm::vec3 color = {0.5f, 0.5f, 0.5f});
};

#endif // MPJVP_WORLD
