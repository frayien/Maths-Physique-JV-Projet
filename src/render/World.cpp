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

std::shared_ptr<BufferedShape> World::makeShape(const std::vector<Vertex> & vertices, const std::vector<uint32_t> & indices)
{
    std::shared_ptr<BufferedShape> entity = std::make_shared<BufferedShape>(m_logicalDevice, m_commandPool, vertices, indices);
    m_entities.push_back(entity);
    m_changed = true;

    return entity;
}

void World::addBufferedShape(const std::shared_ptr<BufferedShape> & entity)
{
    m_entities.push_back(entity);
    m_changed = true;
}

void World::removeBufferedShape(const std::shared_ptr<BufferedShape> & entity)
{
    m_entities.remove(entity);
    m_changed = true;
}

std::shared_ptr<BufferedShape> World::makeSquare(glm::vec3 color)
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

std::shared_ptr<BufferedShape> World::makeDisk(glm::vec3 color)
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

std::shared_ptr<BufferedShape> World::makeCube(glm::vec3 color)
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

std::shared_ptr<BufferedShape> World::makeSphere(glm::vec3 color)
{
    // algorithms and inspiration from : http://www.songho.ca/opengl/gl_sphere.html 

    constexpr float RADIUS = 1.0f;
    constexpr float H_ANGLE = glm::radians(72.0f);
    const float V_ANGLE = glm::atan(0.5f);

    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;

    // ////////////////// first, build an icosahedron ////////////////// //
    // ////// vertices ////// //
    vertices.resize(12);
    // top vertex
    vertices[0] = {{ 0.0f, 0.0f, RADIUS }, color, { 0.0f, 0.0f, 1.0f }};

    const float z  = RADIUS * glm::sin(V_ANGLE); // ring height
    const float xy = RADIUS * glm::cos(V_ANGLE); // ring radius

    float angleRow1 = glm::pi<float>() / 2.0f;
    float angleRow2 = -glm::pi<float>() / 2.0f;

    // 10 vertices, two rings 
    for(size_t i = 1; i < 6; ++i)
    {
        glm::vec3 vertex;

        // top ring
        vertex.x = xy * glm::cos(angleRow1);
        vertex.y = xy * glm::sin(angleRow1);
        vertex.z = z;
        vertices[i]   = {vertex, color, glm::normalize(vertex)};

        angleRow1 += H_ANGLE;

        // bottom ring
        vertex.x = xy * glm::cos(angleRow2);
        vertex.y = xy * glm::sin(angleRow2);
        vertex.z = -z;
        vertices[i+5] = {vertex, color, glm::normalize(vertex)};

        angleRow2 += H_ANGLE;
    }

    // bottom vertex
    vertices[11] = {{ 0.0f, 0.0f, -RADIUS }, color, { 0.0f, 0.0f, -1.0f }};

    // ////// indices ////// //
    for(size_t i = 0; i < 5; ++i)
    {
        // top part
        indices.push_back(0);
        indices.push_back(1+i);
        indices.push_back(1+((i+1)%5));

        // middle part, upside triangles
        indices.push_back(1+i);
        indices.push_back(6+((i+2)%5));
        indices.push_back(6+((i+3)%5));

        // middle part, downside triangles
        indices.push_back(6+i);
        indices.push_back(1+((i+3)%5));
        indices.push_back(1+((i+2)%5));

        // bottom part
        indices.push_back(11);
        indices.push_back(6+((i+1)%5));
        indices.push_back(6+i);
    }

    return makeShape(vertices, indices);
}