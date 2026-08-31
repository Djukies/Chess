#pragma once

#include <godot_cpp/classes/node.hpp>

struct BoardType
{
    uint64_t WhitePiecesBitboard = 0;
    uint64_t BlackPiecesBitboard = 0;
    uint64_t PawnsBitboard = 0;
    uint64_t KnightsBitboard = 0;
    uint64_t BishopsBitboard = 0;
    uint64_t RooksBitboard = 0;
    uint64_t QueensBitboard = 0;
    uint64_t KingsBitboard = 0;

    godot::Dictionary to_dict() const {
        godot::Dictionary dict;
        dict["WhitePiecesBitboard"] = WhitePiecesBitboard;
        dict["BlackPiecesBitboard"] = BlackPiecesBitboard;
        dict["PawnsBitboard"] = PawnsBitboard;
        dict["KnightsBitboard"] = KnightsBitboard;
        dict["BishopsBitboard"] = BishopsBitboard;
        dict["RooksBitboard"] = RooksBitboard;
        dict["QueensBitboard"] = QueensBitboard;
        dict["KingsBitboard"] = KingsBitboard;
        return dict;
    }

    static BoardType from_dict(const godot::Dictionary &dict) {
        BoardType board;
        if (dict.has("WhitePiecesBitboard")) board.WhitePiecesBitboard = dict["WhitePiecesBitboard"];
        if (dict.has("BlackPiecesBitboard")) board.BlackPiecesBitboard = dict["BlackPiecesBitboard"];
        if (dict.has("PawnsBitboard")) board.PawnsBitboard = dict["PawnsBitboard"];
        if (dict.has("KnightsBitboard")) board.KnightsBitboard = dict["KnightsBitboard"];
        if (dict.has("BishopsBitboard")) board.BishopsBitboard = dict["BishopsBitboard"];
        if (dict.has("RooksBitboard")) board.RooksBitboard = dict["RooksBitboard"];
        if (dict.has("QueensBitboard")) board.QueensBitboard = dict["QueensBitboard"];
        if (dict.has("KingsBitboard")) board.KingsBitboard = dict["KingsBitboard"];
        return board;
    }
};

namespace godot {

    class GDMoveGenerator : public Node {
        GDCLASS(GDMoveGenerator, Node);

    private:
        bool legal_moves;

    protected:
        static void _bind_methods();

    public:
        GDMoveGenerator();
        ~GDMoveGenerator();

        void _process(double delta) override;

        PackedInt32Array generate_moves(godot::Dictionary board);

        bool get_legal_moves() const;
        void set_legal_moves(const bool p_legal_moves);
    };

} // namespace godot