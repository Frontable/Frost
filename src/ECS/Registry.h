#pragma once
#include "Entity.h"
#include "ComponentPool.h"
#include "View.h"
#include "Core/EventBus.h"
#include "Core/Events.h"
#include "Core/Assert.h"
#include "Core/Logger.h"
#include <unordered_map>
#include <unordered_set>
#include <typeindex>
#include <queue>
#include <memory>
#include <vector>

namespace Frost
{
    class Registry
    {
    public:
        Registry() = default;
        ~Registry() = default;

        Registry(const Registry&) = delete;
        Registry& operator=(const Registry&) = delete;

        // 
        // entity management
        // 
        Entity Create()
        {
            Entity entity;

            if (!m_FreeList.empty())
            {
                entity = m_FreeList.front();
                m_FreeList.pop();
            }
            else
            {
                entity = m_NextEntity++;
            }

            m_Alive.insert(entity);
            EventBus::Emit(EntityCreatedEvent{ entity });
            return entity;
        }

        void Destroy(Entity entity)
        {
            FROST_ASSERT(IsAlive(entity), "Entity is not alive");

            for (auto& [type, pool] : m_Pools)
                pool->OnEntityDestroyed(entity);

            m_Alive.erase(entity);
            m_FreeList.push(entity);

            EventBus::Emit(EntityDestroyedEvent{ entity });
        }

        // 
        // deffered - safe to call during loop
        // 
        void DestroyDeferred(Entity entity)
        {
            FROST_ASSERT(IsAlive(entity), "Entity is not alive");
            m_DestroyQueue.push_back(entity);
        }

        void FlushDestroyQueue()
        {
            for (Entity e : m_DestroyQueue)
                if (IsAlive(e)) Destroy(e);
            m_DestroyQueue.clear();
        }

        bool IsAlive(Entity entity) const
        {
            return m_Alive.count(entity) > 0;
        }

        // 
        // component management
        // 
        template <typename T>
        void Add(Entity entity, T component = T{})
        {
            FROST_ASSERT(IsAlive(entity), "Entity is not alive");
            GetOrCreatePool<T>().Add(entity, std::move(component));
        }

        template <typename T>
        void Remove(Entity entity)
        {
            FROST_ASSERT(IsAlive(entity), "Entity is not alive");
            GetPool<T>().Remove(entity);
        }

        template <typename T>
        T& Get(Entity entity)
        {
            return GetPool<T>().Get(entity);
        }

        template <typename T>
        const T& Get(Entity entity) const
        {
            return GetPool<T>().Get(entity);
        }

        template <typename T>
        bool Has(Entity entity) const
        {
            auto it = m_Pools.find(std::type_index(typeid(T)));
            if (it == m_Pools.end()) return false;
            return static_cast<ComponentPool<T>*>(
                it->second.get())->Has(entity);
        }

        // 
        // views - very annoying
        // 
        template <typename... Components>
        View<Components...> GetView()
        {
            return View<Components...>{
                &GetOrCreatePool<Components>()...
            };
        }

        // 
        // reset will destroy all entities
        // 
        void Reset()
        {
            std::vector<Entity> alive(m_Alive.begin(), m_Alive.end());
            for (Entity e : alive)
                Destroy(e);

            m_Pools.clear();
            m_NextEntity = 0;
            while (!m_FreeList.empty())
                m_FreeList.pop();
            m_DestroyQueue.clear();
        }

        size_t EntityCount() const { return m_Alive.size(); }

    private:
        template <typename T>
        ComponentPool<T>& GetOrCreatePool()
        {
            auto key = std::type_index(typeid(T));
            if (m_Pools.find(key) == m_Pools.end())
                m_Pools.emplace(key, std::make_unique<ComponentPool<T>>());
            return *static_cast<ComponentPool<T>*>(m_Pools.at(key).get());
        }

        template <typename T>
        ComponentPool<T>& GetPool()
        {
            auto key = std::type_index(typeid(T));
            FROST_ASSERT(m_Pools.count(key), "Component pool does not exist");
            return *static_cast<ComponentPool<T>*>(m_Pools.at(key).get());
        }

        Entity                     m_NextEntity = 0;
        std::unordered_set<Entity> m_Alive;
        std::queue<Entity>         m_FreeList;
        std::vector<Entity>        m_DestroyQueue;

        std::unordered_map<std::type_index,
            std::unique_ptr<IComponentPool>> m_Pools;
    };
}