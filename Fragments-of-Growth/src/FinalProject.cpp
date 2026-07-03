#define _CRT_SECURE_NO_WARNINGS
#define GL_SILENCE_DEPRECATION
#include <GL/glew.h>
#include <GL/freeglut.h> 
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <math.h>
#include <vector>
#include <string>

/*

Student Name: Safaa Ahmed Sobh
Student ID: 446000075



Student Name:Shatha Hamdan Alghamdi
Student ID: 446000126



Student Name: Diala Jamil Qutub
Student ID: 446001937



Student Name:Abrar Yahya Al-Masoudi
Student ID:446004459



Student Name:Dana Abdullah Al-Sulmi
Student ID: 446006208



Student Name:Nour Amin Zehrawi
Student ID:446016977

*/


// ---------------------- Global Variables ----------------------
int currentScene = 1;
bool showAbout = false;
bool showHelp = false;
bool identitySolved = false;

float playerX = -0.75f;
float playerY = -0.45f;
float playerScale = 0.28f;

float fragmentAngle = 0.0f;
float glowValue = 0.0f;
bool glowIncreasing = true;

bool connectionSolved = false;

bool node1Active = false;
bool node2Active = false;
bool node3Active = false;
bool node4Active = false;


float node1X = -0.8f, node1Y = 0.3f;
float node2X = -0.4f, node2Y = 0.3f;
float node3X = 0.0f, node3Y = 0.3f;
float node4X = 0.4f, node4Y = 0.3f;

bool fake1Visible = true;
bool fake2Visible = true;

bool fragmentCollected = false;
bool claritySolved = false;

// Fragment positions
float fake1X = -0.15f, fake1Y = 0.25f;
float fake2X = 0.35f, fake2Y = -0.25f;
float realX = 0.65f, realY = 0.30f;

// Variables for desert scene 
float charX = 0.0; // character position 
float fallAngle = 0.0; // rotation for character when losing 





//--------------texture loading function (for the cover image)
GLuint LoadTexture(const char* filename)
{
    GLuint texture;
    unsigned char header[54];
    FILE* file = fopen(filename, "rb");

    if (!file) {
        // Return 0 so the program skips the texture instead of crashing
        std::cout << "Texture file not found: " << filename << std::endl;
        return 0;
    }

    if (fread(header, 1, 54, file) != 54) {
        fclose(file);
        return 0;
    }

    int width = *(int*)&(header[18]);
    int height = *(int*)&(header[22]);
    int imageSize = width * height * 3;

    unsigned char* data = (unsigned char*)malloc(imageSize);
    fread(data, 1, imageSize, file);
    fclose(file);

    for (int i = 0; i < width * height; ++i) {
        int index = i * 3;
        std::swap(data[index], data[index + 2]);
    }

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

    free(data);
    return texture;
}

