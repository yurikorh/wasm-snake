if(-Not(Test-Path .\build))
{
    New-Item -Path .\build -ItemType Directory
}
emcc web.cc snake.cc -s USE_SDL=2 -s ASSERTIONS=1 -s SAFE_HEAP=1 -s TOTAL_MEMORY=134217728 -O3 -o build/index.html