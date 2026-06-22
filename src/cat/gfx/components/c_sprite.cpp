#include <cat/gfx/components/c_sprite.hpp>

namespace cat 
{

f32 convert_z_index_to_position(u8 z_index)
{
    if(z_index == 0) return 0.f;

    return 1.f / (f32) z_index;
}

}