void drawTexturedQuad(GLuint textureID) {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glColor3f(1.0f, 1.0f, 1.0f); // Set to white so texture colors show correctly

    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex2f(-1.0f, -1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex2f(1.0f, -1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex2f(1.0f, 1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex2f(-1.0f, 1.0f);
    glEnd();

    glDisable(GL_TEXTURE_2D);
}





// ---------------------- Basic Drawing Functions ----------------------
void drawText(float x, float y, const char* text, void* font = GLUT_BITMAP_HELVETICA_18)
{
    glRasterPos2f(x, y);
    while (*text)
    {
        glutBitmapCharacter(font, *text);
        text++;
    }
}

void drawFilledCircle(float x, float y, float radius)
{
    int triangleAmount = 40;
    float twicePi = 2.0f * 3.14159f;

    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x, y);
    for (int i = 0; i <= triangleAmount; i++)
    {
        glVertex2f(
            x + (radius * cos(i * twicePi / triangleAmount)),
            y + (radius * sin(i * twicePi / triangleAmount))
        );
    }
    glEnd();
}

void drawRectangle(float x1, float y1, float x2, float y2)
{
    glBegin(GL_POLYGON);
    glVertex2f(x1, y1);
    glVertex2f(x2, y1);
    glVertex2f(x2, y2);
    glVertex2f(x1, y2);
    glEnd();
}


// ---------------------- Center text function
void drawCenteredText(float y, const char* text)
{
    int width = 0;

    // calculate text width to center it
    for (int i = 0; i < strlen(text); i++)
    {
        width += glutBitmapWidth(GLUT_BITMAP_HELVETICA_18, text[i]);
    }
    float x = -(width / 600.0f);

    drawText(x, y, text);
}

// ---------------------- Main Character ----------------------
void MyMainObj()
{
    // Body
    glColor3f(0.75f, 0.45f, 0.30f);
    drawRectangle(-0.3f, -0.4f, 0.3f, 0.2f);

    // Ears background
    drawFilledCircle(-0.3f, 0.2f, 0.1f);
    drawFilledCircle(0.3f, 0.2f, 0.1f);

    // Ears
    glColor3f(0.4f, 0.2f, 0.1f);
    drawFilledCircle(-0.3f, 0.2f, 0.08f);
    drawFilledCircle(0.3f, 0.2f, 0.08f);

    glColor3f(1.0f, 0.6f, 0.7f);
    drawFilledCircle(-0.3f, 0.2f, 0.04f);
    drawFilledCircle(0.3f, 0.2f, 0.04f);

    // Face
    glColor3f(1.0f, 0.85f, 0.75f);
    drawRectangle(-0.2f, -0.15f, 0.2f, 0.1f);

    // Eyes
    glColor3f(0.3f, 0.15f, 0.05f);
    drawRectangle(-0.1f, -0.08f, -0.05f, 0.0f);
    drawRectangle(0.05f, -0.08f, 0.1f, 0.0f);

    // Nose
    drawFilledCircle(0.0f, -0.05f, 0.02f);

    // Arms
    glColor3f(0.7f, 0.4f, 0.25f);
    drawRectangle(-0.4f, -0.25f, -0.3f, -0.05f);
    drawRectangle(0.3f, -0.25f, 0.4f, -0.05f);

    // Legs
    glColor3f(0.5f, 0.25f, 0.15f);
    drawRectangle(-0.15f, -0.55f, -0.05f, -0.4f);
    drawRectangle(0.05f, -0.55f, 0.15f, -0.4f);
}

void drawPlayer()
{
    glPushMatrix();
    glTranslatef(playerX, playerY, 0.0f);
    glScalef(playerScale, playerScale, 1.0f);
    MyMainObj();
    glPopMatrix();
}

// ---------------------- Scene 1: Cover ----------------------
GLuint myTexture1;
char image1Path[] = "C:\\Users\\sosos\\source\\repos\\Project14\\cover.bmp";


void drawCoverFragment()
{
    glPushMatrix();
    glTranslatef(0.0f, 0.260f, 0.0f);
    glRotatef(fragmentAngle, 0.0f, 0.0f, 1.0f); // Rotation
    glScalef(.7f, .7f, 1.0f);              // Scaling

    glColor3f(0.55f + glowValue, 0.55f + glowValue, 0.55f + glowValue);

    glBegin(GL_POLYGON);
    glVertex2f(0.0f, 0.45f);
    glVertex2f(0.18f, 0.15f);
    glVertex2f(0.45f, 0.0f);
    glVertex2f(0.18f, -0.15f);
    glVertex2f(0.0f, -0.45f);
    glVertex2f(-0.18f, -0.15f);
    glVertex2f(-0.45f, 0.0f);
    glVertex2f(-0.18f, 0.15f);
    glEnd();

    glPopMatrix();
}


void drawCoverScene()
{
    if (myTexture1 != 0) {
        drawTexturedQuad(myTexture1);
    }
    else {
        // Fallback: draw a solid dark background if texture is missing
        glColor3f(0.05f, 0.05f, 0.05f);
        drawRectangle(-1.0f, -1.0f, 1.0f, 1.0f);
    }

    glColor3f(0.92f, 0.92f, 0.92f);
    drawText(-.23f, 0.62f, "Fragments of Hope", GLUT_BITMAP_TIMES_ROMAN_24);

    drawCoverFragment();

    glColor3f(0.86f, 0.86f, 0.86f);
    drawText(-.23f, -0.18f, "Press ENTER to Start");
    drawText(-.2f, -0.31f, "Press A for About");
    drawText(-.2f, -0.44f, "Press H for Help");
    drawText(-.21f, -0.57f, "Press ESC to Exit");

    if (showAbout)
    {
        glColor3f(0.75f, 0.75f, 0.75f);
        drawText(-0.62f, -0.68f, "A grayscale world regains hope by collecting glowing fragments.");
    }
    if (showHelp)
    {
        glColor3f(0.75f, 0.75f, 0.75f);
        drawText(-0.62f, -0.68f, "1- Use arrow keys to move.");
        drawText(-0.62f, -0.75f, "2- Use n to go to the next scene.");
        drawText(-0.62f, -0.82f, "3- Use r to reset the current scene.");
        drawText(-0.62f, -0.89f, "4- Use mouse to move objects when needed.");
        drawText(-0.62f, -0.96f, "5- Use ESC to exit.");
    }
}
// ---------------------- Scene 2: Forest Path ----------------------
void drawTree(float x, float y, float scale)
{
    if (identitySolved)
        glColor3f(0.35f, 0.18f, 0.07f);
    else
        glColor3f(0.25f, 0.25f, 0.25f);

    glPushMatrix();
    glTranslatef(x, y, 0.0f);
    glScalef(scale, scale, 1.0f);

    drawRectangle(-0.05f, -0.4f, 0.05f, 0.1f);

    if (identitySolved)
        glColor3f(0.05f, 0.45f, 0.18f);
    else
        glColor3f(0.33f, 0.33f, 0.33f);

    glBegin(GL_TRIANGLES);
    glVertex2f(-0.25f, 0.0f);
    glVertex2f(0.25f, 0.0f);
    glVertex2f(0.0f, 0.45f);
    glEnd();

    glBegin(GL_TRIANGLES);
    glVertex2f(-0.22f, 0.22f);
    glVertex2f(0.22f, 0.22f);
    glVertex2f(0.0f, 0.62f);
    glEnd();

    glPopMatrix();
}

void drawForestBackground()
{
    if (identitySolved)
        glColor3f(0.45f, 0.72f, 0.95f);
    else
        glColor3f(0.18f, 0.18f, 0.18f);
    drawRectangle(-1.0f, -0.2f, 1.0f, 1.0f);

    if (identitySolved)
        glColor3f(0.18f, 0.55f, 0.20f);
    else
        glColor3f(0.28f, 0.28f, 0.28f);
    drawRectangle(-1.0f, -1.0f, 1.0f, -0.2f);

    if (identitySolved)
        glColor3f(0.72f, 0.55f, 0.33f);
    else
        glColor3f(0.43f, 0.43f, 0.43f);

    glBegin(GL_POLYGON);
    glVertex2f(-0.32f, -1.0f);
    glVertex2f(0.32f, -1.0f);
    glVertex2f(0.12f, -0.2f);
    glVertex2f(-0.12f, -0.2f);
    glEnd();

    drawTree(-0.80f, -0.10f, 0.85f);
    drawTree(-0.52f, 0.02f, 0.65f);
    drawTree(0.78f, -0.08f, 0.85f);
    drawTree(0.48f, 0.06f, 0.65f);
}

void drawFragment(float x, float y, bool real)
{
    glPushMatrix();
    glTranslatef(x, y, 0.0f);
    glRotatef(fragmentAngle, 0.0f, 0.0f, 1.0f);
    glScalef(0.16f, 0.16f, 1.0f);

    if (real)
        glColor3f(0.25f + glowValue, 0.95f, 0.35f + glowValue);
    else
        glColor3f(0.55f + glowValue, 0.55f + glowValue, 0.55f + glowValue);

    glBegin(GL_POLYGON);
    glVertex2f(0.0f, 0.45f);
    glVertex2f(0.18f, 0.15f);
    glVertex2f(0.45f, 0.0f);
    glVertex2f(0.18f, -0.15f);
    glVertex2f(0.0f, -0.45f);
    glVertex2f(-0.18f, -0.15f);
    glVertex2f(-0.45f, 0.0f);
    glVertex2f(-0.18f, 0.15f);
    glEnd();

    glPopMatrix();
}

bool isColliding(float x1, float y1, float x2, float y2)
{
    float dx = x1 - x2;
    float dy = y1 - y2;
    float distance = sqrt(dx * dx + dy * dy);
    return distance < 0.16f;
}

void checkFragmentCollision()
{
    if (fake1Visible && isColliding(playerX, playerY, fake1X, fake1Y))
        fake1Visible = false;

    if (fake2Visible && isColliding(playerX, playerY, fake2X, fake2Y))
        fake2Visible = false;

    if (!identitySolved && isColliding(playerX, playerY, realX, realY))
        identitySolved = true;
}

void drawScene2Forest()
{
    drawForestBackground();

    glColor3f(0.95f, 0.95f, 0.95f);
    drawText(-0.92f, 0.88f, "Scene 2: Forest Path - Identity");
    drawText(-0.92f, 0.78f, "Use arrow keys to move. Find the real fragment.");

    if (fake1Visible)
        drawFragment(fake1X, fake1Y, false);

    if (fake2Visible)
        drawFragment(fake2X, fake2Y, false);

    if (!identitySolved)
        drawFragment(realX, realY, true);

    drawPlayer();

    if (identitySolved)
    {
        glColor3f(1.0f, 1.0f, 1.0f);
        drawText(-0.38f, -0.88f, "Identity restored! The forest has color again.");
    }
}

// ---------------------- Scene 3: Desert ----------------------
void drawDesert(bool dark);
void drawCrystals();



// ---------------------- Draw box 
void drawBox()
{
    glColor3f(0.4, 0.2, 0.1);

    glBegin(GL_QUADS);
    glVertex2f(-0.7, 0.5);
    glVertex2f(0.7, 0.5);
    glVertex2f(0.7, -0.3);
    glVertex2f(-0.7, -0.3);
    glEnd();
}


// ---------------------- Desert
// ground, mountains, cactus, rocks
void drawDesert(bool dark)
{
    // Sky
    if (dark)
        glColor3f(0.6, 0.6, 0.6);
    else
        glColor3f(0.4, 0.7, 1.0);

    glBegin(GL_QUADS);
    glVertex2f(-1, -0.3);
    glVertex2f(1, -0.3);
    glVertex2f(1, 1);
    glVertex2f(-1, 1);
    glEnd();


    // sun
    if (dark)
        glColor3f(0.5, 0.5, 0.5);
    else
        glColor3f(1.0, 0.9, 0.2);

    drawFilledCircle(0.8, 0.8, 0.12);


    // ground
    if (dark)
        glColor3f(0.5, 0.5, 0.5);
    else
        glColor3f(0.8, 0.65, 0.4);

    glBegin(GL_QUADS);
    glVertex2f(-1, -1);
    glVertex2f(1, -1);
    glVertex2f(1, -0.3);
    glVertex2f(-1, -0.3);
    glEnd();

    // big mountains
    if (dark)
        glColor3f(0.3, 0.3, 0.3);
    else
        glColor3f(0.45, 0.3, 0.15);

    glBegin(GL_TRIANGLES);
    glVertex2f(-0.8, -0.3);
    glVertex2f(-0.4, 0.3);
    glVertex2f(0.0, -0.3);

    glVertex2f(0.2, -0.3);
    glVertex2f(0.6, 0.4);
    glVertex2f(1.0, -0.3);
    glEnd();

    // small mountains
    if (dark)
        glColor3f(0.4, 0.4, 0.4);
    else
        glColor3f(0.7, 0.5, 0.25);

    glBegin(GL_TRIANGLES);
    glVertex2f(-0.2, -0.3);
    glVertex2f(0.0, 0.1);
    glVertex2f(0.2, -0.3);

    glVertex2f(0.3, -0.3);
    glVertex2f(0.45, 0.05);
    glVertex2f(0.6, -0.3);
    glEnd();

    // cactus
    if (dark)
        glColor3f(0.3, 0.3, 0.3);
    else
        glColor3f(0.0, 0.6, 0.2);

    glBegin(GL_QUADS);
    glVertex2f(-0.9, -0.3);
    glVertex2f(-0.85, -0.3);
    glVertex2f(-0.85, 0.0);
    glVertex2f(-0.9, 0.0);
    glEnd();

    // rocks
    if (dark)
        glColor3f(0.4, 0.4, 0.4);
    else
        glColor3f(0.35, 0.2, 0.1);

    drawFilledCircle(-0.2, -0.65, 0.05);
    drawFilledCircle(0.3, -0.75, 0.04);
    drawFilledCircle(0.6, -0.65, 0.03);
}

// ---------------------- Crystals
void drawCrystals()
{

    // black crystal (left)
    glColor3f(0.1, 0.1, 0.1);
    glBegin(GL_TRIANGLES);
    glVertex2f(-0.75, -0.5);
    glVertex2f(-0.68, -0.25);
    glVertex2f(-0.61, -0.5);
    glEnd();

    // red crystal (right)
    glColor3f(0.8, 0.1, 0.1);
    glBegin(GL_TRIANGLES);
    glVertex2f(0.60, -0.5);
    glVertex2f(0.67, -0.25);
    glVertex2f(0.74, -0.5);
    glEnd();
    // highlight on crystal
    glColor3f(1.0, 0.5, 0.5);
    glBegin(GL_TRIANGLES);
    glVertex2f(0.67, -0.25);
    glVertex2f(0.69, -0.35);
    glVertex2f(0.65, -0.35);
    glEnd();
    // shadow in front
    glColor3f(0.2, 0.0, 0.0);
    glBegin(GL_TRIANGLES);
    glVertex2f(0.60, -0.5);
    glVertex2f(0.74, -0.5);
    glVertex2f(0.67, -0.65);
    glEnd();
}

// Desert intro screen (story screen)
void drawDesertStart()
{

    glClearColor(1, 0.7, 0.5, 1); // background color
    glClear(GL_COLOR_BUFFER_BIT);

    drawBox(); // draw the box 

    glColor3f(1, 1, 1);

    drawText(-0.6f, 0.85f, "Scene 3: Desert - Courage");

    drawCenteredText(0.3, " In the desert..");
    drawCenteredText(0.2, "The road ahead is scary.");
    drawCenteredText(0.1, "Going back will not help.");
    drawCenteredText(0.0, "Being brave does not mean no fear...");
    drawCenteredText(-0.1, "It means you move even if you are scared.");
    drawCenteredText(-0.25, "Press V to continue");

    glPushMatrix();
    glTranslatef(-0.75, -0.4, 0);
    glScalef(0.35, 0.35, 1);
    MyMainObj();
    glPopMatrix();
}

// Gameplay screen
void drawDesertGame()
{
    drawDesert(true); // dark colors
    drawCrystals();
    // character movement
    glPushMatrix();
    glTranslatef(charX, -0.25, 0);
    glScalef(0.35, 0.35, 1);
    MyMainObj();
    glPopMatrix();

    glColor3f(1, 1, 1);
    drawCenteredText(0.8, "One way looks safe...");
    drawCenteredText(0.7, "The other way is in the storm...");
    drawCenteredText(0.6, "Move with arrows");
    drawCenteredText(0.5, "Press X for the left crystal  -  Press Z for the right crystal");
}
// Win screen
void drawDesertWin()
{
    drawDesert(false); // colored screen

    glPushMatrix();
    glTranslatef(charX, -0.25, 0);
    glScalef(0.35, 0.35, 1);
    MyMainObj();
    glPopMatrix();

    glColor3f(1, 1, 1);
    drawCenteredText(0.5, "You were brave....");
    drawCenteredText(0.4, "Your fear is going away.");
    drawCenteredText(0.3, "Press U to continue");
}
// Lose screen 
void drawDesertLose()
{
    drawDesert(true); // dark screen 

    glPushMatrix();
    glTranslatef(charX, -0.25, 0);
    glRotatef(fallAngle, 0, 0, 1);  // rotation when losing
    glScalef(0.35, 0.35, 1);
    MyMainObj();
    glPopMatrix();

    glColor3f(1, 1, 1);
    drawCenteredText(0.5, "Running away is easy...");
    drawCenteredText(0.4, "But it does not help..");
    drawCenteredText(0.3, "Press R to try again");
}
// Ending screen 
void drawDesertending()
{
    glClearColor(1, 0.7, 0.5, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    drawBox();

    glColor3f(1, 1, 1);
    drawCenteredText(0.2, "You passed the storm.");
    drawCenteredText(0.1, "You are not scared anymore.");
    drawCenteredText(0.0, "Press N to continue");

    glPushMatrix();
    glTranslatef(-0.75, -0.4, 0);
    glScalef(0.35, 0.35, 1);
    MyMainObj();
    glPopMatrix();
}

// ---------------------- Scene 4:  (Beach) ----------------------


void drawBeachBackground(bool CS) {

    //sky
    if (CS) glColor3f(0.53f, 0.81f, 0.98f);
    else glColor3f(0.2f, 0.2f, 0.2f);
    drawRectangle(-1.0f, 0.2f, 1.0f, 1.0f);

    //sea
    if (CS) glColor3f(0.0f, 0.41f, 0.58f);
    else glColor3f(0.15f, 0.15f, 0.15f);
    drawRectangle(-1.0f, -0.2f, 1.0f, 0.2f);

    //sand
    if (CS) glColor3f(0.94f, 0.82f, 0.63f);
    else glColor3f(0.3f, 0.3f, 0.3f);
    drawRectangle(-1.0f, -1.0f, 1.0f, -0.2f);
}


void checkBeachLogic() {
    if (currentScene != 4) return;

    // 1. Sky boundary check
    if (playerY > 0.0f && !claritySolved) {
        playerX = -0.85f;
        playerY = -0.6f;
    }

    // 2. Solve the clarity puzzle
    if (!claritySolved && playerX > 0.65f && playerX < 0.85f && playerY > -0.40f && playerY < -0.20f)
        claritySolved = true;

    // --- High Sensitivity Teleport ---
    // Point is at (0.75, -0.3). We use a very small window (0.04) 
    // so the player's center must align with the point center.
    if (playerX > 0.73f && playerX < 0.77f && playerY > -0.32f && playerY < -0.28f) {
        playerX = -0.85f;
        playerY = -0.6f;
    }

    bool onLeftPoint = (playerX > -0.28f && playerX < -0.22f && playerY > -0.32f && playerY < -0.28f);
    bool onMiddlePoint = (playerX > 0.22f && playerX <  0.28f && playerY > -0.32f && playerY < -0.28f);
    bool onRightPoint = (playerX > 0.73f && playerX <  0.77f && playerY > -0.32f && playerY < -0.28f);

    if (onLeftPoint || onMiddlePoint || onRightPoint) {
        playerX = -0.85f; // Return to start X
        playerY = -0.6f;  // Return to start Y
    }

    // 3. Fragment collection
    if (claritySolved && !fragmentCollected && isColliding(playerX, playerY, 0.75f, 0.5f))
        fragmentCollected = true;
}

void checkConnection() {
    if (currentScene != 5) return; 


    if (!node1Active && isColliding(playerX, playerY, -0.675f, 0.4f)) 
        node1Active = true;

    else if (node1Active && !node2Active && isColliding(playerX, playerY, -0.225f, 0.4f)) 
        node2Active = true;

    else if (node2Active && !node3Active && isColliding(playerX, playerY, 0.225f, 0.4f)) 
        node3Active = true;

    else if (node3Active && !node4Active && isColliding(playerX, playerY, 0.675f, 0.4f)) {
        node4Active = true;
        connectionSolved = true;
    }




}


void drawScene4() {

    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    checkBeachLogic();

    drawBeachBackground(claritySolved);

    glColor3f(0.6f, 0.6f, 0.6f);
    //Points
    drawFilledCircle(-0.25f, -0.3f, 0.03f);
    drawFilledCircle(0.25f, -0.3f, 0.03f);
    drawFilledCircle(0.75f, -0.3f, 0.03f);

    //Crystal condition
    if (claritySolved && !fragmentCollected)
        drawFragment(0.75f, 0.5f, true);

    drawPlayer();
    glColor3f(1.0f, 1.0f, 1.0f);
    drawText(-0.9f, 0.9f, "Scene 4: The Beach - Clarity");

    if (claritySolved) {
        glColor3f(1.0f, 1.0f, 1.0f);
        drawText(-0.15f, -0.9f, "you found the clarity!");
    }


    glFlush();
}



// ---------------------- Scene 5: City (Connection) ----------------------
void drawCityBackground()
{
    if (connectionSolved) {
        glColor3f(0.1f, 0.1f, 0.3f);
        drawRectangle(-1.0f, 0.0f, 1.0f, 1.0f);
        glColor3f(1.0f, 1.0f, 0.9f);
        drawFilledCircle(0.7f, 0.8f, 0.1f);
    }
    else {
        glColor3f(0.05f, 0.05f, 0.1f);
        drawRectangle(-1.0f, 0.0f, 1.0f, 1.0f);
        glColor3f(0.2f, 0.2f, 0.2f);
        drawFilledCircle(0.7f, 0.8f, 0.1f);
    }

    if (connectionSolved) glColor3f(0.15f, 0.15f, 0.15f);
    else glColor3f(0.08f, 0.08f, 0.08f);
    drawRectangle(-1.0f, -1.0f, 1.0f, 0.0f);

    glColor3f(0.4f, 0.4f, 0.4f);
    for (float x = -1.0f; x <= 1.0f; x += 0.4f) {
        drawRectangle(x, -0.48f, x + 0.2f, -0.52f);
    }

    for (float i = -0.9f; i <= 0.7f; i += 0.45f)
    {
        if (connectionSolved) glColor3f(0.15f, 0.15f, 0.25f);
        else glColor3f(0.12f, 0.12f, 0.12f);

        drawRectangle(i, 0.0f, i + 0.3f, 0.75f);

        for (float wy = 0.1f; wy < 0.7f; wy += 0.15f) {
            if (connectionSolved) glColor3f(1.0f, 1.0f, 0.6f);
            else glColor3f(0.2f, 0.2f, 0.2f);

            drawRectangle(i + 0.06f, wy, i + 0.12f, wy + 0.08f);
            drawRectangle(i + 0.18f, wy, i + 0.24f, wy + 0.08f);
        }
    }
}



void drawNode(float x, float y, bool active)
{
    glLineWidth(4.0f);
    glColor3f(0.2f, 0.2f, 0.2f);
    glBegin(GL_LINES);
    glVertex2f(x, 0.0f);
    glVertex2f(x, y);
    glEnd();

    glColor3f(0.3f, 0.3f, 0.3f);
    drawRectangle(x - 0.04f, y - 0.02f, x + 0.04f, y + 0.04f);

    if (active)
    {
        glColor3f(1.0f, 1.0f, 0.6f + glowValue);
        drawFilledCircle(x, y + 0.01f, 0.05f);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glColor4f(1.0f, 1.0f, 0.0f, 0.2f);
        drawFilledCircle(x, y + 0.01f, 0.12f);
        glDisable(GL_BLEND);
    }
    else
    {
        glColor3f(0.1f, 0.1f, 0.1f);
        drawFilledCircle(x, y + 0.01f, 0.05f);
    }
}



void drawScene5City()
{
    checkConnection();
    drawCityBackground();

    drawNode(-0.675f, 0.4f, node1Active);
    drawNode(-0.225f, 0.4f, node2Active);
    drawNode(0.225f, 0.4f, node3Active);
    drawNode(0.675f, 0.4f, node4Active);

    drawPlayer();

    glColor3f(1.0f, 1.0f, 1.0f);
    drawText(-0.9f, 0.9f, "Scene 5: City - Connection");

    if (connectionSolved && !fragmentCollected) {
        drawFragment(0.0f, 0.8f, true);

        // Set the color for the text 
        glColor3f(1.0f, 1.0f, .0f);

        
        drawText(-0.2f, 0.7f, "You found Connection");
    }
}

// ---------------------------------------------------------
// Scene 6: Purpose - (Safaa Ahmed Sobh)
// ---------------------------------------------------------

using namespace std;

GLint win_width = 800, win_hight = 600;

// Logic States
bool isShelfActive = false;
bool isSorted = false;
int selectedBook = 0; // 0 means none, 1-6 for the books

// Aligned Book Positions (Sitting on the floor)
float b1x = -3.8, b1y = -3.8, b1z = 0.2; // B
float b2x = -2.3, b2y = -3.8, b2z = 0.2; // A
float b3x = -0.8, b3y = -3.8, b3z = 0.2; // F
float b4x = 0.7, b4y = -3.8, b4z = 0.2; // C
float b5x = 2.2, b5y = -3.8, b5z = 0.2; // E
float b6x = 3.7, b6y = -3.8, b6z = 0.2; // D

float textZ = 0.3; // Higher than book

void drawLibraryRoom() {
    glDisable(GL_TEXTURE_2D);

    // --- Back Wall ---
    if (!isSorted) glColor3f(0.12f, 0.12f, 0.12f);
    else glColor3f(0.75f, 0.65f, 0.55f);
    glBegin(GL_QUADS);
    glVertex3f(-4.0f, -2.0f, -0.1f); glVertex3f(4.0f, -2.0f, -0.1f);
    glVertex3f(4.0f, 5.0f, -0.1f);  glVertex3f(-4.0f, 5.0f, -0.1f);
    glEnd();

    // --- Side Walls ---
    if (!isSorted) glColor3f(0.08f, 0.08f, 0.08f);
    else glColor3f(0.65f, 0.55f, 0.45f);
    glBegin(GL_QUADS);
    glVertex3f(-5.0f, -5.0f, -0.1f); glVertex3f(-4.0f, -2.0f, -0.1f);
    glVertex3f(-4.0f, 5.0f, -0.1f);  glVertex3f(-5.0f, 5.0f, -0.1f);
    glVertex3f(5.0f, -5.0f, -0.1f);  glVertex3f(4.0f, -2.0f, -0.1f);
    glVertex3f(4.0f, 5.0f, -0.1f);   glVertex3f(5.0f, 5.0f, -0.1f);
    glEnd();

    // --- Floor ---
    if (!isSorted) glColor3f(0.05f, 0.05f, 0.05f);
    else glColor3f(0.35f, 0.2f, 0.1f);
    glBegin(GL_QUADS);
    glVertex3f(-5.0f, -5.0f, -0.1f); glVertex3f(5.0f, -5.0f, -0.1f);
    glVertex3f(4.0f, -2.0f, -0.1f);  glVertex3f(-4.0f, -2.0f, -0.1f);
    glEnd();

    // --- Floor Planks ---
    glColor3f(0.0f, 0.0f, 0.0f);
    glLineWidth(2.0f);
    glBegin(GL_LINES);
    for (float x = -4.0f; x <= 4.0f; x += 1.0f) {
        glVertex3f(x * 1.25f, -5.0f, -0.09f);
        glVertex3f(x, -2.0f, -0.09f);
    }
    glVertex3f(-4.0f, -2.0f, -0.09f); glVertex3f(4.0f, -2.0f, -0.09f);
    glEnd();

    // --- Bigger Rug ---
    if (isSorted) glColor3f(0.4f, 0.1f, 0.1f);
    else glColor3f(0.1f, 0.1f, 0.1f);
    glBegin(GL_QUADS);
    glVertex3f(-3.2f, -4.8f, -0.08f); // Widened and lengthened
    glVertex3f(3.2f, -4.8f, -0.08f);
    glVertex3f(2.5f, -2.5f, -0.08f);
    glVertex3f(-2.5f, -2.5f, -0.08f);
    glEnd();

    // --- Left Window & Garden ---
    glColor3f(0.2f, 0.1f, 0.0f);
    glBegin(GL_QUADS);
    glVertex3f(-5.0f, -2.5f, -0.05f); glVertex3f(-4.2f, -1.5f, -0.05f);
    glVertex3f(-4.2f, 3.5f, -0.05f);  glVertex3f(-5.0f, 4.0f, -0.05f);
    glEnd();

    if (isSorted) glColor3f(0.5f, 0.7f, 1.0f);
    else glColor3f(0.05f, 0.05f, 0.1f);
    glBegin(GL_QUADS);
    glVertex3f(-4.95f, -2.3f, -0.04f); glVertex3f(-4.25f, -1.35f, -0.04f);
    glVertex3f(-4.25f, 3.35f, -0.04f); glVertex3f(-4.95f, 3.8f, -0.04f);
    glEnd();

    if (isSorted) {
        glColor3f(0.2f, 0.5f, 0.2f); // Grass
        glBegin(GL_QUADS);
        glVertex3f(-4.95f, -2.3f, -0.035f); glVertex3f(-4.25f, -1.35f, -0.035f);
        glVertex3f(-4.25f, -0.2f, -0.035f); glVertex3f(-4.95f, -0.5f, -0.035f);
        glEnd();

        glColor3f(0.3f, 0.2f, 0.1f); // Trunk
        glBegin(GL_QUADS);
        glVertex3f(-4.65f, -1.2f, -0.03f); glVertex3f(-4.55f, -1.2f, -0.03f);
        glVertex3f(-4.55f, 0.8f, -0.03f);  glVertex3f(-4.65f, 0.8f, -0.03f);
        glEnd();

        glColor3f(0.1f, 0.4f, 0.1f); // Leaves
        glBegin(GL_TRIANGLES);
        glVertex3f(-4.9f, 0.3f, -0.025f); glVertex3f(-4.3f, 0.3f, -0.025f); glVertex3f(-4.6f, 1.8f, -0.025f);
        glEnd(); // FIXED: Must end triangles before points

        glPointSize(10.0f); // Much bigger flowers
        glBegin(GL_POINTS);
        glColor3f(1.0f, 0.0f, 0.5f); // Pink
        glVertex3f(-4.8f, -0.8f, -0.02f);
        glColor3f(0.8f, 0.6f, 1.0f); // Lilac
        glVertex3f(-4.4f, -1.0f, -0.02f);
        glColor3f(1.0f, 1.0f, 0.0f); // Yellow
        glVertex3f(-4.5f, -0.7f, -0.02f);
        glColor3f(1.0f, 0.5f, 0.0f); // Orange
        glVertex3f(-4.8f, -1.1f, -0.02f);
        glEnd();
    }

    // --- Right Window & Garden ---
    glColor3f(0.2f, 0.1f, 0.0f);
    glBegin(GL_QUADS);
    glVertex3f(5.0f, -2.5f, -0.05f); glVertex3f(4.2f, -1.5f, -0.05f);
    glVertex3f(4.2f, 3.5f, -0.05f);  glVertex3f(5.0f, 4.0f, -0.05f);
    glEnd();

    if (isSorted) glColor3f(0.5f, 0.7f, 1.0f);
    else glColor3f(0.05f, 0.05f, 0.1f);
    glBegin(GL_QUADS);
    glVertex3f(4.95f, -2.3f, -0.04f);
    glVertex3f(4.25f, -1.35f, -0.04f);
    glVertex3f(4.25f, 3.35f, -0.04f);
    glVertex3f(4.95f, 3.8f, -0.04f);
    glEnd();

    if (isSorted) {
        glColor3f(0.2f, 0.5f, 0.2f); // Grass
        glBegin(GL_QUADS);
        glVertex3f(4.95f, -2.3f, -0.035f);
        glVertex3f(4.25f, -1.35f, -0.035f);
        glVertex3f(4.25f, -0.2f, -0.035f);
        glVertex3f(4.95f, -0.5f, -0.035f);
        glEnd();

        glColor3f(0.3f, 0.2f, 0.1f); // Trunk
        glBegin(GL_QUADS);
        glVertex3f(4.55f, -1.2f, -0.03f);
        glVertex3f(4.65f, -1.2f, -0.03f);
        glVertex3f(4.65f, 0.8f, -0.03f);
        glVertex3f(4.55f, 0.8f, -0.03f);
        glEnd();

        glColor3f(0.1f, 0.4f, 0.1f); // Leaves
        glBegin(GL_TRIANGLES);
        glVertex3f(4.3f, 0.3f, -0.025f);
        glVertex3f(4.9f, 0.3f, -0.025f);
        glVertex3f(4.6f, 1.8f, -0.025f);
        glEnd(); // Must end triangles before moveing to points

        glPointSize(10.0f); // Much bigger flowers
        glBegin(GL_POINTS);
        glColor3f(1.0f, 0.5f, 0.0f); // Orange
        glVertex3f(4.8f, -0.8f, -0.02f);
        glColor3f(1.0f, 0.0f, 0.5f); // Pink
        glVertex3f(4.4f, -1.0f, -0.02f);
        glColor3f(0.8f, 0.6f, 1.0f); // Lilac
        glVertex3f(4.5f, -0.7f, -0.02f);
        glColor3f(1.0f, 1.0f, 0.0f); // Yellow
        glVertex3f(4.8f, -1.1f, -0.02f);
        glEnd();
    }
}




void shelve() {
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_DEPTH_TEST);

    if (isSorted != true) {
        // ================= SHELF 1 (Middle) =================
// Shadow
        glColor3f(0.08f, 0.08f, 0.08f);
        glBegin(GL_POLYGON);
        glVertex3f(-3.2f, -0.6f, 0.05f); glVertex3f(3.2f, -0.6f, 0.05f);
        glVertex3f(3.2f, 0.6f, 0.05f);   glVertex3f(-3.2f, 0.6f, 0.05f);
        glEnd();

        // Main Surface
        glBegin(GL_POLYGON);
        glColor3f(0.4f, 0.4f, 0.4f); glVertex3f(-3.0f, -0.3f, 0.1f);
        glColor3f(0.3f, 0.3f, 0.3f); glVertex3f(3.0f, -0.3f, 0.1f);
        glColor3f(0.4f, 0.4f, 0.4f); glVertex3f(3.0f, 0.5f, 0.1f);
        glColor3f(0.5f, 0.5f, 0.5f); glVertex3f(-3.0f, 0.5f, 0.1f);
        glEnd();

        // Edge
        glColor3f(0.2f, 0.2f, 0.2f);
        glBegin(GL_POLYGON);
        glVertex3f(-3.0f, -0.5f, 0.11f); glVertex3f(3.0f, -0.5f, 0.11f);
        glVertex3f(3.0f, -0.3f, 0.11f);  glVertex3f(-3.0f, -0.3f, 0.11f);
        glEnd();

        // ================= SHELF 2 (Top) =================
        // Shadow
        glColor3f(0.08f, 0.08f, 0.08f);
        glBegin(GL_POLYGON);
        glVertex3f(-3.2f, 0.6f, 0.05f); glVertex3f(3.2f, 0.6f, 0.05f);
        glVertex3f(3.2f, 1.8f, 0.05f);  glVertex3f(-3.2f, 1.8f, 0.05f);
        glEnd();

        // Main Surface
        glBegin(GL_POLYGON);
        glColor3f(0.4f, 0.4f, 0.4f); glVertex3f(-3.0f, 0.9f, 0.1f);
        glColor3f(0.3f, 0.3f, 0.3f); glVertex3f(3.0f, 0.9f, 0.1f);
        glColor3f(0.4f, 0.4f, 0.4f); glVertex3f(3.0f, 1.7f, 0.1f);
        glColor3f(0.5f, 0.5f, 0.5f); glVertex3f(-3.0f, 1.7f, 0.1f);
        glEnd();

        // Edge
        glColor3f(0.2f, 0.2f, 0.2f);
        glBegin(GL_POLYGON);
        glVertex3f(-3.0f, 0.7f, 0.11f); glVertex3f(3.0f, 0.7f, 0.11f);
        glVertex3f(3.0f, 0.9f, 0.11f);  glVertex3f(-3.0f, 0.9f, 0.11f);
        glEnd();

        // ================= SHELF 3 (Bottom) =================
        // Shadow
        glColor3f(0.08f, 0.08f, 0.08f);
        glBegin(GL_POLYGON);
        glVertex3f(-3.2f, -1.8f, 0.05f); glVertex3f(3.2f, -1.8f, 0.05f);
        glVertex3f(3.2f, -0.6f, 0.05f);  glVertex3f(-3.2f, -0.6f, 0.05f);
        glEnd();

        // Main Surface
        glBegin(GL_POLYGON);
        glColor3f(0.4f, 0.4f, 0.4f); glVertex3f(-3.0f, -1.5f, 0.1f);
        glColor3f(0.3f, 0.3f, 0.3f); glVertex3f(3.0f, -1.5f, 0.1f);
        glColor3f(0.4f, 0.4f, 0.4f); glVertex3f(3.0f, -0.7f, 0.1f);
        glColor3f(0.5f, 0.5f, 0.5f); glVertex3f(-3.0f, -0.7f, 0.1f);
        glEnd();

        // Edge
        glColor3f(0.2f, 0.2f, 0.2f);
        glBegin(GL_POLYGON);
        glVertex3f(-3.0f, -1.7f, 0.11f); glVertex3f(3.0f, -1.7f, 0.11f);
        glVertex3f(3.0f, -1.5f, 0.11f);  glVertex3f(-3.0f, -1.5f, 0.11f);
        glEnd();

        // ================= SHELF 4  =================
        // Shadow
        glColor3f(0.08f, 0.08f, 0.08f);
        glBegin(GL_POLYGON);
        glVertex3f(-3.2f, 1.8f, 0.05f); glVertex3f(3.2f, 1.8f, 0.05f);
        glVertex3f(3.2f, 3.0f, 0.05f);  glVertex3f(-3.2f, 3.0f, 0.05f);
        glEnd();

        // Main Surface
        glBegin(GL_POLYGON);
        glColor3f(0.4f, 0.4f, 0.4f); glVertex3f(-3.0f, 2.1f, 0.1f);
        glColor3f(0.3f, 0.3f, 0.3f); glVertex3f(3.0f, 2.1f, 0.1f);
        glColor3f(0.4f, 0.4f, 0.4f); glVertex3f(3.0f, 2.9f, 0.1f);
        glColor3f(0.5f, 0.5f, 0.5f); glVertex3f(-3.0f, 2.9f, 0.1f);
        glEnd();

        // Edge
        glColor3f(0.2f, 0.2f, 0.2f);
        glBegin(GL_POLYGON);
        glVertex3f(-3.0f, 1.9f, 0.11f); glVertex3f(3.0f, 1.9f, 0.11f);
        glVertex3f(3.0f, 2.1f, 0.11f);  glVertex3f(-3.0f, 2.1f, 0.11f);
        glEnd();

        // ================= SHELF 5  =================
        // Shadow
        glColor3f(0.08f, 0.08f, 0.08f);
        glBegin(GL_POLYGON);
        glVertex3f(-3.2f, 3.0f, 0.05f); glVertex3f(3.2f, 3.0f, 0.05f);
        glVertex3f(3.2f, 4.2f, 0.05f);  glVertex3f(-3.2f, 4.2f, 0.05f);
        glEnd();

        // Main Surface
        glBegin(GL_POLYGON);
        glColor3f(0.4f, 0.4f, 0.4f); glVertex3f(-3.0f, 3.3f, 0.1f);
        glColor3f(0.3f, 0.3f, 0.3f); glVertex3f(3.0f, 3.3f, 0.1f);
        glColor3f(0.4f, 0.4f, 0.4f); glVertex3f(3.0f, 4.1f, 0.1f);
        glColor3f(0.5f, 0.5f, 0.5f); glVertex3f(-3.0f, 4.1f, 0.1f);
        glEnd();

        // Edge
        glColor3f(0.2f, 0.2f, 0.2f);
        glBegin(GL_POLYGON);
        glVertex3f(-3.0f, 3.1f, 0.11f); glVertex3f(3.0f, 3.1f, 0.11f);
        glVertex3f(3.0f, 3.3f, 0.11f);  glVertex3f(-3.0f, 3.3f, 0.11f);
        glEnd();
    }
    else {
        // ================= SHELF 1 (Middle) =================
        // Shadow
        glColor3f(0.1f, 0.05f, 0.0f);
        glBegin(GL_POLYGON);
        glVertex3f(-3.2f, -0.6f, 0.05f); glVertex3f(3.2f, -0.6f, 0.05f);
        glVertex3f(3.2f, 0.6f, 0.05f);   glVertex3f(-3.2f, 0.6f, 0.05f);
        glEnd();

        // Main Surface
        glBegin(GL_POLYGON);
        glColor3f(0.45f, 0.25f, 0.08f); glVertex3f(-3.0f, -0.3f, 0.1f);
        glColor3f(0.35f, 0.18f, 0.05f); glVertex3f(3.0f, -0.3f, 0.1f);
        glColor3f(0.45f, 0.25f, 0.08f); glVertex3f(3.0f, 0.5f, 0.1f);
        glColor3f(0.55f, 0.35f, 0.15f); glVertex3f(-3.0f, 0.5f, 0.1f);
        glEnd();

        // Edge
        glColor3f(0.25f, 0.12f, 0.03f);
        glBegin(GL_POLYGON);
        glVertex3f(-3.0f, -0.5f, 0.11f); glVertex3f(3.0f, -0.5f, 0.11f);
        glVertex3f(3.0f, -0.3f, 0.11f);  glVertex3f(-3.0f, -0.3f, 0.11f);
        glEnd();

        // ================= SHELF 2 (Top - Touching Middle) =================
        // Shadow starts exactly at 0.6 (where Middle Shadow ended)
        glColor3f(0.1f, 0.05f, 0.0f);
        glBegin(GL_POLYGON);
        glVertex3f(-3.2f, 0.6f, 0.05f); glVertex3f(3.2f, 0.6f, 0.05f);
        glVertex3f(3.2f, 1.8f, 0.05f);  glVertex3f(-3.2f, 1.8f, 0.05f);
        glEnd();

        // Main Surface
        glBegin(GL_POLYGON);
        glColor3f(0.45f, 0.25f, 0.08f); glVertex3f(-3.0f, 0.9f, 0.1f);
        glColor3f(0.35f, 0.18f, 0.05f); glVertex3f(3.0f, 0.9f, 0.1f);
        glColor3f(0.45f, 0.25f, 0.08f); glVertex3f(3.0f, 1.7f, 0.1f);
        glColor3f(0.55f, 0.35f, 0.15f); glVertex3f(-3.0f, 1.7f, 0.1f);
        glEnd();

        // Edge
        glColor3f(0.25f, 0.12f, 0.03f);
        glBegin(GL_POLYGON);
        glVertex3f(-3.0f, 0.7f, 0.11f); glVertex3f(3.0f, 0.7f, 0.11f);
        glVertex3f(3.0f, 0.9f, 0.11f);  glVertex3f(-3.0f, 0.9f, 0.11f);
        glEnd();

        // ================= SHELF 3 (Bottom - Touching Middle) =================
        // Shadow ends exactly at -0.6 (where Middle Shadow started)
        glColor3f(0.1f, 0.05f, 0.0f);
        glBegin(GL_POLYGON);
        glVertex3f(-3.2f, -1.8f, 0.05f); glVertex3f(3.2f, -1.8f, 0.05f);
        glVertex3f(3.2f, -0.6f, 0.05f); glVertex3f(-3.2f, -0.6f, 0.05f);
        glEnd();

        // Main Surface
        glBegin(GL_POLYGON);
        glColor3f(0.45f, 0.25f, 0.08f); glVertex3f(-3.0f, -1.5f, 0.1f);
        glColor3f(0.35f, 0.18f, 0.05f); glVertex3f(3.0f, -1.5f, 0.1f);
        glColor3f(0.45f, 0.25f, 0.08f); glVertex3f(3.0f, -0.7f, 0.1f);
        glColor3f(0.55f, 0.35f, 0.15f); glVertex3f(-3.0f, -0.7f, 0.1f);
        glEnd();

        // Edge
        glColor3f(0.25f, 0.12f, 0.03f);
        glBegin(GL_POLYGON);
        glVertex3f(-3.0f, -1.7f, 0.11f); glVertex3f(3.0f, -1.7f, 0.11f);
        glVertex3f(3.0f, -1.5f, 0.11f); glVertex3f(-3.0f, -1.5f, 0.11f);
        glEnd();

        // ================= SHELF 4  =================
        // Shadow starts at 1.8 (where the previous Top ended)
        glColor3f(0.1f, 0.05f, 0.0f);
        glBegin(GL_POLYGON);
        glVertex3f(-3.2f, 1.8f, 0.05f); glVertex3f(3.2f, 1.8f, 0.05f);
        glVertex3f(3.2f, 3.0f, 0.05f);  glVertex3f(-3.2f, 3.0f, 0.05f);
        glEnd();

        // Main Surface
        glBegin(GL_POLYGON);
        glColor3f(0.45f, 0.25f, 0.08f); glVertex3f(-3.0f, 2.1f, 0.1f);
        glColor3f(0.35f, 0.18f, 0.05f); glVertex3f(3.0f, 2.1f, 0.1f);
        glColor3f(0.45f, 0.25f, 0.08f); glVertex3f(3.0f, 2.9f, 0.1f);
        glColor3f(0.55f, 0.35f, 0.15f); glVertex3f(-3.0f, 2.9f, 0.1f);
        glEnd();

        // Edge
        glColor3f(0.25f, 0.12f, 0.03f);
        glBegin(GL_POLYGON);
        glVertex3f(-3.0f, 1.9f, 0.11f); glVertex3f(3.0f, 1.9f, 0.11f);
        glVertex3f(3.0f, 2.1f, 0.11f);  glVertex3f(-3.0f, 2.1f, 0.11f);
        glEnd();

        // ================= SHELF 5  =================
        // Shadow starts at 3.0 (where Shelf 4 ended)
        glColor3f(0.1f, 0.05f, 0.0f);
        glBegin(GL_POLYGON);
        glVertex3f(-3.2f, 3.0f, 0.05f); glVertex3f(3.2f, 3.0f, 0.05f);
        glVertex3f(3.2f, 4.2f, 0.05f);  glVertex3f(-3.2f, 4.2f, 0.05f);
        glEnd();

        // Main Surface
        glBegin(GL_POLYGON);
        glColor3f(0.45f, 0.25f, 0.08f); glVertex3f(-3.0f, 3.3f, 0.1f);
        glColor3f(0.35f, 0.18f, 0.05f); glVertex3f(3.0f, 3.3f, 0.1f);
        glColor3f(0.45f, 0.25f, 0.08f); glVertex3f(3.0f, 4.1f, 0.1f);
        glColor3f(0.55f, 0.35f, 0.15f); glVertex3f(-3.0f, 4.1f, 0.1f);
        glEnd();

        // Edge
        glColor3f(0.25f, 0.12f, 0.03f);
        glBegin(GL_POLYGON);
        glVertex3f(-3.0f, 3.1f, 0.11f); glVertex3f(3.0f, 3.1f, 0.11f);
        glVertex3f(3.0f, 3.3f, 0.11f);  glVertex3f(-3.0f, 3.3f, 0.11f);
        glEnd();
    }
    glEnable(GL_DEPTH_TEST);
}

void drawLetters(float x, float y, float z, char label) {
    glLineWidth(3.0f);
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_LINES);
    if (label == 'A') {
        glVertex3f(x - 0.2, y - 0.3, z); glVertex3f(x, y + 0.3, z);
        glVertex3f(x + 0.2, y - 0.3, z); glVertex3f(x, y + 0.3, z);
        glVertex3f(x - 0.1, y, z);       glVertex3f(x + 0.1, y, z);
    }
    else if (label == 'B') {
        glVertex3f(x - 0.2, y - 0.3, z); glVertex3f(x - 0.2, y + 0.3, z);
        glVertex3f(x - 0.2, y + 0.3, z); glVertex3f(x + 0.1, y + 0.3, z);
        glVertex3f(x - 0.2, y, z);       glVertex3f(x + 0.1, y, z);
        glVertex3f(x - 0.2, y - 0.3, z); glVertex3f(x + 0.1, y - 0.3, z);
        glVertex3f(x + 0.1, y + 0.3, z); glVertex3f(x + 0.1, y - 0.3, z);
    }
    else if (label == 'C') {
        glVertex3f(x - 0.2, y + 0.3, z); glVertex3f(x + 0.2, y + 0.3, z);
        glVertex3f(x - 0.2, y - 0.3, z); glVertex3f(x + 0.2, y - 0.3, z);
        glVertex3f(x - 0.2, y - 0.3, z); glVertex3f(x - 0.2, y + 0.3, z);
    }
    else if (label == 'D') {
        glVertex3f(x - 0.2, y - 0.3, z); glVertex3f(x - 0.2, y + 0.3, z);
        glVertex3f(x - 0.2, y + 0.3, z); glVertex3f(x + 0.1, y + 0.3, z);
        glVertex3f(x - 0.2, y - 0.3, z); glVertex3f(x + 0.1, y - 0.3, z);
        glVertex3f(x + 0.1, y + 0.3, z); glVertex3f(x + 0.1, y - 0.3, z);
    }
    else if (label == 'E') {
        glVertex3f(x - 0.2, y - 0.3, z); glVertex3f(x - 0.2, y + 0.3, z);
        glVertex3f(x - 0.2, y + 0.3, z); glVertex3f(x + 0.2, y + 0.3, z);
        glVertex3f(x - 0.2, y, z);       glVertex3f(x + 0.1, y, z);
        glVertex3f(x - 0.2, y - 0.3, z); glVertex3f(x + 0.2, y - 0.3, z);
    }
    else if (label == 'F') {
        glVertex3f(x - 0.2, y - 0.3, z); glVertex3f(x - 0.2, y + 0.3, z);
        glVertex3f(x - 0.2, y + 0.3, z); glVertex3f(x + 0.2, y + 0.3, z);
        glVertex3f(x - 0.2, y, z);       glVertex3f(x + 0.1, y, z);
    }
    glEnd();
}

void drawStaticBooks() {
    glDisable(GL_DEPTH_TEST);

    // --- Helper to draw a single small rectangle book spine ---
    auto drawSingleBook = [](float x, float y, float r, float g, float b) {
        glColor3f(r, g, b);
        glBegin(GL_QUADS);
        // Removed + 0.1f so the bottom of the book touches the shelf surface
        glVertex3f(x - 0.12f, y, 0.15f);
        glVertex3f(x + 0.12f, y, 0.15f);
        glVertex3f(x + 0.12f, y + 0.8f, 0.15f);
        glVertex3f(x - 0.12f, y + 0.8f, 0.15f);
        glEnd();
        };

    // --- FILL SHELF 1 (Bottom: -1.5f) ---
    drawSingleBook(-2.5f, -1.5f, 0.5f, 0.2f, 0.2f);
    drawSingleBook(-2.0f, -1.5f, 0.2f, 0.3f, 0.5f);
    drawSingleBook(-1.5f, -1.5f, 0.3f, 0.4f, 0.2f);
    drawSingleBook(-1.0f, -1.5f, 0.4f, 0.3f, 0.1f);
    drawSingleBook(0.5f, -1.5f, 0.5f, 0.2f, 0.2f);
    drawSingleBook(1.0f, -1.5f, 0.2f, 0.3f, 0.5f);

    // --- SHELF 2 IS EMPTY FOR YOUR PUZZLE (0.9f) ---

    // --- FILL SHELF 3 (Middle: -0.3f) ---
    drawSingleBook(-2.2f, -0.3f, 0.2f, 0.3f, 0.5f);
    drawSingleBook(-1.7f, -0.3f, 0.4f, 0.3f, 0.1f);
    drawSingleBook(1.2f, -0.3f, 0.5f, 0.2f, 0.2f);
    drawSingleBook(1.7f, -0.3f, 0.3f, 0.4f, 0.2f);

    // --- FILL SHELF 4 (Top: 2.1f) ---
    drawSingleBook(-2.8f, 2.1f, 0.3f, 0.4f, 0.2f);
    drawSingleBook(-2.3f, 2.1f, 0.5f, 0.2f, 0.2f);
    drawSingleBook(0.0f, 2.1f, 0.2f, 0.3f, 0.5f);
    drawSingleBook(2.5f, 2.1f, 0.4f, 0.3f, 0.1f);

    // --- FILL SHELF 5 (Topmost: 3.3f) ---
    drawSingleBook(-1.0f, 3.3f, 0.4f, 0.3f, 0.1f);
    drawSingleBook(-0.5f, 3.3f, 0.2f, 0.3f, 0.5f);
    drawSingleBook(0.5f, 3.3f, 0.5f, 0.2f, 0.2f);
    drawSingleBook(1.0f, 3.3f, 0.3f, 0.4f, 0.2f);

    glEnable(GL_DEPTH_TEST);
}



void drawBooks() {
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_DEPTH_TEST);

    // Book 1 (B)
    glColor3f(0.6f, 0.2f, 0.2f);
    glBegin(GL_QUADS);
    glVertex3f(b1x - 0.4f, b1y - 0.6f, b1z); glVertex3f(b1x + 0.4f, b1y - 0.6f, b1z);
    glVertex3f(b1x + 0.4f, b1y + 0.6f, b1z); glVertex3f(b1x - 0.4f, b1y + 0.6f, b1z);
    glEnd();
    drawLetters(b1x, b1y, textZ, 'B');

    // Book 2 (A)
    glColor3f(0.6f, 0.2f, 0.2f);
    glBegin(GL_QUADS);
    glVertex3f(b2x - 0.4f, b2y - 0.6f, b2z); glVertex3f(b2x + 0.4f, b2y - 0.6f, b2z);
    glVertex3f(b2x + 0.4f, b2y + 0.6f, b2z); glVertex3f(b2x - 0.4f, b2y + 0.6f, b2z);
    glEnd();
    drawLetters(b2x, b2y, textZ, 'A');

    // Book 3 (F)
    glColor3f(0.6f, 0.2f, 0.2f);
    glBegin(GL_QUADS);
    glVertex3f(b3x - 0.4f, b3y - 0.6f, b3z); glVertex3f(b3x + 0.4f, b3y - 0.6f, b3z);
    glVertex3f(b3x + 0.4f, b3y + 0.6f, b3z); glVertex3f(b3x - 0.4f, b3y + 0.6f, b3z);
    glEnd();
    drawLetters(b3x, b3y, textZ, 'F');

    // Book 4 (C)
    glColor3f(0.6f, 0.2f, 0.2f);
    glBegin(GL_QUADS);
    glVertex3f(b4x - 0.4f, b4y - 0.6f, b4z); glVertex3f(b4x + 0.4f, b4y - 0.6f, b4z);
    glVertex3f(b4x + 0.4f, b4y + 0.6f, b4z); glVertex3f(b4x - 0.4f, b4y + 0.6f, b4z);
    glEnd();
    drawLetters(b4x, b4y, textZ, 'C');

    // Book 5 (E)
    glColor3f(0.6f, 0.2f, 0.2f);
    glBegin(GL_QUADS);
    glVertex3f(b5x - 0.4f, b5y - 0.6f, b5z); glVertex3f(b5x + 0.4f, b5y - 0.6f, b5z);
    glVertex3f(b5x + 0.4f, b5y + 0.6f, b5z); glVertex3f(b5x - 0.4f, b5y + 0.6f, b5z);
    glEnd();
    drawLetters(b5x, b5y, textZ, 'E');

    // Book 6 (D)
    glColor3f(0.6f, 0.2f, 0.2f);
    glBegin(GL_QUADS);
    glVertex3f(b6x - 0.4f, b6y - 0.6f, b6z); glVertex3f(b6x + 0.4f, b6y - 0.6f, b6z);
    glVertex3f(b6x + 0.4f, b6y + 0.6f, b6z); glVertex3f(b6x - 0.4f, b6y + 0.6f, b6z);

    if (!isSorted) glColor3f(0.3f, 0.3f, 0.3f); // Gray books
    else glColor3f(0.6f, 0.2f, 0.2f); // Red books
    glEnd();
    drawLetters(b6x, b6y, textZ, 'D');

    glEnable(GL_DEPTH_TEST);
}




