#include "Debug.h"

Debug::Debug(Renderer* renderer, Board* board, GameLogic* gameLogic) : renderer(renderer), board(board), gameLogic(gameLogic) {

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

    bit_board kingYP1 = kingY << 8;
    bit_board kingYM1 = kingY >> 8;

    bit_board kingXP1 = (kingX << 1) & notFileA;
    bit_board kingXM1 = (kingX >> 1) & notFileH;

    bit_board kingMoves = (kingYM1|kingY|kingYP1) & (kingXM1|kingX|kingXP1) & ~(1ULL << board->whiteKingPos);


    this->bitBoards = {0ULL,
                       kingMoves,
                       kingYM1|kingY|kingYP1,
                       kingXM1|kingX|kingXP1,
                       diagonals, straights, all,
                       board->whitePieces|board->blackPieces, board->whitePieces, board->blackPieces, board->pawns, board->knights, board->bishops, board->rooks, board->queens, board->kings};
    if (IsKeyPressed(this->showNextBitboard)) this->currentBitBoard++;
    if (IsKeyPressed(this->showPreviousBitBoard)) this->currentBitBoard--;

    int bitBoardIndex = this->currentBitBoard % (int)this->bitBoards.size();
    if (bitBoardIndex == 0) return;
    renderer->drawBitBoard(this->bitBoards[bitBoardIndex]);
}

double moveCount = 0;
int captures = 0;
int enPassants = 0;
void MoveGenerationTest(Board* board, const int depth, GameLogic* gameLogic)
{

    if (depth == 0)
    {
        return;
    }
    for (const std::map<integer, std::vector<Move>> moveVector = gameLogic->calculateMoves();
            const auto &[fst, moves] : moveVector)
    {
        if (depth == 1) moveCount += (double)moves.size();
        for (const Move move : moves)
        {
            if (depth == 1) {
                if ((move.move_type == CAPTURE || move.move_type == CAPTURE_PROMOTION || move.move_type == EN_PASSANT)) {
                    captures++;
                    if (move.move_type == EN_PASSANT) {
                        enPassants++;
                    }
                }
                continue;
            }
            makeMove(board, move);
            MoveGenerationTest(board, depth - 1,  gameLogic);
            unmakeMove(board, move);
        }
    }
}

void Debug::moveGenTest(int depth) {
    for (int checkDepth = 0; checkDepth <= depth; checkDepth++) {
        moveCount = 0;
        captures = 0;
        enPassants = 0;
        const auto start = std::chrono::high_resolution_clock::now();
        MoveGenerationTest(board, checkDepth, gameLogic);
        const auto end = std::chrono::high_resolution_clock::now();
        const auto time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        std::cout << "Depth: " << checkDepth << ", Moves: " << std::fixed << std::setprecision(0) << moveCount << ", Time taken: " << time.count() << "ms" << " (Captures: " << captures << ", En Passants: " << enPassants << ")" << std::endl;    }
}