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

    cam.setPosition({0.0f, 0.0f, 0.0f});
    // rotate cam to look at X- (this way Y+ and Z+ looks more natural for a 2D environment, aka. Y+ is on the right)
    cam.rotateYaw(PI);

    sun.setPosition({0.0f, 0.0f, 0.0f});

    // Creation of the background
    auto cube = std::make_unique<CubeShapeGenerator>(glm::vec3{0.9f, 0.9f, 0.9f});
    cube->setPosition({-10.0f, 0.0f, 0.0f});
    cube->scale({0.1f, 15.0f, 10.0f});
    m_gameState.addShapeGenerator("background", std::move(cube));

    // Create force generators
    std::unique_ptr<ParticleForceGenerator> particleGravity = std::make_unique<ParticleGravity>(9.81f);
    m_gameState.addParticleForceGenerator("gravity", std::move(particleGravity));

    std::unique_ptr<ParticleForceGenerator> particleDrag = std::make_unique<ParticleDrag>(0.0f, 0.1f);
    m_gameState.addParticleForceGenerator("drag", std::move(particleDrag));

    createBlob();
    createGround();
    createTestRestingContact();
}

void Application::update(float deltaTime)
{
    static bool canPressPause = true;
    static bool pause = false;
    static constexpr float TIMESTEP = 1.0f / 200.0f;
    static float elapsedTime = 0.0f;
    elapsedTime += deltaTime;

    // If we want to reset the world and the pause isn't active
    if (m_resetMarks && !pause)
    {
        for(std::size_t i = 0; i < m_markN; ++i)
        {
            m_gameState.removeShapeGenerator("mark_" + std::to_string(i));
        }
        m_markN = 0;

        m_resetMarks = false;
    }

    // If we click on the reset button
    if(m_graphicsEngine.getWindow().isKeyPressed(GLFW_KEY_R))
    {
        resetBlob();
        resetTestRestingContact();
    }

    // Map avec l'aide de laquelle nous appellons la fonction moveBlob()
    static std::unordered_map<std::string, std::function<void()>> blobMouvementMap =
    {
        { "Up", [&]() { moveBlob(Vector3f {0,0,0.2}); } },
        { "Down", [&]() { moveBlob(Vector3f {0,0,-0.2}); } },
        { "Left", [&]() { moveBlob(Vector3f {0,-0.2,0}); } },
        { "Right", [&]() { moveBlob(Vector3f {0,0.2,0}); } },
    };

    // Mouvements du blob
    if (m_graphicsEngine.getWindow().isKeyPressed(GLFW_KEY_KP_4)) 
    {
        blobMouvementMap.at("Left")();
    }
    if (m_graphicsEngine.getWindow().isKeyPressed(GLFW_KEY_KP_5))
    {
        blobMouvementMap.at("Down")();
    }
    if (m_graphicsEngine.getWindow().isKeyPressed(GLFW_KEY_KP_6))
    {
        blobMouvementMap.at("Right")();
    }
    if (m_graphicsEngine.getWindow().isKeyPressed(GLFW_KEY_KP_8))
    {
        blobMouvementMap.at("Up")();
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
        { GLFW_KEY_O    , [&](){ cam.move( moveSpeed * glm::vec3{0.0f, 0.0f, 1.0f}); } },
        { GLFW_KEY_L    , [&](){ cam.move(-moveSpeed * glm::vec3{0.0f, 0.0f, 1.0f}); } },
    };

    for(const auto & [key, fun] : keymap)
    {
        if(win.isKeyPressed(key)) fun();
    }
}

