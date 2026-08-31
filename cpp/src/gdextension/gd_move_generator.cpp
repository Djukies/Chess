#include "gd_move_generator.h"
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

void GDMoveGenerator::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_legal_moves"), &GDMoveGenerator::get_legal_moves);
    ClassDB::bind_method(D_METHOD("set_legal_moves", "p_legal_moves"), &GDMoveGenerator::set_legal_moves);

    ClassDB::bind_method(D_METHOD("generate_moves", "piece"), &GDMoveGenerator::generate_moves);

    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "legal moves"), "set_legal_moves", "get_legal_moves");
}

GDMoveGenerator::GDMoveGenerator() {
    // Initialize any variables here.
    legal_moves = true;
}

GDMoveGenerator::~GDMoveGenerator() {
    // Add your cleanup here.
}

void GDMoveGenerator::_process(double delta) {
}

PackedInt32Array GDMoveGenerator::generate_moves(int piece) {
    PackedInt32Array moves = {};
    moves.append(32);
    moves.append(34);
    return moves;
}

void GDMoveGenerator::set_legal_moves(const bool p_legal_moves) {
    legal_moves = p_legal_moves;
}

bool GDMoveGenerator::get_legal_moves() const {
    return legal_moves;
}
