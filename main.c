#include<GL/glut.h>
#include<stdlib.h>
#include<stdio.h>
#include<math.h>

#define TIMER_INTERVAL 20
#define TIMER_ID 1
#define PI 3.14159265358979323

static int window_width, window_height;

static void on_keyboard(unsigned char key, int x, int y);
static void on_reshape(int width, int height);
static void on_display(void);
static void on_timer(int value);

int moving_object = 0;
int animation_ongoing = 0;
int jumping_animation = 0;
float jumping = 0;
int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB |GLUT_DEPTH | GLUT_DOUBLE);

    glutInitWindowSize(600, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Prepreke");

    glutKeyboardFunc(on_keyboard);
    glutReshapeFunc(on_reshape);
    glutDisplayFunc(on_display);

    glClearColor(1, 1, 1, 0);
    glEnable(GL_DEPTH_TEST);

    glutMainLoop();

    return 0;
}

static void on_keyboard(unsigned char key, int x, int y)
{
    switch(key) {
        case 27:
            exit(0);
            break;
        /* Pomeramo objekat udesno i iscrtavamo ga*/
        case 'd':
            moving_object++;
            glutPostRedisplay();
            break;
        /* Pomeramo objekat ulevo i iscrtavamo ga*/
        case 'a':
            moving_object--;
            glutPostRedisplay();
            break;
        case 'w':
            if(!jumping_animation) {
                    jumping_animation = !jumping_animation;
                    glutTimerFunc(TIMER_INTERVAL, on_timer, TIMER_ID);
                }
                break;
        /* Proba timera pritiskom na space */
        /*case ' ':
            animation_ongoing = 1 - animation_ongoing;
            glutTimerFunc(TIMER_INTERVAL, on_timer, TIMER_ID);
            break;*/
    }
}

static void on_timer(int value)
{
    if(value == TIMER_ID)
    {
        jumping += 0.01;
        if(jumping >= 1) {
            jumping_animation = 0;
            jumping = 0;
        }
        glutPostRedisplay();
        
        if(jumping_animation)
            glutTimerFunc(TIMER_INTERVAL, on_timer, TIMER_ID);
           
             
    }
}

static void on_reshape(int width, int height)
{
    window_width = width;
    window_height = height;
}

static void drawCube()
{
    glColor3f(0.7, 0.7, 0.7);
    glutSolidCube(1);

    glLineWidth(4);
    glColor3f(0, 0, 0);
    glutWireCube(1);
}

static void drawStage()
{
    glColor3f(0.3, 0.3, 0.3);
    glBegin(GL_POLYGON);
        glVertex3f(-2, -0.51,   3);
        glVertex3f( 2, -0.51,   3);
        glVertex3f( 2, -0.51, -100);
        glVertex3f(-2, -0.51, -100);
    glEnd();
}

static void on_display(void)
{
    /*Nameštamo osvetljenje*/
    GLfloat light_position[] = {0, 2, 1.5, 0};
    GLfloat light_ambient[] = {1, 1, 1, 1};
    GLfloat light_diffuse[] = {0.8, 0.8, 0.8, 1};        
    GLfloat light_specular[] = {0.1, 0.1, 0.1, 1};
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glViewport(0, 0, window_width, window_height);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, window_width/(float)window_height, 1, 25);
    glMatrixMode(GL_MODELVIEW);

    glLoadIdentity();
    gluLookAt(0, 1.5, /*4*/5, 0, 0, 0, 0, 1, 0);
    
    /*Iscrtavamo podijum*/
    drawStage();
    
    /*Iscrtavamo objekat*/
    glPushMatrix();
        glTranslatef(moving_object,1 - cos(2*jumping*PI), 0);
        glRotatef(15*(1 - cos(2*PI*jumping)), 1, 0, 0);
        drawCube();
    glPopMatrix();

    glutSwapBuffers();
}
