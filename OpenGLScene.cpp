#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#include <cmath>
float robotX = 0.0f;
float robotZ = 0.0f;
bool collisionOccurred = false;
float rocketX = 0.0f;
float rocketZ = 0.0f;

float rocketVelocityX = 0.1f;
float rocketVelocityZ = 0.05f;
bool rocketMovingRight = true;
float robotMinX = -0.5f;
float robotMaxX = 0.5f;
float robotMinZ = 4.5f;
float robotMaxZ = 5.5f;

float castleMinX = -1.0f;
float castleMaxX = 1.0f;
float castleMinZ = -1.0f;
float castleMaxZ = 1.0f;

float cameraX = 0.0f;
float cameraY = 5.0f;
float cameraZ = 10.0f;
bool yellowBall1Collision = false;
bool yellowBall2Collision = false;
bool yellowBall3Collision = false;

void initLighting() {
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

    GLfloat light_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    GLfloat light_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat light_position[] = { 1.0f, 1.0f, 1.0f, 0.0f };

    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    GLfloat mat_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat mat_shininess[] = { 50.0f };

    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
}



void drawRobot() {
   glPushMatrix();
   glTranslatef(robotX, 1.0f, robotZ + 5);
   glColor3f(0.3f, 0.0f, 1.0f);
   glutSolidSphere(0.2, 20, 20);
   glPopMatrix();

   glPushMatrix();
   glTranslatef(robotX, 0.6f, robotZ + 5);
   glColor3f(1.0f, 0.5f, 0.5f);
   glScalef(0.5f, 1.0f, 0.8f);
   glutSolidCube(0.5);
   glPopMatrix();
}
void drawBush() {
   glColor3f(0.36f, 0.25f, 0.20f);
   glPushMatrix();
   glTranslatef(-3.5f, 0.0f, 0.0f);
   glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
   GLUquadric* quad = gluNewQuadric();
   gluCylinder(quad, 0.1, 0.1, 1.5, 20, 20);
   gluDeleteQuadric(quad);
   glPopMatrix();

    glColor3f(0.0f, 0.8f, 0.0f);
    glPushMatrix();
    glTranslatef(-3.5f, 1.5f, 0.0f);
    glutSolidSphere(0.5, 20, 20);
    glPopMatrix();
}

void drawTorus(float inner, float outer, int s, int r) {
   GLUquadric* q = gluNewQuadric();
   gluQuadricDrawStyle(q, GLU_FILL);
   gluQuadricNormals(q, GLU_SMOOTH);
   gluDisk(q, inner, outer, s, r);
   gluDeleteQuadric(q);
}

int checkCollision() {
    if (robotMinX <= castleMaxX && robotMaxX >= castleMinX &&
        robotMinZ <= castleMaxZ && robotMaxZ >= castleMinZ) {
        return 1;
    }

    float yellowBallRadius = 0.5f;

    // Yellow ball 1
    float yellowBall1X = 0.45f;
    float yellowBall1Z = 6.0f;
    float yellowBall1MinX = yellowBall1X - yellowBallRadius;
    float yellowBall1MaxX = yellowBall1X + yellowBallRadius;
    float yellowBall1MinZ = yellowBall1Z - yellowBallRadius;
    float yellowBall1MaxZ = yellowBall1Z + yellowBallRadius;
    if (robotMinX <= yellowBall1MaxX && robotMaxX >= yellowBall1MinX &&
        robotMinZ <= yellowBall1MaxZ && robotMaxZ >= yellowBall1MinZ) {
        yellowBall1Collision = true;
        return 2;
    }

    // Yellow ball 2
    float yellowBall2X = 3.45f;
    float yellowBall2Z = 5.0f;
    float yellowBall2MinX = yellowBall2X - yellowBallRadius;
    float yellowBall2MaxX = yellowBall2X + yellowBallRadius;
    float yellowBall2MinZ = yellowBall2Z - yellowBallRadius;
    float yellowBall2MaxZ = yellowBall2Z + yellowBallRadius;
    if (robotMinX <= yellowBall2MaxX && robotMaxX >= yellowBall2MinX &&
        robotMinZ <= yellowBall2MaxZ && robotMaxZ >= yellowBall2MinZ) {
        yellowBall2Collision = true;
        return 3;
    }

    // Yellow ball 3
    float yellowBall3X = -3.0f;
    float yellowBall3Z = 6.0f;
    float yellowBall3MinX = yellowBall3X - yellowBallRadius;
    float yellowBall3MaxX = yellowBall3X + yellowBallRadius;
    float yellowBall3MinZ = yellowBall3Z - yellowBallRadius;
    float yellowBall3MaxZ = yellowBall3Z + yellowBallRadius;
    if (robotMinX <= yellowBall3MaxX && robotMaxX >= yellowBall3MinX &&
        robotMinZ <= yellowBall3MaxZ && robotMaxZ >= yellowBall3MinZ) {
        yellowBall3Collision = true;
        return 4;
    }
    
    return 0;
}

