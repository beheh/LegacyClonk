#!/bin/bash
echo "::set-env name=CMAKE_CONFIGURE_ARGS::-A x64 -C $PWD/config.txt"
echo "::set-env name=VS_ARCH::amd64"
