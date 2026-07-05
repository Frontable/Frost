#pragma once
#include "ScriptBase.h"
#include <memory>
#include <vector>

namespace Frost
{
    struct ScriptComponent
    {
        struct Entry
        {
            std::unique_ptr<ScriptBase> script;
            bool started = false;
        };

        std::vector<Entry> scripts;
    };
}