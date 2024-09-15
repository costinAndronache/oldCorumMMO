#include "PagedTableWindow.h"


using namespace CustomUiKit;

PagedTableWindow::PagedTableWindow(Rect initialFrame, const Renderer* inputRenderer): 
                  frame(initialFrame), renderer(inputRenderer) {
   
}


void PagedTableWindow::repaintInWindow() {
    Color c = { 124, 122, 0, 0 };
    renderer->fillFrame(frame, c);
}
