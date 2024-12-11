#include "Debug.h"

Debug::Debug(Renderer* renderer, Board* board) : renderer(renderer), board(board) {

}

void Debug::debugAll() {
    if (IsKeyDown(KEY_LEFT_SHIFT)) {
        this->showBitBoards();
    }
}

void Debug::showBitBoards() {
    this->bitBoards = {0ULL, board->whitePieces|board->blackPieces, board->whitePieces, board->blackPieces, board->pawns, board->knights, board->bishops, board->rooks, board->queens, board->kings};
    if (IsKeyPressed(this->showNextBitboard)) this->currentBitBoard++;
    if (IsKeyPressed(this->showPreviousBitBoard)) this->currentBitBoard--;

    int bitBoardIndex = this->currentBitBoard % (int)this->bitBoards.size();
    if (bitBoardIndex == 0) return;
    renderer->drawBitBoard(this->bitBoards[bitBoardIndex]);
}