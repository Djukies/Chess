#include "Debug.h"

Debug::Debug(Renderer* renderer, Board* board) : renderer(renderer), board(board) {

}

void Debug::debugAll() {
    if (IsKeyDown(KEY_LEFT_SHIFT)) {
        this->showBitBoards();
    }
}

void Debug::showBitBoards() {
    int x = intToX(board->whiteKingPos);
    int y = intToY(board->whiteKingPos);

    bit_board kingY = (uint64_t )0xFF << (y*8);
    bit_board kingX = 0x0101010101010101ULL << x;

    bit_board leftSide = ((1ULL << x) - 1) * 0x0101010101010101ULL;
    bit_board rightSide = ~(((1ULL << (x+1)) - 1) * 0x0101010101010101ULL);
    bit_board upSide = y < 7 ? UINT64_MAX << (8 * (y+1)) : 0ULL;
    bit_board downSide =  y > 0 ? UINT64_MAX >> (8 * (8 - y)) : 0ULL;

    bit_board kingDiagonalTopLeft = x>y ? 0x8040201008040201 >> ((x-y)*8) : 0x8040201008040201 << ((y-x)*8);
    bit_board kingDiagonalTopRight = (7-x) > y ? 0x0102040810204080 >> (((7-x)-y)*8) : 0x0102040810204080 << ((y-(7-x))*8);

    bit_board left = kingY & leftSide;
    bit_board right = kingY & rightSide;
    bit_board up = kingX & upSide;
    bit_board down = kingX & downSide;

    bit_board topLeft = kingDiagonalTopLeft & leftSide;
    bit_board topRight = kingDiagonalTopRight & rightSide;
    bit_board bottomLeft = kingDiagonalTopRight & leftSide;
    bit_board bottomRight = kingDiagonalTopLeft & rightSide;

    bit_board straights = left | right | up | down;
    bit_board diagonals = topLeft | topRight | bottomLeft | bottomRight;
    bit_board all = left | right | up | down | topLeft | topRight | bottomLeft | bottomRight;

    bit_board m = 0ULL;

    int z = 7-x;
    if (z >= y) {
        m |= 0x0102040810204080 >> ((z - y) << 3);
    } else {
        m |= 0x0102040810204080 << ((y - z) << 3);
    }
    this->bitBoards = {0ULL,
                       diagonals, straights, all,
                       board->whitePieces|board->blackPieces, board->whitePieces, board->blackPieces, board->pawns, board->knights, board->bishops, board->rooks, board->queens, board->kings};
    if (IsKeyPressed(this->showNextBitboard)) this->currentBitBoard++;
    if (IsKeyPressed(this->showPreviousBitBoard)) this->currentBitBoard--;

    int bitBoardIndex = this->currentBitBoard % (int)this->bitBoards.size();
    if (bitBoardIndex == 0) return;
    renderer->drawBitBoard(this->bitBoards[bitBoardIndex]);
}