void mouse(int button, int state, int x, int y) {
    // New math: converts 0-800 pixels to -1.0 to 1.0 range
    float mx = ((float)x / (win_width / 2.0f) - 1.0f) * 5.0f;
    float my = (1.0f - (float)y / (win_hight / 2.0f)) * 5.0f;

    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        if (mx > b1x - 0.4 && mx < b1x + 0.4 && my > b1y - 0.6 && my < b1y + 0.6) selectedBook = 1;
        else if (mx > b2x - 0.4 && mx < b2x + 0.4 && my > b2y - 0.6 && my < b2y + 0.6) selectedBook = 2;
        else if (mx > b3x - 0.4 && mx < b3x + 0.4 && my > b3y - 0.6 && my < b3y + 0.6) selectedBook = 3;
        else if (mx > b4x - 0.4 && mx < b4x + 0.4 && my > b4y - 0.6 && my < b4y + 0.6) selectedBook = 4;
        else if (mx > b5x - 0.4 && mx < b5x + 0.4 && my > b5y - 0.6 && my < b5y + 0.6) selectedBook = 5;
        else if (mx > b6x - 0.4 && mx < b6x + 0.4 && my > b6y - 0.6 && my < b6y + 0.6) selectedBook = 6;
    }
    if (state == GLUT_UP) {
        selectedBook = 0;
        if (b2x < b1x && b1x < b4x && b4x < b6x && b6x < b5x && b5x < b3x) {
            if (!isSorted) { cout << "You found acceptance" << endl; isSorted = true; }
        }
    }
    glutPostRedisplay();
}

