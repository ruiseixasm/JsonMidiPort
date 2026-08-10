/*
JsonMidiPort - Json Midi Player is intended to be used
in conjugation with the Json Midi Creator to Play its composed Elements
Original Copyright (c) 2026 Rui Seixas Monteiro. All right reserved.
This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.
This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
Lesser General Public License for more details.
https://github.com/ruiseixasm/JsonMidiPlayer
https://github.com/ruiseixasm/JsonMidiPort
*/

#include "TrayIcon.h"

int WINAPI wWinMain(
    HINSTANCE instance,
    HINSTANCE,
    PWSTR,
    int)
{
    TrayIcon trayIcon(instance);

	if (!trayIcon.create())
	{
		MessageBoxW(
			nullptr,
			L"TrayIcon::create() failed.",
			L"JsonMidiPort",
			MB_OK | MB_ICONERROR
		);

		return 1;
	}

    MSG message{};

    while (GetMessageW(&message, nullptr, 0, 0) > 0)
    {
        TranslateMessage(&message);
        DispatchMessageW(&message);
    }

    return 0;
}
