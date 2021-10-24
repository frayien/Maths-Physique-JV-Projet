#include "Application.hpp"

Application::Application() :
    m_graphicsEngine{this}
{
}

Application::~Application()
{
}

void Application::run()
{
    init();

    while (!m_graphicsEngine.windowShouldClose()) 
    {
        static auto previousTime = std::chrono::high_resolution_clock::now();

        auto currentTime = std::chrono::high_resolution_clock::now();
        float deltaTime = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - previousTime).count();
        previousTime = currentTime;

        m_graphicsEngine.windowPollEvents();

        update(deltaTime);

        m_graphicsEngine.clear();

        for(auto & [label, shape] : m_gameState.getShapeGenerators())
        {
            m_graphicsEngine.draw(*shape);
        }

        m_graphicsEngine.display();
    }
}

void Application::init()
{
    Camera & cam = m_graphicsEngine.getCamera();
    LightSource & sun = m_graphicsEngine.getLightSource();

    cam.setPosition({20.0f, 0.0f, 0.0f});
    // rotate cam to look at X- (this way Y+ and Z+ looks more natural for a 2D environment, aka. Y+ is on the right)
    cam.rotateYaw(PI);

    sun.setPosition({0.0f, 0.0f, 0.0f});

    // Create force generators
    std::unique_ptr<ParticleForceGenerator> particleGravity = std::make_unique<ParticleGravity>(9.81f);
    m_gameState.addParticleForceGenerator("gravity", std::move(particleGravity));

    std::unique_ptr<ParticleForceGenerator> particleDrag = std::make_unique<ParticleDrag>(0.0f, 0.1f);
    m_gameState.addParticleForceGenerator("drag", std::move(particleDrag));

    // Create ground and blob
    createGround();
    createBlob();
}

void Application::update(float deltaTime)
{
    static bool canPressPause = true;
    static bool pause = false;
    static constexpr float TIMESTEP = 1.0f / 200.0f;
    static float elapsedTime = 0.0f;
    elapsedTime += deltaTime;

    // If we click on the reset button
    if(m_graphicsEngine.getWindow().isKeyPressed(GLFW_KEY_R))
    {
        resetBlob();
    }

    // Map with which we will call moveBlob, giving it different parameters according to the keys pressed
    static std::unordered_map<std::string, std::function<void()>> blobMouvementMap =
    {
        { "Up", [&]()    { moveBlob(Vector3f{-1.0f,  0.0f, 0.0f}, deltaTime); } },
        { "Down", [&]()  { moveBlob(Vector3f{ 1.0f,  0.0f, 0.0f}, deltaTime); } },
        { "Left", [&]()  { moveBlob(Vector3f{ 0.0f, -1.0f, 0.0f}, deltaTime); } },
        { "Right", [&]() { moveBlob(Vector3f{ 0.0f,  1.0f, 0.0f}, deltaTime); } },
        { "Jump", [&]()  { jumpBlob();                                        } },
    };

    // Movements of the blob
    if (m_graphicsEngine.getWindow().isKeyPressed(GLFW_KEY_G))
    {
        blobMouvementMap.at("Left")();
    }
    if (m_graphicsEngine.getWindow().isKeyPressed(GLFW_KEY_H))
    {
        blobMouvementMap.at("Down")();
    }
    if (m_graphicsEngine.getWindow().isKeyPressed(GLFW_KEY_J))
    {
        blobMouvementMap.at("Right")();
    }
    if (m_graphicsEngine.getWindow().isKeyPressed(GLFW_KEY_Y))
    {
        blobMouvementMap.at("Up")();
    }
    if (m_graphicsEngine.getWindow().isKeyPressed(GLFW_KEY_SPACE))
    {
        blobMouvementMap.at("Jump")();
    }

    // If we click on the pause button
    if(m_graphicsEngine.getWindow().isKeyPressed(GLFW_KEY_P) && canPressPause)
    {
        canPressPause = false;
        pause = !pause;
    }
    else if(!m_graphicsEngine.getWindow().isKeyPressed(GLFW_KEY_P))
    {
        canPressPause = true;
    }

    // If the elapsed time is greater than TIMESTEP, we enter the loop
    while (elapsedTime >= TIMESTEP)
    {
        elapsedTime -= TIMESTEP;

        if (!pause)
        {
            // Update physics engine
            m_physicsEngine.update(TIMESTEP, m_gameState);
        }
    }

    // update graphics
    updateCamera(deltaTime);
}