void Application::createFrame()
{
    ImGui::Begin("Settings");

    // Default size
    float imGuiWidth = 330.0f;
    float imGuiHeight = 400.0f;
    ImGui::SetWindowSize(ImVec2(imGuiWidth, imGuiHeight));

    if (ImGui::BeginTabBar("SettingsTabBar", ImGuiTabBarFlags_None))
    {
        // Tab to select the projectile, see its properties and set initial position, initial acceleration and damping
        if (ImGui::BeginTabItem("Projectile"))
        {
            ImGui::Text("Choose the projectile :");

            // Add a combo box to select the projectile
            if (ImGui::BeginCombo("##combo", m_currentProjectile))
            {
                for (int n = 0; n < IM_ARRAYSIZE(m_projectiles); n++)
                {
                    bool isSelected = (m_currentProjectile == m_projectiles[n]);
                    if (ImGui::Selectable(m_projectiles[n], isSelected))
                    {
                        m_currentProjectile = m_projectiles[n];
                        m_currentIndex = n;
                        currentInitialVelocity = m_projectilesInitialVelocity[m_currentIndex];
                        currentMass = m_projectilesMass[m_currentIndex];
                    }

                    if (isSelected)
                    {
                        ImGui::SetItemDefaultFocus();
                    }

                }
                ImGui::EndCombo();
            }

            ImGui::NewLine();

            ImGui::Text("Projectile's properties : ");

            // Add a table to display projectile's properties (initial velocity and mass)
            if (ImGui::BeginTable("informationTable", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg))
            {
                std::stringstream ss;
                for (int row = 0; row < 2; row++)
                {
                    ImGui::TableNextRow();
                    for (int column = 0; column < 2; column++)
                    {
                        ImGui::TableSetColumnIndex(column);

                        if (column == 0)
                        {
                            // First column : display properties' name
                            if (row == 0)
                            {
                                // First row : initial velocity
                                ImGui::Text("Initial velocity");
                            }
                            else
                            {
                                // Second row : mass
                                ImGui::Text("Mass");
                            }
                        }
                        else
                        {
                            // Second column : display properties' value
                            if (row == 0)
                            {
                                // First row : initial velocity
                                std::stringstream().swap(ss); // clear ss
                                ss << "{";
                                ss << std::fixed << std::setprecision(2) << m_projectilesInitialVelocity[m_currentIndex][0];
                                ss << " ; ";
                                ss << std::fixed << std::setprecision(2) << m_projectilesInitialVelocity[m_currentIndex][1];
                                ss << " ; ";
                                ss << std::fixed << std::setprecision(2) << m_projectilesInitialVelocity[m_currentIndex][2];
                                ss << "}";
                                ImGui::Text(ss.str().c_str());
                            }
                            else
                            {
                                // Second row : mass
                                std::stringstream().swap(ss); // clear ss
                                ss << std::fixed << std::setprecision(2) << m_projectilesMass[m_currentIndex];
                                ImGui::Text(ss.str().c_str());
                            }
                        }
                    }
                }
                ImGui::EndTable();
            }

            ImGui::NewLine();

            ImGui::Text("Adjustable settings :");

            ImGui::Indent(10.0f);

            // Input for the initial position
            ImGui::Text("Initial position (X ; Y ; Z) :");
            ImGui::InputFloat3("##Position", currentInitialPosition.data(), "%.1f");

            // Input for the initial velocity
            ImGui::Text("Initial velocity (X ; Y ; Z) :");
            ImGui::InputFloat3("##Velocity", currentInitialVelocity.data(), "%.1f");

            // Input for the mass
            ImGui::Text("Mass : ");
            ImGui::InputFloat("##Mass", &currentMass, 0.5f, 5.0f, "%.1f");

            // Input for the damping
            ImGui::Text("Damping : ");
            ImGui::InputFloat("##Damping", &damping, 0.001f, 0.01f, "%.3f");

            ImGui::Unindent(10.0f);

            // Button to confirm selection
            ImGui::NewLine();
            ImGui::SetCursorPosX((ImGui::GetWindowSize().x) * 0.4f);
            if (ImGui::Button("Select"))
            {
                Particle * particle = m_gameState.getParticle("particle");
                particle->setPosition(currentInitialPosition);
                particle->setVelocity(currentInitialVelocity);
                particle->setDamping (damping);
                particle->setMass    (currentMass);

                m_resetMarks = true;

                resetBlob();
            }

            ImGui::EndTabItem();
        }

        // Tab to edit and add forces
        if (ImGui::BeginTabItem("Forces"))
        {
            // Edit anchored spring force
            ImGui::Text("Anchored spring settings : ");

            ImGui::Indent(10.0f);

            // Input for the anchor position
            ImGui::Text("Anchor position (X ; Y ; Z) :");
            ImGui::InputFloat3("##AnchorPosition", currentAnchorPosition.data(), "%.1f");

            // Input for spring stiffness (k)
            ImGui::Text("Spring stiffness :");
            ImGui::InputFloat("##SpringStiffness", &currentSpringStiffness, 0.5f, 5.0f, "%.1f");

            // Input for spring rest length
            ImGui::Text("Spring rest length :");
            ImGui::InputFloat("##SpringRestLength", &currentSpringRestLength, 0.2f, 2.0f, "%.1f");

            ImGui::Unindent(10.0f);

            ImGui::Separator();

            // Edit drag force
            ImGui::Text("Drag settings : ");

            ImGui::Indent(10.0f);

            // Input for the first drag coefficient (k1)
            ImGui::Text("First drag coefficient (k1) :");
            ImGui::InputFloat("##k1DragCoef", &currentK1DragCoef, 0.01f, 0.5f, "%.2f");

            // Input for the second drag coefficient (k2)
            ImGui::Text("Second drag coefficient (k2) :");
            ImGui::InputFloat("##k2DragCoef", &currentK2DragCoef, 0.01f, 0.5f, "%.2f");

            ImGui::Unindent(10.0f);

            // Button to apply forces
            ImGui::NewLine();
            ImGui::SetCursorPosX((ImGui::GetWindowSize().x) * 0.4f);
            if (ImGui::Button("Apply"))
            {
                auto anchoredSpring = m_gameState.getParticleForceGenerator<ParticleAnchoredSpring>("anchoredSpring");
                anchoredSpring->setAnchor(currentAnchorPosition);
                anchoredSpring->setK(currentSpringStiffness);
                anchoredSpring->setRestLength(currentSpringRestLength);

                auto drag = m_gameState.getParticleForceGenerator<ParticleDrag>("drag");
                drag->setK1(currentK1DragCoef);
                drag->setK2(currentK2DragCoef);
            }

            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }

    ImGui::End();
}

void Application::createBlob()
{
    std::array<Vector3f, 7> particlePos
    {
        m_positionInit,
        m_positionInit + Vector3f{0.0f, 0.0f, 1.0f} * cos(PI / 3.0f)     + Vector3f{0.0f, 1.0f, 0.0f} * sin(PI / 3.0f),
        m_positionInit + Vector3f{0.0f, 0.0f, 1.0f} * cos(2 * PI / 3.0f) + Vector3f{0.0f, 1.0f, 0.0f} * sin(2 * PI / 3.0f),
        m_positionInit + Vector3f{0.0f, 0.0f, 1.0f} * cos(3 * PI / 3.0f) + Vector3f{0.0f, 1.0f, 0.0f} * sin(3 * PI / 3.0f),
        m_positionInit + Vector3f{0.0f, 0.0f, 1.0f} * cos(4 * PI / 3.0f) + Vector3f{0.0f, 1.0f, 0.0f} * sin(4 * PI / 3.0f),
        m_positionInit + Vector3f{0.0f, 0.0f, 1.0f} * cos(5 * PI / 3.0f) + Vector3f{0.0f, 1.0f, 0.0f} * sin(5 * PI / 3.0f),
        m_positionInit + Vector3f{0.0f, 0.0f, 1.0f} * cos(6 * PI / 3.0f) + Vector3f{0.0f, 1.0f, 0.0f} * sin(6 * PI / 3.0f),
    };

    for(std::size_t i = 0; i < particlePos.size(); ++i)
    {
        // Particle
        auto particleBlob = std::make_unique<Particle>(particlePos[i], 1.0f, 0.999f);
        particleBlob->setVelocity({0.0f, 0.0f, 0.0f});

        // Particle Shape
        auto particuleShapeBlob = std::make_unique<ParticleShapeGenerator>(particleBlob.get(), Color::DARK_GRAY);
        
        m_gameState.addParticle("blob_" + std::to_string(i), std::move(particleBlob));
        m_gameState.addShapeGenerator("blob_" + std::to_string(i), std::move(particuleShapeBlob));

        // Gravity
        m_physicsEngine.registerForce(m_gameState.getParticle("blob_" + std::to_string(i)), m_gameState.getParticleForceGenerator<ParticleGravity>("gravity"), 0.0f);

        // Drag
        m_physicsEngine.registerForce(m_gameState.getParticle("blob_" + std::to_string(i)), m_gameState.getParticleForceGenerator<ParticleDrag>("drag"), 0.0f);
    }

    // Springs between particles
    float k = 60.0f;
    float restLength = 1.0f;

    // Cables between particles
    float cableMaxLength = 1.0f;
    float cableRestitution = 0.3f;

    //We define which pair of particles will be cables
    std::array<std::pair<std::string, std::string>, 6> particleCables
    {{
        {"blob_0", "blob_1"},
        {"blob_0", "blob_2"},
        {"blob_0", "blob_3"},
        {"blob_0", "blob_4"},
        {"blob_0", "blob_5"},
        {"blob_0", "blob_6"}
    }};

    // We add the cable contacts between each particle
    for(auto & [labelBlobA, labelBlobB] : particleCables)
    {
        auto particleA = m_gameState.getParticle(labelBlobA);
        auto particleB = m_gameState.getParticle(labelBlobB);

        // Cable
        auto cable = std::make_unique<ParticleCable>(particleA, particleB, cableMaxLength, cableRestitution);
        m_gameState.addParticleContactGenerator("blob_cable_" + labelBlobA + "_" + labelBlobB, std::move(cable));
    }

    //We define which pair of particles will be springs
    std::array<std::pair<std::string, std::string>, 6> particleSprings
    {{
        {"blob_1", "blob_2"},
        {"blob_2", "blob_3"},
        {"blob_3", "blob_4"},
        {"blob_4", "blob_5"},
        {"blob_5", "blob_6"},
        {"blob_6", "blob_1"}
    }};

    // We add the spring forces between each particle
    for(auto & [labelBlobA, labelBlobB] : particleSprings)
    {
        auto particleA = m_gameState.getParticle(labelBlobA);
        auto particleB = m_gameState.getParticle(labelBlobB);

        // Spring of particle B on particle A
        auto blobSpring = std::make_unique<ParticleSpring>(particleB, k, restLength);
        m_physicsEngine.registerForce(particleA, blobSpring.get(), 0.0);
        m_gameState.addParticleForceGenerator("blobspring_" + labelBlobB + "_" + labelBlobA, std::move(blobSpring));

        // Spring of particle A on particle B
        blobSpring = std::make_unique<ParticleSpring>(particleA, k, restLength);
        m_physicsEngine.registerForce(particleB, blobSpring.get(), 0.0);
        m_gameState.addParticleForceGenerator("blobspring_" + labelBlobA + "_" + labelBlobB, std::move(blobSpring));
    }
}

void Application::moveBlob(Vector3f moveVector)
{
    // Only move the center of the blob
    Particle* particle = m_gameState.getParticle("blob_0");
    particle->setVelocity(particle->getVelocity() + moveVector*10.0f);
    particle->setIsResting(false);
}

void Application::resetBlob()
{
    std::array<std::pair<std::string, Vector3f>, 7> blobPos
    {{
        {"blob_0", m_positionInit},
        {"blob_1", m_positionInit + Vector3f{0.0f, 0.0f, 1.0f} * cos(PI / 3.0f)     + Vector3f{0.0f, 1.0f, 0.0f} * sin(PI / 3.0f)    },
        {"blob_2", m_positionInit + Vector3f{0.0f, 0.0f, 1.0f} * cos(2 * PI / 3.0f) + Vector3f{0.0f, 1.0f, 0.0f} * sin(2 * PI / 3.0f)},
        {"blob_3", m_positionInit + Vector3f{0.0f, 0.0f, 1.0f} * cos(3 * PI / 3.0f) + Vector3f{0.0f, 1.0f, 0.0f} * sin(3 * PI / 3.0f)},
        {"blob_4", m_positionInit + Vector3f{0.0f, 0.0f, 1.0f} * cos(4 * PI / 3.0f) + Vector3f{0.0f, 1.0f, 0.0f} * sin(4 * PI / 3.0f)},
        {"blob_5", m_positionInit + Vector3f{0.0f, 0.0f, 1.0f} * cos(5 * PI / 3.0f) + Vector3f{0.0f, 1.0f, 0.0f} * sin(5 * PI / 3.0f)},
        {"blob_6", m_positionInit + Vector3f{0.0f, 0.0f, 1.0f} * cos(6 * PI / 3.0f) + Vector3f{0.0f, 1.0f, 0.0f} * sin(6 * PI / 3.0f)},
    }};

    for(auto & [label, pos] : blobPos)
    {
        auto particle = m_gameState.getParticle(label);

        particle->setPosition(pos);
        particle->setVelocity({0.f, 0.f, 0.f});
        particle->setIsResting(false);
    }
}

void Application::createGround()
{
    // Ground particle (center)
    auto ground = std::make_unique<Particle>(Vector3f{0.0f, 0.0f, -6.0f}, 1.0f, 0.999f);
    ground->setInverseMass(0.0f);
    ground->setVelocity({0.0f, 0.0f, 0.0f});

    m_gameState.addParticle("ground", std::move(ground));

    Vector3f directionWidth{1.0f, 0.0f, 0.0f}; // X+ axis
    Vector3f directionLength{0.0f, 1.0f, 0.0f}; // Y+ axis
    float length = 20.0f;
    float width = 20.0f;
    float thickness = 0.5f;
    float restitution = 0.3f;

    // Ground Shape
    auto groundShape = std::make_unique<CubeShapeGenerator>(Color::DARK_GRAY);
    groundShape->setScale({length / 2.0f, width / 2.0f, thickness / 2.0f});
    groundShape->setPosition(m_gameState.getParticle("ground")->getPosition());
    m_gameState.addShapeGenerator("ground", std::move(groundShape));

    // Wall contact between blob's particles and the ground
    std::array<std::string, 7> blobLabels
    {
        "blob_0",
        "blob_1",
        "blob_2",
        "blob_3",
        "blob_4",
        "blob_5",
        "blob_6",
    };

    for (auto& blobLabel : blobLabels)
    {
        auto wallContact = std::make_unique<WallContactGenerator>(directionWidth, directionLength, length, width, thickness, m_gameState.getParticle("ground"), m_gameState.getParticle(blobLabel), restitution);
        m_gameState.addParticleContactGenerator("wallContact_ground_" + blobLabel, std::move(wallContact));
    }
}

void Application::createTestRestingContact()
{
    // Particle
    auto particle = std::make_unique<Particle>(m_positionInit + Vector3f{0.0f, 5.0f, 0.0f}, 1.0f, 0.999f);
    particle->setVelocity({0.0f, 0.0f, 0.0f});

    // Particle Shape
    auto particuleShape = std::make_unique<ParticleShapeGenerator>(particle.get(), Color::DARK_GRAY);

    m_gameState.addParticle("particleRest", std::move(particle));
    m_gameState.addShapeGenerator("particleRest", std::move(particuleShape));

    // Gravity
    m_physicsEngine.registerForce(m_gameState.getParticle("particleRest"), m_gameState.getParticleForceGenerator<ParticleGravity>("gravity"), 0.0f);

    // Ground contact
    Vector3f directionWidth{1.0f, 0.0f, 0.0f}; // X+ axis
    Vector3f directionLength{0.0f, 1.0f, 0.0f}; // Y+ axis
    float length = 20.0f;
    float width = 20.0f;
    float thickness = 0.5f;
    float restitution = 0.3f;

    auto wallContact = std::make_unique<WallContactGenerator>(directionWidth, directionLength, length, width, thickness, m_gameState.getParticle("ground"), m_gameState.getParticle("particleRest"), restitution);
    m_gameState.addParticleContactGenerator("wallContact_ground_particleRest", std::move(wallContact));
}

void Application::resetTestRestingContact()
{
    auto particle = m_gameState.getParticle("particleRest");

    particle->setPosition(m_positionInit + Vector3f{0.0f, 5.0f, 0.0f});
    particle->setVelocity({0.0f, 0.0f, 0.0f});
    particle->setIsResting(false);
}