void motion(int x, int y) {
    float mx = ((float)x / (win_width / 2.0f) - 1.0f) * 5.0f;
    float my = (1.0f - (float)y / (win_hight / 2.0f)) * 5.0f;

    if (selectedBook == 1) { b1x = mx; b1y = my; }
    else if (selectedBook == 2) { b2x = mx; b2y = my; }
    else if (selectedBook == 3) { b3x = mx; b3y = my; }
    else if (selectedBook == 4) { b4x = mx; b4y = my; }
    else if (selectedBook == 5) { b5x = mx; b5y = my; }
    else if (selectedBook == 6) { b6x = mx; b6y = my; }
    if (isSorted) return;
    glutPostRedisplay();
}

void drawSceneLibrary() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // 1. Draw UI Text
    glColor3f(1.0f, 1.0f, 1.0f);
    drawText(-0.95f, 0.93f, "Scene 6: Purpose");
    drawText(-0.95f, 0.87f, "Use Click and Drag Mouse to Arrange Books");

    // 2. DRAW LIBRARY (Scaled down)
    glPushMatrix();
    glScalef(0.2f, 0.2f, 1.0f);
    drawLibraryRoom();
    shelve();
    drawBooks();
    drawStaticBooks();

    if (isSorted) {
        glColor3f(1.0f, 1.0f, 0.0f);
        drawText(-1.2f, -3.5f, "You found purpose.");
    }
    glPopMatrix();

    // 3. DRAW PLAYER ON TOP (Static position: Bottom Left of Shelves)
    glDisable(GL_DEPTH_TEST); // Force character to be on top of the shelf layers
    glPushMatrix();
    // These coordinates (-0.65, -0.4) place him near the bottom left corner of the shelf unit
    glTranslatef(-0.65f, -0.4f, 0.0f);
    glScalef(playerScale, playerScale, 1.0f);
    MyMainObj(); // Use the object function directly to avoid double-drawing from drawPlayer
    glPopMatrix();
    glEnable(GL_DEPTH_TEST);

    glutSwapBuffers();
}



