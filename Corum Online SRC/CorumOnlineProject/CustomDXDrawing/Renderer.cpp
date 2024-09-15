#include "Renderer.h"
#include "../InitGame.h"
#include <vector>
#include <d3dx8.h>

using namespace CustomDXDrawing;


//#define _GEFORCE3_

enum DISPLAY_DEVICE_TYPE
{

};

enum RESORUCE_POOL_TYPE
{

    RESORUCE_POOL_TYPE_VBSTATICPOOL = 0,
    RESORUCE_POOL_TYPE_IBSTATICPOOL = 1,
    RESORUCE_POOL_TYPE_VBHEAP = 2,
    RESORUCE_POOL_TYPE_IBHEAP = 3
};

/*
#define ALPHA_TEX_OPASITY		0x10000000

#define	ALPHA_TEX_TRANSP		0x01000000
#define	ALPHA_TEX_TRANSP_FILTER	0x01000000
#define	ALPHA_TEX_TRANSP_SUB	0x03000000
#define	ALPHA_TEX_TRANSP_ADD	0x05000000
#define	ALPHA_TEX_TRANSP_MASK	0x0f000000
*/
enum TEX_FLAG
{

    TEX_FLAG_ALPHA_OPASITY = 0x10000000,
    TEX_FLAG_ALPHA_TRANSP = 0x01000000,
    TEX_FLAG_ALPHA_TRANSP_FILTER = 0x02000000,
    TEX_FLAG_ALPHA_TRANSP_SUB = 0x04000000,
    TEX_FLAG_ALPHA_TRANSP_ADD = 0x08000000

};

#define	ALPHA_TEX_TRANSP_MASK	0x0f000000
#define	ALPHA_TEX_PROPERTY_MASK	0xff000000

#define	TEX_FORMAT_565			0
#define	TEX_FORMAT_4444			1


enum CREATE_FACEGROUP_TYPE
{
    CREATE_FACEGROUP_TYPE_STATIC = 0x00000001
};

enum TEX_COORD_OFFSET
{
    TEX1_COORD_OFFSET_ZERO = 0,
    TEX1_COORD_OFFSET_NODIFFUSE = 12,
    TEX2_COORD_OFFSET_NODIFFUSE = 20,
    TEX1_COORD_OFFSET_RHW_DIFFUSE = 20,

    TEX1_COORD_OFFSET_LMMESH = 12,
    TEX2_COORD_OFFSET_LMMESH = 20,


    TEX1_COORD_OFFSET_VLMESH = 24,
    TEX1_COORD_OFFSET_VLSMESH = 16,



    TEX1_COORD_OFFSET_BBOARD = 12

};

enum DIFFUSE_COORD_OFFSET
{
    DIFFUSE_COORD_OFFSET_ZERO = 0,
    DIFFUSE_COORD_OFFSET_HFIELD = 12,
    DIFFUSE_COORD_OFFSET_XYZ = 24,
    DIFFUSE_COORD_OFFSET_XYZW = 16,
    DIFFUSE_COORD_VLS_OFFSET = 12

};
enum NORMAL_COORD_OFFSET
{
    NORMAL_COORD_OFFSET_VLMESH = 12
};

#define	D3DFVF_VLVERTEX	( D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1  )
#define D3DFVF_BBOARDVERTEX ( D3DFVF_XYZ | D3DFVF_TEX1)

#define D3DFVF_VLSVERTEX (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1)
#define D3DFVF_HFIELDVERTEX ( D3DFVF_XYZ | D3DFVF_DIFFUSE)
#define D3DFVF_IVERTEX	(D3DFVF_XYZ | D3DFVF_TEX1)

//#define D3DFVF_D3DVERTEX  ( D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1  )


#define D3DFVF_D3DLVERTEX ( D3DFVF_XYZ | D3DFVF_DIFFUSE )


#define D3DFVF_D3DDUALVERTEX ( D3DFVF_XYZ | D3DFVF_TEX2)


#define D3DFVF_4DVERTEX	( D3DFVF_XYZRHW )


