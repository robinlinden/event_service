#pragma once

#include <any>
#include <functional>
#include <map>
#include <typeindex>

namespace gr8 {

class event_service {
public:
    template<typename EventT, typename HandlerT>
    void subscribe(HandlerT &&h) {
        const auto idx = std::type_index(typeid(EventT));
        handlers.emplace(idx, [func = std::forward<HandlerT>(h)](auto val) {
            func(std::any_cast<EventT>(val));
        });
    }

    template<typename EventT, typename ClassT, typename Method>
    void subscribe(ClassT* instance, Method&& method) {
        const auto idx = std::type_index(typeid(EventT));
        handlers.emplace(idx, [instance, method](auto value) {
            (instance->*method)(std::any_cast<EventT>(value));
        });
    }

    template<typename EventT>
    void dispatch(const EventT& event) {
        const auto idx = std::type_index(typeid(EventT));
        for(auto [id, last] = handlers.equal_range(idx); id != last; ++id) {
            id->second(std::any_cast<EventT>(event));
        }
    }

private:
    std::multimap<std::type_index, std::function<void(std::any)>> handlers;
};

} // namespace gr8
