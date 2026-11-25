#include "GameRenderer.h"

#include "GUI/Renderer/Renderer.h"

GameRenderer::GameRenderer(Board* board, const Vector2 position, const Vector2 size) : board(board), position(position), size(size) {
    squareSize = std::min(size.x, size.y)/8;
}

GameRenderer::~GameRenderer() {
    
}

void GameRenderer::updateBoardPosition(Vector2 newPosition) {
    position = newPosition;
}

void GameRenderer::updateBoardSize(Vector2 newSize) {
    size = newSize;
    squareSize = std::min(size.x, size.y)/8;
}

void GameRenderer::renderBoard() {
    drawSquares();
    if (isPieceActive) {
        if (containsSquare(board->friendlyPieces, activePiece)) { // Only draw moves when it is your own piece
            drawMoves(activePiece); // Draw the possible moves of the active piece
        }
        DrawRectangleV(intToVector2ScreenPos(activePiece, squareSize), {squareSize, squareSize}, ACTIVEPIECECOLOR); // Draw square to make visible which piece is active
    }
    drawPieces();
}



void GameRenderer::drawSquares() const {
    for (int x = 0; x < 8; x++) {
        for (int y = 0; y < 8; y++) {
            DrawRectangle(position.x + x*squareSize, position.y + y*squareSize, squareSize, squareSize, (x+y)%2 == 0 ? BLACKCOLOR : WHITECOLOR);
        }
    }
}

Texture GameRenderer::getTexture(Piece pieceType, bool pieceColor) {
    if (pieceColor == TEAMWHITE) {
        switch (pieceType) {
            case PAWN: return WHITE_PAWN;
            case KNIGHT: return WHITE_KNIGHT;
            case BISHOP: return WHITE_BISHOP;
            case ROOK: return WHITE_ROOK;
            case QUEEN: return WHITE_QUEEN;
            case KING: return WHITE_KING;
            default: return MISSING_TEXTURE;
        }
    } else {
        switch (pieceType) {
            case PAWN: return BLACK_PAWN;
            case KNIGHT: return BLACK_KNIGHT;
            case BISHOP: return BLACK_BISHOP;
            case ROOK: return BLACK_ROOK;
            case QUEEN: return BLACK_QUEEN;
            case KING: return BLACK_KING;
            default: return MISSING_TEXTURE;
      }
    }
}

void GameRenderer::drawPieces() {
    const float scale = squareSize/TEXTURE_SIZE;

    bitboard pieces = board->whitePieces | board->blackPieces; // Get all Pieces
    while (pieces) {    // Loop through all pieces
        int i = __builtin_ctzll(pieces); // Find index of the least significant set bit
        if (isDraggingPiece && activePiece == i) { // if mouse is dragging the piece
            drawDraggedPiece();
            pieces &= pieces - 1; // Remove the least significant set bit
            continue;
        }
        const Texture texture = getTexture(getPieceType(board, i), getPieceColor(board, i));

        DrawTextureEx(texture, {position.x + intToX(i) * squareSize, position.y + intToY(i) * squareSize}, 0, scale, WHITE); // Draw the texture at right scale and position
        pieces &= pieces - 1; // Remove the least significant set bit
    }
}

void GameRenderer::drawDraggedPiece() {
    const Texture texture = getTexture(getPieceType(board, activePiece), getPieceColor(board, activePiece));
    const float scale = squareSize/TEXTURE_SIZE;

    DrawTextureEx(  texture,
                    {GetMousePosition().x - squareSize/2, GetMousePosition().y - squareSize/2},
                    0,
                    scale * 1.03f,
                    WHITE);
}


void GameRenderer::drawMoves(integer piece) {
    for (Move move : board->movesMap[piece]) { // Loop through all moves of piece
        int newPos = getTargetsquare(move);
        if (getFlag(move) > PromoteToQueenFlag) {
            continue; // Prevent all the other promotions except queen promotion from drawing to avoid duplicate drawings
        }
        if (getFlag(move) == CastleFlag) {
            drawCastleMoves(newPos);
            continue;
        }

        bool isCapture = !containsSquare(board->emptySquares, newPos);
        bool isMouseHoveringMove = vector2ScreenToInt(GetMousePosition(), squareSize) == newPos;
        drawMove(newPos, isCapture, isMouseHoveringMove);
    }
}

void GameRenderer::drawCastleMoves(integer pos) {
    bool castleKingSide = pos % 8 == 6;
    int drawPos = castleKingSide ? board->friendlyKingPos + 2 : board->friendlyKingPos - 2;
    bool isCapture = !containsSquare(board->emptySquares, drawPos);
    bool isMouseHoveringMove = vector2ScreenToInt(GetMousePosition(), squareSize) == drawPos;

    drawMove(drawPos, isCapture, isMouseHoveringMove);
}

void GameRenderer::drawMove(integer pos, bool isCapture, bool isMouseHoveringMove) const {
    // Center of the move circle should be center of the square
    Vector2 center = {  position.x + intToVector2ScreenPos(pos, squareSize).x + (float)squareSize / 2,
                        position.y + intToVector2ScreenPos(pos, squareSize).y + (float)squareSize / 2};

    if (isCapture) {
        float innerRadius = 0.4f * squareSize;
        if (isMouseHoveringMove) {
            innerRadius = 0;
        }
        DrawRing(
            center,
            innerRadius, 0.5f * squareSize,
            0, 360, 0, MOVECOLOR
        );
    } else {
        float radius = 0.2f * squareSize;
        if (isMouseHoveringMove) {
            radius = 0.25f * squareSize;
        }
        DrawCircleV(center, radius, MOVECOLOR);
    }
}