#define D3DFVF_D3DTLVERTEX		(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1 )
//#define D3DFVF_D3DTRIPLEVERTEX ( D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX3 )
/*
struct D3DVERTEX
{
    float		x;
    float		y;
    float		z;

    float		nx;
    float		ny;
    float		nz;

    DWORD		color;

    float		tu;
    float		tv;

};*/
struct D3DVLVERTEX
{
    float		x;
    float		y;
    float		z;

    float		nx;
    float		ny;
    float		nz;

    float		tu;
    float		tv;
};

struct D3DVLSVERTEX
{
    float		x;
    float		y;
    float		z;

    DWORD		color;

    float		tu;
    float		tv;
};


struct D3DLVERTEX
{
    float     x;
    float     y;
    float     z;

    DWORD     color;
};

struct D3DTLVERTEX
{
    float		x;
    float		y;
    float		z;
    float		w;

    DWORD		color;

    float		tu;
    float		tv;
};

struct D3D4DVERTEX
{
    float		x;
    float		y;
    float		z;
    float		w;
};



struct D3DDUALVERTEX
{
    float		x;
    float		y;
    float		z;

    float		tu1;
    float		tv1;

    float		tu2;
    float		tv2;
};
/*
struct D3DTRIPLEVERTEX
{
    float		x;
    float		y;
    float		z;

    DWORD		color;

    float		tu1;
    float		tv1;

    float		tu2;
    float		tv2;

    float		tu3;
    float		tv3;
};
*/
struct HFIELDVERTEX
{
    float		x;
    float		y;
    float		z;
    DWORD		dwColor;
};


struct D3DBBOARDVERTEX
{
    float		x;
    float		y;
    float		z;
    float		tu1;
    float		tv2;
};

struct ARGB
{
    BYTE		a;
    BYTE		r;
    BYTE		g;
    BYTE		b;
};

struct VS_CONSTANT_LIGHT
{
    VECTOR3		v3Pos;
    float		fRsPow;
};

enum DIMATERIAL_PROPERTY_TYPE
{
    DIMATERIAL_PROPERTY_TYPE_DUAL_SIDE = 0x00000001,
    DIMATERIAL_PROPERTY_TYPE_SELF_ILLUMIN = 0x00000002,
    DIMATERIAL_PROPERTY_TYPE_TEX_OPASITY = 0x00000004,

    DIMATERIAL_PROPERTY_TYPE_TEX_TRANSP = 0x00000008,
    DIMATERIAL_PROPERTY_TYPE_TEX_TRANSP_FILTER = 0x00000010,
    DIMATERIAL_PROPERTY_TYPE_TEX_TRANSP_SUB = 0x00000020,
    DIMATERIAL_PROPERTY_TYPE_TEX_TRANSP_ADD = 0x00000040,
};

void Renderer::Reset()
{
    D3DVIEWPORT8 screen;
    pDevice->GetViewport(&screen);

    Screen.Width = screen.Width;
    Screen.Height = screen.Height;
    Screen.x_center = Screen.Width / 2;
    Screen.y_center = Screen.Height / 2;
}

void Renderer::Line(float x1, float y1, float x2, float y2, float width, bool antialias, DWORD color)
{
    return;

    /*
    ID3DXLine* m_Line;

    D3DXCreateLine(pDevice, &m_Line);
    D3DXVECTOR2 line[] = { D3DXVECTOR2(x1, y1), D3DXVECTOR2(x2, y2) };
    m_Line->SetWidth(width);
    if (antialias) m_Line->SetAntialias(1);
    m_Line->Begin();
    m_Line->Draw(line, 2, color);
    m_Line->End();
    m_Line->Release(); */
}

