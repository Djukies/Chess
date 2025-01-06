#include <iostream>
#include "Renderer.h"

Renderer::Renderer(Board* board, GameLogic* gameLogic) : board(board), gameLogic(gameLogic) {}

void Renderer::render() {
    ClearBackground(BLACK);
    squareSize = std::min(GetScreenHeight(), GetScreenWidth()) / 8;



    this->drawBoard();
    if (pieceActive) {
        this->drawMoves();
        DrawRectangleV(intToVector2ScreenPos(activePiece, squareSize), {(float)squareSize, (float)squareSize}, ACTIVEPIECECOLOR);
    }
    this->drawPieces();

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        onClick();
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
        onMouseDown();
    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
        onMouseReleased();
}

void Renderer::drawBoard() {
    for (int x = 0; x < 8; x++) {
        for (int y = 0; y < 8; y++) {
            DrawRectangle(x*squareSize, y*squareSize, squareSize, squareSize, (x+y)%2 == 0 ? BLACKCOLOR : WHITECOLOR);
        }
    }
}

void Renderer::drawMoves() {
    if (containsSquare(board->friendlyPieces, activePiece)) {


        for (Move Move : board->movesMap[activePiece]) {
            int newPos = (Move & targetSquareMask) >> 6;
            bool shouldHaveSmallCircle = containsSquare(board->emptySquares, newPos);
            if (vector2ScreenToInt(GetMousePosition(), squareSize) != newPos) {
                DrawRing({intToVector2ScreenPos(newPos, squareSize).x + (float)squareSize / 2,
                          intToVector2ScreenPos(newPos, squareSize).y + (float)squareSize / 2},
                         shouldHaveSmallCircle ? 0 : 0.4f * (float)squareSize, shouldHaveSmallCircle ? 0.2f * (float)squareSize : 0.5f * (float)squareSize,
                         0, 360, 0, MOVECOLOR);
            }
            else {
                DrawCircle((int)intToVector2ScreenPos(newPos, squareSize).x + squareSize / 2,
                           (int)intToVector2ScreenPos(newPos, squareSize).y + squareSize / 2,
                           shouldHaveSmallCircle ? 0.25f * (float)squareSize : 0.5f * (float)squareSize, MOVECOLOR);
            }
        }
    }
}

void Renderer::drawPieces() {
    float scale = (float)squareSize/TEXTURE_SIZE;

    bit_board pieces = board->whitePieces | board->blackPieces;
    while (pieces) {
        int i = __builtin_ctzll(pieces); // Find index of the least significant set bit
        if (holdingPiece && i == activePiece) {
            pieces &= pieces - 1; // Remove the least significant set bit
            continue;
        }
        bit_board mask = 1ULL << i;
        const Texture texture = getTexture(mask);

        DrawTextureEx(texture, intToVector2ScreenPos(i, squareSize), 0, scale, WHITE);
        pieces &= pieces - 1; // Remove the least significant set bit
    }
}

Texture Renderer::getTexture(bit_board pieceMask) {
    if (pieceMask & board->whitePieces) {
        if (pieceMask & board->pawns) return WHITE_PAWN;
        else if (pieceMask & board->knights) return WHITE_KNIGHT;
        else if (pieceMask & board->bishops) return WHITE_BISHOP;
        else if (pieceMask & board->rooks) return WHITE_ROOK;
        else if (pieceMask & board->queens) return WHITE_QUEEN;
        else if (pieceMask & board->kings) return WHITE_KING;
        else return MISSING_TEXTURE;
    }
    else if (pieceMask & board->blackPieces) {
        if (pieceMask & board->pawns) return BLACK_PAWN;
        else if (pieceMask & board->knights) return BLACK_KNIGHT;
        else if (pieceMask & board->bishops) return BLACK_BISHOP;
        else if (pieceMask & board->rooks) return BLACK_ROOK;
        else if (pieceMask & board->queens) return BLACK_QUEEN;
        else if (pieceMask & board->kings) return BLACK_KING;
        else return MISSING_TEXTURE;
    } else return MISSING_TEXTURE;
}



void Renderer::drawBitBoard(bit_board bitBoard) {
    for (int i = 0; i < 64; i++) {
        bit_board placeI = 1ULL << i;
        placeI &= bitBoard;
        DrawRectangleV(intToVector2ScreenPos(i, squareSize), Vector2((float)squareSize, (float)squareSize), placeI ? BITBOARDTRUECOLOR : BITBOARDFALSECOLOR);
    }
}



void Renderer::onClick() {
    // Create Variable and bitboard mask for the clicked Square
    integer clickedSquare = vector2ScreenToInt(GetMousePosition(), squareSize);
    bit_board clickMask = 1ULL << clickedSquare;
    // Check if there already is an active piece
    if (pieceActive) {
        pieceActive = false;
        if (!gameLogic->tryMove(activePiece, clickedSquare)) { // Check if you can make a Move to the clicked square (if not do next code (also you can't if it is its own square))
            if (clickMask & (board->whitePieces | board->blackPieces)) { // If clicked on a piece (not air)
                activePiece = clickedSquare;
                pieceActive = true;
            }
        }
    } else {
        if (clickMask & (board->whitePieces | board->blackPieces)) { // If clicked on a piece (not air) Make active piece clicked Square
            pieceActive = true;
            activePiece = clickedSquare;
        } else { // Else: No active Piece
            pieceActive = false;
        }
    }
    if (pieceActive) { // Check if the there is now an active piece and assign the right texture
        textureActivePiece = getTexture(1ULL << activePiece);
    }
}

void Renderer::onMouseDown() {
    if (pieceActive) {
        holdingPiece = true;
        float scale = (float) squareSize / TEXTURE_SIZE;
        // Draw the piece at the mouse, with slightly bigger scale
        DrawTextureEx(textureActivePiece,
                      {GetMousePosition().x - (float)squareSize/2, GetMousePosition().y - (float)squareSize/2}, 0,
                      scale * 1.03f, WHITE);
        return;
    }
    holdingPiece = false;
}

void Renderer::onMouseReleased() {
    // Try to make the Move
    if (pieceActive) {
        gameLogic->tryMove(activePiece, vector2ScreenToInt(GetMousePosition(), squareSize));
    }
    holdingPiece = false;
}
