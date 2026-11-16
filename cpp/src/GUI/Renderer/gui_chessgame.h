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
#include "raygui.h"

#include <string.h>     // Required for: strcpy()

#ifndef GUI_CHESSGAME_H
#define GUI_CHESSGAME_H

typedef struct {
    // Define anchors
    Vector2 anchor01;            // ANCHOR ID:1
    
    // Define controls variables
    int ToggleGroupMenuBarActive;            // ToggleGroup: ToggleGroupMenuBar
    Rectangle ScrollPanel003ScrollView;
    Vector2 ScrollPanel003ScrollOffset;
    Vector2 ScrollPanel003BoundsOffset;            // ScrollPanel: ScrollPanel003

    // Define rectangles
    Rectangle layoutRecs[5];

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
void GuiChessgame(GuiChessgameState *state);


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
    state.ScrollPanel003ScrollView = (Rectangle){ 0, 0, 0, 0 };
    state.ScrollPanel003ScrollOffset = (Vector2){ 0, 0 };
    state.ScrollPanel003BoundsOffset = (Vector2){ 0, 0 };            // ScrollPanel: ScrollPanel003

    // Init controls rectangles
    state.layoutRecs[0] = (Rectangle){ state.anchor01.x + 0, state.anchor01.y + 0, 56, 32 };// ToggleGroup: ToggleGroupMenuBar
    state.layoutRecs[1] = (Rectangle){ state.anchor01.x + 0, state.anchor01.y + 32, 520, 720 };// Panel: MenuPanel
    state.layoutRecs[2] = (Rectangle){ state.anchor01.x + 8, state.anchor01.y + 40, 120, 24 };// Label: Label004
    state.layoutRecs[3] = (Rectangle){ 728, 128, 160, 192 };// ScrollPanel: ScrollPanel003
    state.layoutRecs[4] = (Rectangle){ 736, 160, 120, 24 };// Label: Label004

    // Custom variables initialization

    return state;
}


void GuiChessgame(GuiChessgameState *state)
{
    // Draw controls
    GuiToggleGroup(state->layoutRecs[0], "Game;Debug;BitBoards", &state->ToggleGroupMenuBarActive);
    GuiPanel(state->layoutRecs[1], NULL);
    GuiLabel(state->layoutRecs[2], "FEN");
    GuiScrollPanel((Rectangle){state->layoutRecs[3].x, state->layoutRecs[3].y, state->layoutRecs[3].width - state->ScrollPanel003BoundsOffset.x, state->layoutRecs[3].height - state->ScrollPanel003BoundsOffset.y }, "Moves:", state->layoutRecs[3], &state->ScrollPanel003ScrollOffset, &state->ScrollPanel003ScrollView);
    GuiLabel(state->layoutRecs[4], "SAMPLE TEXT");
}

#endif // GUI_CHESSGAME_IMPLEMENTATION
