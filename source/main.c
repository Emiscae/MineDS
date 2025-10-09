#include <nds.h>
#include <stdio.h>
#include <math.h>
#include "render.h"
#include "chunk.h"
#include <gl2d.h>


#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

void drawCrosshair();

extern uint8_t hasBlock[32][32][32];

float moveSpeed = 0.2;

Chunk myChunk;

int main(void) {
    float y = 9, z = 4, x = 4, cloudSpeed = 0;
    float sensibilidade = 1.5f;
    float camRotX = 0, camRotY = 0;

    videoSetMode(MODE_0_3D);
    videoSetModeSub(MODE_0_2D);
    vramSetBankA(VRAM_A_MAIN_BG);
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

    glPolyFmt(POLY_ALPHA(31) | POLY_CULL_BACK);

    initChunk(&myChunk);

    while (1) {
        swiWaitForVBlank();

        float radY = camRotY * (M_PI / 180.0f);
        float forwardX = sinf(radY);
        float forwardZ = -cosf(radY);

        cloudSpeed += 0.01f;

        scanKeys();
        int keys = keysHeld();

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
        if (keys & KEY_START) {
            y += moveSpeed;
        }
        if (keys & KEY_SELECT) {
            y -= moveSpeed;
        }

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glRotateX(camRotX);
        glRotateY(camRotY);
        glRotatef32i(camRotX, inttof32(1), 0, 0);
        glRotatef32i(camRotY, 0, inttof32(1), 0);
        glTranslatef(-x, -y, -z);


        glBoxFilled(10,
            10,
            50,
            50,
            RGB15(31, 31, 31)
        );
        //forma(0, 0, -2, -10);
        //renderScene(10, 10, -10, -16);
        //renderChunk(&myChunk, -10, -4, -10);
        

        /*test(0, 0, 0, 0);
        grass(0, 0, 0, 0);
        grass(0, 0, 1, 0);*/
        //renderScene(10, 10, 0, 0);
        //forma(0, 5, 1, 5);

        /*grass(0, 6, 3, 0);
        grass(0, 7, 3, 0);
        grass(0, 8, 3, 0);
        grass(0, 9, 3, 0);
        grass(0, 10, 3, 0);*/


        chunkGenerator(0, 0, 0, x, y, z);
        grass(0, 3, 9, 3, x, y, z);
        stone(0, 3, 8, 3, x, y, z);
        //chunkGenerator(0, 0, 8, yChecker);
        //chunkGenerator(8, 0, 0, yChecker);
        //chunkGenerator(8, 0, 8, yChecker);
        //chunkGenerator(16, 0, 0);
        //chunkGenerator(32, 0, 0);
        //chunkGenerator(64, 0, 0);

        /*renderCloud(0, 30, cloudSpeed);
        renderCloud(34, 30, cloudSpeed - 20);
        renderCloud(12, 30, cloudSpeed - 24);
        renderCloud(10, 30, cloudSpeed + 36);
        renderCloud(42, 30, cloudSpeed + 24);*/



        consoleClear();
        iprintf("\nX: %d\nY: %d\nZ: %d\n", (int)x, (int)y, (int)z);
        iprintf("camRotY: %d\nradY: %d\n", (int)camRotY, (int)radY);
        iprintf("Tamanho hasBlock: %zu bytes\n", sizeof(hasBlock));

        drawCrosshair();

        glFlush(0);

        
    }

    return 0;
}

void drawCrosshair() {
    int centerX = 256 / 2;
    int centerY = 192 / 2;
    float size = 0.5;

    glBegin2D();


    glBoxFilled(centerX - size, centerY - size, centerX + size, centerY + size, RGB15(31, 31, 31));


    //glBoxFilled(centerX - 1, centerY, centerX, centerY + size + 1, RGB15(31, 31, 31));



    glEnd2D();
}

