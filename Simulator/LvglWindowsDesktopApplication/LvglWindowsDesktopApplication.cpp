#pragma warning(disable : 4244)

#include <Windows.h>
#include <thread>
#include <iostream>
#include <iomanip>
#include <LvglWindowsIconResource.h>

#include "lvgl/lvgl.h"
#include "ui/ui.h"
#include "CommonData.h"
#include "CommonService.h"
#include "CommonLibrary.h"

int WINAPI wWinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR lpCmdLine,
    _In_ int nShowCmd)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    UNREFERENCED_PARAMETER(nShowCmd);

    ::lv_init();

    int32_t zoom_level = 100;
    bool allow_dpi_override = false;
    bool simulator_mode = false;
    lv_display_t* display = ::lv_windows_create_display(
        L"Buckshot_Roulette",
        800,
        480,
        zoom_level,
        allow_dpi_override,
        simulator_mode);
    if (!display)
    {
        return -1;
    }

    HWND window_handle = ::lv_windows_get_display_window_handle(display);
    if (!window_handle)
    {
        return -1;
    }

    HICON icon_handle = ::LoadIconW(
        hInstance,
        MAKEINTRESOURCE(IDI_LVGL_WINDOWS));
    if (icon_handle)
    {
        ::SendMessageW(
            window_handle,
            WM_SETICON,
            TRUE,
            (LPARAM)icon_handle);
        ::SendMessageW(
            window_handle,
            WM_SETICON,
            FALSE,
            (LPARAM)icon_handle);
    }

    lv_indev_t* pointer_indev = ::lv_windows_acquire_pointer_indev(display);
    if (!pointer_indev)
    {
        return -1;
    }

    lv_indev_t* keypad_indev = ::lv_windows_acquire_keypad_indev(display);
    if (!keypad_indev)
    {
        return -1;
    }

    lv_indev_t* encoder_indev = ::lv_windows_acquire_encoder_indev(display);
    if (!encoder_indev)
    {
        return -1;
    }

    // Create UI
    ui_init();

    // Init service
    InitData();

    // Init GUI
    Init();

    while (true)
    {
        ::lv_timer_handler();
        CommonServiceProcess();
        AutoUpdate();
        UpdateAll();
        ::Sleep(10);
    }

    lv_deinit();

    ::FreeConsole();

    return 0;
}
