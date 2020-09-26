#!/bin/bash
echo "::set-env name=CMAKE_CONFIGURE_ARGS::-A Win32 -C $PWD/config.txt"
echo "::set-env name=VS_ARCH::x86"
