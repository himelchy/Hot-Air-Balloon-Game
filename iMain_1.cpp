#include "iGraphics.h"
#include "iSound.h"
#include <stdlib.h>

double x1 = 0, y11 = 0;

char hab[10][13] = {
    "hab001_1.png", "hab002_1.png", "hab003_1.png",
    "hab004_1.png", "hab005_1.png", "hab006_1.png",
    "hab007_1.png", "hab008_1.png", "hab009_1.png", "hab010_1.png"
};

char cloud[7][13] = {
    "cloud1.png", "cloud2.png", "cloud3.png",
    "cloud4.png", "cloud5.png", "cloud6.png", "cloud7.png"
};

// Declare image structs
Image habImg[10];
Image cloudImg[7];
Image skyImg;

int i = 0;
int x = 200, y = 70;

int c1 = 0, c2 = 150, c3 = 200, c4 = 400, c5 = 250, c6 = 320, c7 = 425;

void move() {
    if (isSpecialKeyPressed(GLUT_KEY_RIGHT)) x += 5;
    if (isSpecialKeyPressed(GLUT_KEY_LEFT)) x -= 5;

    if (x > 460) x = -40;
    if (x < -40) x = 460;
}

void incmnt() {
    i++;
    if (i >= 10)
        i = 0;
}

void cld() {
    c1 += 2;
    if (c1 > 490) c1 = -150;
    c2 -= 2;
    if (c2 < -100) c2 = 550;
    c3 -= 3;
    if (c3 < -100) c3 = 550;
    c4 += 1;
    if (c4 > 490) c4 = -150;
    c5 += 2;
    if (c5 > 490) c5 = -150;
    c6 -= 1;
    if (c6 < -100) c6 = 550;
    c7 += 3;
    if (c7 > 490) c7 = -150;
}

void iDraw() {
    iClear();

    iShowLoadedImage(0, 0, &skyImg);

    iShowLoadedImage(c1, 600, &cloudImg[0]);
    iShowLoadedImage(c2, 690, &cloudImg[1]);
    iShowLoadedImage(c3, 520, &cloudImg[2]);
    iShowLoadedImage(c4, 370, &cloudImg[3]);
    iShowLoadedImage(c5, 450, &cloudImg[4]);
    iShowLoadedImage(c6, 550, &cloudImg[5]);
    iShowLoadedImage(c7, 500, &cloudImg[6]);

    iShowLoadedImage(x, y, &habImg[i]);
}

void iKeyboard(unsigned char key) {
    if (key == 'q') {
        exit(0);
    }
}

void iSpecialKeyboard(unsigned char key) {
    // left and right handled in move()
}

int main(int argc, char* argv[]) {
    glutInit(&argc, argv);

    // Load character animation frames
    for (int j = 0; j < 10; j++) {
        iLoadImage(&habImg[j], hab[j]);
    }

    // Load clouds
    for (int j = 0; j < 7; j++) {
        iLoadImage(&cloudImg[j], cloud[j]);
    }

    // Load sky background
    iLoadImage(&skyImg, "sky.jpg");

    // Set timers
    iSetTimer(50, incmnt); // For character animation
    iSetTimer(16, cld);    // For cloud movement
    iSetTimer(1, move);    // For smoother player movement

    // Init sound
    iInitializeSound();
    iPlaySound("Easy.wav", true, 80);

    // Init window
    iInitialize(500, 800, "Demo Rewritten by GPT 🛠️");

    return 0;
}
