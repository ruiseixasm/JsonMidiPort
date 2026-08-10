#pragma once

#include <windows.h>

class TrayIcon
{
public:
    TrayIcon(HINSTANCE instance);
    ~TrayIcon();

    bool create();
    void destroy();

private:
    HINSTANCE instance_;
    HWND window_;
    NOTIFYICONDATAW iconData_;
    bool enabled_;

    static constexpr UINT WM_TRAYICON = WM_APP + 1;
    static constexpr UINT ID_ENABLE = 1001;
    static constexpr UINT ID_DISABLE = 1002;
    static constexpr UINT ID_EXIT = 1003;

    static LRESULT CALLBACK windowProc(
        HWND hwnd,
        UINT message,
        WPARAM wParam,
        LPARAM lParam
    );

    LRESULT handleMessage(
        UINT message,
        WPARAM wParam,
        LPARAM lParam
    );

    void showMenu();
    void setEnabled(bool enabled);
};