void drawTrees() {
    
    float treePositions[][3] = {
        {1.75f, 0.0f, 3.0f},
        {7.0f, 0.0f, -3.0f},
        {7.5f, 0.0f, 2.5f}
      
    };


    for (int i = 0; i < sizeof(treePositions) / sizeof(treePositions[0]); ++i) {
        glPushMatrix();
        glTranslatef(treePositions[i][0], treePositions[i][1], treePositions[i][2]);
        drawBush();
        glPopMatrix();
    }
}
void updateRocketPosition() {
    if (rocketMovingRight) {
        rocketX += rocketVelocityX;
        if (rocketX > 5.0f) { // If rocket goes too far to the right
            rocketMovingRight = false; // Change direction to left
        }
    } else {
        rocketX -= rocketVelocityX;
        if (rocketX < -5.0f) { // If rocket goes too far to the left
            rocketMovingRight = true; // Change direction to right
        }
    }
}
void drawRocket() {
    glColor3f(0.5f, 0.5f, 0.5f);
    glPushMatrix();
    glTranslatef(rocketX, 4.0f, rocketZ); // Use rocketX and rocketZ for position
    glutSolidSphere(0.3, 20, 20);
    glPopMatrix();

    glColor3f(0.5f, 0.5f, 0.5f);
    glPushMatrix();
    glTranslatef(rocketX, 3.65f, rocketZ); // Use rocketX and rocketZ for position
    glScalef(0.5f, 0.2f, 0.5f);
    glutSolidCube(1.7);
    glPopMatrix();
}


void specialKeys(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_LEFT:
            robotX -= 0.1f;
            robotMinX -= 0.1f;
            robotMaxX -= 0.1f;
            if (checkCollision()) {
                robotX += 0.1f;
                robotMinX += 0.1f;
                robotMaxX += 0.1f;
            }
            break;
        case GLUT_KEY_RIGHT:
            robotX += 0.1f;
            robotMinX += 0.1f;
            robotMaxX += 0.1f;
            if (checkCollision()) {
                robotX -= 0.1f;
                robotMinX -= 0.1f;
                robotMaxX -= 0.1f;
            }
            break;
        case GLUT_KEY_UP:
            robotZ -= 0.1f;
            robotMinZ -= 0.1f;
            robotMaxZ -= 0.1f;
            if (checkCollision()) {
                robotZ += 0.1f;
                robotMinZ += 0.1f;
                robotMaxZ += 0.1f;
            }
            break;
        case GLUT_KEY_DOWN:
            robotZ += 0.1f;
            robotMinZ += 0.1f;
            robotMaxZ += 0.1f;
            if (checkCollision()) {
                robotZ -= 0.1f;
                robotMinZ -= 0.1f;
                robotMaxZ -= 0.1f;
            }
            break;
    }

    glutPostRedisplay();
}

void handleKeyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 'l':
            cameraX -= 0.1f;
            break;
        case 'r':
            cameraX += 0.1f;
            break;
        case 'f':
            cameraZ -= 0.1f;
            break;
        case 'b':
            cameraZ += 0.1f;
            break;
        case 'j':
            rocketX -= 0.1f;
            break;
        case 'k':
            rocketX += 0.1f;
            break;
    }
    glutPostRedisplay();
}
void drawPalace() {
   glColor3f(1.0f, 0.7f, 0.8f);

   glPushMatrix();
   glTranslatef(0.0f, 0.0f, 0.0f);
   glutSolidCube(2.0f);
   glPopMatrix();

   glColor3f(1.0f, 0.5f, 0.8f);

   glPushMatrix();
   glTranslatef(0.0f, 1.0f, 0.0f);
   glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
   GLUquadric* quadricTower = gluNewQuadric();
   gluCylinder(quadricTower, 0.5, 0.5, 1.0, 20, 20);
   gluDeleteQuadric(quadricTower);
   glPopMatrix();

   glPushMatrix();
   glTranslatef(1.5f, 0.0f, 0.5f);
   glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
   GLUquadric* quadricTower1 = gluNewQuadric();
   gluCylinder(quadricTower1, 0.5, 0.5, 2.0, 20, 20);
   gluDeleteQuadric(quadricTower1);
   glPopMatrix();

   glPushMatrix();
   glTranslatef(-1.5f, 0.0f, 0.5f);
   glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
   GLUquadric* quadricTower2 = gluNewQuadric();
   gluCylinder(quadricTower2, 0.5, 0.5, 2.0, 20, 20);
   gluDeleteQuadric(quadricTower2);
   glPopMatrix();

    glColor3f(0.5f, 0.5f, 1.0f);

   glPushMatrix();
   glTranslatef(1.6f, 2.0f, 0.0f);
   glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
   glutSolidCone(0.6, 0.7, 20, 20);
   glPopMatrix();

   glPushMatrix();
   glTranslatef(-1.6f, 2.0f, 0.0f);
   glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
   glutSolidCone(0.6, 0.7, 20, 20);
   glPopMatrix();

    glColor3f(0.5f, 0.5f, 1.0f);

   glPushMatrix();
   glTranslatef(0.0f, 2.0f, 0.0f);
   glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
   glutSolidCone(0.4, 0.5, 20, 20);
   glPopMatrix();
   glColor3f(0.5f, 0.5f, 1.0f);

   glPushMatrix();
   glTranslatef(-0.1f, 0.4f, 1.2f);
   glScalef(0.2f, 0.2f, 0.0f);
    
   glBegin(GL_QUADS);
   glVertex3f(-1.0f, -1.0f, 0.0f);
   glVertex3f(1.0f, -1.0f, 0.0f);
   glVertex3f(1.0f, 1.0f, 0.0f);
   glVertex3f(-1.0f, 1.0f, 0.0f);
   glEnd();
   glPopMatrix();

    
}
void drawPolygon()
{
    glColor3f(1.0f, 1.0f, 0.0f);

    glPushMatrix();
    glTranslatef(0.0f, 4.0f, 2.3f);
    glBegin(GL_POLYGON);

    for(int i = 0; i < 6; ++i) {
        glVertex3f(0.5f * cos(i * 2.0f * M_PI / 6), 0.5f * sin(i * 2.0f * M_PI / 6), 0.0f);
    }

    glEnd();
    glPopMatrix();
}
void drawHouse() {
   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

   glColor3f(0.5f, 0.5f, 0.5f);
   glPushMatrix();
   glTranslatef(1.5f, 0.5f, 3.0f);
   glRotatef(10.0f, 0.0f, 1.0f, 0.0f);
   glutSolidCube(1.0);
   glPopMatrix();

   glPushMatrix();
   glTranslatef(1.5f, 1.0f, 3.0f);
   glScalef(1.2f, 0.1f, 1.0f);
   glRotatef(10.0f, 0.0f, 1.0f, 0.0f);
   glutSolidCube(1.0);
   glPopMatrix();

   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

   glColor3f(1.0f, 0.5f, 0.5f);
   glPushMatrix();
   glTranslatef(1.5f, 0.5f, 3.0f);
   glRotatef(10.0f, 0.0f, 1.0f, 0.0f);
   glutSolidCube(1.0);
   glPopMatrix();

   glColor3f(0.0f, 1.0f, 1.0f);
   glPushMatrix();
   glTranslatef(1.5f, 1.0f, 3.0f);
   glScalef(1.2f, 0.1f, 1.0f);
   glRotatef(10.0f, 0.0f, 1.0f, 0.0f);
   glutSolidCube(1.0);
   glPopMatrix();

}