void Renderer::Circle(float x, float y, float radius, int rotate, int type, bool smoothing, int resolution, DWORD color)
{
    std::vector<vertex> circle(resolution + 2);
    float angle = rotate * D3DX_PI / 180;
    float pi;

    if (type == full) pi = D3DX_PI;        // Full circle
    if (type == half) pi = D3DX_PI / 2;      // 1/2 circle
    if (type == quarter) pi = D3DX_PI / 4;   // 1/4 circle

    for (int i = 0; i < resolution + 2; i++)
    {
        circle[i].x = (float)(x - radius * cos(i * (2 * pi / resolution)));
        circle[i].y = (float)(y - radius * sin(i * (2 * pi / resolution)));
        circle[i].z = 0;
        circle[i].rhw = 1;
        circle[i].color = color;
    }

    // Rotate matrix
    int _res = resolution + 2;
    for (int i = 0; i < _res; i++)
    {
        circle[i].x = x + cos(angle) * (circle[i].x - x) - sin(angle) * (circle[i].y - y);
        circle[i].y = y + sin(angle) * (circle[i].x - x) + cos(angle) * (circle[i].y - y);
    }

    pDevice->CreateVertexBuffer((resolution + 2) * sizeof(vertex), D3DUSAGE_WRITEONLY, D3DFVF_XYZRHW | D3DFVF_DIFFUSE, D3DPOOL_DEFAULT, &g_pVB);

    VOID* pVertices;
    g_pVB->Lock(0, (resolution + 2) * sizeof(vertex), (BYTE**)&pVertices, 0);
    memcpy(pVertices, &circle[0], (resolution + 2) * sizeof(vertex));
    g_pVB->Unlock();


    pDevice->SetTexture(0, NULL);
    pDevice->SetPixelShader(NULL);
    if (smoothing)
    {
        pDevice->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, TRUE);
        //pDevice->SetRenderState(D3DRS_ANTIALIASEDLINEENABLE, TRUE);
    }

    pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

    pDevice->SetStreamSource(0, g_pVB, sizeof(vertex));
    pDevice->SetVertexShader(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);

    pDevice->DrawPrimitive(D3DPT_LINESTRIP, 0, resolution);
    if (g_pVB != NULL) g_pVB->Release();
}

void Renderer::CircleFilled(float x, float y, float rad, float rotate, int type, int resolution, DWORD color)
{
    std::vector<vertex> circle(resolution + 2);
    float angle = rotate * D3DX_PI / 180;
    float pi;

    if (type == full) pi = D3DX_PI;        // Full circle
    if (type == half) pi = D3DX_PI / 2;      // 1/2 circle
    if (type == quarter) pi = D3DX_PI / 4;   // 1/4 circle

    circle[0].x = x;
    circle[0].y = y;
    circle[0].z = 0;
    circle[0].rhw = 1;
    circle[0].color = color;

    for (int i = 1; i < resolution + 2; i++)
    {
        circle[i].x = (float)(x - rad * cos(pi * ((i - 1) / (resolution / 2.0f))));
        circle[i].y = (float)(y - rad * sin(pi * ((i - 1) / (resolution / 2.0f))));
        circle[i].z = 0;
        circle[i].rhw = 1;
        circle[i].color = color;
    }

    // Rotate matrix
    int _res = resolution + 2;
    for (int i = 0; i < _res; i++)
    {
        circle[i].x = x + cos(angle) * (circle[i].x - x) - sin(angle) * (circle[i].y - y);
        circle[i].y = y + sin(angle) * (circle[i].x - x) + cos(angle) * (circle[i].y - y);
    }

    pDevice->CreateVertexBuffer((resolution + 2) * sizeof(vertex), D3DUSAGE_WRITEONLY, D3DFVF_XYZRHW | D3DFVF_DIFFUSE, D3DPOOL_DEFAULT, &g_pVB);

    VOID* pVertices;
    g_pVB->Lock(0, (resolution + 2) * sizeof(vertex), (BYTE**)&pVertices, 0);
    memcpy(pVertices, &circle[0], (resolution + 2) * sizeof(vertex));
    g_pVB->Unlock();

    pDevice->SetTexture(0, NULL);
    pDevice->SetPixelShader(NULL);
    pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

    pDevice->SetStreamSource(0, g_pVB, sizeof(vertex));
    pDevice->SetVertexShader(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
    pDevice->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, resolution);
    if (g_pVB != NULL) g_pVB->Release();
}

