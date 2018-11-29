#include<GL/glut.h>

static int window_width, window_height;

static void on_keyboard(unsigned char key, int x, int y);
static void on_reshape(int width, int height);
static void on_display(void);
int k = 0;
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
    glLineWidth(4);

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
            {
                 k = k + 1;
                 on_display();
            }
            break;
        /* Pomeramo objekat ulevo i iscrtavamo ga*/
        case 'a':
            {
                 k = k -1;
                 on_display();
            }
            break;
    }
}

static void on_reshape(int width, int height)
{
    window_width = width;
    window_height = height;
}

static void on_display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glViewport(0, 0, window_width, window_height);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, window_width/(float)window_height, 1, 25);
    glMatrixMode(GL_MODELVIEW);

    glLoadIdentity();
    gluLookAt(0, 2, 5, 0, 0, 0, 0, 1, 0);
    
    /*Iscrtavamo objekat*/
    glTranslatef(k, 0, 0);
    glColor3f(0.7, 0.7, 0.7);
    glutSolidCube(1);
    glColor3f(0, 0, 0);
    glutWireCube(1);

    glutSwapBuffers();
}
