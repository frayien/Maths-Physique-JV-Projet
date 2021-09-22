#include "render/World.hpp"

World::World(const std::shared_ptr<Window> & window, const std::shared_ptr<PhysicalDevice> & physicalDevice, const std::shared_ptr<LogicalDevice> & logicalDevice, const std::shared_ptr<CommandPool> & commandPool) :
    m_window{window},
    m_physicalDevice{physicalDevice},
    m_logicalDevice{logicalDevice},
    m_commandPool{commandPool}
{
}

World::~World()
{
}

void World::addEntity(const std::vector<Vertex> & vertices, const std::vector<uint32_t> & indices)
{
    m_entities.push_back(std::make_unique<Entity>(m_logicalDevice, m_commandPool, *m_physicalDevice, vertices, indices));
    m_changed = true;
}