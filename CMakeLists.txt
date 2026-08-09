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

# List all available generators command: cmake --help
# Run cmake with a specific generator selected: cmake -g "MinGW Makefiles" ..
cmake_minimum_required(VERSION 3.15)
project(JsonMidiPort
        VERSION 1.0.0
        DESCRIPTION "Very simple MIDI Player of JSON files"
        HOMEPAGE_URL "https://github.com/ruiseixasm/JsonMidiPort"
        LANGUAGES CXX)

# Generates a compile_commands.json file containing the exact compiler calls for all translation units of the project in machine-readable form.
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Include directories
include_directories(src single_include)

# Add main.cpp explicitly
set(STATIC_SOURCES src/JsonMidiPort.cpp)

# Add main.cpp explicitly
set(EXECUTABLE_SOURCES main.cpp src/JsonMidiPort.cpp)

# Add the executable target
add_executable(${EXECUTABLE_NAME} ${EXECUTABLE_SOURCES})

add_compile_definitions(__WINDOWS_MM__)

