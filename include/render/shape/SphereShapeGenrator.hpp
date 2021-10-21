#ifndef MPJVP_SPHERESHAPEGENRATOR
#define MPJVP_SPHERESHAPEGENRATOR

#include "render/shape/IShapeGenerator.hpp"
#include "render/shape/Transformable.hpp"

class SphereShapeGenrator : public IShapeGenerator, public Transformable
{
public:
    SphereShapeGenrator(glm::vec3 color = { 0.5f, 0.5f, 0.5f });
    ~SphereShapeGenrator();

    virtual void addShape(std::vector<Shape> & shapes) const override;

private:
    glm::vec3 m_color;
    float m_radius = 1.0f;
};

SphereShapeGenrator::SphereShapeGenrator(glm::vec3 color) :
    m_color{color}
{
}

SphereShapeGenrator::~SphereShapeGenrator()
{
}

void SphereShapeGenrator::addShape(std::vector<Shape> & shapes) const
{
    // algorithms and inspiration from : http://www.songho.ca/opengl/gl_sphere.html

    constexpr size_t SUBDIVISION_N = 3;
    constexpr float H_ANGLE = glm::radians(72.0f);
    const float V_ANGLE = glm::atan(0.5f);

    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;

    // ////////////////// first, build an icosahedron ////////////////// //
    // ////// vertices ////// //
    vertices.resize(12);
    // top vertex
    vertices[0] = {{ 0.0f, 0.0f, m_radius }, m_color, { 0.0f, 0.0f, 1.0f }};

    const float z  = m_radius * glm::sin(V_ANGLE); // ring height
    const float xy = m_radius * glm::cos(V_ANGLE); // ring radius

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
        vertices[i]   = {vertex, m_color, glm::normalize(vertex)};

        angle_row1 += H_ANGLE;

        // bottom ring
        vertex.x = xy * glm::cos(angle_row2);
        vertex.y = xy * glm::sin(angle_row2);
        vertex.z = -z;
        vertices[i+5] = {vertex, m_color, glm::normalize(vertex)};

        angle_row2 += H_ANGLE;
    }

    // bottom vertex
    vertices[11] = {{ 0.0f, 0.0f, -m_radius }, m_color, { 0.0f, 0.0f, -1.0f }};

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
                vertices.push_back({m_radius * v01_vec, m_color, v01_vec});
                middle_indices[v0_i + v1_i*old_index_n] = v01_i;
                middle_indices[v1_i + v0_i*old_index_n] = v01_i;
            }
            if(v12_i == NUMERIC_LIM)
            {
                const Vertex & v1 = vertices[v1_i];
                const Vertex & v2 = vertices[v2_i];
                v12_i = vertices.size();
                glm::vec3 v12_vec = glm::normalize(v1.pos + v2.pos);
                vertices.push_back({m_radius * v12_vec, m_color, v12_vec});
                middle_indices[v1_i + v2_i*old_index_n] = v12_i;
                middle_indices[v2_i + v1_i*old_index_n] = v12_i;
            }
            if(v20_i == NUMERIC_LIM)
            {
                const Vertex & v2 = vertices[v2_i];
                const Vertex & v0 = vertices[v0_i];
                v20_i = vertices.size();
                glm::vec3 v20_vec = glm::normalize(v2.pos + v0.pos);
                vertices.push_back({m_radius * v20_vec, m_color, v20_vec});
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

    shapes.push_back({vertices, indices, getTransform()});
}

#endif // MPJVP_SPHERESHAPEGENRATOR
