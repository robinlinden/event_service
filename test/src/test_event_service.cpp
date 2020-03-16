#include "event_service/event_service.h"

#include <gtest/gtest.h>

namespace {

static bool did_thing = false;

struct my_event {
    int a;
};

void handle_event(const my_event &) {
    did_thing = true;
}

TEST(event_service, free_function) {
    gr8::event_service event_service;
    event_service.subscribe<my_event>(handle_event);
    event_service.dispatch(my_event{12});
    EXPECT_TRUE(did_thing);
}


class some_other_event {};

class amazing_class {
public:
    void do_thing(const some_other_event&) {
        handled_event = true;
    }

    bool handled_event = false;
};

TEST(event_service, subscribe_to_other_member) {
    gr8::event_service event_service;
    amazing_class ac;
    event_service.subscribe<some_other_event>(&ac, &amazing_class::do_thing);
    event_service.dispatch(some_other_event{});
    EXPECT_TRUE(ac.handled_event);
}

struct third_event {
    std::string text;
};

static int strings_found;

class best_class {
public:
    best_class(
            gr8::event_service *srv,
            const std::string &s) : srv_(srv), valid_string(s) {
        token = srv->subscribe<third_event>(this, &best_class::on_event);
    }

    ~best_class() {
        srv_->unsubscribe(token);
    }

    void on_event(const third_event &e) {
        if (e.text == valid_string) {
            ++strings_found;
        }
    }

    gr8::event_service *srv_;
    gr8::subscription token;
    std::string valid_string;
};

TEST(event_service, unsubscribe) {
    gr8::event_service event_service;

    {
        best_class bc{&event_service, "hello"};
        EXPECT_EQ(0, strings_found);

        event_service.dispatch(third_event{"test"});
        EXPECT_EQ(0, strings_found);

        event_service.dispatch(third_event{"hello"});
        EXPECT_EQ(1, strings_found);
    }

    event_service.dispatch(third_event{"hello"});
    EXPECT_EQ(1, strings_found);
}

} // namespace
