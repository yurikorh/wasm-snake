#!/bin/bash
if [ ! -d "build" ]; then
  mkdir build
fi
emcc web.cc snake.cc -s USE_SDL=2 -s ASSERTIONS=1 -s SAFE_HEAP=1 -s TOTAL_MEMORY=134217728 -s LLD_REPORT_UNDEFINED -O3 -o build/index.html
