/*******************************************************************************************
*
*   Chessgame v1.0.0 - Tool Description
*
*   MODULE USAGE:
*       #define GUI_CHESSGAME_IMPLEMENTATION
*       #include "gui_chessgame.h"
*
*       INIT: GuiChessgameState state = InitGuiChessgame();
*       DRAW: GuiChessgame(&state);
*
*   LICENSE: Propietary License
*
*   Copyright (c) 2022 raylib technologies. All Rights Reserved.
*
*   Unauthorized copying of this file, via any medium is strictly prohibited
*   This project is proprietary and confidential unless the owner allows
*   usage in any other form by expresely written permission.
*
**********************************************************************************************/

#include "raylib.h"

// WARNING: raygui implementation is expected to be defined before including this header
#undef RAYGUI_IMPLEMENTATION
#include <charconv>
#include <cmath>
#include <iostream>

#include "raygui.h"

#include <string.h>     // Required for: strcpy()
#include <string>
#include <vector>

#ifndef GUI_CHESSGAME_H
#define GUI_CHESSGAME_H

typedef struct {
    // Define anchors
    Vector2 anchor01;            // ANCHOR ID:1
    
    // Define controls variables
    int ToggleGroupMenuBarActive;            // ToggleGroup: ToggleGroupMenuBar
    Rectangle ScrollView;
    Vector2 ScrollOffset;
    Vector2 BoundsOffset;            // ScrollPanel: ScrollPanel003

    // Define rectangles
    Rectangle layoutRecs[8];

    // Custom state variables (depend on development software)
    // NOTE: This variables should be added manually if required

} GuiChessgameState;

#ifdef __cplusplus
extern "C" {            // Prevents name mangling of functions
#endif

//----------------------------------------------------------------------------------
// Defines and Macros
//----------------------------------------------------------------------------------
//...

//----------------------------------------------------------------------------------
// Types and Structures Definition
//----------------------------------------------------------------------------------
// ...

//----------------------------------------------------------------------------------
// Module Functions Declaration
//----------------------------------------------------------------------------------
GuiChessgameState InitGuiChessgame(void);
void GuiChessgame(GuiChessgameState *state, std::vector<std::string> moves = {});


#ifdef __cplusplus
}
#endif

#endif // GUI_CHESSGAME_H

/***********************************************************************************
*
*   GUI_CHESSGAME IMPLEMENTATION
*
************************************************************************************/
#if defined(GUI_CHESSGAME_IMPLEMENTATION)

#include "raygui.h"

//----------------------------------------------------------------------------------
// Global Variables Definition
//----------------------------------------------------------------------------------
//...

//----------------------------------------------------------------------------------
// Internal Module Functions Definition
//----------------------------------------------------------------------------------
//...

//----------------------------------------------------------------------------------
// Module Functions Definition
//----------------------------------------------------------------------------------
GuiChessgameState InitGuiChessgame(void)
{
    GuiChessgameState state = { 0 };

    // Init anchors
    state.anchor01 = (Vector2){ 720, 0 };            // ANCHOR ID:1
    
    // Initilize controls variables
    state.ToggleGroupMenuBarActive = 0;            // ToggleGroup: ToggleGroupMenuBar
    state.ScrollView = (Rectangle){ 0, 0, 0, 0 };
    state.ScrollOffset = (Vector2){ 0, 0 };
    state.BoundsOffset = (Vector2){ 0, 0 };            // ScrollPanel: ScrollPanel003

    // Init controls rectangles
    state.layoutRecs[0] = (Rectangle){ state.anchor01.x + 0, state.anchor01.y + 0, 56, 32 };    // ToggleGroup: ToggleGroupMenuBar
    state.layoutRecs[1] = (Rectangle){ state.anchor01.x + 0, state.anchor01.y + 32, 520, 720 }; // Panel: MenuPanel
    state.layoutRecs[2] = (Rectangle){ state.anchor01.x + 8, state.anchor01.y + 40, 120, 24 };  // Label: Label004
    state.layoutRecs[3] = (Rectangle){ 728, 128, 240+14, 192 };                                    // ScrollPanel: ScrollPanel003
    state.layoutRecs[4] = (Rectangle){ 736, 160, 120, 24 };                                     // Label: Label004
    state.layoutRecs[5] = (Rectangle){728, 128, 240, 192 +100};                                 // ScrollPanelContent
    state.layoutRecs[6] = (Rectangle){0, 0, 24, 24};                                            // MoveNumberLabel
    state.layoutRecs[7] = (Rectangle){state.layoutRecs[6].width, 0, 120, 24};                   // MoveLabel
    state.layoutRecs[5].width = state.layoutRecs[6].width + 2* state.layoutRecs[7].width;
    state.layoutRecs[3].width = state.layoutRecs[5].width + 14;

    // Custom variables initialization

    return state;
}


void GuiChessgame(GuiChessgameState *state, std::vector<std::string> moves)
{
    // Draw controls
    GuiToggleGroup(state->layoutRecs[0], "Game;Debug;BitBoards", &state->ToggleGroupMenuBarActive);
    GuiPanel(state->layoutRecs[1], NULL);

    GuiLabel(state->layoutRecs[2], "FEN");
    GuiScrollPanel( (Rectangle){state->layoutRecs[3].x, state->layoutRecs[3].y, state->layoutRecs[3].width - state->BoundsOffset.x, state->layoutRecs[3].height - state->BoundsOffset.y},
                        "Moves:",
                        state->layoutRecs[5],
                        &state->ScrollOffset,
                        &state->ScrollView);
    BeginScissorMode(state->ScrollView.x, state->ScrollView.y, state->ScrollView.width, state->ScrollView.height);
    for (int i = 0; i < moves.size(); i++) {
        const std::string& move = moves[i];
        Rectangle placeRec = (Rectangle){state->layoutRecs[7].x + state->ScrollView.x + state->ScrollOffset.x,
                        state->layoutRecs[7].y + state->ScrollView.y + state->ScrollOffset.y,
                        state->layoutRecs[7].width,
                        state->layoutRecs[7].height};
        if (i%2==0) {
            Rectangle numberRec = (Rectangle){state->layoutRecs[6].x + state->ScrollView.x + state->ScrollOffset.x,
                        state->layoutRecs[6].y + state->ScrollView.y + state->ScrollOffset.y + state->layoutRecs[7].height * (float)std::floor(i/2),
                        state->layoutRecs[6].width,
                        state->layoutRecs[6].height};
            GuiPanel(numberRec,
                        nullptr);
            GuiLabel({numberRec.x+8, numberRec.y, numberRec.width, numberRec.height}, std::to_string((i / 2) + 1).c_str());
        }
        else {
            placeRec.x += state->layoutRecs[7].width;
        }
        placeRec.y += state->layoutRecs[7].height * std::floor(i/2);
        GuiPanel(placeRec,nullptr);
        GuiLabel({placeRec.x + 8, placeRec.y, placeRec.width, placeRec.height}, move.c_str());
    }
    EndScissorMode();
}

#endif // GUI_CHESSGAME_IMPLEMENTATION
