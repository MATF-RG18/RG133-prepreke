#include<GL/glut.h>
#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include<time.h>

#define TIMER_INTERVAL 20
#define TIMER_JMP 1
#define TIMER_FALL 2
#define TIMER_OBSTACLES 3
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
float obstacles_animation = 0;
float obstacles_animation_2 = -20;
float obstacles_animation_3 = -40;
float obstacles_animation_4 = -60;
float obstacles_animation_5 = -80;
int position = 0;
int position_2 = 0;
int position_3 = 0;
int position_4 = 0;
int position_5 = 0;
int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB |GLUT_DEPTH | GLUT_DOUBLE);
    
    /*Ispis kontrola u teminalu*/
    printf("ESC - to close the game;\nSPACE - to start the game;\nR - to restart the game;\nA - to go left;\nD - to go right;\nW - to jump;\n");


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
            glutTimerFunc(TIMER_INTERVAL, on_timer, TIMER_OBSTACLES);
            break;
        case 'r':
            printf("Press SPACE to start the game\n");
            game_started = 0;
            moving_object = 0;
            jumping_animation = 0;
            jumping = 0;
            falling_animation = 0;
            falling_rotate = 0;
            falling_translate = 0;
            falling_down = 0;
            obstacles_animation = 0;
            obstacles_animation_2 = -20;
            obstacles_animation_3 = -40;
            obstacles_animation_4 = -60;
            obstacles_animation_5 = -80;
            glutPostRedisplay();
            break;
    }
}

static void on_timer(int value)
{
    if(value == TIMER_JMP) {
        jumping += 0.015;
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
        glutPostRedisplay();
        if(falling_animation)
            glutTimerFunc(TIMER_INTERVAL, on_timer, TIMER_FALL);
        }
        else if(falling_rotate < 4) {
            falling_translate += 0.015;
            falling_rotate += 0.07;
             glutPostRedisplay();
            if(falling_animation)
                glutTimerFunc(TIMER_INTERVAL, on_timer, TIMER_FALL);
        }
        else if(falling_down < 3){
            falling_translate += 0.01;
            falling_down += 0.05;
            falling_rotate += 0.07;
            glutPostRedisplay();
            if(falling_animation)
                glutTimerFunc(TIMER_INTERVAL, on_timer, TIMER_FALL);
            }
         else {
            printf("You died\n");
            game_started = 0;
            falling_animation = 0;
        }
    }
    if(value == TIMER_OBSTACLES) {
        obstacles_animation += 0.15;
        obstacles_animation_2 += 0.15;
        obstacles_animation_3 += 0.15;
        obstacles_animation_4 += 0.15;
        obstacles_animation_5 += 0.15;
        glutPostRedisplay();
        if(game_started)
            glutTimerFunc(TIMER_INTERVAL, on_timer, TIMER_OBSTACLES);
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
    GLfloat ambient_coefs[] = {0.3, 0.3, 0.3, 1};
    GLfloat diffuse_coefs[] = {0.3,  0.3,  0.3,  1};
    GLfloat specular_coefs[] = {0.3,  0.3,  0.3,  1};
    GLfloat shininess = 0;

    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_coefs);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coefs);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular_coefs);
    glMaterialf(GL_FRONT, GL_SHININESS, shininess);
    
    glBegin(GL_POLYGON);
        glVertex3f(-2, -0.51,   4);
        glVertex3f( 2, -0.51,   4);
        glVertex3f( 2, -0.51, -100);
        glVertex3f(-2, -0.51, -100);
    glEnd();
}

