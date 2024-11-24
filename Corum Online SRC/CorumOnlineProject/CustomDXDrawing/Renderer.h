#pragma once

#include <d3d8.h>
#include <d3dx8.h>

#include "Colors.h"

namespace CustomDXDrawing {
    enum circle_type { full, half, quarter };
    enum text_alignment { lefted, centered, righted };

    const int MAX_FONTS = 6;

    struct vertex
    {
        FLOAT x, y, z, rhw;
        DWORD color;
    };

    class Renderer {
    public:
        Renderer(LPDIRECT3DDEVICE8 pDev): 
            pDevice(pDev), g_pVB(NULL), g_pIB(NULL), FontNr(0) {
      
        }

        struct sScreen
        {
            float Width;
            float Height;
            float x_center;
            float y_center;
        } Screen;

      
        void Sprite(LPDIRECT3DTEXTURE8 tex, float x, float y, float resolution, float scale, float rotation);

        //=============================================================================================
        void Line(float x1, float y1, float x2, float y2, float width, bool antialias, DWORD color);

        void Box(float x, float y, float w, float h, float linewidth, DWORD color);
        void BoxFilled(float x, float y, float w, float h, DWORD color);
        void BoxBordered(float x, float y, float w, float h, float border_width, DWORD color, DWORD color_border);
        void BoxRounded(float x, float y, float w, float h, float radius, bool smoothing, DWORD color, DWORD bcolor);

        void Circle(float x, float y, float radius, int rotate, int type, bool smoothing, int resolution, DWORD color);
        void CircleFilled(float x, float y, float rad, float rotate, int type, int resolution, DWORD color);

        void Text(char* text, float x, float y, int orientation, int font, bool bordered, DWORD color, DWORD bcolor);
        void Message(char* text, float x, float y, int font, int orientation);
        //=============================================================================================

        //=============================================================================================
        bool Font();
        void AddFont(char* Caption, float size, bool bold, bool italic);
        void FontReset();
        void OnLostDevice();
        //=============================================================================================

        void Reset();
    private:
        LPDIRECT3DDEVICE8 pDevice;
        LPDIRECT3DVERTEXBUFFER8 g_pVB;    // Buffer to hold vertices
        LPDIRECT3DINDEXBUFFER8 g_pIB;    // Buffer to hold indices
        ID3DXFont* pFont[MAX_FONTS];
        int FontNr;
        void prepareForDrawing(DWORD color);
        void finalizeDrawing();
        //LPD3DXSPRITE sSprite;
    };
};

