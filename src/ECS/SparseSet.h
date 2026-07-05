#pragma once
#include "Entity.h"
#include "Core/Assert.h"
#include <vector>
#include <limits>

namespace Frost
{
    class SparseSet
    {
    public:
        static constexpr size_t NULL_INDEX = std::numeric_limits<size_t>::max();

        // 
        // add entity to set
        // 
        void Insert(Entity entity)
        {
            FROST_ASSERT(!Contains(entity), "Entity already in SparseSet");

            if (entity >= m_Sparse.size())
                m_Sparse.resize(entity + 1, NULL_INDEX);

            m_Sparse[entity] = m_Dense.size();
            m_Dense.push_back(entity);
        }

        // 
        // remove entity will swap with last, then pop
        // O(1) no holes in dense array
        // 
        void Remove(Entity entity)
        {
            FROST_ASSERT(Contains(entity), "Entity not in SparseSet");

            size_t index = m_Sparse[entity];
            Entity last = m_Dense.back();

            
            m_Dense[index] = last;
            m_Sparse[last] = index;

            
            m_Dense.pop_back();
            m_Sparse[entity] = NULL_INDEX;
        }

        bool Contains(Entity entity) const
        {
            return entity < m_Sparse.size()
                && m_Sparse[entity] != NULL_INDEX;
        }

        size_t Index(Entity entity) const
        {
            FROST_ASSERT(Contains(entity), "Entity not in SparseSet");
            return m_Sparse[entity];
        }

        size_t Size()  const { return m_Dense.size(); }
        bool   Empty() const { return m_Dense.empty(); }

        void Clear()
        {
            m_Dense.clear();
            m_Sparse.clear();
        }
        
        const std::vector<Entity>& GetDense() const { return m_Dense; }

        auto begin() { return m_Dense.begin(); }
        auto end() { return m_Dense.end(); }
        auto begin() const { return m_Dense.begin(); }
        auto end()   const { return m_Dense.end(); }

    private:
        std::vector<Entity> m_Dense;   
        std::vector<size_t> m_Sparse;  
    };
}