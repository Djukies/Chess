#ifndef CHESS_RENDERGAME_H
#define CHESS_RENDERGAME_H
#include <vector>

#include "raylib.h"

#include "../RenderConstants.h"
#include "Common/Board/Board.h"
#include "Common/Types.h"

class GameRenderer {
    private:


    const Texture MISSING_TEXTURE = LoadTexture("../Assets/Pieces/missing_texture.png");

    const Texture WHITE_PAWN = LoadTexture("../Assets/Pieces/white_pawn.png");
    const Texture WHITE_KNIGHT = LoadTexture("../Assets/Pieces/white_knight.png");
    const Texture WHITE_BISHOP = LoadTexture("../Assets/Pieces/white_bishop.png");
    const Texture WHITE_ROOK = LoadTexture("../Assets/Pieces/white_rook.png");
    const Texture WHITE_QUEEN = LoadTexture("../Assets/Pieces/white_queen.png");
    const Texture WHITE_KING = LoadTexture("../Assets/Pieces/white_king.png");

    const Texture BLACK_PAWN = LoadTexture("../Assets/Pieces/black_pawn.png");
    const Texture BLACK_KNIGHT = LoadTexture("../Assets/Pieces/black_knight.png");
    const Texture BLACK_BISHOP = LoadTexture("../Assets/Pieces/black_bishop.png");
    const Texture BLACK_ROOK = LoadTexture("../Assets/Pieces/black_rook.png");
    const Texture BLACK_QUEEN = LoadTexture("../Assets/Pieces/black_queen.png");
    const Texture BLACK_KING = LoadTexture("../Assets/Pieces/black_king.png");

    Board* board;

    float squareSize;



    void drawSquares() const;
    void drawPieces();

    void drawDraggedPiece();

    void drawMove(integer pos, bool isCapture, bool isMouseHoveringMove) const;

    void drawMoves(integer piece);

    void drawCastleMoves(integer pos);

    Texture getTexture(Piece pieceType, bool pieceColor);

public:
    GameRenderer(Board* board, Vector2 position, Vector2 size);
    ~GameRenderer();

    Vector2 position;
    Vector2 size;

    bool isPieceActive = false;
    integer activePiece = 0;
    bool isDraggingPiece = false;

    void renderBoard();

    void updateBoardPosition(Vector2 newPosition);
    void updateBoardSize(Vector2 newSize);
};



#endif //CHESS_RENDERGAME_H
