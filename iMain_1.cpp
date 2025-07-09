#include "iGraphics.h"
#include "iSound.h"
#include <time.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <ctype.h>

using namespace std;

struct PlayerScore {
    string name;
    int score;
    bool operator<(const PlayerScore& other) const {
        return score > other.score;
    }
};

vector<PlayerScore> highScores;
const string HIGHSCORE_FILE = "highscores.txt";
const int MAX_HIGHSCORES = 5;

char playerName[50] = "";
int playerNameIndex = 0;
bool inputName = false;
int finalScore = 0;
float obstacleSpeed = 2.0;
int health = 3;

char hab[10][13] = {"hab001_1.png","hab002_1.png","hab003_1.png","hab004_1.png","hab005_1.png",
                    "hab006_1.png","hab007_1.png","hab008_1.png","hab009_1.png","hab010_1.png"};
char obs[3][10] = {"obs1.png","obs2.png","obs3.png"};
char cloud[7][13] = {"cloud1.png","cloud2.png","cloud3.png","cloud4.png","cloud5.png","cloud6.png","cloud7.png"};
char menu[5][15] = {"menu_1.png","menu_2.png","menu_3.png","menu_4.png","menu_5.png"};
char menuBig[5][20] = {"menu_1_big.png","menu_2_big.png","menu_3_big.png","menu_4_big.png","menu_5_big.png"};

bool hoverMenu[5] = {false};
int menuX[5] = {350, 850, 100, 610, 1120};
int menuY[5] = {220, 200, 20, 20, 20};
int menuW = 200, menuH = 200;


Image menuBigImg[5], cloudImg[7], skyImg, heartbreak, menuImg[5], menuImghab, back;
Image habimgfrm[10], obsimgfrm[3];
Sprite habimg[10], obsimg[3];

int gamestate = 0;
int score = 0, scoretick = 0;
int bi = 0;
int obsx[3], obsy[3], obsi[3];
int x = 700, y = 70;
int easy;
bool isObsActive[3] = {true, true, true};
bool showHeartbreak = false;
int heartbreakCounter = 0;

double c1x=200,c2x=350,c3x=500,c4x=800,c5x=600,c6x=920,c7x=1225;
double c1y=114,c2y=229,c3y=343,c4y=457,c5y=500,c6y=686,c7y=800;

void scoreupdate() {
    scoretick++;
    if(scoretick % 9 == 0) {
        score++;
        obstacleSpeed = 2.0 + (score * 0.025);
    }
}

void loadHighScores() {
    highScores.clear();
    ifstream file(HIGHSCORE_FILE);
    if (file.is_open()) {
        string name; int score;
        while (file >> name >> score) {
            highScores.push_back({name, score});
        }
        file.close();
        sort(highScores.begin(), highScores.end());
    }
}

void saveHighScores() {
    ofstream file(HIGHSCORE_FILE);
    for (int i = 0; i < min((int)highScores.size(), MAX_HIGHSCORES); ++i) {
        file << highScores[i].name << " " << highScores[i].score << endl;
    }
    file.close();
}

void addHighScore(string name, int score) {
    for (char &c : name) if (c == ' ') c = '_';
    highScores.push_back({name, score});
    sort(highScores.begin(), highScores.end());
    if (highScores.size() > MAX_HIGHSCORES)
        highScores.resize(MAX_HIGHSCORES);
    saveHighScores();
    loadHighScores(); 
}

void move() {
    if (isSpecialKeyPressed(GLUT_KEY_RIGHT)) x += 15;
    if (isSpecialKeyPressed(GLUT_KEY_LEFT)) x -= 15;
    if (x > 1360) x = -40;
    if (x < -40) x = 1360;
    iSetSpritePosition(&habimg[bi], x, y);
}

void obstacle() {
    for (int i = 0; i < 3; i++) {
        if (isObsActive[i]) {
            obsy[i] -= obstacleSpeed;
            if (obsy[i] < -50) {
                obsy[i] = 800;
                obsx[i] = rand() % 1300;
                obsi[i] = rand() % 3;
                iChangeSpriteFrames(&obsimg[i], &obsimgfrm[obsi[i]], 1);
            }
            iSetSpritePosition(&obsimg[i], obsx[i], obsy[i]);
        } else {
            obsy[i] -= obstacleSpeed;
            if (obsy[i] < -50) {
                isObsActive[i] = true;
                obsy[i] = 800;
                obsx[i] = rand() % 1300;
                obsi[i] = rand() % 3;
                iChangeSpriteFrames(&obsimg[i], &obsimgfrm[obsi[i]], 1);
            }
            iSetSpritePosition(&obsimg[i], obsx[i], obsy[i]);
        }
    }
}

void incmnt() {
    bi = (bi + 1) % 10;
    iChangeSpriteFrames(&habimg[bi], &habimgfrm[bi], 1);
    iSetSpritePosition(&habimg[bi], x, y);
}

