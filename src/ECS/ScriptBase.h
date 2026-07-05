#pragma once
#include "Entity.h"

namespace Frost
{
    class Registry;

    class ScriptBase
    {
    public:
        virtual ~ScriptBase() = default;

        virtual void OnStart() {}
        virtual void OnUpdate(float dt) {}
        virtual void OnDestroy() {}

        template <typename T>
        T& GetComponent();

        template <typename T>
        bool HasComponent() const;

        template <typename T>
        void AddComponent(T component = T{});

        template <typename T>
        void RemoveComponent();

        void Destroy();

        Entity    m_Entity = NULL_ENTITY;
        Registry* m_Registry = nullptr;
    };
}

#include "Registry.h"

namespace Frost
{
    template <typename T>
    T& ScriptBase::GetComponent()
    {
        return m_Registry->Get<T>(m_Entity);
    }

    template <typename T>
    bool ScriptBase::HasComponent() const
    {
        return m_Registry->Has<T>(m_Entity);
    }

    template <typename T>
    void ScriptBase::AddComponent(T component)
    {
        m_Registry->Add<T>(m_Entity, std::move(component));
    }

    template <typename T>
    void ScriptBase::RemoveComponent()
    {
        m_Registry->Remove<T>(m_Entity);
    }

    inline void ScriptBase::Destroy()
    {
        m_Registry->DestroyDeferred(m_Entity);
    }
}