// ---------------------------------------------------------
// Scene 7: Final Purpose - Final Light (Nour Amin Zehrawi)
// ---------------------------------------------------------

bool scene7Solved = false;
int s7FragmentsCollected = 0;
const int s7TotalFragments = 4;
struct S7Fragment { float x, y; bool collected; };
S7Fragment s7Fragments[s7TotalFragments] = {
    {-0.5f, 0.2f, false}, {-0.1f, 0.3f, false}, {0.3f, 0.25f, false}, {0.7f, 0.35f, false}
};

void drawMassiveEndS7(float x, float y, float scale) {
    glPushMatrix();
    glTranslatef(x, y, 0);
    glScalef(scale, scale, 1.0f);
    glLineWidth(6);
    const char* t = "THE";
    for (int i = 0; t[i] != '\0'; i++) glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, t[i]);
    glTranslatef(25.0f, 0, 0);
    const char* e = "END";
    for (int i = 0; e[i] != '\0'; i++) glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, e[i]);
    glPopMatrix();
}

void drawCloudS7(float x, float y) {
    if (!scene7Solved) glColor3f(0.5f, 0.5f, 0.5f); else glColor3f(1.0f, 1.0f, 1.0f);
    drawFilledCircle(x, y, 0.08f);
    drawFilledCircle(x + 0.06f, y + 0.02f, 0.06f);
    drawFilledCircle(x - 0.06f, y + 0.02f, 0.06f);
}

