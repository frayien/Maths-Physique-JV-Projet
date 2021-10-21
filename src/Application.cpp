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

    // We mark the center
    cube = std::make_unique<CubeShapeGenerator>(glm::vec3{0.0f, 0.0f, 0.0f});
    cube->setPosition({-9.0f, 0.0f, 0.0f});
    cube->scale(0.05f);
    m_gameState.addShapeGenerator("center", std::move(cube));

    // Y+ axis
    cube = std::make_unique<CubeShapeGenerator>(glm::vec3{1.0f, 0.0f, 0.0f});
    cube->setPosition({-9.0f, 1.0f, 0.0f});
    cube->scale(0.05f);
    m_gameState.addShapeGenerator("axisY+", std::move(cube));

    // Z+ axis
    cube = std::make_unique<CubeShapeGenerator>(glm::vec3{0.0f, 1.0f, 0.0f});
    cube->setPosition({-9.0f, 0.0f, 1.0f});
    cube->scale(0.05f);
    m_gameState.addShapeGenerator("axisZ+", std::move(cube));

    // Initialisation of the graphic rendering of the particle
    auto sphere = std::make_unique<SphereShapeGenerator>(glm::vec3{ 0.2f, 0.2f, 0.2f });
    sphere->scale(0.2f);
    m_gameState.addShapeGenerator("particle", std::move(sphere));

    sphere = std::make_unique<SphereShapeGenerator>(glm::vec3{ 0.05f, 0.05f, 0.05f });
    sphere->scale(0.2f);
    m_gameState.addShapeGenerator("anchor", std::move(sphere));

    cube = std::make_unique<CubeShapeGenerator>(glm::vec3{ 0.1f, 0.5f, 0.5f });
    cube->scale(0.2f);
    m_gameState.addShapeGenerator("spring", std::move(cube));

    // Create particle
    std::unique_ptr<Particle> particle = std::make_unique<Particle>(m_positionInit, 1.0f, 0.999f);
    particle->setVelocity(m_velocityInit);
    m_gameState.addParticle("particle", std::move(particle));

    // Create force generators
    std::unique_ptr<ParticleForceGenerator> particleGravity = std::make_unique<ParticleGravity>(9.81f);
    m_gameState.addParticleForceGenerator("gravity", std::move(particleGravity));

    std::unique_ptr<ParticleForceGenerator> particleDrag = std::make_unique<ParticleDrag>(0.0f, 0.1f);
    m_gameState.addParticleForceGenerator("drag", std::move(particleDrag));

    std::unique_ptr<ParticleForceGenerator> particleAnchoredSpring = std::make_unique<ParticleAnchoredSpring>(Vector3f{-8.0f, 0.0f, 0.0f}, 20.0f, 2.0f);
    m_gameState.addParticleForceGenerator("anchoredSpring", std::move(particleAnchoredSpring));

    // Register all forces with the particle
    m_physicsEngine.registerForce(m_gameState.getParticle("particle"), m_gameState.getParticleForceGenerator<ParticleGravity>("gravity"), 0.0f);
    m_physicsEngine.registerForce(m_gameState.getParticle("particle"), m_gameState.getParticleForceGenerator<ParticleDrag>("drag"), 0.0f);
    m_physicsEngine.registerForce(m_gameState.getParticle("particle"), m_gameState.getParticleForceGenerator<ParticleAnchoredSpring>("anchoredSpring"), 0.0f);
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

    // If the elapsed time is greater than TIMESTEP, we enter the loop
    while (elapsedTime >= TIMESTEP)
    {
        elapsedTime -= TIMESTEP;
        
        if (!pause)
        {
            // Creation of multiple little spheres to symbolize the path taken by the particle
            m_countTimeStepMarks++;

            if (m_countTimeStepMarks >= m_countTimeStepMarksMax)
            {
                m_countTimeStepMarks = 0;
                auto tmpSphere = std::make_unique<SphereShapeGenerator>(glm::vec3{1.0f, 0.0f, 0.0f});
                tmpSphere->setPosition(m_gameState.getParticle("particle")->getPosition());
                tmpSphere->scale(0.03f);

                m_gameState.addShapeGenerator("mark_" + std::to_string(m_markN), std::move(tmpSphere));
                m_markN++;
            }

            // Update physics engine
            m_physicsEngine.update(TIMESTEP, m_gameState);
        }
    }

    // update graphics
    Vector3f particulePos = m_gameState.getParticle("particle")->getPosition();
    ParticleAnchoredSpring * anchoredSpring = m_gameState.getParticleForceGenerator<ParticleAnchoredSpring>("anchoredSpring");
    Vector3f anchorPos = anchoredSpring->getAnchor();
    Vector3f springPos = (particulePos + anchorPos)/2.0f;

    const Vector3f springDefaultDir {0.f, 0.f, 1.f};

    Vector3f springDir = (particulePos - anchorPos).normalize();
    float springAngle = -glm::acos(springDir.dotProduct(springDefaultDir));
    float springLen = (particulePos - anchorPos).norm();

    m_gameState.getShapeGenerator<SphereShapeGenerator>("particle")->setPosition(particulePos);
    m_gameState.getShapeGenerator<SphereShapeGenerator>("anchor")->setPosition(anchorPos);

    auto spring = m_gameState.getShapeGenerator<CubeShapeGenerator>("spring");
    spring->setPosition(springPos);
    spring->resetRotation();
    spring->rotate(springAngle, springDir.crossProduct(springDefaultDir));
    spring->setScale({0.05f, 0.05f, springLen / 2.f});

    //updateBlob();

    // If we click on the reset button
    if(m_graphicsEngine.getWindow().isKeyPressed(GLFW_KEY_R))
    {
        m_gameState.getParticle("particle")->setPosition(m_positionInit);
        m_gameState.getParticle("particle")->setVelocity(m_velocityInit);
        m_resetMarks = true;
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
    /*
    Particle * particleBlob1 = new Particle(m_positionInit + Vector3f{0.0f, 0.0f, 1.0f} * cos(PI / 3.0f) + Vector3f(0.0f, 1.0f, 0.0f) * sin(PI / 3.0f), 1.0f, 0.999f);
    particleBlob1->setVelocity({0.0f, 0.0f, 0.0f});
    m_particles.push_back(particleBlob1);
    auto particuleShapeBlob1 = world.makeSphere({ 0.2f, 0.2f, 0.2f });
    particuleShapeBlob1->scale(0.2f);
    m_particleShapes.push_back(particuleShapeBlob1);

    Particle * particleBlob2 = new Particle(m_positionInit + Vector3f{0.0f, 0.0f, 1.0f} * cos(2 * PI / 3.0f) + Vector3f(0.0f, 1.0f, 0.0f) * sin(2 * PI / 3.0f), 1.0f, 0.999f);
    particleBlob2->setVelocity({0.0f, 0.0f, 0.0f});
    m_particles.push_back(particleBlob2);
    auto particuleShapeBlob2 = world.makeSphere({ 0.2f, 0.2f, 0.2f });
    particuleShapeBlob2->scale(0.2f);
    m_particleShapes.push_back(particuleShapeBlob2);

    Particle * particleBlob3 = new Particle(m_positionInit + Vector3f{0.0f, 0.0f, 1.0f} * cos(3 * PI / 3.0f) + Vector3f(0.0f, 1.0f, 0.0f) * sin(3 * PI / 3.0f), 1.0f, 0.999f);
    particleBlob3->setVelocity({0.0f, 0.0f, 0.0f});
    m_particles.push_back(particleBlob3);
    auto particuleShapeBlob3 = world.makeSphere({ 0.2f, 0.2f, 0.2f });
    particuleShapeBlob3->scale(0.2f);
    m_particleShapes.push_back(particuleShapeBlob3);

    Particle * particleBlob4 = new Particle(m_positionInit + Vector3f{0.0f, 0.0f, 1.0f} * cos(4 * PI / 3.0f) + Vector3f(0.0f, 1.0f, 0.0f) * sin(4 * PI / 3.0f), 1.0f, 0.999f);
    particleBlob4->setVelocity({0.0f, 0.0f, 0.0f});
    m_particles.push_back(particleBlob4);
    auto particuleShapeBlob4 = world.makeSphere({ 0.2f, 0.2f, 0.2f });
    particuleShapeBlob4->scale(0.2f);
    m_particleShapes.push_back(particuleShapeBlob4);

    Particle * particleBlob5 = new Particle(m_positionInit + Vector3f{0.0f, 0.0f, 1.0f} * cos(5 * PI / 3.0f) + Vector3f(0.0f, 1.0f, 0.0f) * sin(5 * PI / 3.0f), 1.0f, 0.999f);
    particleBlob5->setVelocity({0.0f, 0.0f, 0.0f});
    m_particles.push_back(particleBlob5);
    auto particuleShapeBlob5 = world.makeSphere({ 0.2f, 0.2f, 0.2f });
    particuleShapeBlob5->scale(0.2f);
    m_particleShapes.push_back(particuleShapeBlob5);

    Particle * particleBlob6 = new Particle(m_positionInit + Vector3f{0.0f, 0.0f, 1.0f} * cos(6 * PI / 3.0f) + Vector3f(0.0f, 1.0f, 0.0f) * sin(6 * PI / 3.0f), 1.0f, 0.999f);
    particleBlob6->setVelocity({0.0f, 0.0f, 0.0f});
    m_particles.push_back(particleBlob6);
    auto particuleShapeBlob6 = world.makeSphere({ 0.2f, 0.2f, 0.2f });
    particuleShapeBlob6->scale(0.2f);
    m_particleShapes.push_back(particuleShapeBlob6);

    // Gravity
    m_physicsEngine.getParticleRegistry().addForce(particleBlob1, &m_particleGravity, 0.0);
    m_physicsEngine.getParticleRegistry().addForce(particleBlob2, &m_particleGravity, 0.0);
    m_physicsEngine.getParticleRegistry().addForce(particleBlob3, &m_particleGravity, 0.0);
    m_physicsEngine.getParticleRegistry().addForce(particleBlob4, &m_particleGravity, 0.0);
    m_physicsEngine.getParticleRegistry().addForce(particleBlob5, &m_particleGravity, 0.0);
    m_physicsEngine.getParticleRegistry().addForce(particleBlob6, &m_particleGravity, 0.0);

    // Anchored spring
    m_physicsEngine.getParticleRegistry().addForce(particleBlob1, &m_particleAnchoredSpring, 0.0);

    // Springs between particles
    float k = 60.0f;
    float restLength = 1.0f;

    // Spring of particle 2 on particle 1
    ParticleSpring * blobSpring = new ParticleSpring(particleBlob2, k, restLength);
    m_blobSprings.push_back(blobSpring);
    m_physicsEngine.getParticleRegistry().addForce(particleBlob1, blobSpring, 0.0);

    // Spring of particle 1 on particle 2
    blobSpring = new ParticleSpring(particleBlob1, k, restLength);
    m_blobSprings.push_back(blobSpring);
    m_physicsEngine.getParticleRegistry().addForce(particleBlob2, blobSpring, 0.0);

    // Spring of particle 3 on particle 2
    blobSpring = new ParticleSpring(particleBlob3, k, restLength);
    m_blobSprings.push_back(blobSpring);
    m_physicsEngine.getParticleRegistry().addForce(particleBlob2, blobSpring, 0.0);

    // Spring of particle 2 on particle 3
    blobSpring = new ParticleSpring(particleBlob2, k, restLength);
    m_blobSprings.push_back(blobSpring);
    m_physicsEngine.getParticleRegistry().addForce(particleBlob3, blobSpring, 0.0);

    // Spring of particle 4 on particle 3
    blobSpring = new ParticleSpring(particleBlob4, k, restLength);
    m_blobSprings.push_back(blobSpring);
    m_physicsEngine.getParticleRegistry().addForce(particleBlob3, blobSpring, 0.0);

    // Spring of particle 3 on particle 4
    blobSpring = new ParticleSpring(particleBlob3, k, restLength);
    m_blobSprings.push_back(blobSpring);
    m_physicsEngine.getParticleRegistry().addForce(particleBlob4, blobSpring, 0.0);

    // Spring of particle 5 on particle 4
    blobSpring = new ParticleSpring(particleBlob5, k, restLength);
    m_blobSprings.push_back(blobSpring);
    m_physicsEngine.getParticleRegistry().addForce(particleBlob4, blobSpring, 0.0);

    // Spring of particle 4 on particle 5
    blobSpring = new ParticleSpring(particleBlob4, k, restLength);
    m_blobSprings.push_back(blobSpring);
    m_physicsEngine.getParticleRegistry().addForce(particleBlob5, blobSpring, 0.0);

    // Spring of particle 6 on particle 5
    blobSpring = new ParticleSpring(particleBlob6, k, restLength);
    m_blobSprings.push_back(blobSpring);
    m_physicsEngine.getParticleRegistry().addForce(particleBlob5, blobSpring, 0.0);

    // Spring of particle 5 on particle 6
    blobSpring = new ParticleSpring(particleBlob5, k, restLength);
    m_blobSprings.push_back(blobSpring);
    m_physicsEngine.getParticleRegistry().addForce(particleBlob6, blobSpring, 0.0);

    // Spring of particle 6 on particle 1
    blobSpring = new ParticleSpring(particleBlob6, k, restLength);
    m_blobSprings.push_back(blobSpring);
    m_physicsEngine.getParticleRegistry().addForce(particleBlob1, blobSpring, 0.0);

    // Spring of particle 1 on particle 6
    blobSpring = new ParticleSpring(particleBlob1, k, restLength);
    m_blobSprings.push_back(blobSpring);
    m_physicsEngine.getParticleRegistry().addForce(particleBlob6, blobSpring, 0.0);
    */
}

