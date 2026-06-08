#pragma once
#include <stdlib.h>

#define CHUNK_SIZE 16
#define CHUNK_HEIGHT 128

void renderBlock(int x, int y, int z, uint8_t Chunk[CHUNK_SIZE][CHUNK_HEIGHT][CHUNK_SIZE]);