// Function used to translate and rotate the camera
void Application::updateCamera(float deltaTime)
{
    const Window & win = m_graphicsEngine.getWindow();
          Camera & cam = m_graphicsEngine.getCamera();

    const float rotationSpeed = 1.0f * PI; // rad/s
    const float moveSpeed     = 0.1f; // unit/s

    static std::vector<std::tuple<int, std::function<void ()>>> keymap = 
    {
        { GLFW_KEY_A, [&](){ cam.rotateYaw(   deltaTime * rotationSpeed); } },
        { GLFW_KEY_D, [&](){ cam.rotateYaw(  -deltaTime * rotationSpeed); } },
        { GLFW_KEY_W, [&](){ cam.rotatePitch( deltaTime * rotationSpeed); } },
        { GLFW_KEY_S, [&](){ cam.rotatePitch(-deltaTime * rotationSpeed); } },

        { GLFW_KEY_LEFT , [&](){ cam.move( moveSpeed * glm::vec3{0.0f, 1.0f, 0.0f}); } },
        { GLFW_KEY_RIGHT, [&](){ cam.move(-moveSpeed * glm::vec3{0.0f, 1.0f, 0.0f}); } },
        { GLFW_KEY_UP   , [&](){ cam.move( moveSpeed * glm::vec3{1.0f, 0.0f, 0.0f}); } },
        { GLFW_KEY_DOWN , [&](){ cam.move(-moveSpeed * glm::vec3{1.0f, 0.0f, 0.0f}); } },
        { GLFW_KEY_Q    , [&](){ cam.move( moveSpeed * glm::vec3{0.0f, 0.0f, 1.0f}); } },
        { GLFW_KEY_E    , [&](){ cam.move(-moveSpeed * glm::vec3{0.0f, 0.0f, 1.0f}); } },
    };

    for(const auto & [key, fun] : keymap)
    {
        if(win.isKeyPressed(key)) fun();
    }
}

void Application::createFrame()
{
    ImGui::Begin("Settings");

    if (ImGui::BeginTabBar("SettingsTabBar", ImGuiTabBarFlags_None))
    {
        // Tab to edit blob data
        if (ImGui::BeginTabItem("Blob"))
        {
            // Initial position
            ImGui::Text("Initial position (X ; Y ; Z) :");
            ImGui::InputFloat3("##BlobInitPosition", currentBlobInitialPosition.data(), "%.1f");

            // Particles radius
            ImGui::Text("Particles' radius : ");
            ImGui::InputFloat("##Radius", &m_blobParticleRadius, 0.1f, 1.0f, "%.1f");

            // Edges length
            ImGui::Text("Edges length : ");
            ImGui::InputFloat("##EdgesLength", &m_blobEdgeLength, 0.01f, 0.1f, "%.2f");

            // Distance between center and vertices
            ImGui::Text("Center-to-Vertices length : ");
            ImGui::InputFloat("##CenterToVerticesLength", &m_blobCenterToVerticesLength, 0.01f, 0.1f, "%.2f");

            // Springs constant
            ImGui::Text("Springs stiffness : ");
            ImGui::InputFloat("##SpringStiffness", &m_blobK, 1.0f, 5.0f, "%.2f");

            // Cables restitution
            ImGui::Text("Cables restitution : ");
            ImGui::SliderFloat("##CablesRestitution", &m_blobCableRestitution, 0.0f, 1.0f, "%.1f");

            ImGui::EndTabItem();
        }

        // Tab to edit ground data
        if (ImGui::BeginTabItem("Ground"))
        {
            // Length
            ImGui::Text("Ground length : ");
            ImGui::InputFloat("##GroundLength", &m_groundLength, 2.0f, 10.0f, "%.1f");

            // Width
            ImGui::Text("Ground width : ");
            ImGui::InputFloat("##GroundWidth", &m_groundWidth, 2.0f, 10.0f, "%.1f");

            // Thickness
            ImGui::Text("Ground thickness : ");
            ImGui::InputFloat("##GroundThickness", &m_groundThickness, 0.1f, 1.0f, "%.1f");

            // Restitution
            ImGui::Text("Ground restitution : ");
            ImGui::SliderFloat("##GroundRestitution", &m_groundRestitution, 0.0f, 1.0f, "%.1f");

            // Center position
            ImGui::Text("Center position (X ; Y ; Z) :");
            ImGui::InputFloat3("##GroundCenterPosition", currentGroundCenterPosition.data(), "%.1f");

            ImGui::EndTabItem();
        }

        ImGui::EndTabBar();

        ImGui::NewLine();


        ImGui::Separator();

        // Apply
        ImGui::NewLine();
        ImGui::SetCursorPosX((ImGui::GetWindowSize().x) * 0.45f);
        ImGui::SetCursorPosY((ImGui::GetWindowSize().y) * 0.9f);
        if (ImGui::Button("Apply"))
        {
            changeBlobSettings();
            changeGroundSettings();
        }
    }
    ImGui::End();
}

