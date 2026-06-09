#pragma once

namespace cat::csl
{

inline const char PREAMBLE[] = R"preamble(
    #version 460

    // TODO: structs & general-stubs declaration
    
    // vertex hooks
    void pre_process_hook();

    // fragment hooks
    void post_process_hook();
)preamble";

}
