# Snake-wasm

Snake in C++ for WebAssembly

This is a primitive greedy snake game demo in C++(ish) to get started with WebAssembly

![deploy workflow](https://github.com/yurikorh/wasm-snake/actions/workflows/main.yml/badge.svg)
## Getting Started

### Install emscripten

[emscripten](https://kripken.github.io/emscripten-site/docs/getting_started/downloads.html) is required for compiling. 

### Compile

```
emcc web.cc snake.cc -s USE_SDL=2 -s ASSERTIONS=1 -s SAFE_HEAP=1 -s TOTAL_MEMORY=134217728 -O3 -o index.html
```

You can use a http server like [Live Server](https://marketplace.visualstudio.com/items?itemName=ritwickdey.LiveServer) to view the demo, or use    
`python3 -m http.server`.

### Notes    
 
SDL2 API reference [SDL Wiki](https://wiki.libsdl.org/APIByCategory)

## Demo

https://yurikorh.github.io/wasm-snake/
