gcc main.c player.c tiles.c -o main.exe -O1 -Wall -std=c99 -Wno-missing-braces --static -I./include -L./lib -lpthread -lraylib -lopengl32 -lgdi32 -lwinmm
