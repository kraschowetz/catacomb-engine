#pragma once

#include <cat/util/memory.hpp>
#include <cat/util/math.hpp>

namespace cat
{

struct RenderContext
{
    glm::mat4 view;
    glm::mat4 projection;

    // TODO: FrameBuffer* output;
};

}
