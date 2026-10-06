#include <cmath>
#include <iostream>
#include "Renderer.h"

Renderer::Renderer(Board* board, GameLogic* gameLogic, UCI* uci) : board(board), gameLogic(gameLogic), uci(uci) {
    SetTextureFilter(basicFont.texture, TEXTURE_FILTER_BILINEAR);
    BitboardEntry whitepieces = {"White Pieces", false, &board->whitePieces};
    BitboardEntry blackPieces = {"Black Pieces", false, &board->blackPieces};
    BitboardEntry pawns = {"Pawns", false, &board->pawns};
    BitboardEntry knights = {"Knights", false, &board->knights};
    BitboardEntry bishops = {"Bishops", false, &board->bishops};
    BitboardEntry rooks = {"Rooks", false, &board->rooks};
    BitboardEntry queens = {"Queens", false, &board->queens};
    BitboardEntry kings = {"Kings", false, &board->kings};
    BitboardEntry attackSquares = {"Attack Squares", false, &board->opponentControlledSquares};
    BitboardEntry moves = {"Moves", false, &board->movesBitboard};
    BitboardEntry pawnMoves = {"Pawn Moves", false, &board->pawnMoves};
    BitboardEntry knightMoves = {"Knight Moves", false, &board->knightMoves};
    BitboardEntry bishopMoves = {"Bishop Moves", false, &board->bishopMoves};
    BitboardEntry rookMoves = {"Rook Moves", false, &board->rookMoves};
    BitboardEntry queenMoves = {"Queen Moves", false, &board->queenMoves};
    BitboardEntry kingMoves = {"King Moves", false, &board->kingMoves};
    bitboards = {whitepieces, blackPieces, pawns, knights, bishops, rooks, queens, kings, attackSquares, moves, pawnMoves, knightMoves, bishopMoves, rookMoves, queenMoves, kingMoves};
}

