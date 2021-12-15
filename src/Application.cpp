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

    initRigidbodyForceGenerators();
    createDemoPhase4();
    m_selected_mode = 4;
}

void Application::initParticleForceGenerators()
{
    std::unique_ptr<ParticleForceGenerator> particleGravity = std::make_unique<ParticleGravity>(9.81f);
    m_gameState.addParticleForceGenerator("gravity", std::move(particleGravity));

    std::unique_ptr<ParticleForceGenerator> particleDrag = std::make_unique<ParticleDrag>(0.0f, 0.1f);
    m_gameState.addParticleForceGenerator("drag", std::move(particleDrag));
}

void Application::initRigidbodyForceGenerators()
{
    std::unique_ptr<RigidBodyForceGenerator> rigidBodyGravity = std::make_unique<RigidBodyGravity>(9.81f);
    m_gameState.addRigidBodyForceGenerator("gravity", std::move(rigidBodyGravity));
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
        m_physicsEngine.clear();
        m_gameState.clear();

        switch(m_selected_mode)
        {
        case 0:
            initParticleForceGenerators();
            createGround();
            createBlobDemo();
            break;
        case 1:
            initParticleForceGenerators();
            createGround();
            createExamplePhase2Demo();
            break;
        case 2:
            initRigidbodyForceGenerators();
            createRigidCubeDemo();
            break;
        case 3:
            initRigidbodyForceGenerators();
            createCarCollisionDemo();
            break;
        case 4:
            initRigidbodyForceGenerators();
            createDemoPhase4();
            break;
        default:
            break;
        }
    }

    if(m_selected_mode == 0)
    {
        // Map with which we will call moveBlob, giving it different parameters according to the keys pressed
        static std::vector<std::tuple<int, std::function<void ()>>> keymap = 
        {
            { GLFW_KEY_Y,     [&]() { moveBlob(Vector3f{-1.0f,  0.0f, 0.0f}, deltaTime); } },
            { GLFW_KEY_H,     [&]() { moveBlob(Vector3f{ 1.0f,  0.0f, 0.0f}, deltaTime); } },
            { GLFW_KEY_G,     [&]() { moveBlob(Vector3f{ 0.0f, -1.0f, 0.0f}, deltaTime); } },
            { GLFW_KEY_J,     [&]() { moveBlob(Vector3f{ 0.0f,  1.0f, 0.0f}, deltaTime); } },
            { GLFW_KEY_SPACE, [&]()  { jumpBlob();                                        } },
        };

        for(const auto & [key, fun] : keymap)
        {
            if(m_graphicsEngine.getWindow().isKeyPressed(key)) fun();
        }
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

    if (m_selected_mode == 2)
    {
        checkRigidCubeDemo();
    }
    else if (m_selected_mode == 3)
    {
        // Check for car collision
        checkCarCollision();
    }

    // If the elapsed time is greater than TIMESTEP, we enter the loop
    while (elapsedTime >= TIMESTEP)
    {
        elapsedTime -= TIMESTEP;

        if (!pause)
        {
            // Update physics engine
            m_physicsEngine.update(TIMESTEP, m_gameState);

            // If there are rigidbody contacts
            auto contacts = m_gameState.getRigidBodyContacts();
            static int nbContact = 0;

            if (contacts.size() > 0)
            {
                if (m_displayContactShape && m_resetContactShape)
                {
                    while (nbContact > 0)
                    {
                        // Remove previous contact shapes
                        m_gameState.removeShapeGenerator("contact" + nbContact);
                        nbContact--;
                    }
                }

                for (RigidBodyContact & contact : contacts)
                {
                    nbContact++;

                    if (m_displayContactText)
                    {
                        std::cout << "Donnees du contact " << nbContact << " : \n" << contact << std::endl;
                    }

                    if (m_displayContactShape)
                    {
                        auto contactShape = std::make_unique<ContactShapeGenerator>(contact, Color::DARK_BLUE);
                        m_gameState.addShapeGenerator("contact" + nbContact, std::move(contactShape));
                    }
                }

                if (m_pauseAtContact) pause = true;
            }
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

    static bool isSizeInitialized = false;

    if (!isSizeInitialized)
    {
        // Default size
        float imGuiWidth = 200.0f;
        float imGuiHeight = 250.0f;
        ImGui::SetWindowSize(ImVec2(imGuiWidth, imGuiHeight));
        isSizeInitialized = true;
    }

    if (ImGui::BeginCombo("##combo", m_modes[m_selected_mode].c_str()))
    {
        for (int n = 0; n < m_modes.size(); n++)
        {
            bool isSelected = n == m_selected_mode;
            if (ImGui::Selectable(m_modes[n].c_str(), isSelected))
            {
                m_selected_mode = n;

                m_physicsEngine.clear();
                m_gameState.clear();

                // Create force generators
                std::unique_ptr<ParticleForceGenerator> particleGravity = std::make_unique<ParticleGravity>(9.81f);
                m_gameState.addParticleForceGenerator("gravity", std::move(particleGravity));

                std::unique_ptr<RigidBodyForceGenerator> rigidBodyGravity = std::make_unique<RigidBodyGravity>(9.81f);
                m_gameState.addRigidBodyForceGenerator("gravity", std::move(rigidBodyGravity));

                std::unique_ptr<ParticleForceGenerator> particleDrag = std::make_unique<ParticleDrag>(0.0f, 0.1f);
                m_gameState.addParticleForceGenerator("drag", std::move(particleDrag));

                switch(m_selected_mode)
                {
                case 0:
                    createGround();
                    createBlobDemo();
                    break;
                case 1:
                    createGround();
                    createExamplePhase2Demo();
                    break;
                case 2:
                    createRigidCubeDemo();
                    break;
                case 3:
                    createCarCollisionDemo();
                    break;
                case 4:
                    createDemoPhase4();
                    break;
                default:
                    break;
                }
            }

            if (isSelected)
            {
                ImGui::SetItemDefaultFocus();
            }

        }
        ImGui::EndCombo();
    }

    ImGui::Separator();
    ImGui::NewLine();

    if (m_selected_mode < 2 && ImGui::BeginTabBar("SettingsTabBar", ImGuiTabBarFlags_None))
    {
        // Tab to edit ground data
        if (m_selected_mode < 2 && ImGui::BeginTabItem("Ground"))
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

        // Tab to edit blob data
        if (m_selected_mode == 0 && ImGui::BeginTabItem("Blob"))
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

        ImGui::EndTabBar();

        ImGui::NewLine();

        if (m_selected_mode < 2)
        {
            ImGui::Separator();

            // Apply
            ImGui::SetCursorPosX((ImGui::GetWindowSize().x) * 0.40f);
            // ImGui::SetCursorPosY((ImGui::GetWindowSize().y) - 25.0f);
            if (ImGui::Button("Apply"))
            {
                if (m_selected_mode == 0)
                {
                    changeBlobSettings();
                }
                changeGroundSettings();
            }
        }
    }

    if (m_selected_mode == 4)
    {
        // Demo phase 4
        ImGui::Text("Pause when contact occurs : ");
        ImGui::Checkbox("##PauseAtContact", &m_pauseAtContact);

        ImGui::Text("Display contact data (text) : ");
        ImGui::Checkbox("##ContactDataText", &m_displayContactText);

        ImGui::Text("Display contact shape : ");
        ImGui::Checkbox("##ContactShape", &m_displayContactShape);

        ImGui::Text("Reset contact shapes (when new contact occurs) : ");
        ImGui::Checkbox("##ResetContactShape", &m_resetContactShape);
    }

    ImGui::End();
}

void Application::createExamplePhase2Demo()
{

    // DEMONSTRATION OF PARTICLE COLLISIONS AND CABLES
    // Particle
    auto particle1 = std::make_unique<Particle>(Vector3f{0, -2.5, 2}, 1.0f, 0.999f, false);
    particle1->setVelocity({ 0.0f, 0.0f, 0.0f });
    m_gameState.addParticle("particle_1", std::move(particle1));

    // Particle Shape
    auto particuleShape1 = std::make_unique<ParticleShapeGenerator>(m_gameState.getParticle("particle_1"), Color::DARK_GRAY);
    m_gameState.addShapeGenerator("particle_1", std::move(particuleShape1));

    // Gravity
    m_physicsEngine.registerForce(m_gameState.getParticle("particle_1"), m_gameState.getParticleForceGenerator<ParticleGravity>("gravity"), 0.0f);

    // Drag
    m_physicsEngine.registerForce(m_gameState.getParticle("particle_1"), m_gameState.getParticleForceGenerator<ParticleDrag>("drag"), 0.0f);



    // Particle
    auto particle2 = std::make_unique<Particle>(Vector3f{ 0, -1.5, 2 }, 1.0f, 0.999f, false);
    particle2->setVelocity({ 0.0f, 0.0f, 0.0f });
    m_gameState.addParticle("particle_2", std::move(particle2));

    // Particle Shape
    auto particuleShape2 = std::make_unique<ParticleShapeGenerator>(m_gameState.getParticle("particle_2"), Color::DARK_GRAY);
    m_gameState.addShapeGenerator("particle_2", std::move(particuleShape2));

    // Gravity
    m_physicsEngine.registerForce(m_gameState.getParticle("particle_2"), m_gameState.getParticleForceGenerator<ParticleGravity>("gravity"), 0.0f);

    // Drag
    m_physicsEngine.registerForce(m_gameState.getParticle("particle_2"), m_gameState.getParticleForceGenerator<ParticleDrag>("drag"), 0.0f);



    // Particle
    auto particle3 = std::make_unique<Particle>(Vector3f{ 0, -2, 2 }, 100000000000.0f, 0.999f, false);
    particle3->setVelocity({ 0.0f, 0.0f, 0.0f });
    m_gameState.addParticle("particle_3", std::move(particle3));

    // Particle Shape
    auto particuleShape3 = std::make_unique<ParticleShapeGenerator>(m_gameState.getParticle("particle_3"), Color::DARK_GRAY);
    m_gameState.addShapeGenerator("particle_3", std::move(particuleShape3));



    // Cable
    auto cable1 = std::make_unique<ParticleCable>(m_gameState.getParticle("particle_1"), m_gameState.getParticle("particle_3"), 1.0f, 0.5f);
    m_gameState.addParticleContactGenerator("cable_1_3", std::move(cable1));

    m_gameState.addShapeGenerator("cable_1_3", std::make_unique<LinkShapeGenerator>(m_gameState.getParticle("particle_1"), m_gameState.getParticle("particle_3"), Color::BLUE));

    // Cable
    auto cable2 = std::make_unique<ParticleCable>(m_gameState.getParticle("particle_2"), m_gameState.getParticle("particle_3"), 1.0f, 0.5f);
    m_gameState.addParticleContactGenerator("cable_2_3", std::move(cable2));

    m_gameState.addShapeGenerator("cable_2_3", std::make_unique<LinkShapeGenerator>(m_gameState.getParticle("particle_2"), m_gameState.getParticle("particle_3"), Color::BLUE));

    std::vector<Particle*> particleToTest;

    particleToTest.push_back(m_gameState.getParticle("particle_1"));
    particleToTest.push_back(m_gameState.getParticle("particle_2"));

    // Contact
    auto contact = std::make_unique<NaiveParticleContactGenerator>(particleToTest);
    m_gameState.addParticleContactGenerator("contact_1_2", std::move(contact));


    // DEMONSTRATION OF RODS
    // Particle
    auto particle4 = std::make_unique<Particle>(Vector3f{ 0, 2, 1 }, 1.0f, 0.999f, false);
    particle4->setVelocity({ 0.0f, 0.0f, 0.0f });
    m_gameState.addParticle("particle_4", std::move(particle4));

    // Particle Shape
    auto particuleShape4 = std::make_unique<ParticleShapeGenerator>(m_gameState.getParticle("particle_4"), Color::DARK_GRAY);
    m_gameState.addShapeGenerator("particle_4", std::move(particuleShape4));

    // Gravity
    m_physicsEngine.registerForce(m_gameState.getParticle("particle_4"), m_gameState.getParticleForceGenerator<ParticleGravity>("gravity"), 0.0f);

    // Drag
    m_physicsEngine.registerForce(m_gameState.getParticle("particle_4"), m_gameState.getParticleForceGenerator<ParticleDrag>("drag"), 0.0f);



    // Particle
    auto particle5 = std::make_unique<Particle>(Vector3f{ 0, 2, 2 }, 1.0f, 0.999f, false);
    particle5->setVelocity({ 0.0f, 0.0f, 0.0f });
    m_gameState.addParticle("particle_5", std::move(particle5));

    // Particle Shape
    auto particuleShape5 = std::make_unique<ParticleShapeGenerator>(m_gameState.getParticle("particle_5"), Color::DARK_GRAY);
    m_gameState.addShapeGenerator("particle_5", std::move(particuleShape5));

    // Gravity
    m_physicsEngine.registerForce(m_gameState.getParticle("particle_5"), m_gameState.getParticleForceGenerator<ParticleGravity>("gravity"), 0.0f);

    // Drag
    m_physicsEngine.registerForce(m_gameState.getParticle("particle_5"), m_gameState.getParticleForceGenerator<ParticleDrag>("drag"), 0.0f);



    // Particle
    auto particle6 = std::make_unique<Particle>(Vector3f{ 0, 1, 2 }, 100000000000.0f, 0.999f, false);
    particle6->setVelocity({ 0.0f, 0.0f, 0.0f });
    m_gameState.addParticle("particle_6", std::move(particle6));

    // Particle Shape
    auto particuleShape6 = std::make_unique<ParticleShapeGenerator>(m_gameState.getParticle("particle_6"), Color::DARK_GRAY);
    m_gameState.addShapeGenerator("particle_6", std::move(particuleShape6));



    // Rod
    auto rod1 = std::make_unique<ParticleRod>(m_gameState.getParticle("particle_4"), m_gameState.getParticle("particle_5"), 1.0f);
    m_gameState.addParticleContactGenerator("cable_4_5", std::move(rod1));

    m_gameState.addShapeGenerator("cable_4_5", std::make_unique<LinkShapeGenerator>(m_gameState.getParticle("particle_4"), m_gameState.getParticle("particle_5"), Color::GREEN));

    // Rod
    auto rod2 = std::make_unique<ParticleRod>(m_gameState.getParticle("particle_5"), m_gameState.getParticle("particle_6"), 1.0f);
    m_gameState.addParticleContactGenerator("cable_5_6", std::move(rod2));

    m_gameState.addShapeGenerator("cable_5_6", std::make_unique<LinkShapeGenerator>(m_gameState.getParticle("particle_5"), m_gameState.getParticle("particle_6"), Color::GREEN));


    // DEMONSTRATION OF GROUND CONTACT
    // Particle
    auto particle7 = std::make_unique<Particle>(Vector3f{ 0, 0, -1 }, 1.0f, 0.999f, false);
    particle7->setVelocity({ 0.0f, 0.0f, 0.0f });
    m_gameState.addParticle("particle_7", std::move(particle7));

    // Particle Shape
    auto particuleShape7 = std::make_unique<ParticleShapeGenerator>(m_gameState.getParticle("particle_7"), Color::DARK_GRAY);
    m_gameState.addShapeGenerator("particle_7", std::move(particuleShape7));

    // Gravity
    m_physicsEngine.registerForce(m_gameState.getParticle("particle_7"), m_gameState.getParticleForceGenerator<ParticleGravity>("gravity"), 0.0f);

    // Drag
    m_physicsEngine.registerForce(m_gameState.getParticle("particle_7"), m_gameState.getParticleForceGenerator<ParticleDrag>("drag"), 0.0f);

    // Ground contact
    auto groundContact = m_gameState.getParticleContactGenerator<WallContactGenerator>("groundContact");
    groundContact->addParticle(m_gameState.getParticle("particle_7"));





    // DEMONSTRATION OF SPRINGS
    // Particle
    auto particle8 = std::make_unique<Particle>(Vector3f{ 0, -4, 0 }, 1.0f, 0.999f, false);
    particle8->setVelocity({ 0.0f, 0.0f, 0.0f });
    m_gameState.addParticle("particle_8", std::move(particle8));

    // Particle Shape
    auto particuleShape8 = std::make_unique<ParticleShapeGenerator>(m_gameState.getParticle("particle_8"), Color::DARK_GRAY);
    m_gameState.addShapeGenerator("particle_8", std::move(particuleShape8));

    // Gravity
    m_physicsEngine.registerForce(m_gameState.getParticle("particle_8"), m_gameState.getParticleForceGenerator<ParticleGravity>("gravity"), 0.0f);

    // Drag
    m_physicsEngine.registerForce(m_gameState.getParticle("particle_8"), m_gameState.getParticleForceGenerator<ParticleDrag>("drag"), 0.0f);

    // Spring
    auto spring = std::make_unique<ParticleAnchoredSpring>(Vector3f{0,-4,1}, 5.0f, 2.0f);
    m_physicsEngine.registerForce(m_gameState.getParticle("particle_8"), spring.get(), 0.0);
    m_gameState.addParticleForceGenerator("spring", std::move(spring));

    auto particle9 = std::make_unique<Particle>(Vector3f{ 0, -4, 1 }, 1.0f, 0.999f, false);
    particle9->setVelocity({ 0.0f, 0.0f, 0.0f });
    m_gameState.addParticle("particle_9", std::move(particle9));

    // Particle Shape
    auto particuleShape9 = std::make_unique<ParticleShapeGenerator>(m_gameState.getParticle("particle_9"), Color::DARK_GRAY);
    m_gameState.addShapeGenerator("particle_9", std::move(particuleShape9));

    m_gameState.addShapeGenerator("spring_8_9", std::make_unique<LinkShapeGenerator>(m_gameState.getParticle("particle_8"), m_gameState.getParticle("particle_9"), Color::RED));
}

void Application::createBlobDemo()
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
        auto particuleShapeBlob = std::make_unique<ParticleShapeGenerator>(m_gameState.getParticle("blob_" + std::to_string(i)), Color::DARK_GRAY);
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
        auto linkShapeBlob = std::make_unique<LinkShapeGenerator>(particleA, particleB, Color::BLUE);
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
    auto wall1 = std::make_unique<CubeShapeGenerator>(Color::WHITE);
    wall1->setPosition(m_groundCenterPosition + Vector3f{-m_wallsLength / 2.0f, 0.0f, m_wallsWidth / 2.0f});
    wall1->scale({m_wallsThickness / 2.0f, m_wallsLength / 2.0f, m_wallsWidth / 2.0f});
    m_gameState.addShapeGenerator("wall1", std::move(wall1));

    // Wall 1 contact
    Vector3f directionWidthWall1{0.0f, 0.0f, -1.0f}; // Z-
    Vector3f directionLengthWall1{0.0f, 1.0f, 0.0f}; // Y+
    auto wall1Contact = std::make_unique<WallContactGenerator>(directionWidthWall1, directionLengthWall1, m_wallsLength, m_wallsWidth, m_wallsThickness, m_groundCenterPosition + Vector3f{-m_wallsLength / 2.0f, 0.0f, m_wallsWidth / 2.0f}, m_wallsRestitution);
    m_gameState.addParticleContactGenerator("wall1Contact", std::move(wall1Contact));

    // Wall 2 shape
    auto wall2 = std::make_unique<CubeShapeGenerator>(Color::WHITE);
    wall2->setPosition(m_groundCenterPosition + Vector3f{0.0f, m_wallsLength / 2.0f, m_wallsWidth / 2.0f});
    wall2->scale({m_wallsLength / 2.0f, m_wallsThickness / 2.0f, m_wallsWidth / 2.0f});
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

void Application::createRigidCubeDemo()
{
    auto rigidbodyMoving = std::make_unique<RigidBody>(Vector3f{0.0f, 0.0f, 3.0f}, 1.0f, 1.0f, 1.0f, false);
    rigidbodyMoving->setVelocity({ 0.0f, 0.0f, 0.5f });
    rigidbodyMoving->setAngularVelocity({0.0f, 0.0f, 3.0f});

    float angle = PI / 4.0;
    float nx = 1.0f;
    float ny = 0.0f;
    float nz = 0.0f;
    float norm = glm::sqrt(nx * nx + ny * ny + nz * nz);
    Quaternion initialQuaternion
    {
        glm::cos(angle / 2.0f),
        glm::sin(angle / 2.0f) * nx / norm,
        glm::sin(angle / 2.0f) * ny / norm,
        glm::sin(angle / 2.0f) * nz / norm
    };
    rigidbodyMoving->setQuaternion(initialQuaternion.normalize());

    m_gameState.addRigidBody("rigidbodyMoving", std::move(rigidbodyMoving));

    // Rigidbody moving Shape
    auto rigidbodyMovingShape = std::make_unique<RigidCubeShapeGenerator>(m_gameState.getRigidbody("rigidbodyMoving"), Color::DARK_GRAY);
    m_gameState.addShapeGenerator("rigidbodyMovingShape", std::move(rigidbodyMovingShape));


    // Cube submitted to gravity
    auto rigidBodySubmittedToGravity = std::make_unique<RigidBody>(Vector3f{ 0.0f, -2.5f, 0.0f }, 1.0f, 1.0f, 1.0f, false);
    rigidBodySubmittedToGravity->setVelocity({ 0.0f, 0.0f, 10.0f });
    rigidBodySubmittedToGravity->setAngularVelocity({ 0.0f, 0.0f, 0.0f });
    rigidBodySubmittedToGravity->setQuaternion({ 1.0f, 0.0f, 0.0f, 0.0f });
    m_gameState.addRigidBody("rigidBodySubmittedToGravity", std::move(rigidBodySubmittedToGravity));
    
    // Cube submitted to gravity's shape
    auto rigidBodySubmittedToGravityShape = std::make_unique<RigidCubeShapeGenerator>(m_gameState.getRigidbody("rigidBodySubmittedToGravity"), Color::DARK_RED);
    m_gameState.addShapeGenerator("rigidBodySubmittedToGravityShape", std::move(rigidBodySubmittedToGravityShape));

    // Cube submitted to gravity's gravity
    m_physicsEngine.registerForce(m_gameState.getRigidbody("rigidBodySubmittedToGravity"), m_gameState.getRigidBodyForceGenerator<RigidBodyGravity>("gravity"), 0.0f);


    // Cube with anchored spring
    auto rigidbodyWithAnchoredSpring = std::make_unique<RigidBody>(Vector3f{ 0.0f, -8.0f, 0.0f }, 1.0f, 0.999f, 0.999f, false);
    rigidbodyWithAnchoredSpring->setVelocity({ 0.0f, 0.0f, 0.0f });
    rigidbodyWithAnchoredSpring->setAngularVelocity({ 0.0f, 0.0f, 0.0f });
    rigidbodyWithAnchoredSpring->setQuaternion({ 1.0f, 0.0f, 0.0f, 0.0f });
    m_gameState.addRigidBody("rigidbodyWithAnchoredSpring", std::move(rigidbodyWithAnchoredSpring));
    
    // Cube with anchored spring shape
    auto rigidbodyWithAnchoredSpringShape = std::make_unique<RigidCubeShapeGenerator>(m_gameState.getRigidbody("rigidbodyWithAnchoredSpring"), Color::BLUE);
    m_gameState.addShapeGenerator("rigidbodyWithAnchoredSpringShape", std::move(rigidbodyWithAnchoredSpringShape));

    // Cube with anchored spring : anchored spring force
    Vector3f bodyAnchorPosition{0.0f, -0.5f, 0.5f};
    Vector3f anchorPosition{ 0.0f, -8.0f, 6.0f };
    float k = 20.0f;
    float restLength = 3.0f;

    std::unique_ptr<RigidBodyForceGenerator> rigidbodyAnchoredSpringForce = std::make_unique<RigidBodyAnchoredSpring>(bodyAnchorPosition, anchorPosition, k, restLength);
    m_physicsEngine.registerForce(m_gameState.getRigidbody("rigidbodyWithAnchoredSpring"), rigidbodyAnchoredSpringForce.get(), 0.0f);
    m_gameState.addRigidBodyForceGenerator("rigidbodyAnchoredSpring", std::move(rigidbodyAnchoredSpringForce));

    // Cube with anchored spring : gravity
    m_physicsEngine.registerForce(m_gameState.getRigidbody("rigidbodyWithAnchoredSpring"), m_gameState.getRigidBodyForceGenerator<RigidBodyGravity>("gravity"), 0.0f);

    // Anchor
    auto anchor = std::make_unique<RigidBody>(anchorPosition, 1.0f, 1.0F, 1.0f, false);
    anchor->setInverseMass(0.0f);
    anchor->setVelocity({ 0.0f, 0.0f, 0.0f });
    anchor->setAngularVelocity({ 0.0f, 0.0f, 0.0f });
    anchor->setQuaternion({ 1.0f, 0.0f, 0.0f, 0.0f });
    m_gameState.addRigidBody("anchor", std::move(anchor));

    // Anchor shape
    auto anchorShape = std::make_unique<RigidCubeShapeGenerator>(m_gameState.getRigidbody("anchor"), Color::DARK_GREEN);
    anchorShape->setScale(0.15f);
    m_gameState.addShapeGenerator("anchorShape", std::move(anchorShape));

    // Rigidbody link shape
    m_gameState.addShapeGenerator("ridibodyAnchoredSpringLinkShape", std::make_unique<RigidBodyLinkShapeGenerator>(m_gameState.getRigidbody("rigidbodyWithAnchoredSpring"), bodyAnchorPosition, m_gameState.getRigidbody("anchor"), Vector3f{0.0f, 0.0f, 0.0f}, Color::WHITE));


    // Little cube spring
    auto littleCubeSpring = std::make_unique<RigidBody>(Vector3f{ 0.0f, -11.0f, -1.0f }, 0.5f, 0.999f, 0.999f, false);
    littleCubeSpring->setVelocity({ 0.0f, 0.0f, 0.0f });
    littleCubeSpring->setAngularVelocity({ 0.0f, 0.0f, 0.0f });
    littleCubeSpring->setQuaternion({ 1.0f, 0.0f, 0.0f, 0.0f });
    m_gameState.addRigidBody("littleCubeSpring", std::move(littleCubeSpring));

    // Little cube spring shape
    auto littleCubeSpringShape = std::make_unique<RigidCubeShapeGenerator>(m_gameState.getRigidbody("littleCubeSpring"), Color::YELLOW);
    littleCubeSpringShape->setScale(0.25f);
    m_gameState.addShapeGenerator("littleCubeSpringShape", std::move(littleCubeSpringShape));

    // Little cube spring : gravity
    m_physicsEngine.registerForce(m_gameState.getRigidbody("littleCubeSpring"), m_gameState.getRigidBodyForceGenerator<RigidBodyGravity>("gravity"), 0.0f);

    // Little cube spring : spring force
    Vector3f littleCubeBodyAnchorPosition{0.0f, -0.25f, 0.25f};
    Vector3f otherCubeBodyAnchorPosition{0.0f, 0.5f, -0.5f};
    k = 30.0f;
    restLength = 2.0f;

    std::unique_ptr<RigidBodyForceGenerator> littleCubeSpringForce = std::make_unique<RigidBodySpring>(littleCubeBodyAnchorPosition, m_gameState.getRigidbody("rigidbodyWithAnchoredSpring"), otherCubeBodyAnchorPosition, k, restLength);
    m_physicsEngine.registerForce(m_gameState.getRigidbody("littleCubeSpring"), littleCubeSpringForce.get(), 0.0f);
    m_gameState.addRigidBodyForceGenerator("littleCubeSpring", std::move(littleCubeSpringForce));

    std::unique_ptr<RigidBodyForceGenerator> otherLittleCubeSpringForce = std::make_unique<RigidBodySpring>(otherCubeBodyAnchorPosition, m_gameState.getRigidbody("littleCubeSpring"), littleCubeBodyAnchorPosition, k, restLength);
    m_physicsEngine.registerForce(m_gameState.getRigidbody("rigidbodyWithAnchoredSpring"), otherLittleCubeSpringForce.get(), 0.0f);
    m_gameState.addRigidBodyForceGenerator("otherLittleCubeSpring", std::move(otherLittleCubeSpringForce));

    // Rigidbody link shape
    m_gameState.addShapeGenerator("littleCubeSpringLinkShape", std::make_unique<RigidBodyLinkShapeGenerator>(m_gameState.getRigidbody("littleCubeSpring"), littleCubeBodyAnchorPosition, m_gameState.getRigidbody("rigidbodyWithAnchoredSpring"), otherCubeBodyAnchorPosition, Color::LIGHT_PURPLE));


    // Ground shape
    auto groundShape = std::make_unique<CubeShapeGenerator>(Color::DARK_GRAY);
    groundShape->setScale({25.0f / 2.0f, 25.0f / 2.0f, 0.5f / 2.0f});
    groundShape->setPosition({0.0f, 0.0f, -6.0f});
    m_gameState.addShapeGenerator("ground", std::move(groundShape));

    // Background shape
    auto backgroundgroundShape = std::make_unique<CubeShapeGenerator>(Color::WHITE);
    backgroundgroundShape->setScale({1.0f, 50.0f, 50.0f});
    backgroundgroundShape->setPosition({-30.0f, 0.0f, 0.0f});
    m_gameState.addShapeGenerator("background", std::move(backgroundgroundShape));

    // Human rigidbody
    auto humanRigidbody = std::make_unique<RigidBody>(Vector3f{0.0f, 2.0f, 2.0f}, 1.0f, 1.0f, 1.0f, false);
    humanRigidbody->setVelocity({ 0.0f, 2.0f, 0.0f });
    humanRigidbody->setAngularVelocity({0.0f, 0.0f, 4.0f});
    humanRigidbody->setQuaternion({1.0f, 0.0f, 0.0f, 0.0f});

    m_gameState.addRigidBody("humanRigidbody", std::move(humanRigidbody));

    // Human shape
    auto humanShape = std::make_unique<HumanShapeGenerator>(m_gameState.getRigidbody("humanRigidbody"), Color::GOLD);
    humanShape->setScale(1.5f);
    m_gameState.addShapeGenerator("humanShape", std::move(humanShape));
}

void Application::createCarCollisionDemo()
{
    m_carCollision = false;

    // ------ Car 1 ------
    auto car1 = std::make_unique<RigidBody>(Vector3f{-3.0f, -20.0f, -6.0f + 0.25f + 1.0f}, 50.0f, 0.999f, 0.999f, false);
    car1->setVelocity({ 0.0f, 18.0f, 0.0f });
    car1->setAngularVelocity({0.0f, 0.0f, 0.0f});
    car1->setQuaternion({1.0f, 0.0f, 0.0f, 0.0f});
    m_gameState.addRigidBody("car1", std::move(car1));

    // Car 1 shape
    auto car1Shape = std::make_unique<RigidCubeShapeGenerator>(m_gameState.getRigidbody("car1"), Color::RED);
    car1Shape->setScale({2.0f, 3.0f, 1.0f});
    m_gameState.addShapeGenerator("car1", std::move(car1Shape));

    // ------ Car 2 ------
    auto car2 = std::make_unique<RigidBody>(Vector3f{-6.0f, 20.0f, -6.0f + 0.25f + 1.0f}, 50.0f, 0.999f, 0.999f, false);
    car2->setVelocity({ 0.0f, -18.0f, 0.0f });
    car2->setAngularVelocity({0.0f, 0.0f, 0.0f});
    car2->setQuaternion({1.0f, 0.0f, 0.0f, 0.0f});
    m_gameState.addRigidBody("car2", std::move(car2));

    // Car 2 shape
    auto car2Shape = std::make_unique<RigidCubeShapeGenerator>(m_gameState.getRigidbody("car2"), Color::BLUE);
    car2Shape->setScale({2.0f, 3.0f, 1.0f});
    m_gameState.addShapeGenerator("car2", std::move(car2Shape));

    // Ground shape
    auto groundShape = std::make_unique<CubeShapeGenerator>(Color::DARK_GRAY);
    groundShape->setScale({100.0f, 100.0f, 0.5f / 2.0f});
    groundShape->setPosition({0.0f, 0.0f, -6.0f});
    m_gameState.addShapeGenerator("ground", std::move(groundShape));

    // Background shape
    auto backgroundgroundShape = std::make_unique<CubeShapeGenerator>(Color::WHITE);
    backgroundgroundShape->setScale({1.0f, 50.0f, 50.0f});
    backgroundgroundShape->setPosition({-30.0f, 0.0f, 0.0f});
    m_gameState.addShapeGenerator("background", std::move(backgroundgroundShape));
}

void Application::checkCarCollision()
{
    if (!m_carCollision)
    {
        auto car1 = m_gameState.getRigidbody("car1");
        auto car2 = m_gameState.getRigidbody("car2");

        if (car1->getPosition().getY() + 3.0f >= car2->getPosition().getY() - 3.0f)
        {
            car1->setAngularVelocity({0.0f, 0.0f, 2.5f});
            car1->setVelocity({2.0f, -4.0f, 0.0f});

            car2->setAngularVelocity({0.0f, 0.0f, 1.5f});
            car2->setVelocity({-1.0f, 3.0f, 0.0f});

            m_carCollision = true;
        }
    }
}

void Application::checkRigidCubeDemo()
{
    // Check 'humanRigidbody' (goes back and forth)
    auto humanRigidbody = m_gameState.getRigidbody("humanRigidbody");
    Vector3f humanRigidbodyPosition = humanRigidbody->getPosition();
    if (humanRigidbodyPosition.getY() > 7.0f)
    {
        humanRigidbodyPosition.setY(6.8f);
        humanRigidbody->setVelocity(-humanRigidbody->getVelocity());
        humanRigidbody->setAngularVelocity(-humanRigidbody->getAngularVelocity());
    }
    else if (humanRigidbodyPosition.getY() < 2.0f)
    {
        humanRigidbodyPosition.setY(2.2f);
        humanRigidbody->setVelocity(-humanRigidbody->getVelocity());
        humanRigidbody->setAngularVelocity(-humanRigidbody->getAngularVelocity());
    }

    // Check if the cube hits the ground
    auto gravityCube = m_gameState.getRigidbody("rigidBodySubmittedToGravity");
    if (gravityCube->getPosition().getZ() < -6.0f + 0.25f + 0.5f)
    {
        if (gravityCube->getVelocity().norm() <= 0.3f)
        {
            gravityCube->setVelocity({0.0f, 0.0f, 0.0f});
        }
        else
        {
            gravityCube->setVelocity(- gravityCube->getVelocity() / 2.0f);
        }

        Vector3f position = gravityCube->getPosition();
        position.setZ(-6.0f + 0.25f + 0.5f);
        gravityCube->setPosition(position);
    }
}

void Application::createDemoPhase4()
{

    // Ground shape
    Vector3f groundCenterPosition{0.0f, 0.0f, -6.0f};
    float groundHalfLength = 70.0f / 2.0f;
    float groundHalfWidth = 70.0f / 2.0f;
    float groundHalfHeight = 0.5f / 2.0f;

    auto groundShape = std::make_unique<CubeShapeGenerator>(Color::DARK_GRAY);
    groundShape->setScale({groundHalfLength, groundHalfWidth, groundHalfHeight});
    groundShape->setPosition(groundCenterPosition);
    m_gameState.addShapeGenerator("ground", std::move(groundShape));

    // Walls
    float wallHalfLength = 50.0f / 2.0f;
    float wallHalfWidth = 0.4f / 2.0f;
    float wallHalfHeight = 5.0f / 2.0f;

    // Wall 1 shape
    Vector3f wall1CenterPosition = groundCenterPosition + Vector3f{0.0f, 0.0f, groundHalfHeight} + Vector3f{-wallHalfLength, 0.0f, wallHalfHeight};

    float angle = PI / 2.0;
    float nx = 0.0f;
    float ny = 0.0f;
    float nz = 1.0f;
    float norm = glm::sqrt(nx * nx + ny * ny + nz * nz);
    Quaternion wall1Quaternion
    {
        glm::cos(angle / 2.0f),
        glm::sin(angle / 2.0f) * nx / norm,
        glm::sin(angle / 2.0f) * ny / norm,
        glm::sin(angle / 2.0f) * nz / norm
    };
    Matrix33 rotationWall1;
    rotationWall1.setOrientation(wall1Quaternion);

    auto wall1 = std::make_unique<CubeShapeGenerator>(Color::WHITE);
    wall1->setPosition(wall1CenterPosition);
    wall1->setRotation(rotationWall1);
    wall1->scale({wallHalfLength + wallHalfWidth, wallHalfWidth, wallHalfHeight});
    m_gameState.addShapeGenerator("wall1", std::move(wall1));

    // Wall 1 bounding volume (sphere)
    float radius = glm::sqrt(wallHalfLength * wallHalfLength + wallHalfWidth * wallHalfWidth + wallHalfHeight * wallHalfHeight);
    std::unique_ptr<BoundingVolumeSphere> wall1BoundingVolume = std::make_unique<BoundingVolumeSphere>(wall1CenterPosition, radius);

    // Wall 1 primitive (plane)
    Matrix34 wall1Offset;
    wall1Offset.setOrientationAndPosition(wall1Quaternion, wall1CenterPosition);
    std::unique_ptr<Plane> wall1Primitive = std::make_unique<Plane>(nullptr, wall1Offset, Vector3f{1.0f, 0.0f, 0.0f}, wallHalfWidth * 2.0f);

    m_gameState.getLinksBetweenBoundingVolumesAndPrimitives().emplace(std::make_pair<BoundingVolumeSphere*, std::vector<Primitive*>>(wall1BoundingVolume.get(), {wall1Primitive.get()}));

    m_gameState.getBoundingVolumeSphere().push_back(std::move(wall1BoundingVolume));
    m_gameState.getPrimitives().push_back(std::move(wall1Primitive));

    // Wall 2 shape
    Vector3f wall2CenterPosition = groundCenterPosition + Vector3f{0.0f, 0.0f, groundHalfHeight} + Vector3f{0.0f, wallHalfLength, wallHalfHeight};

    angle = 0.0f;
    nx = 0.0f;
    ny = 0.0f;
    nz = 1.0f;
    norm = glm::sqrt(nx * nx + ny * ny + nz * nz);
    Quaternion wall2Quaternion
    {
        glm::cos(angle / 2.0f),
        glm::sin(angle / 2.0f) * nx / norm,
        glm::sin(angle / 2.0f) * ny / norm,
        glm::sin(angle / 2.0f) * nz / norm
    };
    Matrix33 rotationWall2;
    rotationWall2.setOrientation(wall2Quaternion);

    auto wall2 = std::make_unique<CubeShapeGenerator>(Color::WHITE);
    wall2->setPosition(wall2CenterPosition);
    wall2->setRotation(rotationWall2);
    wall2->scale({wallHalfLength - wallHalfWidth, wallHalfWidth, wallHalfHeight});
    m_gameState.addShapeGenerator("wall2", std::move(wall2));

    // Wall 2 bounding volume (sphere)
    std::unique_ptr<BoundingVolumeSphere> wall2BoundingVolume = std::make_unique<BoundingVolumeSphere>(wall2CenterPosition, radius);

    // Wall 2 primitive (plane)
    Matrix34 wall2Offset;
    wall2Offset.setOrientationAndPosition(wall2Quaternion, wall2CenterPosition);
    std::unique_ptr<Plane> wall2Primitive = std::make_unique<Plane>(nullptr, wall2Offset, Vector3f{0.0f, -1.0f, 0.0f}, wallHalfWidth * 2.0f);

    m_gameState.getLinksBetweenBoundingVolumesAndPrimitives().emplace(std::make_pair<BoundingVolumeSphere*, std::vector<Primitive*>>(wall2BoundingVolume.get(), {wall2Primitive.get()}));

    m_gameState.getBoundingVolumeSphere().push_back(std::move(wall2BoundingVolume));
    m_gameState.getPrimitives().push_back(std::move(wall2Primitive));

    // Wall 3 shape
    Vector3f wall3CenterPosition = groundCenterPosition + Vector3f{0.0f, 0.0f, groundHalfHeight} + Vector3f{0.0f, -wallHalfLength, wallHalfHeight};

    angle = 0.0f;
    nx = 0.0f;
    ny = 0.0f;
    nz = 1.0f;
    norm = glm::sqrt(nx * nx + ny * ny + nz * nz);
    Quaternion wall3Quaternion
    {
        glm::cos(angle / 2.0f),
        glm::sin(angle / 2.0f) * nx / norm,
        glm::sin(angle / 2.0f) * ny / norm,
        glm::sin(angle / 2.0f) * nz / norm
    };
    Matrix33 rotationWall3;
    rotationWall3.setOrientation(wall3Quaternion);

    auto wall3 = std::make_unique<CubeShapeGenerator>(Color::WHITE);
    wall3->setPosition(wall3CenterPosition);
    wall3->setRotation(rotationWall3);
    wall3->scale({wallHalfLength - wallHalfWidth, wallHalfWidth, wallHalfHeight});
    m_gameState.addShapeGenerator("wall3", std::move(wall3));

    // Wall 3 bounding volume (sphere)
    std::unique_ptr<BoundingVolumeSphere> wall3BoundingVolume = std::make_unique<BoundingVolumeSphere>(wall3CenterPosition, radius);

    // Wall 3 primitive (plane)
    Matrix34 wall3Offset;
    wall3Offset.setOrientationAndPosition(wall3Quaternion, wall3CenterPosition);
    std::unique_ptr<Plane> wall3Primitive = std::make_unique<Plane>(nullptr, wall3Offset, Vector3f{0.0f, 1.0f, 0.0f}, wallHalfWidth * 2.0f);

    m_gameState.getLinksBetweenBoundingVolumesAndPrimitives().emplace(std::make_pair<BoundingVolumeSphere*, std::vector<Primitive*>>(wall3BoundingVolume.get(), {wall3Primitive.get()}));

    m_gameState.getBoundingVolumeSphere().push_back(std::move(wall3BoundingVolume));
    m_gameState.getPrimitives().push_back(std::move(wall3Primitive));

    // Wall 4 shape
    Vector3f wall4CenterPosition = groundCenterPosition + Vector3f{0.0f, 0.0f, groundHalfHeight} + Vector3f{wallHalfLength, 0.0f, wallHalfHeight};

    angle = PI / 2.0;
    nx = 0.0f;
    ny = 0.0f;
    nz = 1.0f;
    norm = glm::sqrt(nx * nx + ny * ny + nz * nz);
    Quaternion wall4Quaternion
    {
        glm::cos(angle / 2.0f),
        glm::sin(angle / 2.0f) * nx / norm,
        glm::sin(angle / 2.0f) * ny / norm,
        glm::sin(angle / 2.0f) * nz / norm
    };
    Matrix33 rotationWall4;
    rotationWall4.setOrientation(wall4Quaternion);

    auto wall4 = std::make_unique<CubeShapeGenerator>(Color::WHITE);
    wall4->setPosition(wall4CenterPosition);
    wall4->setRotation(rotationWall4);
    wall4->scale({wallHalfLength + wallHalfWidth, wallHalfWidth, wallHalfHeight});
    m_gameState.addShapeGenerator("wall4", std::move(wall4));

    // Wall 4 bounding volume (sphere)
    std::unique_ptr<BoundingVolumeSphere> wall4BoundingVolume = std::make_unique<BoundingVolumeSphere>(wall4CenterPosition, radius);

    // Wall 4 primitive (plane)
    Matrix34 wall4Offset;
    wall4Offset.setOrientationAndPosition(wall4Quaternion, wall4CenterPosition);
    std::unique_ptr<Plane> wall4Primitive = std::make_unique<Plane>(nullptr, wall4Offset, Vector3f{-1.0f, 0.0f, 0.0f}, wallHalfWidth * 2.0f);

    m_gameState.getLinksBetweenBoundingVolumesAndPrimitives().emplace(std::make_pair<BoundingVolumeSphere*, std::vector<Primitive*>>(wall4BoundingVolume.get(), {wall4Primitive.get()}));

    m_gameState.getBoundingVolumeSphere().push_back(std::move(wall4BoundingVolume));
    m_gameState.getPrimitives().push_back(std::move(wall4Primitive));

    // Box
    float boxHalfLength = 6.0f / 2.0f;
    float boxHalfWidth = 3.0f / 2.0f;
    float boxHalfHeight = 1.0f / 2.0f;
    Vector3f boxPosition = groundCenterPosition + Vector3f{0.0f, 0.0f, groundHalfHeight} + Vector3f{0.0F, 10.0f, boxHalfHeight};

    angle = PI / 2.0;
    nx = 0.0f;
    ny = 0.0f;
    nz = 1.0f;
    norm = glm::sqrt(nx * nx + ny * ny + nz * nz);
    Quaternion boxQuaternion
    {
        glm::cos(angle / 2.0f),
        glm::sin(angle / 2.0f) * nx / norm,
        glm::sin(angle / 2.0f) * ny / norm,
        glm::sin(angle / 2.0f) * nz / norm
    };

    Vector3f boxInitialLinearVelocity{-10.0f, 1.0f, 0.0f};
    Vector3f boxInitialAngularVelocity{0.0f, 0.0f, 5.0f};

    auto box = std::make_unique<RigidBody>(boxPosition, 1.0f, 0.999f, 0.999f, false);
    box->setVelocity(boxInitialLinearVelocity);
    box->setAngularVelocity(boxInitialAngularVelocity);
    box->setQuaternion(boxQuaternion);
    m_gameState.addRigidBody("box", std::move(box));

    // Box bounding volume (sphere)
    radius = glm::sqrt(boxHalfLength * boxHalfLength + boxHalfWidth * boxHalfWidth + boxHalfHeight * boxHalfHeight);
    std::unique_ptr<BoundingVolumeSphere> boxBoundingVolume = std::make_unique<BoundingVolumeSphere>(boxPosition, radius, m_gameState.getRigidbody("box"));

    // Box primitive (box)
    Matrix34 boxOffset
    {
        {
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f
        }
    };
    std::unique_ptr<Box> boxPrimitive = std::make_unique<Box>(m_gameState.getRigidbody("box"), boxOffset, Vector3f{boxHalfLength, boxHalfWidth, boxHalfHeight});

    m_gameState.getLinksBetweenBoundingVolumesAndPrimitives().emplace(std::make_pair<BoundingVolumeSphere*, std::vector<Primitive*>>(boxBoundingVolume.get(), {boxPrimitive.get()}));

    m_gameState.getBoundingVolumeSphere().push_back(std::move(boxBoundingVolume));
    m_gameState.getPrimitives().push_back(std::move(boxPrimitive));

    // Box shape
    auto boxShape = std::make_unique<RigidCubeShapeGenerator>(m_gameState.getRigidbody("box"), Color::RED);
    boxShape->setScale({boxHalfLength, boxHalfWidth, boxHalfHeight});
    m_gameState.addShapeGenerator("box", std::move(boxShape));
}