void drawFlowerS7(float x, float y, float r, float g, float b) {
    if (!scene7Solved) glColor3f(0.2f, 0.2f, 0.2f); else glColor3f(0.0f, 0.6f, 0.1f);
    glLineWidth(2);
    glBegin(GL_LINES); glVertex2f(x, y); glVertex2f(x, y - 0.12f); glEnd();
    if (!scene7Solved) glColor3f(0.3f, 0.3f, 0.3f); else glColor3f(r, g, b);
    drawFilledCircle(x, y + 0.03f, 0.025f); drawFilledCircle(x, y - 0.03f, 0.025f);
    drawFilledCircle(x + 0.03f, y, 0.025f); drawFilledCircle(x - 0.03f, y, 0.025f);
    if (!scene7Solved) glColor3f(0.4f, 0.4f, 0.4f); else glColor3f(1.0f, 0.9f, 0.2f);
    drawFilledCircle(x, y, 0.02f);
}

void drawScene7Garden() {
    if (scene7Solved) glClearColor(0.5f, 0.8f, 1.0f, 1.0f); else glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    if (scene7Solved) glColor3f(1.0f, 1.0f, 0.0f); else glColor3f(0.3f, 0.3f, 0.3f);
    drawFilledCircle(0.85f, 0.85f, 0.12f);
    drawCloudS7(-0.8f, 0.8f); drawCloudS7(-0.4f, 0.85f); drawCloudS7(0.3f, 0.8f); drawCloudS7(0.7f, 0.85f);

    if (scene7Solved) glColor3f(0.1f, 0.5f, 0.1f); else glColor3f(0.1f, 0.1f, 0.1f);
    drawRectangle(-1.0f, -1.0f, 1.0f, -0.4f);
    drawFlowerS7(-0.9f, -0.3f, 1.0f, 0.0f, 0.0f); drawFlowerS7(-0.6f, -0.25f, 0.8f, 0.2f, 0.8f);
    drawFlowerS7(-0.3f, -0.35f, 1.0f, 0.6f, 0.2f); drawFlowerS7(0.2f, -0.25f, 0.4f, 0.9f, 0.4f);
    drawFlowerS7(0.6f, -0.3f, 1.0f, 0.4f, 0.6f); drawFlowerS7(0.85f, -0.25f, 1.0f, 0.8f, 0.0f);

    for (int i = 0; i < s7TotalFragments; i++) {
        if (!s7Fragments[i].collected) {
            glColor3f(1.0f, 0.4f, 0.7f); drawFilledCircle(s7Fragments[i].x, s7Fragments[i].y, 0.06f);
        }
    }

    drawPlayer();

    if (scene7Solved) {
        glColor3f(0.0f, 0.0f, 0.0f);
        drawText(-0.7f, 0.5f, "You found acceptance.");

    }
}



