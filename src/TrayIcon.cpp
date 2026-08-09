#include "TrayIcon.h"

#include <shellapi.h>
#include <windowsx.h>

TrayIcon::TrayIcon()
    : instance_(nullptr),
      window_(nullptr),
      trayIcon_{},
      enabled_(false)
{
}

TrayIcon::~TrayIcon()
{
    destroy();
}

bool TrayIcon::create(
    HINSTANCE instance,
    const std::wstring& tooltip)
{
    instance_ = instance;

    // Register a private window class used to receive tray messages.
    const wchar_t* className = L"JsonMidiPortTrayWindow";

    WNDCLASSW windowClass{};
    windowClass.lpfnWndProc = &TrayIcon::windowProc;
    windowClass.hInstance = instance_;
    windowClass.lpszClassName = className;

    if (!RegisterClassW(&windowClass))
    {
        if (GetLastError() != ERROR_CLASS_ALREADY_EXISTS)
            return false;
    }

    // Create an invisible window.
    window_ = CreateWindowExW(
        0,
        className,
        L"JsonMidiPort",
        0,
        0,
        0,
        0,
        0,
        HWND_MESSAGE,
        nullptr,
        instance_,
        this
    );

    if (!window_)
        return false;

    trayIcon_ = {};

    trayIcon_.cbSize = sizeof(NOTIFYICONDATAW);
    trayIcon_.hWnd = window_;
    trayIcon_.uID = 1;

    trayIcon_.uFlags =
        NIF_MESSAGE |
        NIF_ICON |
        NIF_TIP;

    trayIcon_.uCallbackMessage = WM_TRAYICON;

    // Temporary standard Windows application icon.
    trayIcon_.hIcon = LoadIconW(
        nullptr,
        IDI_APPLICATION
    );

    wcsncpy_s(
        trayIcon_.szTip,
        tooltip.c_str(),
        _TRUNCATE
    );

    if (!Shell_NotifyIconW(
            NIM_ADD,
            &trayIcon_))
    {
        DestroyWindow(window_);
        window_ = nullptr;
        return false;
    }

    updateIcon();

    return true;
}

void TrayIcon::destroy()
{
    if (!window_)
        return;

    Shell_NotifyIconW(
        NIM_DELETE,
        &trayIcon_
    );

    DestroyWindow(window_);
    window_ = nullptr;
}

void TrayIcon::setEnabled(bool enabled)
{
    enabled_ = enabled;
    updateIcon();
}

bool TrayIcon::isEnabled() const
{
    return enabled_;
}

void TrayIcon::setOnToggle(
    std::function<void(bool)> callback)
{
    onToggle_ = std::move(callback);
}

void TrayIcon::setOnExit(
    std::function<void()> callback)
{
    onExit_ = std::move(callback);
}

bool TrayIcon::processMessage(MSG& message)
{
    if (message.message == WM_QUIT)
        return false;

    TranslateMessage(&message);
    DispatchMessageW(&message);

    return true;
}

void TrayIcon::updateIcon()
{
    if (!window_)
        return;

    trayIcon_.hIcon = LoadIconW(
        nullptr,
        enabled_
            ? IDI_INFORMATION
            : IDI_APPLICATION
    );

    trayIcon_.uFlags = NIF_ICON;

    Shell_NotifyIconW(
        NIM_MODIFY,
        &trayIcon_
    );
}

void TrayIcon::showContextMenu()
{
    HMENU menu = CreatePopupMenu();

    if (!menu)
        return;

    AppendMenuW(
        menu,
        MF_STRING,
        ID_TOGGLE,
        enabled_
            ? L"Turn Off"
            : L"Turn On"
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

    POINT cursorPosition{};
    GetCursorPos(&cursorPosition);

    // Required for popup menus associated with tray icons.
    SetForegroundWindow(window_);

    TrackPopupMenu(
        menu,
        TPM_RIGHTBUTTON,
        cursorPosition.x,
        cursorPosition.y,
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
        const auto* createStruct =
            reinterpret_cast<CREATESTRUCTW*>(lParam);

        trayIcon =
            static_cast<TrayIcon*>(createStruct->lpCreateParams);

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
                GetWindowLongPtrW(
                    hwnd,
                    GWLP_USERDATA
                )
            );
    }

    if (trayIcon)
    {
        return trayIcon->handleMessage(
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
    UINT message,
    WPARAM wParam,
    LPARAM lParam)
{
    if (message == WM_TRAYICON)
    {
        switch (LOWORD(lParam))
        {
        case WM_RBUTTONUP:
            showContextMenu();
            return 0;

        case WM_LBUTTONUP:
            enabled_ = !enabled_;
            updateIcon();

            if (onToggle_)
                onToggle_(enabled_);

            return 0;
        }
    }

    if (message == WM_COMMAND)
    {
        switch (LOWORD(wParam))
        {
        case ID_TOGGLE:
            enabled_ = !enabled_;
            updateIcon();

            if (onToggle_)
                onToggle_(enabled_);

            return 0;

        case ID_EXIT:
            if (onExit_)
                onExit_();

            return 0;
        }
    }

    return DefWindowProcW(
        window_,
        message,
        wParam,
        lParam
    );
}
