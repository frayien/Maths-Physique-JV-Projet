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

std::shared_ptr<Shape> World::makeShape(const std::vector<Vertex> & vertices, const std::vector<uint32_t> & indices)
{
    std::shared_ptr<Shape> entity = std::make_shared<Shape>(m_logicalDevice, m_commandPool, vertices, indices);
    m_entities.push_back(entity);
    m_changed = true;

    return entity;
}

void World::addShape(const std::shared_ptr<Shape> & entity)
{
    m_entities.push_back(entity);
    m_changed = true;
}

void World::removeShape(const std::shared_ptr<Shape> & entity)
{
    m_entities.remove(entity);
    m_changed = true;
}

std::shared_ptr<Shape> World::makeSquare(glm::vec3 color)
{
    std::vector<Vertex> vertices = 
    {
        {{-1.0f, -1.0f, 0.0f}, color, {0.0f, 0.0f, 1.0f}},
        {{ 1.0f, -1.0f, 0.0f}, color, {0.0f, 0.0f, 1.0f}},
        {{ 1.0f,  1.0f, 0.0f}, color, {0.0f, 0.0f, 1.0f}},
        {{-1.0f,  1.0f, 0.0f}, color, {0.0f, 0.0f, 1.0f}},
    };
    std::vector<uint32_t> indices = 
    {
        0,1,2,2,3,0,
    };

    return makeShape(vertices, indices);
}

std::shared_ptr<Shape> World::makeDisk(glm::vec3 color)
{
    const size_t VERTEX_N = 50;
    std::vector<Vertex> vertices = { {{0.0f, 0.0f, 0.0f}, color, {0.0f, 0.0f, 1.0f}} };
    std::vector<uint32_t> indices;

    for(size_t i = 0; i<VERTEX_N; ++i)
    {
        float angle = static_cast<float>(i) / static_cast<float>(VERTEX_N) * 2.0f * glm::pi<float>();

        vertices.push_back({{ glm::cos(angle), glm::sin(angle), 0.0f}, color, {0.0f, 0.0f, 1.0f}});
        indices.push_back(0);
        indices.push_back(i+1);
        indices.push_back(i+2);
    }
    indices.back() = 1;

    return makeShape(vertices, indices);
}

std::shared_ptr<Shape> World::makeCube(glm::vec3 color)
{
    std::vector<Vertex> vertices = 
    {
        {{ 1.0f, -1.0f,  1.0f}, color, { 1.0f, 0.0f, 0.0f}}, // x+
        {{ 1.0f, -1.0f, -1.0f}, color, { 1.0f, 0.0f, 0.0f}},
        {{ 1.0f,  1.0f, -1.0f}, color, { 1.0f, 0.0f, 0.0f}},
        {{ 1.0f,  1.0f,  1.0f}, color, { 1.0f, 0.0f, 0.0f}},

        {{-1.0f, -1.0f,  1.0f}, color, {-1.0f, 0.0f, 0.0f}}, // x-
        {{-1.0f,  1.0f,  1.0f}, color, {-1.0f, 0.0f, 0.0f}},
        {{-1.0f,  1.0f, -1.0f}, color, {-1.0f, 0.0f, 0.0f}},
        {{-1.0f, -1.0f, -1.0f}, color, {-1.0f, 0.0f, 0.0f}},

        {{ 1.0f,  1.0f,  1.0f}, color, {0.0f,  1.0f, 0.0f}}, // y+
        {{ 1.0f,  1.0f, -1.0f}, color, {0.0f,  1.0f, 0.0f}},
        {{-1.0f,  1.0f, -1.0f}, color, {0.0f,  1.0f, 0.0f}},
        {{-1.0f,  1.0f,  1.0f}, color, {0.0f,  1.0f, 0.0f}},

        {{ 1.0f, -1.0f,  1.0f}, color, {0.0f, -1.0f, 0.0f}}, // y-
        {{-1.0f, -1.0f,  1.0f}, color, {0.0f, -1.0f, 0.0f}},
        {{-1.0f, -1.0f, -1.0f}, color, {0.0f, -1.0f, 0.0f}},
        {{ 1.0f, -1.0f, -1.0f}, color, {0.0f, -1.0f, 0.0f}},

        {{-1.0f, -1.0f,  1.0f}, color, {0.0f, 0.0f,  1.0f}}, // z+
        {{ 1.0f, -1.0f,  1.0f}, color, {0.0f, 0.0f,  1.0f}},
        {{ 1.0f,  1.0f,  1.0f}, color, {0.0f, 0.0f,  1.0f}},
        {{-1.0f,  1.0f,  1.0f}, color, {0.0f, 0.0f,  1.0f}},

        {{-1.0f, -1.0f, -1.0f}, color, {0.0f, 0.0f, -1.0f}}, // z-
        {{-1.0f,  1.0f, -1.0f}, color, {0.0f, 0.0f, -1.0f}},
        {{ 1.0f,  1.0f, -1.0f}, color, {0.0f, 0.0f, -1.0f}},
        {{ 1.0f, -1.0f, -1.0f}, color, {0.0f, 0.0f, -1.0f}},
    };
    std::vector<uint32_t> indices = 
    {
         0, 1, 2, 2, 3, 0,
         4, 5, 6, 6, 7, 4,
         8, 9,10,10,11, 8,
        12,13,14,14,15,12,
        16,17,18,18,19,16,
        20,21,22,22,23,20,
    };

    return makeShape(vertices, indices);
}

std::shared_ptr<Shape> World::makeSphere(glm::vec3 color)
{
    const size_t VERTEX_N = 50;
    std::vector<Vertex> vertices = { {{0.0f, 0.0f, 0.0f}, color, {0.0f, 0.0f, 1.0f}} };
    std::vector<uint32_t> indices;

    for(size_t i = 0; i<VERTEX_N; ++i)
    {
        float angle = static_cast<float>(i) / static_cast<float>(VERTEX_N) * 2.0f * glm::pi<float>();

        vertices.push_back({{ glm::cos(angle), glm::sin(angle), 0.0f}, color, {0.0f, 0.0f, 1.0f}});
        indices.push_back(0);
        indices.push_back(i+1);
        indices.push_back(i+2);
    }
    indices.back() = 1;

    return makeShape(vertices, indices);
}