void drawRing() {
   glColor3f(0.0f, 0.0f, 0.5f);
   glPushMatrix();
   glTranslatef(-2.5f, 0.80f, 3.0f);
   float inner = 0.3f;
   float outer = 0.4f;
   int s = 30;
   int r = 30;
   drawTorus(inner, outer+0.1, s, r);
   glPopMatrix();
}
void drawRing2() {
   glColor3f(0.0f, 0.0f, 0.5f);
   glPushMatrix();
   glTranslatef(2.8f, 0.60f, 3.0f);
   float inner = 0.3f;
   float outer = 0.4f;
   int s = 30;
   int r = 30;
   drawTorus(inner, outer+0.1, s, r);
   glPopMatrix();
}
void yellowball(){
    glPushMatrix();
    glTranslatef(0.45f, 1.0f, 6.0f);
    glColor3f(1.0f, 1.0f, 0.0f);
    glutSolidSphere(0.5, 20, 20);
    glPopMatrix();
}
void yellowball2(){
    glPushMatrix();
    glTranslatef(2.95f, 1.0f, 5.0f);
    glColor3f(1.0f, 1.0f, 0.0f);
    glutSolidSphere(0.5, 20, 20);
    glPopMatrix();
}
void yellowball3(){
    glPushMatrix();
    glTranslatef(-2.f, 1.0f, 6.0f);
    glColor3f(1.0f, 1.0f, 0.0f);
    glutSolidSphere(0.5, 20, 20);
    glPopMatrix();
}
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, 800.0f / 600.0f, 0.1f, 100.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(cameraX, cameraY, cameraZ, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

    GLfloat light_position[] = { 1.0f, 1.0f, 1.0f, 0.0f }; // Directional light
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    GLfloat mat_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat mat_shininess[] = { 50.0f };
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

    glColor3f(0.0f, 0.0f, 1.0f);
    glBegin(GL_QUADS);
        glNormal3f(0.0f, 0.0f, 1.0f);
        glVertex3f(-10.0f, -10.0f, -10.0f);
        glVertex3f(-10.0f, 10.0f, -10.0f);
        glVertex3f(10.0f, 10.0f, -10.0f);
        glVertex3f(10.0f, -10.0f, -10.0f);
    glEnd();

    glColor3f(0.0f, 1.0f, 0.0f);
    glBegin(GL_QUADS);
        glNormal3f(0.0f, 1.0f, 0.0f);
        glVertex3f(-5.0f, 0.0f, -6.0f);
        glVertex3f(-4.0f, 0.0f, 6.0f);
        glVertex3f(4.0f, 0.0f, 6.0f);
        glVertex3f(5.0f, 0.0f, -6.0f);
    glEnd();

    updateRocketPosition();
    drawRobot();
    drawRing();
    drawRing2();
    drawHouse();
    drawBush();
    drawTrees();
    drawPalace();
    drawPolygon();
    drawRocket();
    yellowball();
    yellowball2();
    yellowball3();

    int collisionResult = checkCollision();
    if (collisionResult == 1) {
        collisionOccurred = true;
    } else if (collisionResult == 2) {
        yellowBall1Collision = true;
    } else if (collisionResult == 3) {
        yellowBall2Collision = true;
    } else if (collisionResult == 4) {
        yellowBall3Collision = true;
    } else {
        collisionOccurred = false;
    }

    if (yellowBall1Collision || yellowBall2Collision || yellowBall3Collision) {
        glColor3f(0.5f, 0.5f, 0.5f);
        if (yellowBall1Collision) {
            glPushMatrix();
            glTranslatef(0.0f, 0.5f, 3.0f);
            glutSolidCube(1.0f);
            glPopMatrix();
        }
        if (yellowBall2Collision) {
            glPushMatrix();
            glTranslatef(0.0f, 1.5f, 3.0f);
            glutSolidCube(1.0f);
            glPopMatrix();
        }
        if (yellowBall3Collision) {
            glPushMatrix();
            glTranslatef(0.0f, 2.5f, 3.0f);
            glutSolidCube(1.0f);
            glPopMatrix();
        }
    }

    glutSwapBuffers();
}


void initEnvironment() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
}

void reshape(int width, int height) {
    if (height == 0) height = 1;
    float aspect = (float)width / height;
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, aspect, 0.1f, 100.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}
void timer(int value) {
 
    glutPostRedisplay();

    glutTimerFunc(16, timer, 0);
}
int main(int argc, char** argv) {
   
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1200, 750);
    glutCreateWindow("OpenGL Scene");

   
    glutKeyboardFunc(handleKeyboard);
    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutSpecialFunc(specialKeys);

 
    initEnvironment();
    glutTimerFunc(0, timer, 0);


    glutMainLoop();

    return 0;
}
ß