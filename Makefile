all:
    gcc -I src/include -L src/lib -o main main.C -lmingw32 -lSDL2main -LSDL2