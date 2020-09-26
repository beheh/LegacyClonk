#!/bin/bash
echo "::set-env name=CMAKE_CONFIGURE_ARGS::-DCMAKE_BUILD_TYPE=RelWithDebInfo -DWITH_DEVELOPER_MODE=On"