void cldx() {
    double* cx[] = {&c1x, &c2x, &c3x, &c4x, &c5x, &c6x, &c7x};
    double speed[] = {.2, -.1, -.3, .5, .25, -.3, .8};
    for (int i = 0; i < 7; i++) {
        *cx[i] += speed[i];
               if (speed[i] > 0 && *cx[i] > 1450) *cx[i] = -150;
        
        else if (speed[i] < 0 && *cx[i] < -150) *cx[i] = 1450;
        }
    }

void cldy() {
    double* cy[] = {&c1y, &c2y, &c3y, &c4y, &c5y, &c6y, &c7y};
    for (int i = 0; i < 7; i++) {
        *cy[i] -= 2;
        if (*cy[i] < -50) *cy[i] = 800;
    }
}

void iDraw() {
    iClear();

    // Draw background and clouds
    iShowLoadedImage(0, 0, &skyImg);
    iShowLoadedImage(c1x, c1y, &cloudImg[0]);
    iShowLoadedImage(c2x, c2y, &cloudImg[1]);
    iShowLoadedImage(c3x, c3y, &cloudImg[2]);
    iShowLoadedImage(c4x, c4y, &cloudImg[3]);
    iShowLoadedImage(c5x, c5y, &cloudImg[4]);
    iShowLoadedImage(c6x, c6y, &cloudImg[5]);
    iShowLoadedImage(c7x, c7y, &cloudImg[6]);

    if (gamestate == 0) {
        // Main menu
        iShowLoadedImage(50, 530, &menuImghab);
        for (int i = 0; i < 5; i++) {
            if (hoverMenu[i]) 
                iShowLoadedImage(menuX[i] - 10, menuY[i] - 10, &menuBigImg[i]);
            else 
                iShowLoadedImage(menuX[i], menuY[i], &menuImg[i]);
        }
    }
    else if (gamestate == 1) {
        // Gameplay
        iShowSprite(&habimg[bi]);
        for (int i = 0; i < 3; i++) {
            if (isObsActive[i]) {
                iShowSprite(&obsimg[i]);
                if (iCheckCollision(&habimg[bi], &obsimg[i])) {
                    showHeartbreak = true;
                    heartbreakCounter = 60;
                    iPauseSound(easy);
                    iPlaySound("wrong.wav", false, 80);
                    health--;
                    isObsActive[i] = false;

                    if (health <= 0) {
                        finalScore = score;
                        addHighScore(playerName, finalScore);
                        gamestate = 6;  // Switch to Game Over screen
                    } else {
                        iResumeSound(easy);
                    }
                }
            }
        }

        if (showHeartbreak) {
            iShowLoadedImage(200, 400, &heartbreak);
            if (--heartbreakCounter <= 0) 
                showHeartbreak = false;
        }

        char str[30];
        sprintf(str, "Score: %d", score);
        iText(10, 760, str, GLUT_BITMAP_HELVETICA_18);
        sprintf(str, "Speed: %.2f", obstacleSpeed);
        iText(10, 740, str, GLUT_BITMAP_HELVETICA_18);
        char lifeStr[20];
        sprintf(lifeStr, "Lives: %d", health);
        iText(10, 720, lifeStr, GLUT_BITMAP_HELVETICA_18);
    }
    else if (gamestate == 3) {
        // Name input screen
        iSetColor(255, 255, 255);
        iText(100, 400, "Enter Your Name:", GLUT_BITMAP_HELVETICA_18);
        iText(100, 370, playerName, GLUT_BITMAP_HELVETICA_18);
        iText(100, 300, "Press ENTER to start the game.", GLUT_BITMAP_HELVETICA_18);
    }
    else if (gamestate == 4) {
        // About screen
        iShowImage(0, 0, "aboutus.PNG");
    }
    else if (gamestate == 6) {
        // Game Over screen
        iText(150, 450, "Game Over", GLUT_BITMAP_HELVETICA_18);
        char finalScoreStr[50];
        sprintf(finalScoreStr, "Final Score: %d", finalScore);
        iText(150, 420, finalScoreStr, GLUT_BITMAP_HELVETICA_18);

        // Show Back button so player can return to main menu
        iShowLoadedImage(420, 740, &back);
    }
    else if (gamestate == 2) {
    iSetColor(255, 255, 255);
    iText(180, 700, "High Scores", GLUT_BITMAP_HELVETICA_18);
    int startY = 650;
    for (size_t i = 0; i < highScores.size(); i++) {
        char scoreLine[100];
        sprintf(scoreLine, "%d. %s - %d", (int)(i+1), highScores[i].name.c_str(), highScores[i].score);
        iText(150, startY - i * 30, scoreLine, GLUT_BITMAP_HELVETICA_18);
    }
    // Back button to return to main menu
    iShowLoadedImage(420, 740, &back);
}

    // Show Back button on other screens except main menu and game over (which has own back button)
    if (gamestate > 0 && gamestate <= 5 && gamestate != 6) {
        iShowLoadedImage(420, 740, &back);
    }
}



/*
function iMouseMove() is called when the user moves the mouse.
(mx, my) is the position where the mouse pointer is.
*/
void iMouseMove(int mx, int my)
{
    // place your codes here
     for (int i = 0; i < 5; i++)
    {
        hoverMenu[i] = (mx >= menuX[i] && mx <= menuX[i] + menuW &&
                        my >= menuY[i] && my <= menuY[i] + menuH);
    }
}

