#ifndef CHESS_RENDERER_H
#define CHESS_RENDERER_H
#include "raylib.h"
#include "../../Common/Types.h"
#include "../GameLogic/GameLogic.h"
#include "../../Common/BitboardUtility/BitboardUtility.h"
#include "cmath"
#include "../../Engine/Algorithm/UCI/UCI.h"

struct BitboardEntry {
    std::string name;
    bool visualised = false;
    bitboard* board = nullptr;
};

enum menuOptions {
    GAME_MENU,
    DEBUG_MENU,
    BITBOARD_MENU,
    menuOptionsCount
};

class Renderer {
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

    Texture getTexture(bitboard pieceMask);

    const float TEXTURE_SIZE = 256;

    const Color BLACKCOLOR = Color(230, 187, 149, 255);
    const Color WHITECOLOR = Color(133, 78, 33, 255);
    const Color MOVECOLOR = Color(138, 138, 138, 138);
    const Color ACTIVEPIECECOLOR = Color(250, 216, 62, 100);

    const Color BITBOARDTRUECOLOR = Color(66, 212, 245, 180);
    const Color BITBOARDFALSECOLOR = Color(17, 44, 166, 180);

    const Color MENUPRIMARYCOLOR = Color(48, 46, 43, 255);
    const Color MENUSECONDARYCOLOR = Color(26, 25, 23, 255);
    const Color MENUHOVERCOLOR = Color(38, 37, 34, 255);

    const Font basicFont = LoadFontEx("../Assets/Pieces/Font.ttf", 100, nullptr, 0);
    const Font openSansFont = LoadFontEx("../Assets/Fonts/OpenSans.ttf", 18, nullptr, 0);

    const Texture GAMEMENUTEXTURE = LoadTexture("../Assets/MenuIcons/GameIcon.png");
    const Texture DEBUGMENUTEXTURE = LoadTexture("../Assets/MenuIcons/DebugIcon.png");

    std::vector<BitboardEntry> bitboards;


    void checkInput();

    void drawBoard();
    void drawPieces();
    void drawMoves();
    void drawFEN();
    void drawBitboardMenu();
    void drawMenu();
    void drawMenuTabs();
    void drawMenuTab(int tabIndex, Color color) const;
    void drawMenuTabsHover();
    void drawMenuTabsIcons();
    void drawMouseToolTip(std::string text);

    void onClick();
    void onBoardClicked();
    void onBitboardMenuClicked();
    void onMouseDown();
    void onMouseReleased();

    void updateScreenElementsRecs();

    // Sizes+Places of elements on screen
    Rectangle boardRec = {0, 0, (float)GetScreenHeight(), (float)GetScreenHeight()};

    Rectangle menuRec = {boardRec.width, 0, (float)GetScreenWidth()-boardRec.width, (float) GetScreenHeight()};
    Rectangle menuTopBarRec = {menuRec.x, menuRec.y, menuRec.width, (float)GetScreenHeight()/12};
    Rectangle menuUsableRec = {menuRec.x, menuRec.y + menuTopBarRec.height, menuRec.width, menuRec.height-menuTopBarRec.height};
    Vector2 menuTopBarSquareSize = {(float)GetScreenHeight()/12, (float)GetScreenHeight()/12};


    int squareSize = 32;
    Board* board;
    GameLogic* gameLogic;
    UCI* uci;

    integer activePiece = 0;
    int selectedMenu = GAME_MENU;
    bool pieceActive = false;
    bool holdingPiece = false;
    Texture textureActivePiece = MISSING_TEXTURE;



public:
    explicit Renderer(Board* board, GameLogic* gameLogic, UCI* uci);
    void render();
    void drawBitBoard(bitboard bitBoard);

    std::string FEN;
};


#endif //CHESS_RENDERER_H
