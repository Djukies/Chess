#ifndef CHESS_RENDERER_H
#define CHESS_RENDERER_H
#include "raylib.h"
#include "../Types.h"
#include "../GameLogic/GameLogic.h"
#include "../GameLogic/BitboardUtility/BitboardUtility.h"
#include "cmath"
#include "../Bot/UCI/UCI.h"


class Renderer {
private:
    const Texture MISSING_TEXTURE = LoadTexture("../Pieces/missing_texture.png");

    const Texture WHITE_PAWN = LoadTexture("../Pieces/white_pawn.png");
    const Texture WHITE_KNIGHT = LoadTexture("../Pieces/white_knight.png");
    const Texture WHITE_BISHOP = LoadTexture("../Pieces/white_bishop.png");
    const Texture WHITE_ROOK = LoadTexture("../Pieces/white_rook.png");
    const Texture WHITE_QUEEN = LoadTexture("../Pieces/white_queen.png");
    const Texture WHITE_KING = LoadTexture("../Pieces/white_king.png");

    const Texture BLACK_PAWN = LoadTexture("../Pieces/black_pawn.png");
    const Texture BLACK_KNIGHT = LoadTexture("../Pieces/black_knight.png");
    const Texture BLACK_BISHOP = LoadTexture("../Pieces/black_bishop.png");
    const Texture BLACK_ROOK = LoadTexture("../Pieces/black_rook.png");
    const Texture BLACK_QUEEN = LoadTexture("../Pieces/black_queen.png");
    const Texture BLACK_KING = LoadTexture("../Pieces/black_king.png");

    Texture getTexture(bit_board pieceMask);

    const float TEXTURE_SIZE = 256;

    const Color BLACKCOLOR = Color(230, 187, 149, 255);
    const Color WHITECOLOR = Color(133, 78, 33, 255);
    const Color MOVECOLOR = Color(138, 138, 138, 138);
    const Color ACTIVEPIECECOLOR = Color(250, 216, 62, 100);

    const Color BITBOARDTRUECOLOR = Color(66, 212, 245, 180);
    const Color BITBOARDFALSECOLOR = Color(17, 44, 166, 180);

    void drawBoard();
    void drawPieces();
    void drawMoves();

    void onClick();
    void onMouseDown();
    void onMouseReleased();

    int squareSize = 32;
    Board* board;
    GameLogic* gameLogic;
    UCI* uci;

    integer activePiece = 0;
    bool pieceActive = false;
    bool holdingPiece = false;
    Texture textureActivePiece = MISSING_TEXTURE;



public:
    explicit Renderer(Board* board, GameLogic* gameLogic, UCI* uci);
    void render();
    void drawBitBoard(bit_board bitBoard);
};


#endif //CHESS_RENDERER_H