void Application::createBlob()
{
    float phi = (1.0f + sqrt(5.0f)) / 2.0f;

    // Dodecahedron shape
    std::array<Vector3f, 21> particlePos
    {
        m_blobCenterInitPos,

        m_blobCenterInitPos + Vector3f{1.0f, 1.0f, 1.0f},
        m_blobCenterInitPos + Vector3f{-1.0f, 1.0f, 1.0f},
        m_blobCenterInitPos + Vector3f{1.0f, -1.0f, 1.0f},
        m_blobCenterInitPos + Vector3f{1.0f, 1.0f, -1.0f},
        m_blobCenterInitPos + Vector3f{-1.0f, -1.0f, 1.0f},
        m_blobCenterInitPos + Vector3f{-1.0f, 1.0f, -1.0f},
        m_blobCenterInitPos + Vector3f{1.0f, -1.0f, -1.0f},
        m_blobCenterInitPos + Vector3f{-1.0f, -1.0f, -1.0f},

        m_blobCenterInitPos + Vector3f{0.0f, phi, 1.0f / phi},
        m_blobCenterInitPos + Vector3f{0.0f, -phi, 1.0f / phi},
        m_blobCenterInitPos + Vector3f{0.0f, phi, -1.0f / phi},
        m_blobCenterInitPos + Vector3f{0.0f, -phi, -1.0f / phi},

        m_blobCenterInitPos + Vector3f{1.0f / phi, 0.0f, phi},
        m_blobCenterInitPos + Vector3f{-1.0f / phi, 0.0f, phi},
        m_blobCenterInitPos + Vector3f{1.0f / phi, 0.0f, -phi},
        m_blobCenterInitPos + Vector3f{-1.0f / phi, 0.0f, -phi},

        m_blobCenterInitPos + Vector3f{phi, 1.0f / phi, 0.0f},
        m_blobCenterInitPos + Vector3f{-phi, 1.0f / phi, 0.0f},
        m_blobCenterInitPos + Vector3f{phi, -1.0f / phi, 0.0f},
        m_blobCenterInitPos + Vector3f{-phi, -1.0f / phi, 0.0f}
    };

    // For each particle we create the particle, its shape and apply to it gravity and drag forces + ground contact
    for(std::size_t i = 0; i < particlePos.size(); ++i)
    {
        // Particle
        auto particleBlob = std::make_unique<Particle>(particlePos[i], 1.0f, 0.999f, false, m_blobParticleRadius);
        particleBlob->setVelocity({0.0f, 0.0f, 0.0f});
        m_gameState.addParticle("blob_" + std::to_string(i), std::move(particleBlob));

        // Particle Shape
        auto particuleShapeBlob = std::make_unique<ParticleShapeGenerator>(m_gameState.getParticle("blob_" + std::to_string(i)), glm::vec3{ 0.2f, 0.2f, 0.2f });
        m_gameState.addShapeGenerator("blob_" + std::to_string(i), std::move(particuleShapeBlob));

        // Gravity
        m_physicsEngine.registerForce(m_gameState.getParticle("blob_" + std::to_string(i)), m_gameState.getParticleForceGenerator<ParticleGravity>("gravity"), 0.0f);

        // Drag
        m_physicsEngine.registerForce(m_gameState.getParticle("blob_" + std::to_string(i)), m_gameState.getParticleForceGenerator<ParticleDrag>("drag"), 0.0f);

        // Ground contact
        auto groundContact = m_gameState.getParticleContactGenerator<WallContactGenerator>("groundContact");
        groundContact->addParticle(m_gameState.getParticle("blob_" + std::to_string(i)));

        // Wall 1 contact
        auto wall1Contact = m_gameState.getParticleContactGenerator<WallContactGenerator>("wall1Contact");
        wall1Contact->addParticle(m_gameState.getParticle("blob_" + std::to_string(i)));

        // Wall 2 contact
        auto wall2Contact = m_gameState.getParticleContactGenerator<WallContactGenerator>("wall2Contact");
        wall2Contact->addParticle(m_gameState.getParticle("blob_" + std::to_string(i)));
    }


    // Link shape between blob's particles
    std::array<std::pair<std::string, std::string>, 50> allBlobLinks
    {{
        // Between center and vertices
        {"blob_0", "blob_1"},
        {"blob_0", "blob_2"},
        {"blob_0", "blob_3"},
        {"blob_0", "blob_4"},
        {"blob_0", "blob_5"},
        {"blob_0", "blob_6"},
        {"blob_0", "blob_7"},
        {"blob_0", "blob_8"},
        {"blob_0", "blob_9"},
        {"blob_0", "blob_10"},
        {"blob_0", "blob_11"},
        {"blob_0", "blob_12"},
        {"blob_0", "blob_13"},
        {"blob_0", "blob_14"},
        {"blob_0", "blob_15"},
        {"blob_0", "blob_16"},
        {"blob_0", "blob_17"},
        {"blob_0", "blob_18"},
        {"blob_0", "blob_19"},
        {"blob_0", "blob_20"},

        // Edges
        {"blob_1", "blob_9"},
        {"blob_1", "blob_13"},
        {"blob_1", "blob_17"},
        {"blob_2", "blob_9"},
        {"blob_2", "blob_14"},
        {"blob_2", "blob_18"},
        {"blob_3", "blob_10"},
        {"blob_3", "blob_13"},
        {"blob_3", "blob_19"},
        {"blob_4", "blob_11"},
        {"blob_4", "blob_15"},
        {"blob_4", "blob_17"},
        {"blob_5", "blob_10"},
        {"blob_5", "blob_14"},
        {"blob_5", "blob_20"},
        {"blob_6", "blob_11"},
        {"blob_6", "blob_16"},
        {"blob_6", "blob_18"},
        {"blob_7", "blob_12"},
        {"blob_7", "blob_15"},
        {"blob_7", "blob_19"},
        {"blob_8", "blob_12"},
        {"blob_8", "blob_16"},
        {"blob_8", "blob_20"},
        {"blob_9", "blob_11"},
        {"blob_10", "blob_12"},
        {"blob_13", "blob_14"},
        {"blob_15", "blob_16"},
        {"blob_17", "blob_19"},
        {"blob_18", "blob_20"},
    }};

    // We draw the links between particles
    for(auto & [labelBlobA, labelBlobB] : allBlobLinks)
    {
        auto particleA = m_gameState.getParticle(labelBlobA);
        auto particleB = m_gameState.getParticle(labelBlobB);

        // Link Shape
        auto linkShapeBlob = std::make_unique<LinkShapeGenerator>(particleA, particleB, glm::vec3{ 0.0f, 0.0f, 1.0f });
        m_gameState.addShapeGenerator("blobLink_" + labelBlobA + "_" + labelBlobB, std::move(linkShapeBlob));
    }


    std::array<std::pair<std::string, std::string>, 20> centerToVertices
    {{
        {"blob_0", "blob_1"},
        {"blob_0", "blob_2"},
        {"blob_0", "blob_3"},
        {"blob_0", "blob_4"},
        {"blob_0", "blob_5"},
        {"blob_0", "blob_6"},
        {"blob_0", "blob_7"},
        {"blob_0", "blob_8"},
        {"blob_0", "blob_9"},
        {"blob_0", "blob_10"},
        {"blob_0", "blob_11"},
        {"blob_0", "blob_12"},
        {"blob_0", "blob_13"},
        {"blob_0", "blob_14"},
        {"blob_0", "blob_15"},
        {"blob_0", "blob_16"},
        {"blob_0", "blob_17"},
        {"blob_0", "blob_18"},
        {"blob_0", "blob_19"},
        {"blob_0", "blob_20"},
    }};

    // For each link between the center and the vertices, we create spring forces
    for(auto & [labelBlobA, labelBlobB] : centerToVertices)
    {
        auto particleA = m_gameState.getParticle(labelBlobA);
        auto particleB = m_gameState.getParticle(labelBlobB);

        // Spring of particle B on particle A
        auto blobSpring = std::make_unique<ParticleSpring>(particleB, m_blobK, m_blobCenterToVerticesLength);
        m_physicsEngine.registerForce(particleA, blobSpring.get(), 0.0);
        m_gameState.addParticleForceGenerator("blobSpring_" + labelBlobB + "_" + labelBlobA, std::move(blobSpring));

        // Spring of particle A on particle B
        blobSpring = std::make_unique<ParticleSpring>(particleA, m_blobK, m_blobCenterToVerticesLength);
        m_physicsEngine.registerForce(particleB, blobSpring.get(), 0.0);
        m_gameState.addParticleForceGenerator("blobSpring_" + labelBlobA + "_" + labelBlobB, std::move(blobSpring));
    }

    // For each link between the center and the vertices, we create cable contacts
    for(auto & [labelBlobA, labelBlobB] : centerToVertices)
    {
        auto particleA = m_gameState.getParticle(labelBlobA);
        auto particleB = m_gameState.getParticle(labelBlobB);

        // Cable
        auto blobCable = std::make_unique<ParticleCable>(particleA, particleB, m_blobCenterToVerticesLength, m_blobCableRestitution);
        m_gameState.addParticleContactGenerator("blobCable_" + labelBlobA + "_" + labelBlobB, std::move(blobCable));
    }


    std::array<std::pair<std::string, std::string>, 30> edges
    {{
        {"blob_1", "blob_9"},
        {"blob_1", "blob_13"},
        {"blob_1", "blob_17"},
        {"blob_2", "blob_9"},
        {"blob_2", "blob_14"},
        {"blob_2", "blob_18"},
        {"blob_3", "blob_10"},
        {"blob_3", "blob_13"},
        {"blob_3", "blob_19"},
        {"blob_4", "blob_11"},
        {"blob_4", "blob_15"},
        {"blob_4", "blob_17"},
        {"blob_5", "blob_10"},
        {"blob_5", "blob_14"},
        {"blob_5", "blob_20"},
        {"blob_6", "blob_11"},
        {"blob_6", "blob_16"},
        {"blob_6", "blob_18"},
        {"blob_7", "blob_12"},
        {"blob_7", "blob_15"},
        {"blob_7", "blob_19"},
        {"blob_8", "blob_12"},
        {"blob_8", "blob_16"},
        {"blob_8", "blob_20"},
        {"blob_9", "blob_11"},
        {"blob_10", "blob_12"},
        {"blob_13", "blob_14"},
        {"blob_15", "blob_16"},
        {"blob_17", "blob_19"},
        {"blob_18", "blob_20"},
    }};

    // For each edge, we create spring forces
    for(auto & [labelBlobA, labelBlobB] : edges)
    {
        auto particleA = m_gameState.getParticle(labelBlobA);
        auto particleB = m_gameState.getParticle(labelBlobB);

        // Spring of particle B on particle A
        auto blobSpring = std::make_unique<ParticleSpring>(particleB, m_blobK, m_blobEdgeLength);
        m_physicsEngine.registerForce(particleA, blobSpring.get(), 0.0);
        m_gameState.addParticleForceGenerator("blobSpring_" + labelBlobB + "_" + labelBlobA, std::move(blobSpring));

        // Spring of particle A on particle B
        blobSpring = std::make_unique<ParticleSpring>(particleA, m_blobK, m_blobEdgeLength);
        m_physicsEngine.registerForce(particleB, blobSpring.get(), 0.0);
        m_gameState.addParticleForceGenerator("blobSpring_" + labelBlobA + "_" + labelBlobB, std::move(blobSpring));
    }

    // For each edge, we create cable contacts
    for(auto & [labelBlobA, labelBlobB] : edges)
    {
        auto particleA = m_gameState.getParticle(labelBlobA);
        auto particleB = m_gameState.getParticle(labelBlobB);

        // Cable
        auto blobCable = std::make_unique<ParticleCable>(particleA, particleB, m_blobEdgeLength, m_blobCableRestitution);
        m_gameState.addParticleContactGenerator("blobCable_" + labelBlobA + "_" + labelBlobB, std::move(blobCable));
    }
}

