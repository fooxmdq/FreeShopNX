#include "common.hpp"

uint8_t *g_framebuf;
u32 g_framebuf_width;
int g_scene;
uint g_idselected = 0;
SortOrder g_sort = NAME_ASC;
bool g_titlesLoaded = false;
bool g_infoLoaded = false;
FsStorageId g_storageID = FsStorageId_SdCard;

int main(int argc, char **argv)
{
    Result rc = 0;
    g_scene = TITLE_SCENE;
    g_infoLoaded = loadInfo();
    g_titlesLoaded = loadTitles();

    gfxInitDefault();
    socketInitializeDefault();

    appletSetScreenShotPermission(1);

    ColorSetId theme;
    rc = setsysInitialize();

    if (R_FAILED(ncmInitialize()))
        fatalSimple(-1);

    if (R_FAILED(nsInitialize()))
        fatalSimple(-2);
    
    if (R_FAILED(nsextInitialize()))
        fatalSimple(-3);

    if (R_FAILED(esInitialize()))
        fatalSimple(-4);

    if (R_FAILED(rc))
        fatalSimple(-5);

    setsysGetColorSetId(&theme);
    themeStartup((ThemePreset)theme);

    rc = plInitialize();
    if (R_FAILED(rc))
        fatalSimple(-6);

    if (!fontInitialize())
        fatalSimple(-7);

    while (appletMainLoop())
    {
        g_framebuf = gfxGetFramebuffer(&g_framebuf_width, NULL);
        memset(g_framebuf, 237, gfxGetFramebufferSize());

        drawUI();

        if (g_scene == TITLE_SCENE)
        {
            titleScene();
            drawSeperators();
        }
        if (g_scene == INFO_SCENE)
            infoScene();
        if (g_scene == UPDATE_SCENE)
            updateScene();
        if (g_scene == ABOUT_SCENE)
            aboutScene();
        if (g_scene == INSTALL_SCENE)
            installScene();

        gfxFlushBuffers();

        gfxSwapBuffers();
        gfxWaitForVsync();

        hidScanInput();

        u32 kDown = hidKeysDown(CONTROLLER_P1_AUTO);
        u32 kHeld = hidKeysHeld(CONTROLLER_P1_AUTO);

        if (kDown & KEY_PLUS)
            break;
        if (kDown & KEY_A)
            buttonA();
        if (kDown & KEY_B)
            buttonB();
        if (kDown & KEY_X)
            buttonX();
        if (kDown & KEY_Y)
            buttonY();
        if (kDown & KEY_MINUS)
            buttonMinus();
        if (kHeld & KEY_UP || kHeld & KEY_DOWN)
            buttonUpDown();
        if (kDown & KEY_LEFT || kDown & KEY_RIGHT)
            buttonLeftRight();
        if (kDown & KEY_L || kDown & KEY_R)
            buttonLR();
        if (kDown & KEY_LSTICK)
            buttonLStick();
    }

    socketExit();
    fontExit();
    plExit();
    esExit();
    nsextExit();
    nsExit();
    ncmExit();
    setsysExit();

    gfxExit();
    return 0;
}