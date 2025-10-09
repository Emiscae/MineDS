#include <nds.h>
#include <math.h>
#include "render.h"
#include "chunk.h"

#define CHUNK_SIZE 8

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

typedef struct {
    bool active;
} Block;

Block chunk[CHUNK_SIZE][CHUNK_SIZE][CHUNK_SIZE];

bool hasNeighbor(int x, int y, int z, int nx, int ny, int nz) {
    int xx = x + nx;
    int yy = y + ny;
    int zz = z + nz;

    if (xx < 0 || xx >= CHUNK_SIZE || yy < 0 || yy >= CHUNK_SIZE || zz < 0 || zz >= CHUNK_SIZE)
        return false;

    return chunk[xx][yy][zz].active;
}



void test(int angle, float x, float y, float z) {
    glPushMatrix();
    glTranslatef(x, y, z);
    glRotatef32i(degreesToAngle(angle), inttof32(1), inttof32(1), inttof32(1));

    glBegin(GL_QUADS);

    // Frente
    glColor3b(255, 222, 89);
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glVertex3f(-0.5f, 0.5f, -0.5f);
    glVertex3f(0.5f, 0.5f, -0.5f);
    glVertex3f(0.5f, -0.5f, -0.5f);

    glVertex3f(-0.5f, -0.5f, 0.5f);
    glVertex3f(-0.5f, 0.5f, 0.5f);
    glVertex3f(-0.5f, 0.5f, -0.5f);
    glVertex3f(-0.5f, -0.5f, -0.5f);

    glVertex3f(0.5f, -0.5f, 0.5f);
    glVertex3f(0.5f, 0.5f, 0.5f);
    glVertex3f(-0.5f, 0.5f, 0.5f);
    glVertex3f(-0.5f, -0.5f, 0.5f);

    glVertex3f(0.5f, -0.5f, -0.5f);
    glVertex3f(0.5f, 0.5f, -0.5f);
    glVertex3f(0.5f, 0.5f, 0.5f);
    glVertex3f(0.5f, -0.5f, 0.5f);

    glVertex3f(-0.5f, -0.5f, -0.5f);
    glVertex3f(0.5f, -0.5f, -0.5f);
    glVertex3f(0.5f, -0.5f, 0.5f);
    glVertex3f(-0.5f, -0.5f, 0.5f);

    glVertex3f(-0.5f, 0.5f, 0.5f);
    glVertex3f(0.5f, 0.5f, 0.5f);
    glVertex3f(0.5f, 0.5f, -0.5f);
    glVertex3f(-0.5f, 0.5f, -0.5f);

    glEnd();
    glPopMatrix(1);
}

//-------------------------------------------------------
/*void grass(int angle, float x, float y, float z) {
    glPushMatrix();
    glTranslatef(x, y, z);
    glRotatef32i(degreesToAngle(angle), inttof32(1), inttof32(1), inttof32(1));

    glBegin(GL_QUADS);

    if (!hasNeighbor(x, y, z, 0, 0, -1)) {
        glColor3b(100, 65, 35);
        glVertex3f(0, 0, 0);
        glVertex3f(0, 1, 0);
        glVertex3f(1, 1, 0);
        glVertex3f(1, 0, 0);
    }

    if (!hasNeighbor(x, y, z, 1, 0, 0)) {
        glColor3b(204, 108, 231);
        glVertex3f(0, 0, 1);
        glVertex3f(0, 1, 1);
        glVertex3f(0, 1, 0);
        glVertex3f(0, 0, 0);
    }

    if (!hasNeighbor(x, y, z, 0, 0, 1)) {
        glColor3b(152, 245, 249);
        glVertex3f(1, 0, 1);
        glVertex3f(1, 1, 1);
        glVertex3f(0, 1, 1);
        glVertex3f(0, 0, 1);
    }

    if (!hasNeighbor(x, y, z, -1, 0, 0)) {
        glColor3b(210, 1, 3);
        glVertex3f(1, 0, 0);
        glVertex3f(1, 1, 0);
        glVertex3f(1, 1, 1);
        glVertex3f(1, 0, 1);
    }

    if (!hasNeighbor(x, y, z, 0, 0, 0)) {
        glVertex3f(0, 0, 0);
        glVertex3f(1, 0, 0);
        glVertex3f(1, 0, 1);
        glVertex3f(0, 0, 1);
    }

    if (!hasNeighbor(x, y, z, 0, 1, 0)) {
        glColor3b(0, 200, 0);
        glVertex3f(0, 1, 1);
        glVertex3f(1, 1, 1);
        glVertex3f(1, 1, 0);
        glVertex3f(0, 1, 0);
    }

    glEnd();
    glPopMatrix(1);
}*/