void Application::moveBlob(Vector3f moveDirection, float deltaTime)
{
    std::array<std::string, 21> blobLabels
    {
        "blob_0",
        "blob_1",
        "blob_2",
        "blob_3",
        "blob_4",
        "blob_5",
        "blob_6",
        "blob_7",
        "blob_8",
        "blob_9",
        "blob_10",
        "blob_11",
        "blob_12",
        "blob_13",
        "blob_14",
        "blob_15",
        "blob_16",
        "blob_17",
        "blob_18",
        "blob_19",
        "blob_20",
    };

    float moveForce = 100.0f;
    float maxVelocity = 10.0f;
    moveDirection = moveDirection.normalize();

    // Find particle with highest altitude
    float maxAltitude = std::numeric_limits<float>().lowest();
    Particle * particleHighestAltitude = nullptr;

    for(auto & blobLabel : blobLabels)
    {
        auto particle = m_gameState.getParticle(blobLabel);

        if (maxAltitude < particle->getPosition().getZ())
        {
            maxAltitude = particle->getPosition().getZ();
            particleHighestAltitude = particle;
        }
    }

    // Find particle with lowest altitude
    float minAltitude = std::numeric_limits<float>().max();
    Particle * particleLowestAltitude = nullptr;

    for(auto & blobLabel : blobLabels)
    {
        auto particle = m_gameState.getParticle(blobLabel);

        if (minAltitude > particle->getPosition().getZ())
        {
            minAltitude = particle->getPosition().getZ();
            particleLowestAltitude = particle;
        }
    }

    Particle * blobCentre = m_gameState.getParticle("blob_0");
    Vector3f currentVelocity = blobCentre->getVelocity();
    float currentVelocityInMovementDirection = currentVelocity.dotProduct(moveDirection);

    float velocityToAdd = moveForce * deltaTime;

    if (currentVelocityInMovementDirection + velocityToAdd > maxVelocity)
    {
        velocityToAdd = std::max(0.0f, maxVelocity - currentVelocityInMovementDirection);
    }

    // Move highest and lowest particles for the rotation
    particleHighestAltitude->addVelocity( moveDirection * velocityToAdd);
    particleLowestAltitude->addVelocity( -moveDirection * velocityToAdd);

    // Move the center for global movement
    blobCentre->addVelocity( moveDirection * velocityToAdd);
}

