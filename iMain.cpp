#include "iGraphics.h"
#include "iSound.h"
#include <time.h> 
#include <stdlib.h> 
double x1=0,y11=0; 
char hab[10][13]={"hab001_1.png","hab002_1.png","hab003_1.png","hab004_1.png","hab005_1.png","hab006_1.png","hab007_1.png","hab008_1.png","hab009_1.png","hab010_1.png"};
char obs[3][10]={"obs1.png","obs2.png","obs3.png"};
char cloud[7][13]={"cloud1.png","cloud2.png","cloud3.png","cloud4.png","cloud5.png","cloud6.png","cloud7.png"};
char menu[5][15]={"menu_1.png","menu_2.png","menu_3.png","menu_4.png","menu_5.png"};
char menuBig[5][20] = {"menu_1_big.png","menu_2_big.png","menu_3_big.png","menu_4_big.png","menu_5_big.png"};
bool hoverMenu[5] = {false};
int menuX[5] = {100, 300, 20, 185, 350};
int menuY[5] = {300, 300, 50, 50, 50};
int menuW = 120, menuH = 160;
Image menuBigImg[5];
Image cloudImg[7];
Image skyImg;
Image heartbreak;
Image menuImg[5];
Image menuImghab;
bool showHeartbreak = false;
int heartbreakCounter = 0;
Sprite habimg[10];
Sprite obsimg[3];
Image habimgfrm[10];
Image obsimgfrm[3];
int gamestate=0;
int score=0,scoretick=0;
int bi=0;
int obsx[3],obsy[3],obsi[3];
int x=200,y=70;
int easy,wrong;
void scoreupdate()
{
    scoretick++;
    if(scoretick%9==0)
      score++;
}
void move()
{
    if(isSpecialKeyPressed(GLUT_KEY_RIGHT))x+=15;
    if(isSpecialKeyPressed(GLUT_KEY_LEFT)) x-=15;
    if(x > 460) x = -40;
    if(x < -40) x = 460;
    iSetSpritePosition(&habimg[bi], x, y);
}

double c1x=100,c2x=150,c3x=200,c4x=400,c5x=250,c6x=320,c7x=425;
double c1y=114,c2y=229,c3y=343,c4y=457,c5y=500,c6y=686,c7y=800;
double yy=800;

