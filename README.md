# Snake-wasm

Snake in C++ for WebAssembly

This is a primitive greedy snake game in C++ish to test WebAssembly

## Getting Started

### Install emscripten

You'll need [emscripten](https://kripken.github.io/emscripten-site/docs/getting_started/downloads.html) to compile the program 

### Compile

```
emcc web.cc snake.cc -s USE_SDL=2 -s ASSERTIONS=1 -s SAFE_HEAP=1 -s TOTAL_MEMORY=134217728 -O3 -o index.html
```

You'll need a http server like [Live Server](https://marketplace.visualstudio.com/items?itemName=ritwickdey.LiveServer) to view the result, or you can use `python3 -m http.server`.


## Demo