void Application::resetBlob()
{
    float phi = (1.0f + sqrt(5.0f)) / 2.0f;

    std::array<std::pair<std::string, Vector3f>, 21> blobPos
    {{
        {"blob_0", m_blobCenterInitPos},

        {"blob_1",  m_blobCenterInitPos + Vector3f{1.0f, 1.0f, 1.0f}},
        {"blob_2",  m_blobCenterInitPos + Vector3f{-1.0f, 1.0f, 1.0f}},
        {"blob_3",  m_blobCenterInitPos + Vector3f{1.0f, -1.0f, 1.0f}},
        {"blob_4",  m_blobCenterInitPos + Vector3f{1.0f, 1.0f, -1.0f}},
        {"blob_5",  m_blobCenterInitPos + Vector3f{-1.0f, -1.0f, 1.0f}},
        {"blob_6",  m_blobCenterInitPos + Vector3f{-1.0f, 1.0f, -1.0f}},
        {"blob_7",  m_blobCenterInitPos + Vector3f{1.0f, -1.0f,-1.0f}},
        {"blob_8",  m_blobCenterInitPos + Vector3f{-1.0f, -1.0f, -1.0f}},

        {"blob_9",  m_blobCenterInitPos + Vector3f{0.0f, phi, 1.0f / phi}},
        {"blob_10", m_blobCenterInitPos + Vector3f{0.0f, -phi, 1.0f / phi}},
        {"blob_11", m_blobCenterInitPos + Vector3f{0.0f, phi, -1.0f / phi}},
        {"blob_12", m_blobCenterInitPos + Vector3f{0.0f, -phi, -1.0f / phi}},

        {"blob_13", m_blobCenterInitPos + Vector3f{1.0f / phi, 0.0f, phi}},
        {"blob_14", m_blobCenterInitPos + Vector3f{-1.0f / phi, 0.0f, phi}},
        {"blob_15", m_blobCenterInitPos + Vector3f{1.0f / phi, 0.0f, -phi}},
        {"blob_16", m_blobCenterInitPos + Vector3f{-1.0f / phi, 0.0f, -phi}},

        {"blob_17", m_blobCenterInitPos + Vector3f{phi, 1.0f / phi, 0.0f}},
        {"blob_18", m_blobCenterInitPos + Vector3f{-phi, 1.0f / phi, 0.0f}},
        {"blob_19", m_blobCenterInitPos + Vector3f{phi, -1.0f / phi, 0.0f}},
        {"blob_20", m_blobCenterInitPos + Vector3f{-phi, -1.0f / phi, 0.0f}}
    }};

    for(auto & [label, pos] : blobPos)
    {
        auto particle = m_gameState.getParticle(label);

        particle->setPosition(pos);
        particle->setVelocity({0.0f, 0.0f, 0.0f});
        particle->setIsResting(false);
    }
}

