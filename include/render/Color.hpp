#ifndef MPJVP_COLOR
#define MPJVP_COLOR

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

typedef glm::vec3 color_t;

// minecraft color code https://minecraft.fr/faq/code-couleur-minecraft/
namespace Color
{
    constexpr color_t DARK_RED     {0.667f, 0.f   , 0.f   };
    constexpr color_t RED          {1.f   , 0.333f, 0.333f};
    constexpr color_t GOLD         {1.f   , 0.667f, 0.f   };
    constexpr color_t YELLOW       {1.f   , 1.f   , 0.333f};
    constexpr color_t DARK_GREEN   {0.f   , 0.667f, 0.f   };
    constexpr color_t GREEN        {0.333f, 1.f   , 0.333f};
    constexpr color_t AQUA         {0.333f, 1.f   , 1.f   };
    constexpr color_t DARK_AQUA    {0.f   , 0.667f, 0.667f};
    constexpr color_t DARK_BLUE    {0.f   , 0.f   , 0.667f};
    constexpr color_t BLUE         {0.333f, 0.333f, 1.f   };
    constexpr color_t LIGHT_PURPLE {1.f   , 0.333f, 1.f   };
    constexpr color_t DARK_PURPLE  {0.667f, 0.f   , 0.667f};
    constexpr color_t WHITE        {1.f   , 1.f   , 1.f   };
    constexpr color_t GRAY         {0.667f, 0.667f, 0.667f};
    constexpr color_t DARK_GRAY    {0.333f, 0.333f, 0.333f};
    constexpr color_t BLACK        {0.f   , 0.f   , 0.f   };

    constexpr color_t fromHex(std::uint32_t hex)
    {
        if(hex < 0x1000000) hex <<= 8;

        union
        {
            std::uint32_t l_hex;
            struct
            {
                std::byte l_a : 8;
                std::byte l_b : 8;
                std::byte l_g : 8;
                std::byte l_r : 8;
            };
        };

        l_hex = hex;

        return color_t {static_cast<float>(l_r) / 255.f, static_cast<float>(l_g) / 255.f, static_cast<float>(l_b) / 255.f };
    }
}

#endif // MPJVP_COLOR
