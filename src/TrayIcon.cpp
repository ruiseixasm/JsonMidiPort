#include "TrayIcon.h"

#include <shellapi.h>

#include "../resources/resource.h"

TrayIcon::TrayIcon(HINSTANCE instance)
    : instance_(instance),
      window_(nullptr),
      iconData_{},
      enabled_(false)
{
}

TrayIcon::~TrayIcon()
{
    destroy();
}

bool TrayIcon::create()
{
    const wchar_t* className = L"JsonMidiPortTray";

    WNDCLASSW wc{};
    wc.lpfnWndProc = TrayIcon::windowProc;
    wc.hInstance = instance_;
    wc.lpszClassName = className;

    if (!RegisterClassW(&wc) &&
        GetLastError() != ERROR_CLASS_ALREADY_EXISTS)
    {
        MessageBoxW(
            nullptr,
            L"RegisterClassW failed.",
            L"JsonMidiPort",
            MB_OK | MB_ICONERROR
        );

        return false;
    }

    window_ = CreateWindowExW(
        0,
        className,
        L"JsonMidiPort",
        0,
        0, 0, 0, 0,
        nullptr,
        nullptr,
        instance_,
        this
    );

    if (!window_)
    {
        DWORD error = GetLastError();

        wchar_t message[256];
        swprintf_s(
            message,
            256,
            L"CreateWindowExW failed.\nError: %lu",
            error
        );

        MessageBoxW(
            nullptr,
            message,
            L"JsonMidiPort",
            MB_OK | MB_ICONERROR
        );

        return false;
    }

    iconData_.cbSize = sizeof(iconData_);
    iconData_.hWnd = window_;
    iconData_.uID = 1;
    iconData_.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
    iconData_.uCallbackMessage = WM_TRAYICON;

    iconData_.hIcon = LoadIconW(
        instance_,
        MAKEINTRESOURCEW(IDI_JSONMIDIPORT)
    );

    if (!iconData_.hIcon)
    {
        MessageBoxW(
            nullptr,
            L"Failed to load JsonMidiPort icon.",
            L"JsonMidiPort",
            MB_OK | MB_ICONERROR
        );

        return false;
    }

    lstrcpyW(
        iconData_.szTip,
        L"JsonMidiPort"
    );

    if (!Shell_NotifyIconW(NIM_ADD, &iconData_))
    {
        DWORD error = GetLastError();

        wchar_t message[256];

        swprintf_s(
            message,
            L"Shell_NotifyIconW failed.\nError: %lu",
            error
        );

        MessageBoxW(
            nullptr,
            message,
            L"JsonMidiPort",
            MB_OK | MB_ICONERROR
        );

        DestroyWindow(window_);
        window_ = nullptr;

        return false;
    }

    return true;
}

void TrayIcon::destroy()
{
    if (!window_)
        return;

    Shell_NotifyIconW(
        NIM_DELETE,
        &iconData_
    );

    DestroyWindow(window_);
    window_ = nullptr;
}

void TrayIcon::setEnabled(bool enabled)
{
    enabled_ = enabled;
}

void TrayIcon::showMenu()
{
    HMENU menu = CreatePopupMenu();

    if (!menu)
        return;

    AppendMenuW(
        menu,
        MF_STRING,
        enabled_ ? ID_DISABLE : ID_ENABLE,
        enabled_ ? L"Disable" : L"Enable"
    );

    AppendMenuW(
        menu,
        MF_SEPARATOR,
        0,
        nullptr
    );

    AppendMenuW(
        menu,
        MF_STRING,
        ID_EXIT,
        L"Exit"
    );

    POINT point;
    GetCursorPos(&point);

    SetForegroundWindow(window_);

    TrackPopupMenu(
        menu,
        TPM_RIGHTBUTTON,
        point.x,
        point.y,
        0,
        window_,
        nullptr
    );

    DestroyMenu(menu);
}

LRESULT CALLBACK TrayIcon::windowProc(
    HWND hwnd,
    UINT message,
    WPARAM wParam,
    LPARAM lParam)
{
    TrayIcon* trayIcon = nullptr;

    if (message == WM_NCCREATE)
    {
        auto* create =
            reinterpret_cast<CREATESTRUCTW*>(lParam);

        trayIcon =
            static_cast<TrayIcon*>(create->lpCreateParams);

        SetWindowLongPtrW(
            hwnd,
            GWLP_USERDATA,
            reinterpret_cast<LONG_PTR>(trayIcon)
        );
    }
    else
    {
        trayIcon =
            reinterpret_cast<TrayIcon*>(
                GetWindowLongPtrW(hwnd, GWLP_USERDATA)
            );
    }

    if (trayIcon)
    {
        return trayIcon->handleMessage(
            hwnd,
            message,
            wParam,
            lParam
        );
    }

    return DefWindowProcW(
        hwnd,
        message,
        wParam,
        lParam
    );
}

LRESULT TrayIcon::handleMessage(
    HWND hwnd,
    UINT message,
    WPARAM wParam,
    LPARAM lParam)
{
    if (message == WM_TRAYICON)
    {
        if (lParam == WM_RBUTTONUP)
        {
            showMenu();
            return 0;
        }
    }

    if (message == WM_COMMAND)
    {
        switch (LOWORD(wParam))
        {
        case ID_ENABLE:
            setEnabled(true);
            return 0;

        case ID_DISABLE:
            setEnabled(false);
            return 0;

        case ID_EXIT:
            PostQuitMessage(0);
            return 0;
        }
    }

    return DefWindowProcW(
        hwnd,
        message,
        wParam,
        lParam
    );
}