bool isObsActive[3] = {true, true, true};
void obstacle()
{
    for(int i=0;i<3;i++)
    {
        if(isObsActive[i]){
            obsy[i]-=2;
        if(obsy[i]<-50)
        {
                obsy[i]=800;
                obsx[i]=rand()%300;
                obsi[i]=rand()%3;
                iChangeSpriteFrames(&obsimg[i], &obsimgfrm[obsi[i]], 1);   
        }
        iSetSpritePosition(&obsimg[i], obsx[i], obsy[i]);
    }
    else{
          obsy[i] -= 2;

        if (obsy[i] < -50)
            {
                isObsActive[i] = true; // Reactivate it!

                obsy[i] = 800;
                obsx[i] = rand() % 300;
                obsi[i] = rand() % 3;
                iChangeSpriteFrames(&obsimg[i], &obsimgfrm[obsi[i]], 1);
               isObsActive[i]=true;
            }
                        iSetSpritePosition(&obsimg[i], obsx[i], obsy[i]);
    }
    }
}
/*bool collision(int x1, int y1,int h1,int w1, int x2, int y2,int h2, int w2)
{
    return (x1 < x2 + w2 && x1 + w1 > x2 && y1 < y2 + h2 && y1 + h1 > y2);
}*/

    
void incmnt()
{
    bi++;
    if(bi==10)
     bi=0;
     iChangeSpriteFrames(&habimg[bi], &habimgfrm[bi], 1);
     iSetSpritePosition(&habimg[bi], x, y);
}
void cldx()
{
    c1x+=.2;
    if(c1x>490)c1x=-150;
    c2x-=.1;
    if(c2x<-100)c2x=550;
    c3x-=.3;
    if(c3x<-100)c3x=550;
    c4x+=.5;
    if(c4x>490)c4x=-150;
    c5x+=.25;
    if(c5x>490)c5x=-150;
    c6x-=.3;
    if(c6x<-100)c6x=550;
    c7x+=.8;
    if(c7x>490)c7x=-150;
}
void cldy()
{
    c1y-=2;
    if(c1y<-50)c1y=800;
    c2y-=2;
    if(c2y<-50)c2y=800;
    c3y-=2;
    if(c3y<-50)c3y=800;
    c4y-=2;
    if(c4y<-50)c4y=800; 
    c5y-=2;
    if(c5y<-50)c5y=800;
    c6y-=2;
    if(c6y<-50)c6y=800;
    c7y-=2;
    if(c7y<-50)c7y=800; 
}
void iDraw()
{
    iClear();
   
    
    iShowLoadedImage(0,0,&skyImg);
    iShowLoadedImage(c1x,c1y,&cloudImg[0]);
    iShowLoadedImage(c2x,c2y,&cloudImg[1]);
    iShowLoadedImage(c3x,c3y,&cloudImg[2]);
    iShowLoadedImage(c4x,c4y,&cloudImg[3]);
    iShowLoadedImage(c5x,c5y,&cloudImg[4]);
    iShowLoadedImage(c6x,c6y,&cloudImg[5]);
    iShowLoadedImage(c7x,c7y,&cloudImg[6]);
     if(gamestate==0)
    {
      iShowLoadedImage(50, 530, &menuImghab);

    for (int i = 0; i < 5; i++)
    {
        if (hoverMenu[i])
        {
            // Show big image (assume it's already pre-sized)
            iShowLoadedImage(menuX[i] - 10, menuY[i] - 10, &menuBigImg[i]);
        }
        else
        {
            // Show normal image
            iShowLoadedImage(menuX[i], menuY[i], &menuImg[i]);
        }
    }
    }

    if(gamestate==1){
      iShowSprite(&habimg[bi]);
  for(int i=0;i<3;i++)
  {
    if(isObsActive[i])
    {
    iShowSprite(&obsimg[i]);
    if(iCheckCollision(&habimg[bi], &obsimg[i]))
    {
         showHeartbreak = true;
    heartbreakCounter = 60;
        iPauseSound(easy);
        iPlaySound("wrong.wav", false, 80);
        iResumeSound(easy);
        isObsActive[i] = false; 
    }
    }
  }

if(showHeartbreak)
{
    iShowLoadedImage(200, 400, &heartbreak);
    heartbreakCounter--;
    if(heartbreakCounter <= 0)
    {
        showHeartbreak = false;
    }
}
char str[30];
sprintf(str,"Score: %d",score);
iText(10,760,str,GLUT_BITMAP_HELVETICA_18);
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
            if(mx >= 110 && mx <= 220 && my >=310 && my <= 462) // Start Game
            {
                gamestate = 1;
                iResumeSound(easy);
            }
            else if(mx >= 290 && mx <=420 && my >=310 && my <=470) // Instructions
            {
                gamestate=2;
                // Show instructions (not implemented)
            }
            else if(mx >= 30 && mx <= 140 && my >= 60 && my <= 210) // Exit
            {
                gamestate = 3;
            }
            else if(mx >= 195 && mx <= 310 && my >= 60 && my <=220) // About
            {
                gamestate = 4;
            }
            else if(mx >= 360 && mx <= 480 && my >= 60 && my <= 210) // Credits
            {
                gamestate = 5;
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
    switch (key)
    {
    case 'q':
        // do something with 'q'
        break;
    // place your codes for other keys here
    default:
        break;
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

    for (int i = 0; i < 3; i++) {
        iInitSprite(&obsimg[i]);
        obsi[i] = rand() % 3; // Randomly select an obstacle image
        iChangeSpriteFrames(&obsimg[i], &obsimgfrm[obsi[i]], 1);
        obsx[i] = rand() % 300;
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
    iSetTimer(500,incmnt);
    iSetTimer(16,cldx);
    iSetTimer(16,cldy);
    iSetTimer(1, move); 
    iSetTimer(16,obstacle); 
    iSetTimer(33,scoreupdate);
    iInitializeSound();
    if (gamestate == 1)
    {easy=iPlaySound("Easy.wav",true,80);} 
    iInitialize(500, 800, "Hot Air Balloon");
    
    return 0;
}

