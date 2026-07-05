#pragma once
#include <functional>
#include <vector>
#include <unordered_map>
#include <typeindex>
#include <memory>

namespace Frost
{
    class EventBus
    {
    public:
        // 
        // every instance of On function per type will register its callback
        // in its Listener container and return given id so it can be removed if 
        // needed
        // 
        template <typename T>
        static size_t On(std::function<void(const T&)> callback)
        {
            size_t id = s_NextID++;
            GetListeners<T>().push_back({ id, std::move(callback) });
            return id;
        }

        // 
        // calls every reg callback for given Event Type
        //
        template <typename T>
        static void Emit(const T& event)
        {
            for (auto& [id, cb] : GetListeners<T>())
                cb(event);
        }

        // 
        // removes callback
        // 
        template <typename T>
        static void Off(size_t id)
        {
            auto& listeners = GetListeners<T>();
            listeners.erase(
                std::remove_if(listeners.begin(), listeners.end(),
                    [id](const auto& e) { return e.first == id; }),
                listeners.end()
            );
        }

        // 
        // clears all for a type
        // 
        template <typename T>
        static void Clear()
        {
            GetListeners<T>().clear();
        }

        // 
        // clears all
        // 
        static void ClearAll()
        {
            for (auto& [key, clearFn] : GetClearFunctions())
                clearFn();
        }

    private:
        template <typename T>
        using Listener = std::pair<size_t, std::function<void(const T&)>>;

        template <typename T>
        static std::vector<Listener<T>>& GetListeners()
        {
            static std::vector<Listener<T>> listeners;

            
            static bool registered = false;
            if (!registered)
            {
                registered = true;
                GetClearFunctions()[std::type_index(typeid(T))] = []()
                    {
                        GetListeners<T>().clear();
                    };
            }

            return listeners;
        }

        static std::unordered_map<std::type_index,
            std::function<void()>>&GetClearFunctions()
        {
            static std::unordered_map<std::type_index,
                std::function<void()>> fns;
            return fns;
        }

        static inline size_t s_NextID = 0;
    };

} // namespace Frost