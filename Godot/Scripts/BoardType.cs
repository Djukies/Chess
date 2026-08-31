using System;

namespace ChessUI.Scripts;

public struct BoardType
{
    UInt64 WhitePiecesBitboard;
    UInt64 BlackPiecesBitboard;
    UInt64 PawnsBitboard;
    UInt64 KnightsBitboard;
    UInt64 BishopsBitboard;
    UInt64 RooksBitboard;
    UInt64 QueensBitboard;
    UInt64 KingsBitboard;
}