#include "MenuRenderer.h"

#include <iostream>

#include "raygui.h"


MenuRenderer::MenuRenderer(Vector2 position, Vector2 size) : position(position), size(size) {

}

MenuRenderer::~MenuRenderer() {}

void MenuRenderer::renderMenu() {
    drawMenuTopBar();
    GuiScrollPanel({position.x, position.y + 45+2, size.x-1, size.y-45-2}, nullptr,
        {0, 0, 800, 400}, &panelScroll, &panelView);
    //std::cout << "panelscroll: \tx:" << panelScroll.x << "\t, y:" << panelScroll.y << "\t panelview: x: " << panelView.x << "\t, y: " << panelView.y << "\t, width: " << panelView.width << "\t, height: " << panelView.height  << std::endl;
}

void MenuRenderer::drawMenuTopBar() {
    GuiToggleGroup({position.x, position.y, (size.x-MenuCount*2+1)/MenuCount, 45}, menusText.c_str(), &menuActive);
}