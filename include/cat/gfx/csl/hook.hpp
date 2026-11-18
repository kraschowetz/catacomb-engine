#pragma once

namespace cat::csl
{

    struct Hook
    {
        const char* signature;
        const char* stub_body;
    };

    namespace hooks
    {
        inline constexpr Hook fragment_hooks[] = {
            {   // post_process_hook
                .signature = "void post_process_hook()",
                .stub_body = "{}\n"
            }
        };

        inline constexpr Hook vertex_hooks[] = {
            {   // pre_process_hook
                .signature = "void pre_process_hook()",
                .stub_body = "{}\n"
            }
        };
    }

}
