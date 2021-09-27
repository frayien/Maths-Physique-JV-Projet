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
    m_shapes.push_back(entity);
    m_changed = true;

    return entity;
}

void World::addShape(const std::shared_ptr<BufferedShape> & entity)
{
    m_shapes.push_back(entity);
    m_changed = true;
}

void World::removeShape(const std::shared_ptr<BufferedShape> & entity)
{
    m_shapes.remove(entity);
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

    constexpr size_t SUBDIVISION_N = 3;
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

    float angle_row1 = glm::pi<float>() / 2.0f;
    float angle_row2 = -glm::pi<float>() / 2.0f;

    // 10 vertices, two rings 
    for(size_t i = 1; i < 6; ++i)
    {
        glm::vec3 vertex;

        // top ring
        vertex.x = xy * glm::cos(angle_row1);
        vertex.y = xy * glm::sin(angle_row1);
        vertex.z = z;
        vertices[i]   = {vertex, color, glm::normalize(vertex)};

        angle_row1 += H_ANGLE;

        // bottom ring
        vertex.x = xy * glm::cos(angle_row2);
        vertex.y = xy * glm::sin(angle_row2);
        vertex.z = -z;
        vertices[i+5] = {vertex, color, glm::normalize(vertex)};

        angle_row2 += H_ANGLE;
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

    // ////////////////// then, subdivide edges ////////////////// //
    for(size_t i = 0; i < SUBDIVISION_N; ++i)
    {
        auto old_indices = indices;
        indices.clear();

        constexpr uint32_t NUMERIC_LIM = std::numeric_limits<uint32_t>::max();

        size_t old_index_n = vertices.size();
        std::vector<uint32_t> middle_indices;
        middle_indices.resize(old_index_n*old_index_n, NUMERIC_LIM);
        
        // subdivide each triangles
        for(size_t j = 0; j < old_indices.size(); j += 3)
        {
            //         v0
            //        / \ 
            //   v20 *---* v01
            //      / \ / \ 
            //    v2---*---v1
            //        v12

            uint32_t v0_i = old_indices[j];
            uint32_t v1_i = old_indices[j+1];
            uint32_t v2_i = old_indices[j+2];
            uint32_t v01_i = middle_indices[v0_i + v1_i*old_index_n];
            uint32_t v12_i = middle_indices[v1_i + v2_i*old_index_n];
            uint32_t v20_i = middle_indices[v2_i + v0_i*old_index_n];

            // for each vertex, create it if non existent
            if(v01_i == NUMERIC_LIM)
            {
                const Vertex & v0 = vertices[v0_i];
                const Vertex & v1 = vertices[v1_i];
                v01_i = vertices.size();
                glm::vec3 v01_vec = glm::normalize(v0.pos + v1.pos);
                vertices.push_back({RADIUS * v01_vec, color, v01_vec});
                middle_indices[v0_i + v1_i*old_index_n] = v01_i;
                middle_indices[v1_i + v0_i*old_index_n] = v01_i;
            }
            if(v12_i == NUMERIC_LIM)
            {
                const Vertex & v1 = vertices[v1_i];
                const Vertex & v2 = vertices[v2_i];
                v12_i = vertices.size();
                glm::vec3 v12_vec = glm::normalize(v1.pos + v2.pos);
                vertices.push_back({RADIUS * v12_vec, color, v12_vec});
                middle_indices[v1_i + v2_i*old_index_n] = v12_i;
                middle_indices[v2_i + v1_i*old_index_n] = v12_i;
            }
            if(v20_i == NUMERIC_LIM)
            {
                const Vertex & v2 = vertices[v2_i];
                const Vertex & v0 = vertices[v0_i];
                v20_i = vertices.size();
                glm::vec3 v20_vec = glm::normalize(v2.pos + v0.pos);
                vertices.push_back({RADIUS * v20_vec, color, v20_vec});
                middle_indices[v2_i + v0_i*old_index_n] = v20_i;
                middle_indices[v0_i + v2_i*old_index_n] = v20_i;
            }

            //         v0
            //        / \ 
            //   v20 *---* v01
            //      / \ / \ 
            //    v2---*---v1
            //        v12

            // add indices to create 4 new triangles
            indices.push_back(v0_i);
            indices.push_back(v01_i);
            indices.push_back(v20_i);

            indices.push_back(v01_i);
            indices.push_back(v1_i);
            indices.push_back(v12_i);

            indices.push_back(v20_i);
            indices.push_back(v12_i);
            indices.push_back(v2_i);

            indices.push_back(v12_i);
            indices.push_back(v20_i);
            indices.push_back(v01_i);
        }
    }

    return makeShape(vertices, indices);
}