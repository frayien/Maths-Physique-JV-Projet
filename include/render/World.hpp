#ifndef FNES_WORLD
#define FNES_WORLD

#include <vector>

#include "render/Entity.hpp"

#include "render/PhysicalDevice.hpp"
#include "render/LogicalDevice.hpp"
#include "render/CommandPool.hpp"

class World
{
private:
    std::shared_ptr<PhysicalDevice> m_physicalDevice;
    std::shared_ptr<LogicalDevice> m_logicalDevice;
    std::shared_ptr<CommandPool> m_commandPool;

    std::vector<Entity> m_entities;

    bool m_changed = false;

public:
    World(const std::shared_ptr<PhysicalDevice> & physicalDevice, const std::shared_ptr<LogicalDevice> & logicalDevice, const std::shared_ptr<CommandPool> & commandPool);
    virtual ~World();

    void addEntity(const std::vector<Vertex> & vertices, const std::vector<uint32_t> & indices);

    inline const std::vector<Entity> & getEntities() const { return m_entities; }
    inline bool hasChanged() const { return m_changed; }
    inline void setChanged(bool val) { m_changed = val; }
};

#endif // FNES_WORLD