/*void stone(int angle, float x, float y, float z) {
    glPushMatrix();
    glTranslatef(x, y, z);
    glRotatef32i(degreesToAngle(angle), inttof32(1), inttof32(1), inttof32(1));

    glBegin(GL_QUADS);

    // Frente
    glColor3b(80, 80, 80);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 1, 0);
    glVertex3f(1, 1, 0);
    glVertex3f(1, 0, 0);

    glVertex3f(0, 0, 1);
    glVertex3f(0, 1, 1);
    glVertex3f(0, 1, 0);
    glVertex3f(0, 0, 0);

    glVertex3f(1, 0, 1);
    glVertex3f(1, 1, 1);
    glVertex3f(0, 1, 1);
    glVertex3f(0, 0, 1);

    glVertex3f(1, 0, 0);
    glVertex3f(1, 1, 0);
    glVertex3f(1, 1, 1);
    glVertex3f(1, 0, 1);

    glVertex3f(0, 0, 0);
    glVertex3f(1, 0, 0);
    glVertex3f(1, 0, 1);
    glVertex3f(0, 0, 1);

    glVertex3f(0, 1, 1);
    glVertex3f(1, 1, 1);
    glVertex3f(1, 1, 0);
    glVertex3f(0, 1, 0);

    glEnd();
    glPopMatrix(1);
}*/

//-------------------------------------------------------
/*void forma(int angle, float x, float y, float z) {
    grass(angle, x, y, z);
    grass(angle, x - 1, y, z);
    grass(angle, x + 1, y, z);
    grass(angle, x - 2, y + 1, z);
    grass(angle, x + 2, y + 1, z);
    grass(angle, x - 1, y + 3, z);
    grass(angle, x + 1, y + 3, z);
}*/

//-------------------------------------------------------
/*void renderScene(int v, int h, int startX, int startZ) {
    for (int i = 0; i < v; i++) {
        for (int j = 0; j < h; j++) {
            int x = startX + j;
            int z = startZ + i;
            grass(0, x, 0, z);
        }
    }
}*/

void renderCloud(float x, float y, float z) {
    glPushMatrix();
    glTranslatef(x, y, z);
    glRotatef32i(degreesToAngle(0), inttof32(1), inttof32(1), inttof32(1));

    // Escala para formar um paralelepípedo 3x5x8
    glScalef(6.0f, 2.0f, 8.0f);

    glBegin(GL_QUADS);

    // Frente
    glColor3b(255, 255, 255);
    glVertex3f(-1.0f, 1.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);

    // Trás
    glVertex3f(-1.0f, 1.0f, -1.0f);
    glVertex3f(1.0f, 1.0f, -1.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);

    // Topo
    glColor3b(255, 255, 255);
    glVertex3f(-1.0f, 1.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, -1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);

    // Base
    glColor3b(255, 255, 255);
    glVertex3f(-1.0f, -1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);

    // Lados
    glVertex3f(1.0f, 1.0f, -1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);

    glVertex3f(-1.0f, 1.0f, -1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);

    glEnd();
    glPopMatrix(1);
}