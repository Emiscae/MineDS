#include <nds.h>
#include <stdio.h>
#include <math.h>
#include "render.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

float moveSpeed = 0.2;

int main(void) {
    float y = 0, z = 0, a = 0, x = 0, cloudSpeed = 0;
    float sensibilidade = 1.5f;
    float camRotX = 0, camRotY = 0;

    videoSetMode(MODE_0_3D);
    videoSetModeSub(MODE_0_2D);
    consoleDemoInit();

    glInit();
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_ANTIALIAS);

    glClearColor(41, 215, 254, 31);
    glClearPolyID(63);
    glClearDepth(0x7FFF);
    glViewport(0, 0, 255, 191);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(70, 256.0 / 192.0, 0.1, 100);

    glPolyFmt(POLY_ALPHA(31) | POLY_CULL_NONE);

    while (1) {
        swiWaitForVBlank();

        float radY = camRotY * (M_PI / 180.0f);
        float forwardX = sinf(radY);
        float forwardZ = -cosf(radY);

        cloudSpeed += 0.01f;

        scanKeys();
        int keys = keysHeld();
        if (keys & KEY_START) break;

        if (keys & KEY_Y) camRotY -= sensibilidade;
        if (keys & KEY_A) camRotY += sensibilidade;
        if (keys & KEY_X && camRotX >= -90) camRotX -= sensibilidade;
        if (keys & KEY_B && camRotX <= 90) camRotX += sensibilidade;

        if (keys & KEY_UP) {
            x += forwardX * moveSpeed;
            z += forwardZ * moveSpeed;
        }
        if (keys & KEY_DOWN) {
            x -= forwardX * moveSpeed;
            z -= forwardZ * moveSpeed;
        }

        float rightX = cosf(radY);
        float rightZ = sinf(radY);
        if (keys & KEY_LEFT) {
            x -= rightX * moveSpeed;
            z -= rightZ * moveSpeed;
        }
        if (keys & KEY_RIGHT) {
            x += rightX * moveSpeed;
            z += rightZ * moveSpeed;
        }

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glRotateX(camRotX);
        glRotateY(camRotY);
        glRotatef32i(camRotX, inttof32(1), 0, 0);
        glRotatef32i(camRotY, 0, inttof32(1), 0);
        glTranslatef(-x, 0, -z);

        forma(0, 0, -2, -10);
        renderScene(10, 10, -10, -16);

        renderCloud(0, 30, cloudSpeed);
        renderCloud(34, 30, cloudSpeed - 20);
        renderCloud(12, 30, cloudSpeed - 24);
        renderCloud(10, 30, cloudSpeed + 36);
        renderCloud(42, 30, cloudSpeed + 24);



        consoleClear();
        iprintf("\nX: %d\nY: %d\nZ: %d\n", (int)x, (int)y, (int)z);
        iprintf("camRotY: %d\nradY: %d\n", (int)camRotY, (int)radY);

        glFlush(0);
    }

    return 0;
}
