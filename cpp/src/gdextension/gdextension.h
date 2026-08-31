#pragma once

#include <godot_cpp/classes/sprite2d.hpp>

namespace godot {

    class GDExample : public Sprite2D {
        GDCLASS(GDExample, Sprite2D);

    private:
        double time_passed;
        double time_emit;
        double amplitude;

    protected:
        static void _bind_methods();

    public:
        GDExample();
        ~GDExample();

        void _process(double delta) override;

        double get_amplitude() const;
        void set_amplitude(const double p_amplitude);
    };

} // namespace godot