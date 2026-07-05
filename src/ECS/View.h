#pragma once
#include "Entity.h"
#include "ComponentPool.h"
#include <tuple>
#include <vector>

namespace Frost
{
    // 
    // View<Components...>
    // iterates entities that have ALL specified components
    //   
    // 
    template <typename... Components>
    class View
    {
    public:
        View(ComponentPool<Components>*... pools)
            : m_Pools{ pools... }
        {
            BuildEntityList();
        }

        
        struct ViewEntry
        {
            Entity entity;
            std::tuple<Components&...> components;

            // Structured binding support
            template <size_t I>
            decltype(auto) get() { return std::get<I>(components); }
        };

        
        struct Iterator
        {
            View* view;
            size_t index;

            ViewEntry operator*()
            {
                Entity e = view->m_Entities[index];
                return ViewEntry{
                    e,
                    std::tuple<Components&...>{
                        view->GetPool<Components>()->Get(e)...
                    }
                };
            }

            Iterator& operator++() { ++index; return *this; }

            bool operator!=(const Iterator& other) const
            {
                return index != other.index;
            }
        };

        Iterator begin() { return { this, 0 }; }
        Iterator end() { return { this, m_Entities.size() }; }

        size_t Size()  const { return m_Entities.size(); }
        bool   Empty() const { return m_Entities.empty(); }

    private:
        void BuildEntityList()
        {
            // find smallest pool to minimizes iterations
            size_t           minSize = std::numeric_limits<size_t>::max();
            const SparseSet* smallest = nullptr;

            auto checkPool = [&](auto* pool)
                {
                    if (pool && pool->Size() < minSize)
                    {
                        minSize = pool->Size();
                        smallest = &pool->GetSet();
                    }
                };

            (checkPool(std::get<ComponentPool<Components>*>(m_Pools)), ...);

            if (!smallest) return;

            for (Entity e : *smallest)
            {
                if (HasAll(e))
                    m_Entities.push_back(e);
            }
        }

        bool HasAll(Entity e) const
        {
            return (std::get<ComponentPool<Components>*>(m_Pools)->Has(e) && ...);
        }

        template <typename T>
        ComponentPool<T>* GetPool()
        {
            return std::get<ComponentPool<T>*>(m_Pools);
        }

        std::tuple<ComponentPool<Components>*...> m_Pools;
        std::vector<Entity>                       m_Entities;
    };
}