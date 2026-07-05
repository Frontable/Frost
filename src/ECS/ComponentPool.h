#pragma once
#include "SparseSet.h"
#include "Core/Assert.h"
#include <vector>

namespace Frost
{

    class IComponentPool
    {
    public:
        virtual ~IComponentPool() = default;
        virtual void OnEntityDestroyed(Entity entity) = 0;
        virtual bool Has(Entity entity) const = 0;
    };
    
    template <typename T>
    class ComponentPool : public IComponentPool
    {
    public:
        
        void Add(Entity entity, T component)
        {
            FROST_ASSERT(!m_Set.Contains(entity),
                "Entity already has this component");

            m_Set.Insert(entity);
            m_Components.push_back(std::move(component));
        }
       
        void Remove(Entity entity)
        {
            FROST_ASSERT(m_Set.Contains(entity),
                "Entity does not have this component");

            size_t index = m_Set.Index(entity);

            // Swap component data with last
            m_Components[index] = std::move(m_Components.back());
            m_Components.pop_back();

            // SparseSet handles index bookkeeping
            m_Set.Remove(entity);
        }

        T& Get(Entity entity)
        {
            FROST_ASSERT(m_Set.Contains(entity),
                "Entity does not have this component");
            return m_Components[m_Set.Index(entity)];
        }

        const T& Get(Entity entity) const
        {
            FROST_ASSERT(m_Set.Contains(entity),
                "Entity does not have this component");
            return m_Components[m_Set.Index(entity)];
        }

        bool Has(Entity entity) const override
        {
            return m_Set.Contains(entity);
        }

        void OnEntityDestroyed(Entity entity) override
        {
            if (m_Set.Contains(entity))
                Remove(entity);
        }

       
        
        std::vector<T>& GetComponents() { return m_Components; }
        const std::vector<T>& GetComponents() const { return m_Components; }
        const SparseSet& GetSet()        const { return m_Set; }

        size_t Size() const { return m_Components.size(); }

    private:
        SparseSet      m_Set;
        std::vector<T> m_Components;
    };
}