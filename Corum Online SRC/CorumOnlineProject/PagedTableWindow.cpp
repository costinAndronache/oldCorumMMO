#include "PagedTableWindow.h"
#include <GdiPlus.h>

using namespace CustomUiKit;

PagedTableWindow::PagedTableWindow(Rect initialFrame, HWND inputWindowHandle): 
                  frame(initialFrame), windowHandle(inputWindowHandle) {
    graphics = Gdiplus::Graphics::FromHWND(windowHandle);
}


void PagedTableWindow::repaintInWindow() {
    Gdiplus::Color c = Gdiplus::Color(255, 255, 255);

    const Gdiplus::Brush* brush = new Gdiplus::SolidBrush(c);

    Gdiplus::Rect rect(frame.origin.x, 
                       frame.origin.y, 
                       frame.size.width, 
                       frame.size.height);
    
    graphics->FillRectangle(brush, rect);
}