/*
function iMouseDrag() is called when the user presses and drags the mouse.
(mx, my) is the position where the mouse pointer is.
*/
void iMouseDrag(int mx, int my)
{
    // place your codes here
}

/*
function iMouse() is called when the user presses/releases the mouse.
(mx, my) is the position where the mouse pointer is.
*/
void iMouse(int button, int state, int mx, int my)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        if(gamestate== 0) 
        {
            if(mx >= 350 && mx <= (350+190) && my >=220 && my <= (220+268)) // Start Game
            {
               gamestate = 3;
               inputName = true;
              playerName[0] = '\0';
               playerNameIndex = 0;
                iResumeSound(easy);
            }
            else if(mx >= 850 && mx <=(850+190) && my >=200 && my <=(200+268)) // high
            {
                gamestate=2;
                // Show instructions (not implemented)
            }
            else if(mx >= 100 && mx <= (100+190)&& my >= 20 && my <= (20+268)) // help
            {
                 
            }
            else if(mx >= 610 && mx <= (610+190) && my >= 20 && my <=288) // About
            {
                gamestate = 4;
            }
            else if(mx >=1120 && mx <= (1120+190)&& my >= 20 && my <=288 ) // exit
            {
                exit(0);
            }

        }
        if (gamestate >= 1 && gamestate <= 6)
    {
            if(mx >= 420 && mx <= 500 && my >= 740 && my <= 800) // Back button
            {
                gamestate = 0;
                iPauseSound(easy);
                iResumeSound(easy);
            }
        }
            
    }
}

/*
function iMouseWheel() is called when the user scrolls the mouse wheel.
dir = 1 for up, -1 for down.
*/
void iMouseWheel(int dir, int mx, int my)
{
    // place your code here
}
void iKeyboard(unsigned char key)
{
    if (gamestate == 3 && inputName) {
    if (key == '\b') { // backspace
        if (playerNameIndex > 0) {
            playerNameIndex--;
            playerName[playerNameIndex] = '\0';
        }
    } else if (key == '\r' || key == '\n') { // enter
        inputName = false;
        gamestate = 1;
        easy = iPlaySound("Easy.wav", true, 80);

        // Reset game values
        score = 0;
        scoretick = 0;
        health = 3;
        obstacleSpeed = 2.0;

        for (int i = 0; i < 3; i++) {
            obsy[i] = 800 + i * 267;
            obsx[i] = rand() % 1300;
            obsi[i] = rand() % 3;
            isObsActive[i] = true;
            iChangeSpriteFrames(&obsimg[i], &obsimgfrm[obsi[i]], 1);
            iSetSpritePosition(&obsimg[i], obsx[i], obsy[i]);
        }
    } else if (playerNameIndex < sizeof(playerName) - 1) {
        if (isalnum(key) || key == ' ') {
            playerName[playerNameIndex++] = key;
            playerName[playerNameIndex] = '\0';
        }
    }
}

}
void iSpecialKeyboard(unsigned char key)
{
   // start moving left
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    srand(time(0));
    for(int i = 0; i < 3; i++) {
        iLoadImage(&obsimgfrm[i], obs[i]);
    }
    iInitSprite(&habimg[0]);
    iChangeSpriteFrames(&habimg[0], &habimgfrm[0], 1);
    iSetSpritePosition(&habimg[0], x, y);
    loadHighScores();

    for (int i = 0; i < 3; i++) {
        iInitSprite(&obsimg[i]);
        obsi[i] = rand() % 3; // Randomly select an obstacle image
        iChangeSpriteFrames(&obsimg[i], &obsimgfrm[obsi[i]], 1);
        obsx[i] = rand() % 1300;
        obsy[i] = 800 + i * 267; // Start above the screen
        iSetSpritePosition(&obsimg[i], obsx[i], obsy[i]);
    }
    for (int i = 0; i < 7; i++) {
    iLoadImage(&cloudImg[i], cloud[i]); }


    for(int j=0;j<10;j++)
    {
        iLoadImage(&habimgfrm[j],hab[j]);
    }
    for(int i=0;i<5;i++)
    {
        iLoadImage(&menuImg[i],menu[i]);
        iLoadImage(&menuBigImg[i], menuBig[i]);
    }
    iLoadImage(&menuImghab, "menuhab.png");
    iLoadImage(&skyImg, "sky.jpg");
    iLoadImage(&heartbreak, "heartbreak.png");
    iLoadImage(&back, "backbutton.png");
    iSetTimer(500,incmnt);
    iSetTimer(16,cldx);
    iSetTimer(16,cldy);
    iSetTimer(1, move); 
    iSetTimer(16,obstacle); 
    iSetTimer(33,scoreupdate);
    iInitializeSound();
    if (gamestate == 1)
    {easy=iPlaySound("Easy.wav",true,80);} 
    iInitialize(1400, 800, "Hot Air Balloon");
    
    return 0;
}


