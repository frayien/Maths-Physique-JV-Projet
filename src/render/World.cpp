#include "render/World.hpp"

World::World(const std::shared_ptr<Window> & window, const std::shared_ptr<LogicalDevice> & logicalDevice, const std::shared_ptr<CommandPool> & commandPool) :
    m_window{window},
    m_logicalDevice{logicalDevice},
    m_commandPool{commandPool}
{
}

World::~World()
{
}

std::shared_ptr<Entity> World::makeEntity(const std::vector<Vertex> & vertices, const std::vector<uint32_t> & indices)
{
    std::shared_ptr<Entity> entity = std::make_shared<Entity>(m_logicalDevice, m_commandPool, vertices, indices);
    m_entities.push_back(entity);
    m_changed = true;

    return entity;
}

void World::addEntity(const std::shared_ptr<Entity> & entity)
{
    m_entities.push_back(entity);
    m_changed = true;
}

void World::removeEntity(const std::shared_ptr<Entity> & entity)
{
    m_entities.remove(entity);
    m_changed = true;
}