////////////////////////END SCENE//////////////////////////////

GLuint myTexture2;
char image2Path[] = "C:\\Users\\sosos\\source\\repos\\Project14\\end.bmp";


void drawScene8End() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // 1. Draw the ending texture
    if (myTexture2 != 0) {
        drawTexturedQuad(myTexture2);
    }
    else {
        // Fallback if texture fails
        glColor3f(0.1f, 0.1f, 0.1f);
        drawRectangle(-1.0f, -1.0f, 1.0f, 1.0f);
    }

    
}




// ---------------------- Keyboard Controls ----------------------
void normalKeyboard(unsigned char key, int x, int y)
{
    //----------------Scene 1
    if (currentScene == 1)
    {
        if (key == 13)
            currentScene = 2;
        else if (key == 'a' || key == 'A')
            showAbout = !showAbout;
        else if (key == 'h' || key == 'H')
            showHelp = !showHelp;
        else if (key == 27)
            exit(0);
    }
    //----------------Scene 2
    else if (currentScene == 2)
    {
        if (key == 8)
            currentScene = 1;
        else if (key == 'r' || key == 'R')
        {
            playerX = -0.85f;
            playerY = -0.6f;
            fake1Visible = true;
            fake2Visible = true;
            identitySolved = false;
        }
        else if (identitySolved && (key == 'n' || key == 'N'))
            currentScene = 3;
        else if (key == 27)
            exit(0);
    }
    //--------------------------Desert Controls
    //----------------Scene 3
    else if (currentScene == 3)
    {
        // rotation when losing
        if (key == 'v' || key == 'V')
            currentScene = 31;

        else if (key == 'z' || key == 'Z')
        {
            if (playerX > 0.5f)
                currentScene = 5;
        }

        else if (key == 'x' || key == 'X')
        {
            if (playerX < -0.5f)
            {
                playerX = -0.85f;
                playerY = -0.6f;
            }
        }
    }

    else if (currentScene == 31)
    {
        if (key == 'z' || key == 'Z')
        {
            if (charX > 0.5)
                currentScene = 32;
        }

        else if (key == 'x' || key == 'X')
        {
            if (charX < -0.5)
                currentScene = 33;
        }
    }

    else if (currentScene == 33)
    {
        if (key == 'r' || key == 'R')
        {
            charX = 0;
            fallAngle = 0;
            currentScene = 31;
        }
    }

    else if (currentScene == 32)
    {
        if (key == 'u' || key == 'U')
            currentScene = 34;
    }

    else if (currentScene == 34)
    {
        if (key == 'n' || key == 'N') {
            currentScene = 4;
            // Reset player to the start of the beach
            playerX = -0.85f;
            playerY = -0.6f;
        }
    }
    //--------------------------Desert
    //----------------Scene 4
    else if (currentScene == 4)
    {
        if (key == 'n' || key == 'N') {
            currentScene = 5;
            playerX = -0.85f;
            playerY = -0.6f;
        }
        else if (key == 27)
            exit(0);
    }
    //----------------Scene 5
    else if (currentScene == 5)
    {
        if (key == 'n' || key == 'N') {
            currentScene = 6;
            playerX = -0.85f;
            playerY = -0.6f;
        }
        else if (key == 27)
            exit(0);
    }
    //----------------Scene 6
    else if (currentScene == 6)
    {
        if (key == 'n' || key == 'N') {
            currentScene = 7;
            playerX = -0.85f;
            playerY = -0.6f;
        }
    }
    //----------------Scene 7
    else if (currentScene == 7)
    {
        // Transition to the final 8th scene once the garden is solved
        if (scene7Solved && (key == 'n' || key == 'N')) {
            currentScene = 8;
            playerX = -0.85f;
            playerY = -0.6f;
        }
        else if (key == 27)
            exit(0);
    }
    


    glutPostRedisplay();
}