void obstacles()
{
    GLfloat ambient_coefs[] = {0, 0, 1, 1};
    GLfloat diffuse_coefs[] = {0.07568,  0.61424,  0.07568,  1};
    GLfloat specular_coefs[] = {0.633,  0.727811,  0.633,  1};
    GLfloat shininess = 0.6 * 128.0;

    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_coefs);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coefs);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular_coefs);
    glMaterialf(GL_FRONT, GL_SHININESS, shininess);
    
    if(obstacles_animation == 0) {
        srand(time(0));
        int r = rand() % 3;
    
        if(r == 0) 
            position = -1;
        else if(r == 1) 
            position = 1;
        else
            position = 0;
    }
    if(obstacles_animation_2 == 0) {
        srand(time(0));
        int r = rand() % 3;
        if(r == 0)
            position_2 = 0;
        else if(r == 1)
            position_2 = -1;
        else
            position_2 = 1;
    }
    if(obstacles_animation_3 == 0) {
        srand(time(0));
        int r = rand() % 3;
        if(r == 0)
            position_3 = 1;
        else if(r == 1)
            position_3 = 0;
        else
            position_3 = -1;
    }
    if(obstacles_animation_4 == 0) {
        srand(time(0));
        int r = rand() % 3;
        if(r == 0)
            position_4 = 0;
        else if(r == 1)
            position_4 = 1;
        else
            position_4 = -1;
    }
    if(obstacles_animation_5 == 0) {
        srand(time(0));
        int r = rand() % 3;
        if(r == 0)
            position_5 = -1;
        else if(r == 1)
            position_5 = 1;
        else
            position_5 = 0;
    }
    glutSolidCube(1);
}

static void on_display(void)
{
    
    if(moving_object > 1 || moving_object < -1) { 
        /*game_started = 0;*/
        if(!falling_animation) {
            falling_animation = 1;
            glutTimerFunc(TIMER_INTERVAL, on_timer, TIMER_FALL);
        }
    }
    
    /*Da li smo udarili u prepreku*/
    if(jumping < 0.25 || jumping > 0.75) {
        if(obstacles_animation > 99.1 && (position == moving_object)&& obstacles_animation < 101) {
            game_started = 0;
            printf("You died\n");
        }
        if(obstacles_animation_2 > 99.1 && (position_2 == moving_object)&& obstacles_animation_2 < 101) {
            game_started = 0;
            printf("You died\n");
        }
        if(obstacles_animation_3 > 99.1 && (position_3 == moving_object)&& obstacles_animation_3 < 101) {
            game_started = 0;
            printf("You died\n");
        }
        if(obstacles_animation_4 > 99.1 && (position_4 == moving_object)&& obstacles_animation_4 < 101) {
            game_started = 0;
            printf("You died\n");
        }
        if(obstacles_animation_5 > 99.1 && (position_5 == moving_object)&& obstacles_animation_5 < 101) {
            game_started = 0;
            printf("You died\n");
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
        /*Rotacija dole za padanje*/
        glRotatef(-(moving_object)/2.0*falling_down*15, 0, 0, 1);
        /*Translacija za pomeranje i skakanje*/
        glTranslatef(moving_object, 1 - cos(2*jumping*PI), 0);
        /*Rotacija pri skakanju*/
        glRotatef(15*(1 - cos(2*PI*jumping)), 1, 0, 0);
        /*Translacija i Rotacija pri padanju*/
        glTranslatef((moving_object)/2.0*falling_translate, -falling_translate/3.0, 0);
        glRotatef(-(moving_object)/2.0*falling_rotate*15, 0, 0, 1);
        /*Iscrtavanje objekta*/
        drawCube();
    glPopMatrix();

    if(obstacles_animation >= 105) 
        obstacles_animation = 0;
    /*Iscrtava se prepreka*/
    glPushMatrix();
        glTranslatef(position, 0,obstacles_animation - 100);
        obstacles();
    glPopMatrix();
    
    if(obstacles_animation_2 >= 105)
        obstacles_animation_2 = 0;
    glPushMatrix();
        glTranslatef(position_2, 0, obstacles_animation_2 - 100);
        obstacles();
    glPopMatrix();

    if(obstacles_animation_3 >= 105)
        obstacles_animation_3 = 0;
    glPushMatrix();
        glTranslatef(position_3, 0, obstacles_animation_3 - 100);
        obstacles();
    glPopMatrix();

    if(obstacles_animation_4 >= 105)
        obstacles_animation_4 = 0;
    glPushMatrix();
        glTranslatef(position_4, 0, obstacles_animation_4 - 100);
        obstacles();
    glPopMatrix();
    
    if(obstacles_animation_5 >= 105)
        obstacles_animation_5 = 0;
    glPushMatrix();
        glTranslatef(position_5, 0, obstacles_animation_5 - 100);
        obstacles();
    glPopMatrix();

  glutSwapBuffers();
}
