#ifndef EVENT_MANAGER_H
#define EVENT_MANAGER_H

#include <functional>
#include <unordered_map>

#include "Any.h"
#include "Singleton.h"

class EventManager : public Singleton<EventManager>
{
public:
    template <typename EventType>
    using FunctionType = std::function<void (EventType&)>;

    template <typename EventType, typename ClassType>
    void RegisterEvent(void (ClassType::*method)(EventType&), ClassType* obj)
    {
        FunctionType<EventType> f = [method, obj](EventType& event) {
            (obj->*method)(event);
        };
        Add(typeid(EventType).name(), std::move(f));
    }

    template <typename EventType>
    void RegisterEvent(void (*func)(EventType&))
    {
        FunctionType<EventType> f = func;
        Add(typeid(EventType).name(), std::move(f));
    }

    template <typename EventType>
    void RegisterEvent(FunctionType<EventType>&& f)
    {
        Add(typeid(EventType).name(), std::move(f));
    }

    template <typename EventType>
    void FireEvent(EventType& event)
    {
        using function_type = FunctionType<EventType>;
        auto range = eventMap_.equal_range(typeid(EventType).name());
        for (auto it = range.first; it != range.second; ++it)
        {
            auto& f = it->second.AnyCast<function_type>();
            f(event);
        }
    }

private:
    template <typename F>
    void Add(const char* name, F&& f)
    {
        eventMap_.emplace(name, std::move(f));
    }

    std::unordered_multimap<const char*, Any> eventMap_;
};

#define REGISTER_EVENT(method, obj) EventManager::GetSingleton().RegisterEvent(method, obj)
#define FIRE_EVENT(event) EventManager::GetSingleton().FireEvent(event);

#endif