void Renderer::Box(float x, float y, float w, float h, float linewidth, DWORD color)
{
    if (linewidth == 0 || linewidth == 1)
    {
        BoxFilled(x, y, w, 1, color);             // Top
        BoxFilled(x, y + h - 1, w, 1, color);         // Bottom
        BoxFilled(x, y + 1, 1, h - 2 * 1, color);       // Left
        BoxFilled(x + w - 1, y + 1, 1, h - 2 * 1, color);   // Right
    }
    else
    {
        BoxFilled(x, y, w, linewidth, color);                                     // Top
        BoxFilled(x, y + h - linewidth, w, linewidth, color);                         // Bottom
        BoxFilled(x, y + linewidth, linewidth, h - 2 * linewidth, color);               // Left
        BoxFilled(x + w - linewidth, y + linewidth, linewidth, h - 2 * linewidth, color);   // Right
    }
}

void Renderer::BoxBordered(float x, float y, float w, float h, float border_width, DWORD color, DWORD color_border)
{
    BoxFilled(x, y, w, h, color);
    Box(x - border_width, y - border_width, w + 2 * border_width, h + border_width, border_width, color_border);
}

void SetColorFactorShader(LPDIRECT3DDEVICE8 pDevice, D3DCOLOR color, DWORD dwTexStageIndex)
{
    pDevice->SetRenderState(D3DRS_TEXTUREFACTOR, color);

    pDevice->SetTextureStageState(dwTexStageIndex, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    pDevice->SetTextureStageState(dwTexStageIndex, D3DTSS_COLORARG2, D3DTA_TFACTOR);
    pDevice->SetTextureStageState(dwTexStageIndex, D3DTSS_COLOROP, D3DTOP_SELECTARG2);
    pDevice->SetTextureStageState(dwTexStageIndex, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
}

void RestoreTextureShader(LPDIRECT3DDEVICE8 pDevice, DWORD dwTexStageIndex)
{
    pDevice->SetTextureStageState(dwTexStageIndex, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    pDevice->SetTextureStageState(dwTexStageIndex, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
    pDevice->SetTextureStageState(dwTexStageIndex, D3DTSS_COLOROP, D3DTOP_MODULATE);

}

void Renderer::prepareForDrawing(DWORD color) {
    pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

    pDevice->SetVertexShader(D3DFVF_D3DLVERTEX);

    pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

    pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_DISABLE);
    SetColorFactorShader(pDevice, color, 0);

    MATRIX4 identity;
    SetIdentityMatrix(&identity);
    //pDevice->SetTransform(D3DTS_WORLD, (const D3DMATRIX*)&identity);
}

void Renderer::finalizeDrawing() {
    RestoreTextureShader(pDevice, 0);

    pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
    pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}

void Renderer::BoxFilled(float x, float y, float w, float h, DWORD color)
{
    VECTOR4		v4Point[5];

    v4Point[0].x = 100; v4Point[0].y = 100;
    v4Point[1].x = 1000; v4Point[1].y = 100;
    v4Point[2].x = 1000; v4Point[2].y = 500;
    v4Point[3].x = 100; v4Point[3].y = 500;
    v4Point[4] = v4Point[0];

    for (int i = 0; i < 5; i++)
    {
        v4Point[i].z = 0.1f;
        v4Point[i].w = 1.0f / v4Point[i].z;
    }

    DWORD stage = 0;
    pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
    pDevice->SetTextureStageState(stage, D3DTSS_COLOROP, D3DTOP_DISABLE);
    pDevice->SetVertexShader(D3DFVF_4DVERTEX);
    pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);


    pDevice->SetTextureStageState(stage, D3DTSS_COLOROP, D3DTOP_DISABLE);

    SetColorFactorShader(pDevice, RED(250), stage);
    //m_pD3DDevice->SetTransform(D3DTS_WORLD, (D3DMATRIX*)GetIdentityMatrix());


    pDevice->DrawPrimitiveUP(
        D3DPT_LINELIST,
        1,
        v4Point,
        sizeof(VECTOR4));

    pDevice->DrawPrimitiveUP(
        D3DPT_LINELIST,
        1,
        v4Point + 1,
        sizeof(VECTOR4));

    pDevice->DrawPrimitiveUP(
        D3DPT_LINELIST,
        1,
        v4Point + 2,
        sizeof(VECTOR4));

    pDevice->DrawPrimitiveUP(
        D3DPT_LINELIST,
        1,
        v4Point + 3,
        sizeof(VECTOR4));

    RestoreTextureShader(pDevice, stage);

    pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

    pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

    return;

    vertex V[4];

    V[0].color = V[1].color = V[2].color = V[3].color = color;

    V[0].z = V[1].z = V[2].z = V[3].z = 0;
    V[0].rhw = V[1].rhw = V[2].rhw = V[3].rhw = 0;

    V[0].x = x;
    V[0].y = y;
    V[1].x = x + w;
    V[1].y = y;
    V[2].x = x + w;
    V[2].y = y + h;
    V[3].x = x;
    V[3].y = y + h;

    unsigned short indexes[] = { 0, 1, 3, 1, 2, 3 };

    pDevice->CreateVertexBuffer(4 * sizeof(vertex), D3DUSAGE_WRITEONLY, D3DFVF_XYZRHW | D3DFVF_DIFFUSE, D3DPOOL_DEFAULT, &g_pVB);
    pDevice->CreateIndexBuffer(2 * sizeof(short), D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_DEFAULT, &g_pIB);

    VOID* pVertices;
    g_pVB->Lock(0, sizeof(V), (BYTE**)&pVertices, 0);
    memcpy(pVertices, V, sizeof(V));
    g_pVB->Unlock();

    VOID* pIndex;
    g_pIB->Lock(0, sizeof(indexes), (BYTE**)&pIndex, 0);
    memcpy(pIndex, indexes, sizeof(indexes));
    g_pIB->Unlock();


    pDevice->SetStreamSource(0, g_pVB, sizeof(vertex));
    pDevice->SetVertexShader(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
    pDevice->SetIndices(g_pIB, 0);
    
    prepareForDrawing(color);
    pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 4, 0, 2);
    finalizeDrawing();

    g_pVB->Release();
    g_pIB->Release();
}

void Renderer::BoxRounded(float x, float y, float w, float h, float radius, bool smoothing, DWORD color, DWORD bcolor)
{
    BoxFilled(x + radius, y + radius, w - 2 * radius - 1, h - 2 * radius - 1, color);   // Center rect.
    BoxFilled(x + radius, y + 1, w - 2 * radius - 1, radius - 1, color);            // Top rect.
    BoxFilled(x + radius, y + h - radius - 1, w - 2 * radius - 1, radius, color);     // Bottom rect.
    BoxFilled(x + 1, y + radius, radius - 1, h - 2 * radius - 1, color);            // Left rect.
    BoxFilled(x + w - radius - 1, y + radius, radius, h - 2 * radius - 1, color);     // Right rect.

    // Smoothing method
    if (smoothing)
    {
        CircleFilled(x + radius, y + radius, radius - 1, 0, quarter, 16, color);             // Top-left corner
        CircleFilled(x + w - radius - 1, y + radius, radius - 1, 90, quarter, 16, color);        // Top-right corner
        CircleFilled(x + w - radius - 1, y + h - radius - 1, radius - 1, 180, quarter, 16, color);   // Bottom-right corner
        CircleFilled(x + radius, y + h - radius - 1, radius - 1, 270, quarter, 16, color);       // Bottom-left corner

        Circle(x + radius + 1, y + radius + 1, radius, 0, quarter, true, 16, bcolor);          // Top-left corner
        Circle(x + w - radius - 2, y + radius + 1, radius, 90, quarter, true, 16, bcolor);       // Top-right corner
        Circle(x + w - radius - 2, y + h - radius - 2, radius, 180, quarter, true, 16, bcolor);    // Bottom-right corner
        Circle(x + radius + 1, y + h - radius - 2, radius, 270, quarter, true, 16, bcolor);      // Bottom-left corner

        Line(x + radius, y + 1, x + w - radius - 1, y + 1, 1, false, bcolor);       // Top line
        Line(x + radius, y + h - 2, x + w - radius - 1, y + h - 2, 1, false, bcolor);   // Bottom line
        Line(x + 1, y + radius, x + 1, y + h - radius - 1, 1, false, bcolor);       // Left line
        Line(x + w - 2, y + radius, x + w - 2, y + h - radius - 1, 1, false, bcolor);   // Right line
    }
    else
    {
        CircleFilled(x + radius, y + radius, radius, 0, quarter, 16, color);             // Top-left corner
        CircleFilled(x + w - radius - 1, y + radius, radius, 90, quarter, 16, color);        // Top-right corner
        CircleFilled(x + w - radius - 1, y + h - radius - 1, radius, 180, quarter, 16, color);   // Bottom-right corner
        CircleFilled(x + radius, y + h - radius - 1, radius, 270, quarter, 16, color);       // Bottom-left corner
    }
}

void Renderer::Text(char* text, float x, float y, int orientation, int font, bool bordered, DWORD color, DWORD bcolor)
{
    RECT rect;

    switch (orientation)
    {
    case lefted:
        if (bordered)
        {
            SetRect(&rect, x - 1, y, x - 1, y);
            pFont[font]->DrawTextA(text, -1, &rect, DT_LEFT | DT_NOCLIP, bcolor);
            SetRect(&rect, x + 1, y, x + 1, y);
            pFont[font]->DrawTextA(text, -1, &rect, DT_LEFT | DT_NOCLIP, bcolor);
            SetRect(&rect, x, y - 1, x, y - 1);
            pFont[font]->DrawTextA(text, -1, &rect, DT_LEFT | DT_NOCLIP, bcolor);
            SetRect(&rect, x, y + 1, x, y + 1);
            pFont[font]->DrawTextA(text, -1, &rect, DT_LEFT | DT_NOCLIP, bcolor);
        }
        SetRect(&rect, x, y, x, y);
        pFont[font]->DrawTextA(text, -1, &rect, DT_LEFT | DT_NOCLIP, color);
        break;
    case centered:
        if (bordered)
        {
            SetRect(&rect, x - 1, y, x - 1, y);
            pFont[font]->DrawTextA(text, -1, &rect, DT_CENTER | DT_NOCLIP, bcolor);
            SetRect(&rect, x + 1, y, x + 1, y);
            pFont[font]->DrawTextA(text, -1, &rect, DT_CENTER | DT_NOCLIP, bcolor);
            SetRect(&rect, x, y - 1, x, y - 1);
            pFont[font]->DrawTextA(text, -1, &rect, DT_CENTER | DT_NOCLIP, bcolor);
            SetRect(&rect, x, y + 1, x, y + 1);
            pFont[font]->DrawTextA(text, -1, &rect, DT_CENTER | DT_NOCLIP, bcolor);
        }
        SetRect(&rect, x, y, x, y);
        pFont[font]->DrawTextA(text, -1, &rect, DT_CENTER | DT_NOCLIP, color);
        break;
    case righted:
        if (bordered)
        {
            SetRect(&rect, x - 1, y, x - 1, y);
            pFont[font]->DrawTextA(text, -1, &rect, DT_RIGHT | DT_NOCLIP, bcolor);
            SetRect(&rect, x + 1, y, x + 1, y);
            pFont[font]->DrawTextA(text, -1, &rect, DT_RIGHT | DT_NOCLIP, bcolor);
            SetRect(&rect, x, y - 1, x, y - 1);
            pFont[font]->DrawTextA(text, -1, &rect, DT_RIGHT | DT_NOCLIP, bcolor);
            SetRect(&rect, x, y + 1, x, y + 1);
            pFont[font]->DrawTextA(text, -1, &rect, DT_RIGHT | DT_NOCLIP, bcolor);
        }
        SetRect(&rect, x, y, x, y);
        pFont[font]->DrawTextA(text, -1, &rect, DT_RIGHT | DT_NOCLIP, color);
        break;
    }
}

void Renderer::Message(char* text, float x, float y, int font, int orientation)
{
    RECT rect = { x, y, x, y };

    switch (orientation)
    {
    case lefted:
        pFont[font]->DrawTextA(text, -1, &rect, DT_CALCRECT | DT_LEFT, BLACK(255));

        BoxRounded(x - 5, rect.top - 5, rect.right - x + 10, rect.bottom - rect.top + 10, 5, true, DARKGRAY(150), SKYBLUE(255));

        SetRect(&rect, x, y, x, y);
        pFont[font]->DrawTextA(text, -1, &rect, DT_LEFT | DT_NOCLIP, ORANGE(255));
        break;
    case centered:
        pFont[font]->DrawTextA(text, -1, &rect, DT_CALCRECT | DT_CENTER, BLACK(255));

        BoxRounded(rect.left - 5, rect.top - 5, rect.right - rect.left + 10, rect.bottom - rect.top + 10, 5, true, DARKGRAY(150), SKYBLUE(255));

        SetRect(&rect, x, y, x, y);
        pFont[font]->DrawTextA(text, -1, &rect, DT_CENTER | DT_NOCLIP, ORANGE(255));
        break;
    case righted:
        pFont[font]->DrawTextA(text, -1, &rect, DT_CALCRECT | DT_RIGHT, BLACK(255));

        BoxRounded(rect.left - 5, rect.top - 5, rect.right - rect.left + 10, rect.bottom - rect.top + 10, 5, true, DARKGRAY(150), SKYBLUE(255));

        SetRect(&rect, x, y, x, y);
        pFont[font]->DrawTextA(text, -1, &rect, DT_RIGHT | DT_NOCLIP, ORANGE(255));
        break;
    }
}

void Renderer::Sprite(LPDIRECT3DTEXTURE8 tex, float x, float y, float resolution, float scale, float rotation)
{
    return;

    /* float screen_pos_x = x;
    float screen_pos_y = y;

    // Texture being used is 64x64:
    D3DXVECTOR2 spriteCentre = D3DXVECTOR2(resolution / 2, resolution / 2);

    // Screen position of the sprite
    D3DXVECTOR2 trans = D3DXVECTOR2(screen_pos_x, screen_pos_y);

    // Build our matrix to rotate, scale and position our sprite
    D3DXMATRIX mat;

    D3DXVECTOR2 scaling(scale, scale);

    // out, scaling centre, scaling rotation, scaling, rotation centre, rotation, translation
    D3DXMatrixTransformation2D(&mat, NULL, 0.0, &scaling, &spriteCentre, rotation, &trans);

    //pDevice->SetRenderState(D3DRS_ZENABLE, false);
    pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
    pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
    pDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    pDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
    pDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);
    pDevice->SetPixelShader(NULL);
    sSprite->Begin(NULL);
    sSprite->SetTransform(&mat); // Tell the sprite about the matrix
    sSprite->Draw(tex, NULL, NULL, NULL, 0xFFFFFFFF);
    sSprite->End();
    */
}

bool Renderer::Font()
{
    for (int i = 0; i < FontNr; i++)
        if (pFont[i]) return false;
    return true;
}

void Renderer::AddFont(char* Caption, float size, bool bold, bool italic)
{
    //D3DXCreateFont(pDevice, size, 0, (bold) ? FW_BOLD : FW_NORMAL, 1, (italic) ? 1 : 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH, Caption, &pFont[++FontNr]);
}

void Renderer::FontReset()
{
    return;
    //for (int i = 0; i < FontNr; i++) pFont[i]->OnLostDevice();
    //for (int i = 0; i < FontNr; i++) pFont[i]->OnResetDevice();
}

void Renderer::OnLostDevice()
{
    return;
    //for (int i = 0; i < FontNr; i++) pFont[i]->OnLostDevice();
}