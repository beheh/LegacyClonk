#!/bin/bash
echo "::set-env name=CMAKE_BUILD_ARGS::--config RelWithDebInfo"
echo "set(EXTRA_LINK_LIBS user32 gdi32 winmm imm32 ole32 oleaut32 version uuid advapi32 setupapi shell32 CACHE STRING \"Extra libs needed for static linking\")" > config.txt