void Application::createGround()
{
    // Ground Shape
    auto groundShape = std::make_unique<CubeShapeGenerator>(Color::DARK_GRAY);
    groundShape->setScale({m_groundLength / 2.0f, m_groundWidth / 2.0f, m_groundThickness / 2.0f});
    groundShape->setPosition(m_groundCenterPosition);
    m_gameState.addShapeGenerator("ground", std::move(groundShape));

    // Ground contact
    auto groundContact = std::make_unique<WallContactGenerator>(m_groundDirectionWidth, m_groundDirectionLength, m_groundLength, m_groundWidth, m_groundThickness, m_groundCenterPosition, m_groundRestitution);
    m_gameState.addParticleContactGenerator("groundContact", std::move(groundContact));

    // Create walls on the ground

    // Wall 1 shape
    auto wall1 = std::make_unique<CubeShapeGenerator>(glm::vec3{0.9f, 0.9f, 0.9f});
    wall1->setPosition(m_groundCenterPosition + Vector3f{-m_wallsLength / 2.0f, 0.0f, m_wallsWidth / 2.0f});
    wall1->scale({m_wallsThickness, m_wallsLength / 2.0f, m_wallsWidth / 2.0f});
    m_gameState.addShapeGenerator("wall1", std::move(wall1));

    // Wall 1 contact
    Vector3f directionWidthWall1{0.0f, 0.0f, -1.0f}; // Z-
    Vector3f directionLengthWall1{0.0f, 1.0f, 0.0f}; // Y+
    auto wall1Contact = std::make_unique<WallContactGenerator>(directionWidthWall1, directionLengthWall1, m_wallsLength, m_wallsWidth, m_wallsThickness, m_groundCenterPosition + Vector3f{-m_wallsLength / 2.0f, 0.0f, m_wallsWidth / 2.0f}, m_wallsRestitution);
    m_gameState.addParticleContactGenerator("wall1Contact", std::move(wall1Contact));

    // Wall 2 shape
    auto wall2 = std::make_unique<CubeShapeGenerator>(glm::vec3{0.9f, 0.9f, 0.9f});
    wall2->setPosition(m_groundCenterPosition + Vector3f{0.0f, m_wallsLength / 2.0f, m_wallsWidth / 2.0f});
    wall2->scale({m_wallsLength / 2.0f, m_wallsThickness, m_wallsWidth / 2.0f});
    m_gameState.addShapeGenerator("wall2", std::move(wall2));

    // Wall 2 contact
    Vector3f directionWidthWall2{0.0f, 0.0f, -1.0f}; // Z-
    Vector3f directionLengthWall2{1.0f, 0.0f, 0.0f}; // X+
    auto wall2Contact = std::make_unique<WallContactGenerator>(directionWidthWall2, directionLengthWall2, m_wallsLength, m_wallsWidth, m_wallsThickness, m_groundCenterPosition + Vector3f{0.0f, m_wallsLength / 2.0f, m_wallsWidth / 2.0f}, m_wallsRestitution);
    m_gameState.addParticleContactGenerator("wall2Contact", std::move(wall2Contact));
}