void Renderer::render() {
    this->updateScreenElementsRecs();
    ClearBackground(MENUSECONDARYCOLOR);
    squareSize = std::min(GetScreenHeight(), GetScreenWidth()) / 8;

    this->drawBoard();
    this->drawFEN();
    this->drawMenu();
    //this->drawBitboardMenu();
    if (pieceActive) {
        this->drawMoves();
        DrawRectangleV(intToVector2ScreenPos(activePiece, squareSize), {(float)squareSize, (float)squareSize}, ACTIVEPIECECOLOR);
    }
    this->drawPieces();

    this->checkInput();
    bitboard colorpiecesToDraw = 0ULL;
    for (int i = 0; i < 2; i++) {
        if (bitboards.at(i).visualised) {
            colorpiecesToDraw |= *bitboards.at(i).board;
        }
    }
    bitboard piecesToDraw = 0ULL;
    for (int i = 2; i < 8; i++) {
        if (bitboards.at(i).visualised) {
            piecesToDraw |= *bitboards.at(i).board;
        }
    }
    bitboard attackSquares = 0ULL;
    if (bitboards.at(8).visualised) {
        attackSquares = *bitboards.at(8).board;
    }
    bitboard movesToDraw = 0ULL;
    for (int i = 9; i < 16; i++) {
        if (bitboards.at(i).visualised) {
            movesToDraw |= *bitboards.at(i).board;
        }
    }
    if ((colorpiecesToDraw & piecesToDraw) | attackSquares | movesToDraw != 0) {
        this->drawBitBoard((colorpiecesToDraw & piecesToDraw) | attackSquares | movesToDraw);
    }

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
        for (Move move : board->movesMap[activePiece]) {
            int newPos = (move & targetSquareMask) >> 6;
            if ((move & flagMask) >> 12 >= 5) {
                continue; // Prevent all the other promotions except queen promotion from drawing to avoid duplicate drawings
            }
            if ((move & flagMask) >> 12 == CastleFlag) {
                bool castleKingSide = newPos % 8 == 6;
                int drawCircleAt = castleKingSide ? board->friendlyKingPos + 2 : board->friendlyKingPos - 2;
                bool shouldHaveSmallCircle = containsSquare(board->emptySquares, drawCircleAt);
                if (vector2ScreenToInt(GetMousePosition(), squareSize) != drawCircleAt) {
                    DrawRing({intToVector2ScreenPos(drawCircleAt, squareSize).x + (float)squareSize / 2,
                              intToVector2ScreenPos(drawCircleAt, squareSize).y + (float)squareSize / 2},
                             shouldHaveSmallCircle ? 0 : 0.4f * (float)squareSize, shouldHaveSmallCircle ? 0.2f * (float)squareSize : 0.5f * (float)squareSize,
                             0, 360, 0, MOVECOLOR);
                }
                else {
                    DrawCircle((int)intToVector2ScreenPos(drawCircleAt, squareSize).x + squareSize / 2,
                               (int)intToVector2ScreenPos(drawCircleAt, squareSize).y + squareSize / 2,
                               shouldHaveSmallCircle ? 0.25f * (float)squareSize : 0.5f * (float)squareSize, MOVECOLOR);
                }
                continue;
            }
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

    bitboard pieces = board->whitePieces | board->blackPieces;
    while (pieces) {
        int i = __builtin_ctzll(pieces); // Find index of the least significant set bit
        if (holdingPiece && i == activePiece) {
            pieces &= pieces - 1; // Remove the least significant set bit
            continue;
        }
        bitboard mask = 1ULL << i;
        const Texture texture = getTexture(mask);

        DrawTextureEx(texture, intToVector2ScreenPos(i, squareSize), 0, scale, WHITE);
        pieces &= pieces - 1; // Remove the least significant set bit
    }
}

Texture Renderer::getTexture(bitboard pieceMask) {
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



void Renderer::drawBitBoard(bitboard bitBoard) {
    for (int i = 0; i < 64; i++) {
        bitboard placeI = 1ULL << i;
        placeI &= bitBoard;
        DrawRectangleV(intToVector2ScreenPos(i, squareSize), Vector2((float)squareSize, (float)squareSize), placeI ? BITBOARDTRUECOLOR : BITBOARDFALSECOLOR);
    }
}

void Renderer::drawFEN() {
    DrawTextEx(basicFont, FEN.c_str(), {(float)9 * squareSize, (float)1 * squareSize}, 35, 3, BLACK);
}

void Renderer::drawBitboardMenu() {

    for (int i = 0; i < bitboards.size(); i++) {
        std::string bitboardName = bitboards.at(i).name;
        DrawRectangleRounded({.x = (float)squareSize * 8 + 10, .y = 10 + (float)45*i, .width = 200, .height = 39}, 0.5f, 10, bitboards.at(i).visualised ? GREEN : RED);
        DrawTextEx(basicFont, bitboardName.c_str(), {(float)8 * squareSize + 15, (float)15 + (float)45*i}, 30, 3, BLACK);
    }
}

void Renderer::drawMenu() {
    drawMenuTabs();

    DrawRectangleRec(menuUsableRec, MENUPRIMARYCOLOR);
}

void Renderer::drawMenuTab(const int tabIndex, const Color color) const {
    DrawRectangleRounded({
        .x = menuTopBarRec.x + (menuTopBarSquareSize.x*static_cast<float>(tabIndex)),
        .y = menuTopBarRec.y + 1,
        .width = menuTopBarSquareSize.x,
        .height = menuTopBarSquareSize.y + 10 /*a little extra so the roundness dissapears into the main menu*/},
        0.1, 0, color);

}

void Renderer::drawMenuTabsHover() {
    // Check if mouse is hovering over the top bar
    if (CheckCollisionPointRec(GetMousePosition(), menuTopBarRec)) {
        // Get relativeMouseX and calculate which menu buttons is being hovered
        const float relativeMouseX = GetMousePosition().x - menuTopBarRec.x;
        const int mouseSquare = std::floor(relativeMouseX / menuTopBarSquareSize.x);
        if (mouseSquare >=  menuOptionsCount) {
            return;
        }
        drawMenuTab(mouseSquare, MENUHOVERCOLOR);
    }
}

void Renderer::drawMenuTabsIcons() {
    DrawTextureEx(GAMEMENUTEXTURE, {.x = menuTopBarRec.x + (menuTopBarSquareSize.x*0), .y = menuTopBarRec.y}, 0, menuTopBarSquareSize.y/(float)GAMEMENUTEXTURE.height, WHITE);
    DrawTextureEx(DEBUGMENUTEXTURE, {.x = menuTopBarRec.x + (menuTopBarSquareSize.x*1), .y = menuTopBarRec.y}, 0, menuTopBarSquareSize.y/(float)DEBUGMENUTEXTURE.height, WHITE);

}

void Renderer::drawMenuTabs() {
    drawMenuTabsHover(); // Draw the menu tab the mouse is hovering over
    drawMenuTab(selectedMenu, MENUPRIMARYCOLOR); // Draw the menu tab currently selected
    drawMenuTabsIcons();
}

void Renderer::drawMouseToolTip(std::string text) {
    Vector2 mousePos = GetMousePosition();
    Vector2 textSize = MeasureTextEx(openSansFont, text.c_str(), 18, 1);
    DrawRectangleRec({mousePos.x + 10, mousePos.y + 10, textSize.x+4, textSize.y + 4}, WHITE);
    DrawRectangleLinesEx({mousePos.x + 10, mousePos.y + 10, textSize.x+4, textSize.y + 4}, 1, BLACK);
    DrawTextEx(openSansFont, text.c_str(), {mousePos.x + 11, mousePos.y + 11}, 18, 1, BLACK);
}

void Renderer::checkInput() {
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        onClick();
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
        onMouseDown();
    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
        onMouseReleased();
    if (IsKeyPressed(KEY_D)) {
        if (board->checkMate || board->staleMate) {
            return;
        }
        if (board->whiteToMove && board->algorithmWhite) {
            std::string movesString = " moves";
            for (Move mademove : board->movesMade) {
                movesString += " " + moveToString(mademove);
            }
            if (movesString == " moves") {movesString = "";}
            // uci->sendToEngine("position fen " + board->startingFen + movesString);
            // uci->sendToEngine("go depth 4");
        }
        else if ((!board->whiteToMove) && board->algorithmBlack) {
            std::string movesString = " moves";
            for (Move mademove : board->movesMade) {
                movesString += " " + moveToString(mademove);
            }
            if (movesString == " moves") {movesString = "";}
            // uci->sendToEngine("position fen " + board->startingFen + movesString);
            // uci->sendToEngine("go depth 4");
        }
    }
}

void Renderer::onClick() {
    if (GetMousePosition().x <= squareSize * 8 && GetMousePosition().y <= squareSize * 8) {
        onBoardClicked();
    }
    if (GetMousePosition().y >= 0 && GetMousePosition().y < (float)squareSize*0.75f
        && GetMousePosition().x > (float)squareSize*8) {
        int mouseXinMenu = (int)GetMousePosition().x - squareSize*8;
        int mouseSquare = mouseXinMenu / (int)(squareSize*0.75);
        selectedMenu = mouseSquare;
        }
    if (GetMousePosition().x >= squareSize * 8 + 10 && GetMousePosition().x <= squareSize * 8 + 210
        && GetMousePosition().y >= 10 && GetMousePosition().y <= 49 + (bitboards.size()-1)*45) {
        onBitboardMenuClicked();
    }
}

void Renderer::onBoardClicked() {
    // Create Variable and bitboard mask for the clicked Square
    integer clickedSquare = vector2ScreenToInt(GetMousePosition(), squareSize);
    bitboard clickMask = 1ULL << clickedSquare;
    // Check if there already is an active piece
    if (pieceActive) {
        pieceActive = false;
        if (!tryMove(board, activePiece, clickedSquare)) { // Check if you can make a Move to the clicked square (if not do next code (also you can't if it is its own square))
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

void Renderer::onBitboardMenuClicked() {
    int i = (GetMousePosition().y-10)/45;
    // Check collision with bitboardmenu at index i
    if (CheckCollisionPointRec(GetMousePosition(), {.x = (float)squareSize * 8 + 10, .y = 10 + (float)45*i, .width = 200, .height = 39})) {
        bitboards.at(i).visualised = !bitboards.at(i).visualised;
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
    int newPos = vector2ScreenToInt(GetMousePosition(), squareSize);
    if (pieceActive) {
        tryMove(board, activePiece, newPos);
    }
    holdingPiece = false;
}

void Renderer::updateScreenElementsRecs() {
    boardRec = {0, 0, (float)GetScreenHeight(), (float)GetScreenHeight()};

    menuRec = {boardRec.width, 0, (float)GetScreenWidth()-boardRec.width, (float) GetScreenHeight()};
    menuTopBarRec = {menuRec.x, menuRec.y, menuRec.width, (float)GetScreenHeight()/12};
    menuUsableRec = {menuRec.x, menuRec.y + menuTopBarRec.height, menuRec.width, menuRec.height-menuTopBarRec.height};
    menuTopBarSquareSize = {(float)GetScreenHeight()/12, (float)GetScreenHeight()/12};
}
