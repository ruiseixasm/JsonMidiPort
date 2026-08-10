#[===[
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
]===]
cmake_minimum_required(VERSION 3.15)

project(JsonMidiPort
    VERSION 1.0.0
    DESCRIPTION "A Windows virtual MIDI port"
    HOMEPAGE_URL "https://github.com/ruiseixasm/JsonMidiPort"
    LANGUAGES CXX
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(JsonMidiPort WIN32
    src/main.cpp
    src/JsonMidiPort.cpp
    src/TrayIcon.cpp
    resources/JsonMidiPort.rc
)

target_include_directories(JsonMidiPort PRIVATE
    src
    resources
)