void Application::jumpBlob()
{
    std::array<std::string, 21> blobLabels
    {
        "blob_0",
        "blob_1",
        "blob_2",
        "blob_3",
        "blob_4",
        "blob_5",
        "blob_6",
        "blob_7",
        "blob_8",
        "blob_9",
        "blob_10",
        "blob_11",
        "blob_12",
        "blob_13",
        "blob_14",
        "blob_15",
        "blob_16",
        "blob_17",
        "blob_18",
        "blob_19",
        "blob_20",
    };

    float jumpHeight = 10.0f;

    float jumpVelocity = sqrt(2.0f * 9.81f * jumpHeight);

    for(auto & blobLabel : blobLabels)
    {
        auto particle = m_gameState.getParticle(blobLabel);

        Vector3f newVelocity = particle->getVelocity();
        newVelocity.setZ(jumpVelocity);
        particle->setVelocity(newVelocity);
    }
}

void Application::changeBlobSettings()
{
    // Update initial position
    m_blobCenterInitPos = currentBlobInitialPosition;

    std::array<std::string, 21> blobLabels
    {
        "blob_0",
        "blob_1",
        "blob_2",
        "blob_3",
        "blob_4",
        "blob_5",
        "blob_6",
        "blob_7",
        "blob_8",
        "blob_9",
        "blob_10",
        "blob_11",
        "blob_12",
        "blob_13",
        "blob_14",
        "blob_15",
        "blob_16",
        "blob_17",
        "blob_18",
        "blob_19",
        "blob_20",
    };

    for(auto & blobLabel : blobLabels)
    {
        auto particle = m_gameState.getParticle(blobLabel);
        auto shape = m_gameState.getShapeGenerator<ParticleShapeGenerator>(blobLabel);

        // ----- Update radius -----
        particle->setRadius(m_blobParticleRadius);
        shape->getSphere().setScale(m_blobParticleRadius);
    }

    // ----- Update links between particles -----
    std::array<std::tuple<std::string, std::string, float>, 50> allBlobLinks
    {{
        // Between center and vertices
        {"blob_0", "blob_1", m_blobCenterToVerticesLength},
        {"blob_0", "blob_2", m_blobCenterToVerticesLength},
        {"blob_0", "blob_3", m_blobCenterToVerticesLength},
        {"blob_0", "blob_4", m_blobCenterToVerticesLength},
        {"blob_0", "blob_5", m_blobCenterToVerticesLength},
        {"blob_0", "blob_6", m_blobCenterToVerticesLength},
        {"blob_0", "blob_7", m_blobCenterToVerticesLength},
        {"blob_0", "blob_8", m_blobCenterToVerticesLength},
        {"blob_0", "blob_9", m_blobCenterToVerticesLength},
        {"blob_0", "blob_10", m_blobCenterToVerticesLength},
        {"blob_0", "blob_11", m_blobCenterToVerticesLength},
        {"blob_0", "blob_12", m_blobCenterToVerticesLength},
        {"blob_0", "blob_13", m_blobCenterToVerticesLength},
        {"blob_0", "blob_14", m_blobCenterToVerticesLength},
        {"blob_0", "blob_15", m_blobCenterToVerticesLength},
        {"blob_0", "blob_16", m_blobCenterToVerticesLength},
        {"blob_0", "blob_17", m_blobCenterToVerticesLength},
        {"blob_0", "blob_18", m_blobCenterToVerticesLength},
        {"blob_0", "blob_19", m_blobCenterToVerticesLength},
        {"blob_0", "blob_20", m_blobCenterToVerticesLength},

        // Edges
        {"blob_1", "blob_9", m_blobEdgeLength},
        {"blob_1", "blob_13", m_blobEdgeLength},
        {"blob_1", "blob_17", m_blobEdgeLength},
        {"blob_2", "blob_9", m_blobEdgeLength},
        {"blob_2", "blob_14", m_blobEdgeLength},
        {"blob_2", "blob_18", m_blobEdgeLength},
        {"blob_3", "blob_10", m_blobEdgeLength},
        {"blob_3", "blob_13", m_blobEdgeLength},
        {"blob_3", "blob_19", m_blobEdgeLength},
        {"blob_4", "blob_11", m_blobEdgeLength},
        {"blob_4", "blob_15", m_blobEdgeLength},
        {"blob_4", "blob_17", m_blobEdgeLength},
        {"blob_5", "blob_10", m_blobEdgeLength},
        {"blob_5", "blob_14", m_blobEdgeLength},
        {"blob_5", "blob_20", m_blobEdgeLength},
        {"blob_6", "blob_11", m_blobEdgeLength},
        {"blob_6", "blob_16", m_blobEdgeLength},
        {"blob_6", "blob_18", m_blobEdgeLength},
        {"blob_7", "blob_12", m_blobEdgeLength},
        {"blob_7", "blob_15", m_blobEdgeLength},
        {"blob_7", "blob_19", m_blobEdgeLength},
        {"blob_8", "blob_12", m_blobEdgeLength},
        {"blob_8", "blob_16", m_blobEdgeLength},
        {"blob_8", "blob_20", m_blobEdgeLength},
        {"blob_9", "blob_11", m_blobEdgeLength},
        {"blob_10", "blob_12", m_blobEdgeLength},
        {"blob_13", "blob_14", m_blobEdgeLength},
        {"blob_15", "blob_16", m_blobEdgeLength},
        {"blob_17", "blob_19", m_blobEdgeLength},
        {"blob_18", "blob_20", m_blobEdgeLength},
    }};

    for(auto & [labelBlobA, labelBlobB, length] : allBlobLinks)
    {
        // ----- Springs -----
        ParticleSpring* springBA = static_cast<ParticleSpring*>(m_gameState.getParticleForceGenerator("blobSpring_" + labelBlobB + "_" + labelBlobA));
        ParticleSpring* springAB = static_cast<ParticleSpring*>(m_gameState.getParticleForceGenerator("blobSpring_" + labelBlobA + "_" + labelBlobB));

        // Stifness
        springBA->setK(m_blobK);
        springAB->setK(m_blobK);

        // Rest length
        springBA->setRestLength(length);
        springAB->setRestLength(length);

        // ----- Cables -----
        ParticleCable* cable = static_cast<ParticleCable*>(m_gameState.getParticleContactGenerator("blobCable_" + labelBlobA + "_" + labelBlobB));

        // Restitution
        cable->setRestitution(m_blobCableRestitution);

        // Max length
        cable->setMaxLength(length);
    }
}

