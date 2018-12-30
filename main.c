#include<GL/glut.h>
#include<stdlib.h>
#include<stdio.h>
#include<math.h>

#define TIMER_INTERVAL 20
#define TIMER_JMP 1
#define TIMER_FALL 2
#define PI 3.14159265358979323

static int window_width, window_height;

static void on_keyboard(unsigned char key, int x, int y);
static void on_reshape(int width, int height);
static void on_display(void);
static void on_timer(int value);

int moving_object = 0;
int game_started = 0;
int jumping_animation = 0;
float jumping = 0;
int falling_animation = 0;
float falling_rotate = 0;
float falling_translate = 0;
float falling_down = 0;
int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB |GLUT_DEPTH | GLUT_DOUBLE);
    
    /*Ispis kontrola u teminalu*/
    printf("##############################\nESC - to close the game;\nSPACE - to start the game;\nR - to restart the game;\nA - to go left;\nD - to go right;\nW - to jump;\n");


    glutInitWindowSize(600, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Prepreke");

    glutKeyboardFunc(on_keyboard);
    glutReshapeFunc(on_reshape);
    glutDisplayFunc(on_display);

    glClearColor(1, 1, 1, 0);
    glEnable(GL_DEPTH_TEST);

    /*Nameštamo osvetljenje*/
    GLfloat light_position[] = {3, 3, 1, 0};
    GLfloat light_ambient[] = {0.7, 0.7, 0.7, 1};
    GLfloat light_diffuse[] = {0.4, 0.4, 0.4, 1};        
    GLfloat light_specular[] = {0.8, 0.8, 0.8, 0};
    glEnable(GL_LIGHTING);
    /*Postavlja se desno svetlo*/
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    /*Postavlja se levo svetlo*/
    light_position[0] = -3;
    glEnable(GL_LIGHT1);
    glLightfv(GL_LIGHT1, GL_POSITION, light_position);
    glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);

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
            if(!game_started) break;
            moving_object++;
            glutPostRedisplay();
            break;
        /* Pomeramo objekat ulevo i iscrtavamo ga*/
        case 'a':
            if(!game_started) break;
            moving_object--;
            glutPostRedisplay();
            break;
        case 'w':
            if(!game_started) break;
            else if(!jumping_animation) {
                    jumping_animation = !jumping_animation;
                    glutTimerFunc(TIMER_INTERVAL, on_timer, TIMER_JMP);
                }
                break;
        /* Proba timera pritiskom na space */
        case ' ':
            game_started = 1;
            /*glutTimerFunc(TIMER_INTERVAL, on_timer, TIMER_ID);*/
            break;
        case 'r':
            game_started = 0;
            moving_object = 0;
            jumping_animation = 0;
            jumping = 0;
            falling_animation = 0;
            falling_rotate = 0;
            falling_translate = 0;
            falling_down = 0;
            glutPostRedisplay();
            break;
    }
}

static void on_timer(int value)
{
    if(value == TIMER_JMP) {
        jumping += 0.01;
        if(jumping >= 1) {
            jumping_animation = 0;
            jumping = 0;
        }
        glutPostRedisplay();
        
        if(jumping_animation)
            glutTimerFunc(TIMER_INTERVAL, on_timer, TIMER_JMP);
    }
    if(value == TIMER_FALL) {
        if(falling_rotate < 1) {
            falling_rotate += 0.05;
        }
        else if(falling_rotate < 4) {
            falling_translate += 0.015;
            falling_rotate += 0.07;
        }
        else {
            falling_translate += 0.01;
            falling_down += 0.05;
            falling_rotate += 0.07;
        }
        glutPostRedisplay();
        if(falling_animation)
            glutTimerFunc(TIMER_INTERVAL, on_timer, TIMER_FALL);
    }
}

static void on_reshape(int width, int height)
{
    window_width = width;
    window_height = height;
}

static void drawCube()
{
    /*Materijali za objekat*/
    /*Koeficijenti preuzeti sa http://devernay.free.fr/cours/opengl/materials.html*/ 
    GLfloat ambient_coefs[] = {0.0215, 0.1745, 0.0215, 1};
    GLfloat diffuse_coefs[] = {0.07568,  0.61424,  0.07568,  1};
    GLfloat specular_coefs[] = {0.633,  0.727811,  0.633,  1};
    GLfloat shininess = 0.6 * 128.0;

    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_coefs);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coefs);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular_coefs);
    glMaterialf(GL_FRONT, GL_SHININESS, shininess);
        
    glutSolidCube(1);
      
    ambient_coefs[0] = 0;
    ambient_coefs[1] = 0;
    ambient_coefs[2] = 0;
    diffuse_coefs[0] = 0;
    diffuse_coefs[1] = 0;
    diffuse_coefs[2] = 0;
    specular_coefs[0] = 0;
    specular_coefs[1] = 0;
    specular_coefs[2] = 0;
    shininess = 0;

    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_coefs);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coefs);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular_coefs);
    glMaterialf(GL_FRONT, GL_SHININESS, shininess);
       
    glLineWidth(4);    
    glutWireCube(1);
}

static void drawStage()
{
    glColor3f(0.3, 0.3, 0.3);
    glBegin(GL_POLYGON);
        glVertex3f(-2, -0.51,   4);
        glVertex3f( 2, -0.51,   4);
        glVertex3f( 2, -0.51, -100);
        glVertex3f(-2, -0.51, -100);
    glEnd();
}

static void on_display(void)
{
    
    if(moving_object > 1 || moving_object < -1) { 
        game_started = 0;
        if(!falling_animation) {
            falling_animation = 1;
            glutTimerFunc(TIMER_INTERVAL, on_timer, TIMER_FALL);
        }
    }
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glViewport(0, 0, window_width, window_height);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, window_width/(float)window_height, 1, 100);
    glMatrixMode(GL_MODELVIEW);

    glLoadIdentity();
    gluLookAt(0, 1, 6, 0, 1, 0, 0, 1, 0);
    
    /*Iscrtavamo podijum*/
    drawStage();
    
    /*Iscrtavamo objekat*/
    glPushMatrix();
        glRotatef(-(moving_object)/2.0*falling_down*15, 0, 0, 1);
        glTranslatef(moving_object, 1 - cos(2*jumping*PI), 0);
        glRotatef(15*(1 - cos(2*PI*jumping)), 1, 0, 0);
        glTranslatef((moving_object)/2.0*falling_translate, -falling_translate/3.0, 0);
        glRotatef(-(moving_object)/2.0*falling_rotate*15, 0, 0, 1);
        drawCube();
    glPopMatrix();

    glutSwapBuffers();
}
