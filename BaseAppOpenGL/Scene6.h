#pragma once

#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <string>
#include <cstdio>

// ======================================================
//  FAKE BASE CLASSES — para compilar mesmo sem o projeto completo
// ======================================================
class CSceneBaseClass {};  // Base vazia

class CTexture {};
class CTimer {
public:
    void Init() {}
    void Update() {}
    unsigned int GetTime() { return GetTickCount(); }
};

class CTexto {
public:
    void glPrint(const char* fmt, ...) {}
};

class CCamera {
public:
    float Position[3] = { 0.0f, 0.0f, 0.0f };
    float Forward[3] = { 0.0f, 0.0f, -1.0f };
    float Right[3] = { 1.0f, 0.0f, 0.0f };
    float Up[3] = { 0.0f, 1.0f, 0.0f };

    CCamera(float x = 0, float y = 0, float z = 0) {
        Position[0] = x; Position[1] = y; Position[2] = z;
    }

    void setView() {}
    void rotateGlob(float, float, float, float) {}
    void rotateLoc(float, float, float, float) {}
    void moveGlob(float, float, float) {}
};

// ======================================================
// Constantes globais (substitui temporariamente teu header principal)
// ======================================================
#define WIDTH  800
#define HEIGHT 600

// ======================================================
// Classe da cena
// ======================================================
class CScene6 : public CSceneBaseClass
{
public:
    CScene6();
    ~CScene6(void);

    virtual void MouseMove(void);
    virtual void KeyPressed(void);
    virtual void KeyDownPressed(WPARAM wParam);
    virtual int DrawGLScene(void);

    void Draw3DSGrid(float width, float length);
    void DrawAxis();

private:
    bool keys[256];
    bool active;
    bool fullscreen;

    CCamera* pCamera;
    float fDeltaY;
    float fDeltaX;

    CTexto* pTexto;
    CTexture* pTextures;
    CTimer* pTimer;

    int iFPS;
    int iFrames;
    DWORD ulLastFPS;
    char szTitle[256];

    bool bIsWireframe;
    bool bIsCameraFPS;

    float fRenderPosY;
    float fTimerPosY;
};
