/**
 * pass
 */

#pragma once

#include "had/had.h"
#include <functional>
#include <vector>



class SwitchPanel {

    int count = 0;
    int cur = 0;

    struct Component {
        std::function<had::Res(void)> draw;
        std::function<had::Res(void)> resize;
    };
    std::vector<Component> component_list;

    public:
        SwitchPanel();
        ~SwitchPanel();

        class ComponentId {
            int id;
            ComponentId(int id) : id(id) {}
            friend SwitchPanel;
            static constexpr int INVALID_ID = -1;
            public:
                ComponentId() : id(INVALID_ID) {}
        };

        ComponentId add_component(
            std::function<had::Res(void)> draw,
            std::function<had::Res(void)> resize
        );

        bool compare_id(ComponentId id) {
            return cur == id.id;
        }

        void inc();
        void dec();
        void set(int ch);

        had::Res draw();
        had::Res resize();

        template <typename Ret, typename... Args>
        Ret call_wrapper(
            ComponentId id,
            std::function<Ret(Args... args)> func,
            Ret default_ret,
            Args... args
        );

        void call_wrapper(
            ComponentId id,
            std::function<void(void)> func
        );
};
