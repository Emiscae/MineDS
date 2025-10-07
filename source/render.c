#include <nds.h>
#include <math.h>
#include "render.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

//-------------------------------------------------------
void renderCube(int angle, float x, float y, float z) {
    glPushMatrix();
    glTranslatef(x, y, z);
    glRotatef32i(degreesToAngle(angle), inttof32(1), inttof32(1), inttof32(1));

    glBegin(GL_QUADS);

    // Frente
    glColor3b(100, 65, 35);
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
    glColor3b(0, 200, 0);
    glVertex3f(-1.0f, 1.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, -1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);

    // Base
    glColor3b(100, 65, 35);
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

//-------------------------------------------------------
void forma(int angle, float x, float y, float z) {
    renderCube(angle, x, y, z);
    renderCube(angle, x - 2, y, z);
    renderCube(angle, x + 2, y, z);
    renderCube(angle, x - 4, y + 2, z);
    renderCube(angle, x + 4, y + 2, z);
    renderCube(angle, x - 2, y + 6, z);
    renderCube(angle, x + 2, y + 6, z);
}

//-------------------------------------------------------
void renderScene(int v, int h, int startX, int startZ) {
    for (int i = 0; i < v; i++) {
        for (int j = 0; j < h; j++) {
            int x = startX + j * 2;
            int z = startZ + i * 2;
            renderCube(0, x, -4, z);
        }
    }
}

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