void specialKeyboard(int key, int x, int y)
{
    float speed = 0.05f;

    if (currentScene == 2)
    {
        if (key == GLUT_KEY_UP)
            playerY += speed;
        else if (key == GLUT_KEY_DOWN)
            playerY -= speed;
        else if (key == GLUT_KEY_LEFT)
            playerX -= speed;
        else if (key == GLUT_KEY_RIGHT)
            playerX += speed;

        if (playerX > 0.92f) playerX = 0.92f;
        if (playerX < -0.92f) playerX = -0.92f;
        if (playerY > 0.82f) playerY = 0.82f;
        if (playerY < -0.82f) playerY = -0.82f;

        checkFragmentCollision();
    }

    else if (currentScene == 3)
    {
        float speed = 0.05f;

        if (key == GLUT_KEY_UP)
            playerY += speed;
        else if (key == GLUT_KEY_DOWN)
            playerY -= speed;
        else if (key == GLUT_KEY_LEFT)
            playerX -= speed;
        else if (key == GLUT_KEY_RIGHT)
            playerX += speed;

        if (playerX > 0.92f) playerX = 0.92f;
        if (playerX < -0.92f) playerX = -0.92f;
        if (playerY > 0.82f) playerY = 0.82f;
        if (playerY < -0.82f) playerY = -0.82f;
    }
    //--------------------------Desert
    else if (currentScene == 31 || currentScene == 32 || currentScene == 33)
    {
        if (key == GLUT_KEY_LEFT) charX -= speed;
        else if (key == GLUT_KEY_RIGHT) charX += speed;

        if (charX > 0.9f) charX = 0.9f;
        if (charX < -0.9f) charX = -0.9f;
    }

    else if (currentScene == 4)
    {
        if (key == GLUT_KEY_UP) playerY += speed;
        else if (key == GLUT_KEY_DOWN) playerY -= speed;
        else if (key == GLUT_KEY_LEFT) playerX -= speed;
        else if (key == GLUT_KEY_RIGHT) playerX += speed;
    }

    else if (currentScene == 5)
    {
        if (key == GLUT_KEY_UP)
            playerY += speed;
        else if (key == GLUT_KEY_DOWN)
            playerY -= speed;
        else if (key == GLUT_KEY_LEFT)
            playerX -= speed;
        else if (key == GLUT_KEY_RIGHT)
            playerX += speed;

        if (playerX > 0.92f) playerX = 0.92f;
        if (playerX < -0.92f) playerX = -0.92f;
        if (playerY > 0.82f) playerY = 0.82f;
        if (playerY < -0.82f) playerY = -0.82f;

        checkConnection();
    }

    else if (currentScene == 6)
    {
        if (key == GLUT_KEY_UP)
            playerY += speed;
        else if (key == GLUT_KEY_DOWN)
            playerY -= speed;
        else if (key == GLUT_KEY_LEFT)
            playerX -= speed;
        else if (key == GLUT_KEY_RIGHT)
            playerX += speed;

        if (playerX > 0.92f) playerX = 0.92f;
        if (playerX < -0.92f) playerX = -0.92f;
        if (playerY > 0.82f) playerY = 0.82f;
        if (playerY < -0.82f) playerY = -0.82f;

        
    }


    else if (currentScene == 7)
    {
        if (key == GLUT_KEY_UP)
            playerY += speed;
        else if (key == GLUT_KEY_DOWN)
            playerY -= speed;
        else if (key == GLUT_KEY_LEFT)
            playerX -= speed;
        else if (key == GLUT_KEY_RIGHT)
            playerX += speed;

        for (int i = 0; i < s7TotalFragments; i++) {
            if (!s7Fragments[i].collected) {
                float dx = playerX - s7Fragments[i].x;
                float dy = playerY - s7Fragments[i].y;
                float dist = sqrt(dx * dx + dy * dy);
                if (dist < 0.2f) {
                    s7Fragments[i].collected = true; s7FragmentsCollected++;
                    if (s7FragmentsCollected == s7TotalFragments) scene7Solved = true;
                }
            }
        }
    }


    glutPostRedisplay();
}

// ---------------------- Display and Animation ----------------------
void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    if (currentScene == 1)
        drawCoverScene();

    else if (currentScene == 2)
        drawScene2Forest();

    else if (currentScene == 3)
        drawDesertStart();

    else if (currentScene == 31)
        drawDesertGame();

    else if (currentScene == 32)
        drawDesertWin();

    else if (currentScene == 33)
        drawDesertLose();
    else if (currentScene == 34)
        drawDesertending();

    else if (currentScene == 4) {
        checkBeachLogic();    // Logic for the beach
        drawScene4();         // Draw the beach
    }
    else if (currentScene == 5) {
        drawScene5City();     // Draw the city
    }
    else if (currentScene == 6)
        drawSceneLibrary();

    else if (currentScene == 7) {
        // RESET STATES FOR 2D:
        glDisable(GL_DEPTH_TEST); // Prevents library 3D depth from blocking 2D shapes
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Force clear the depth buffer

        drawScene7Garden(); // Call the existing function as is

        glutSwapBuffers(); // Ensure the frame is actually sent to the screen
    }
    else if (currentScene == 8) {
        // 1. Standard Coordinate Reset
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluOrtho2D(-1, 1, -1, 1);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        glDisable(GL_DEPTH_TEST);
        drawScene8End(); // Background image
        glDisable(GL_TEXTURE_2D);


        // 1. DRAW THE BLACK POLYGON (BOX)
        // This must come BEFORE the text to act as a background
        glColor3f(0.0f, 0.0f, 0.0f); // Set color to Black
        // Coordinates: (Left X, Bottom Y, Right X, Top Y)
        // We make it wide enough (-0.9 to 0.9) to fit the long growth statement
        drawRectangle(-0.9f, -0.75f, 0.9f, -0.25f);


        // 2. Set Text Color to Black (as seen in your image)
        glColor3f(1.0f, 1.0f, 1.0f);

        // 3. Move "THE END" higher into the sky
        drawCenteredText(-0.35f, "THE END");

        // 4. Align the Growth Statement in the grass
        // Splitting it into two balanced lines makes centering much more stable
        drawCenteredText(-0.45f, "True growth is the harmony between who we");
        drawCenteredText(-0.55f, "were and who we choose to become.");

        // 4. Position the Exit prompt near the very bottom
        drawCenteredText(-0.65f, "Press ESC to Exit");

        glEnable(GL_TEXTURE_2D);
    }
    glutSwapBuffers();
    glFlush();
}

void update(int value)
{
    fragmentAngle += 2.0f;
    if (fragmentAngle > 360.0f)
        fragmentAngle -= 360.0f;

    if (glowIncreasing)
    {
        glowValue += 0.006f;
        if (glowValue >= 0.30f)
            glowIncreasing = false;
    }
    else
    {
        glowValue -= 0.006f;
        if (glowValue <= 0.0f)
            glowIncreasing = true;
    }

    //--------------------------Desert
    // rotation when losing
    if (currentScene == 33)
    {
        fallAngle += 5.0f;

        if (fallAngle > 90)
            fallAngle = 90;
    }

    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}

void init()
{
    glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1, 1, -1, 1);
    glMatrixMode(GL_MODELVIEW);

    // --- Texture Initialization ---
    myTexture1 = LoadTexture(image1Path);
    myTexture2 = LoadTexture(image2Path);

    // Basic check for loading errors
    if (myTexture1 == -1 || myTexture2 == -1) {
        std::cout << "Warning: One or more textures failed to load." << std::endl;
    }

    glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);//gemini says this fixes the texture transparency issues
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Fragments of Growth - Final Project");

#ifndef __APPLE__
    glewInit();
#endif

    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(normalKeyboard);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutSpecialFunc(specialKeyboard);
    glutTimerFunc(16, update, 0);

    glutMainLoop();
    return 0;
}