void Application::updateBlob()
{
    /*
    m_particleShapes[3]->setPosition(m_particles[1]->getPosition());
    m_particleShapes[4]->setPosition(m_particles[2]->getPosition());
    m_particleShapes[5]->setPosition(m_particles[3]->getPosition());
    m_particleShapes[6]->setPosition(m_particles[4]->getPosition());
    m_particleShapes[7]->setPosition(m_particles[5]->getPosition());
    m_particleShapes[8]->setPosition(m_particles[6]->getPosition());
    */
}

void Application::resetBlob()
{
    /*
    m_particles[1]->setPosition(m_positionInit + Vector3f{0.0f, 0.0f, 1.0f} * cos(PI / 3.0f) + Vector3f(0.0f, 1.0f, 0.0f) * sin(PI / 3.0f));
    m_particles[1]->setVelocity({0.0f, 0.0f, 0.0f});

    m_particles[2]->setPosition(m_positionInit + Vector3f{0.0f, 0.0f, 1.0f} * cos(2 * PI / 3.0f) + Vector3f(0.0f, 1.0f, 0.0f) * sin(2 * PI / 3.0f));
    m_particles[2]->setVelocity({0.0f, 0.0f, 0.0f});

    m_particles[3]->setPosition(m_positionInit + Vector3f{0.0f, 0.0f, 1.0f} * cos(3 * PI / 3.0f) + Vector3f(0.0f, 1.0f, 0.0f) * sin(3 * PI / 3.0f));
    m_particles[3]->setVelocity({0.0f, 0.0f, 0.0f});

    m_particles[4]->setPosition(m_positionInit + Vector3f{0.0f, 0.0f, 1.0f} * cos(4 * PI / 3.0f) + Vector3f(0.0f, 1.0f, 0.0f) * sin(4 * PI / 3.0f));
    m_particles[4]->setVelocity({0.0f, 0.0f, 0.0f});

    m_particles[5]->setPosition(m_positionInit + Vector3f{0.0f, 0.0f, 1.0f} * cos(5 * PI / 3.0f) + Vector3f(0.0f, 1.0f, 0.0f) * sin(5 * PI / 3.0f));
    m_particles[5]->setVelocity({0.0f, 0.0f, 0.0f});

    m_particles[6]->setPosition(m_positionInit + Vector3f{0.0f, 0.0f, 1.0f} * cos(6 * PI / 3.0f) + Vector3f(0.0f, 1.0f, 0.0f) * sin(6 * PI / 3.0f));
    m_particles[6]->setVelocity({0.0f, 0.0f, 0.0f});
    */
}