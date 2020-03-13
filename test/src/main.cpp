#include "event_service/event_service.h"

namespace {

static bool did_thing = false;

struct my_event {
    int a;
};

void handle_event(const my_event &e) {
    did_thing = true;
}

} // namespace

int main() {
    gr8::event_service es;
    es.subscribe<my_event>(handle_event);
    es.dispatch(my_event{12});

    if (did_thing) {
        return 0;
    } else {
        return 1;
    }
}
