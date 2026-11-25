#ifndef CHESS_MENURENDERER_H
#define CHESS_MENURENDERER_H

#include <string>

#include "raylib.h"

#include "../GameMenuRenderer/GameMenuRenderer.h"
#include "../BitboardMenuRenderer/BitboardMenuRenderer.h"
#include "../SettingsMenuRenderer/SettingsMenuRenderer.h"

enum Menu {
    GAME_MENU,
    BITBOARD_MENU,
    SETTINGS_MENU,
    MenuCount,
};

class MenuRenderer {
    private:
    const std::string menusText = "Game;Bitboard;Settings";

    GameMenuRenderer* gameMenuRenderer = new GameMenuRenderer();
    BitboardMenuRenderer* bitboardMenuRenderer = new BitboardMenuRenderer();
    SettingsMenuRenderer* settingsMenuRenderer = new SettingsMenuRenderer();

    Vector2 position;
    Vector2 size;

    int menuActive = GAME_MENU;
    Vector2 panelScroll;
    Rectangle panelView;

    void drawMenuTopBar();


    public:
    MenuRenderer(Vector2 position, Vector2 size);
    ~MenuRenderer();

    void renderMenu();
};


#endif //CHESS_MENURENDERER_H
