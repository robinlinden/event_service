#include "event_service/event_service.h"

namespace {

static bool did_thing = false;

struct my_event {
    int a;
};

void handle_event(const my_event &) {
    did_thing = true;
}

class some_other_event {};

class amazing_class {
public:
    void do_thing(const some_other_event&) {
        handled_event = true;
    }

    bool handled_event = false;
};

struct third_event {
    std::string text;
};

class best_class {
public:
    best_class(
            gr8::event_service *srv,
            const std::string &s) : valid_string(s) {
        srv->subscribe<third_event>(this, &best_class::on_event);
    }

    void on_event(const third_event &e) {
        if (e.text == valid_string) {
            found_valid_string = true;
        }
    }

    std::string valid_string;
    bool found_valid_string = false;
};

} // namespace

int main() {
    gr8::event_service event_service;
    event_service.subscribe<my_event>(handle_event);
    event_service.dispatch(my_event{12});
    if (!did_thing) {
        return 1;
    }

    amazing_class ac;
    event_service.subscribe<some_other_event>(&ac, &amazing_class::do_thing);
    event_service.dispatch(some_other_event{});
    if (!ac.handled_event) {
        return 1;
    }

    best_class bc{&event_service, "hello"};
    if (bc.found_valid_string) {
        return 1;
    }

    event_service.dispatch(third_event{"test"});
    if (bc.found_valid_string) {
        return 1;
    }

    event_service.dispatch(third_event{"hello"});
    if (!bc.found_valid_string) {
        return 1;
    }

    return 0;
}
