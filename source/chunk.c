#include <nds.h>
#include <stdlib.h>
#include "chunk.h"
#include "render.h" // para renderCube ou suas funções de blocos

uint8_t hasBlock[32][32][32];

void grass(int angle, int x, int y, int z, int px, int py, int pz);
void stone(int angle, int x, int y, int z, int px, int py, int pz);

void initChunk(Chunk* chunk) {
    for (int x = 0; x < CHUNK_SIZE; x++)
        for (int y = 0; y < CHUNK_SIZE; y++)
            for (int z = 0; z < CHUNK_SIZE; z++)
                chunk->blocks[x][y][z] = rand() % MAX_BLOCKS; // 0-99
}

uint8_t getBlock(Chunk* chunk, int x, int y, int z) {
    if (x < 0 || y < 0 || z < 0 || x >= CHUNK_SIZE || y >= CHUNK_SIZE || z >= CHUNK_SIZE)
        return 0; // vazio
    return chunk->blocks[x][y][z];
}

void setBlock(Chunk* chunk, int x, int y, int z, uint8_t blockType) {
    if (x < 0 || y < 0 || z < 0 || x >= CHUNK_SIZE || y >= CHUNK_SIZE || z >= CHUNK_SIZE)
        return;
    chunk->blocks[x][y][z] = blockType;
}

void chunkGenerator(int startX, int startY, int startZ, int px, int py, int pz) {
    int block;
    for (int x = 0; x < CHUNK_SIZE; x++) {
        for (int y = 0; y < CHUNK_SIZE; y++) {
            for (int z = 0; z < CHUNK_SIZE; z++) {
                if (y < 5) block = 2;
                else block = 1;
                int worldX = startX + x;
                int worldY = startY + y;
                int worldZ = startZ + z;

                

                switch (block) {
                case 1: grass(0, worldX, worldY, worldZ, px, py, pz); break;
                case 2: stone(0, worldX, worldY, worldZ, px, py, pz); break;
                default: break;
                }
            }
        }
    }
}

void grass(int angle, int x, int y, int z, int px, int py, int pz) {
    hasBlock[x][y][z] = 1;

    glPushMatrix();
    glTranslatef(x, y, z);
    glRotatef32i(degreesToAngle(angle), inttof32(1), inttof32(1), inttof32(1));

    glBegin(GL_QUADS);

    if (hasBlock[x][y][z - 1] == 0 & pz < z + 1) {
        glColor3b(100, 65, 35);
        glVertex3f(0, 0, 0);
        glVertex3f(0, 1, 0);
        glVertex3f(1, 1, 0);
        glVertex3f(1, 0, 0);
    }

    if (hasBlock[x - 1][y][z] == 0 & px < x + 1) {
        glColor3b(100, 65, 35);
        glVertex3f(0, 0, 1);
        glVertex3f(0, 1, 1);
        glVertex3f(0, 1, 0);
        glVertex3f(0, 0, 0);
    }

    if (hasBlock[x][y][z + 1] == 0 & pz > z - 1) {
        glColor3b(100, 65, 35);
        glVertex3f(1, 0, 1);
        glVertex3f(1, 1, 1);
        glVertex3f(0, 1, 1);
        glVertex3f(0, 0, 1);
    }

    if (hasBlock[x + 1][y][z] == 0 & px > x - 1) {
        glColor3b(100, 65, 35);
        glVertex3f(1, 0, 0);
        glVertex3f(1, 1, 0);
        glVertex3f(1, 1, 1);
        glVertex3f(1, 0, 1);
    }

    if (hasBlock[x][y - 1][z] == 0 & py < y + 1) {
        glColor3b(100, 65, 35);
        glVertex3f(0, 0, 0);
        glVertex3f(1, 0, 0);
        glVertex3f(1, 0, 1);
        glVertex3f(0, 0, 1);
    }

    if (hasBlock[x][y + 1][z] == 0 & py > y - 1) {
        glColor3b(0, 200, 0);
        glVertex3f(0, 1, 1);
        glVertex3f(1, 1, 1);
        glVertex3f(1, 1, 0);
        glVertex3f(0, 1, 0);
    }
    glEnd();
    glPopMatrix(1);
}
void stone(int angle, int x, int y, int z, int px, int py, int pz) {
    hasBlock[x][y][z] = 1;

    glPushMatrix();
    glTranslatef(x, y, z);
    glRotatef32i(degreesToAngle(angle), inttof32(1), inttof32(1), inttof32(1));

    glBegin(GL_QUADS);

    glColor3b(80, 80, 80);

    if (hasBlock[x][y][z - 1] == 0 & pz < z + 1) {
        glVertex3f(0, 0, 0);
        glVertex3f(0, 1, 0);
        glVertex3f(1, 1, 0);
        glVertex3f(1, 0, 0);
    }

    if (hasBlock[x - 1][y][z] == 0 & px < x + 1) {
        glVertex3f(0, 0, 1);
        glVertex3f(0, 1, 1);
        glVertex3f(0, 1, 0);
        glVertex3f(0, 0, 0);
    }

    if (hasBlock[x][y][z + 1] == 0 & pz > z - 1) {
        glVertex3f(1, 0, 1);
        glVertex3f(1, 1, 1);
        glVertex3f(0, 1, 1);
        glVertex3f(0, 0, 1);
    }

    if (hasBlock[x + 1][y][z] == 0 & px > x - 1) {
        glVertex3f(1, 0, 0);
        glVertex3f(1, 1, 0);
        glVertex3f(1, 1, 1);
        glVertex3f(1, 0, 1);
    }

    if (hasBlock[x][y - 1][z] == 0 & py < y + 1) {
        glVertex3f(0, 0, 0);
        glVertex3f(1, 0, 0);
        glVertex3f(1, 0, 1);
        glVertex3f(0, 0, 1);
    }

    if (hasBlock[x][y + 1][z] == 0 & py > y - 1) {
        glVertex3f(0, 1, 1);
        glVertex3f(1, 1, 1);
        glVertex3f(1, 1, 0);
        glVertex3f(0, 1, 0);
    }

    glEnd();
    glPopMatrix(1);
}