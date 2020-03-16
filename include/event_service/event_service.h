#pragma once

#include <any>
#include <functional>
#include <typeindex>
#include <unordered_map>

namespace gr8 {

struct subscription {
    const void *handle{nullptr};
};

class event_service {
public:
    template<typename EventT, typename HandlerT>
    subscription subscribe(HandlerT &&h) {
        const auto idx = std::type_index(typeid(EventT));
        auto it = handlers.emplace(idx, [func = std::forward<HandlerT>(h)](auto val) {
            func(std::any_cast<EventT>(val));
        });
        return {&it->second};
    }


    template<typename EventT, typename ClassT, typename Method>
    subscription subscribe(ClassT* instance, Method&& method) {
        const auto idx = std::type_index(typeid(EventT));
        auto it = handlers.emplace(idx, [instance, method](auto value) {
            (instance->*method)(std::any_cast<EventT>(value));
        });
        return {&it->second};
    }

    template<typename EventT>
    void dispatch(const EventT& event) {
        const auto idx = std::type_index(typeid(EventT));
        for(auto [id, last] = handlers.equal_range(idx); id != last; ++id) {
            id->second(std::any_cast<EventT>(event));
        }
    }

    void unsubscribe(const subscription &s) {
        for(auto it = handlers.begin(); it != handlers.end(); ++it) {
            if (&it->second == s.handle) {
                handlers.erase(it);
                return;
            }
        }
    }

private:
    std::unordered_multimap<
            std::type_index,
            std::function<void(std::any)>> handlers;
};

} // namespace gr8
