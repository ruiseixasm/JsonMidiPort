#pragma once

#include <windows.h>
#include <functional>
#include <string>

class TrayIcon
{
public:
    TrayIcon();
    ~TrayIcon();

    bool create(HINSTANCE instance, const std::wstring& tooltip);
    void destroy();

    void setEnabled(bool enabled);
    bool isEnabled() const;

    bool processMessage(MSG& message);

    void setOnToggle(std::function<void(bool)> callback);
    void setOnExit(std::function<void()> callback);

private:
    static constexpr UINT WM_TRAYICON = WM_APP + 1;

    static constexpr UINT ID_TOGGLE = 1001;
    static constexpr UINT ID_EXIT   = 1002;

    HINSTANCE instance_;
    HWND window_;
    NOTIFYICONDATAW trayIcon_;
    bool enabled_;

    std::function<void(bool)> onToggle_;
    std::function<void()> onExit_;

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

    void showContextMenu();
    void updateIcon();
};
