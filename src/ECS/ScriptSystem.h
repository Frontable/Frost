#pragma once
#include "Registry.h"
#include "ScriptComponent.h"

namespace Frost
{
    class ScriptSystem
    {
    public:
        ScriptSystem(Registry& registry) : m_Registry(registry) {}

        void Update(float dt)
        {
            auto view = m_Registry.GetView<ScriptComponent>();
            for (auto entry : view)
            {
                ScriptComponent& sc = std::get<0>(entry.components);

                for (auto& scriptEntry : sc.scripts)
                {
                    if (!scriptEntry.script) continue;

                    if (!scriptEntry.started)
                    {
                        scriptEntry.script->OnStart();
                        scriptEntry.started = true;
                    }

                    scriptEntry.script->OnUpdate(dt);
                }
            }

            m_Registry.FlushDestroyQueue();
        }

    private:
        Registry& m_Registry;
    };
}