void Application::changeGroundSettings()
{
    // Ground shape
    m_groundCenterPosition = currentGroundCenterPosition;
    auto groundShape = m_gameState.getShapeGenerator<CubeShapeGenerator>("ground");
    groundShape->setPosition(m_groundCenterPosition);
    groundShape->setScale({m_groundLength / 2.0f, m_groundWidth / 2.0f, m_groundThickness / 2.0f});

    // Ground contact
    auto groundContact = m_gameState.getParticleContactGenerator<WallContactGenerator>("groundContact");
    groundContact->setLength(m_groundLength);
    groundContact->setWidth(m_groundWidth);
    groundContact->setThickness(m_groundThickness);
    groundContact->setRestitution(m_groundRestitution);
    groundContact->setCenterPosition(m_groundCenterPosition);


    // Walls

    // Wall 1 shape
    auto wall1Shape = m_gameState.getShapeGenerator<CubeShapeGenerator>("wall1");
    wall1Shape->setPosition(m_groundCenterPosition + Vector3f{-m_wallsLength / 2.0f, 0.0f, m_wallsWidth / 2.0f});

    // Wall 1 contact
    auto wall1Contact = m_gameState.getParticleContactGenerator<WallContactGenerator>("wall1Contact");
    wall1Contact->setCenterPosition(m_groundCenterPosition + Vector3f{-m_wallsLength / 2.0f, 0.0f, m_wallsWidth / 2.0f});

    // Wall 2 shape
    auto wall2Shape = m_gameState.getShapeGenerator<CubeShapeGenerator>("wall2");
    wall2Shape->setPosition(m_groundCenterPosition + Vector3f{0.0f, m_wallsLength / 2.0f, m_wallsWidth / 2.0f});

    // Wall 2 contact
    auto wall2Contact = m_gameState.getParticleContactGenerator<WallContactGenerator>("wall2Contact");
    wall2Contact->setCenterPosition(m_groundCenterPosition + Vector3f{0.0f, m_wallsLength / 2.0f, m_wallsWidth / 2.0f});
}