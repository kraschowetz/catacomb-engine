#include <cat/physics/2d/components/c_character_body.hpp>

using namespace cat::physics2D;

glm::vec2 cCharacterBody::get_linear_velocity() const
{
    return m_linear_velocity;
}
