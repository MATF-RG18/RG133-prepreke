#include<GL/glut.h>
#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include<time.h>

/*Definisanje naziva timera*/
#define TIMER_INTERVAL 20
#define TIMER_JMP 1 /*Timer za skakanje*/
#define TIMER_FALL 2 /*Timer za padanje*/
#define TIMER_OBSTACLES 3 /*Timer za prepreke*/

#define PI 3.14159265358979323

static int window_width, window_height;

/*Deklaracija callback funkcija*/
static void on_keyboard(unsigned char key, int x, int y);
static void on_reshape(int width, int height);
static void on_display(void);
static void on_timer(int value);

/*Definisanje flegova i parametara*/
int moving_object = 0; /*Fleg za pomeranje objekta levo i desno*/
int game_started = 0; /*fleg za startovanje igre*/

int jumping_animation = 0; /*fleg za zapoceto skakanje*/
float jumping = 0; /*parametar za skakanje*/

int falling_animation = 0; /*fleg za padanje*/
float falling_rotate = 0; /*parametar za rotaciju padanja oko svoje ose*/
float falling_translate = 0; /*parametar za translaciju pri padanju*/
float falling_down = 0; /*parametar za rotaciju i translaciju padanja sa podijuma*/

/*Parametri za animaciju prepreka, ima ih 5 na udaljenosti 20, duzina podijuma je 100 100/20=5*/
float obstacles_animation = 0;     /*1*/ 
float obstacles_animation_2 = -20; /*2*/
float obstacles_animation_3 = -40; /*3*/
float obstacles_animation_4 = -60; /*4*/
float obstacles_animation_5 = -80; /*5*/

/*Parametri za pozicuju prepreka*/
int position = 0;   /*1*/
int position_2 = 0; /*2*/
int position_3 = 0; /*3*/
int position_4 = 0; /*4*/
int position_5 = 0; /*5*/

unsigned Score = 0; /*Parametar za racunanje rezultata*/
float speed = 0; /*Parametar za racunanje brzine prepreka*/

/*Funkcije za iscrtavanje*/
static void drawCube();
static void drawStage();
static void obstacles();

int main(int argc, char **argv)
{
    /*Inicijalizuje se GLUT*/
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB |GLUT_DEPTH | GLUT_DOUBLE);
    
    /*Ispis kontrola u teminalu*/
    printf("ESC - to close the game;\n"
	   "SPACE - to start the game;\n"
	   "R - to restart the game;\n"
	   "A - to go left;\n"
	   "D - to go right;\n"
	   "W - to jump;\n");

    /*Kreira se prozor*/
    glutInitWindowSize(600, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Prepreke");
    
    /*Registruje se funkcija za obradu dogadjaja*/
    glutKeyboardFunc(on_keyboard);
    glutReshapeFunc(on_reshape);
    glutDisplayFunc(on_display);
     
    /*Obavlja se OpenGL Inicijalizuja*/
    glClearColor(0.9, 0.9, 0.9, 0);
    glEnable(GL_DEPTH_TEST);

    /*Nameštamo osvetljenje*/
    GLfloat light_position[] = {3, 3, 1, 0};
    GLfloat light_ambient[] = {0.6, 0.6, 0.6, 1};
    GLfloat light_diffuse[] = {0.3, 0.3, 0.3, 1};        
    GLfloat light_specular[] = {0.5, 0.5, 0.5, 0};
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

    
    /*Inicijalizuju se pozicije prepreka random,  s obzirom da sam ih po defalutu 
     *stavio na 0 da bi i na prvom generisanju bile random mora i ovde*/
    srand(time(NULL));
    /*Za svaku poziciju generisemo random broj 0, 1, 2
     od kojeg zavisi na kojoj ce se poziciji naci prepeka*/
    int r = rand() % 3;
    if(r == 0) 	position = -1;
    else if(r == 1) position = 1;
    else position = 0;
    /*Kod svake pozicije pokusavam da na jedinstven nacin dodelim poziciju 
     *odnosno zato se razikuju vrednosti pozicija za npr r == 0 
     *Valjda sam time zaista nesto postigao, ali to je bila ideja, da bude sto vise random*/
    r = rand() % 3;
    if(r == 0) position_2 = 0;
    else if(r == 1) position_2 = 1;
    else position_2 = -1;
    
    r = rand() % 3;
    if(r == 0) position_3 = 1;
    else if(r == 1) position_3 = 0;
    else position_3 = -1;
    
    r = rand() % 3;
    if(r == 0) position_4 = -1;
    else if(r == 1) position_4 = 0;
    else position_4 = 1;

    r = rand() % 3;
    if(r == 0) position_5 = -1;
    else if(r == 1) position_5 = 0;
    else position_5 = 1;
    
    /*Ulazimo u glavnu petlju*/
    glutMainLoop();

    return 0;
}

static void on_keyboard(unsigned char key, int x, int y)
{
    switch(key) {
        /*ESC - exit*/
        case 27:
            exit(0);
            break;
        /* Pomeramo objekat udesno i iscrtavamo ga*/
	case 'D':
        case 'd': 
            if(!game_started) break;
            moving_object++;
            glutPostRedisplay();
            break;
        /* Pomeramo objekat ulevo i iscrtavamo ga*/
        case 'a':
	case 'A':
            if(!game_started) break;
            moving_object--;
            glutPostRedisplay();
            break;
	/*Skace - tj zapocinje se animacija skakanja*/
        case 'w':
	case 'W':
            if(!game_started) break;
            else if(!jumping_animation) {
                    jumping_animation = !jumping_animation;
                    glutTimerFunc(TIMER_INTERVAL, on_timer, TIMER_JMP);
                }
                break;
        /*SPACE - Startuje se igra, provera za jumping animaciju je tu 
	 *da ne bi moglo da mi se desi da animacija prepeka nastavi da ide 
	 *ponovnim pritiskanjem tastera SPACE*/
        case ' ':
            if(!game_started && !jumping_animation) {
                game_started = 1;
                glutTimerFunc(TIMER_INTERVAL, on_timer, TIMER_OBSTACLES);
            }
            break;
	/*Restart igre svi parametri i flegovi se vracaju na pocetnu poziciju*/
        case 'r':
	case 'R':
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
            speed = 0;
            Score = 0;
            glutPostRedisplay();
            break;
    }
}

static void on_timer(int value)
{
   /*Timer za skakanje*/
    if(value == TIMER_JMP) {
	/*Povecavamo parametar skakanja*/
        jumping += 0.025;
	/*Ako je doslo do 1 skakanje je gotovo*/
        if(jumping >= 1) {
            jumping_animation = 0;
            jumping = 0;
        }
        /*Ponovo iscrtavamo*/
        glutPostRedisplay();
        /*Ako animacija skakanja nije zavrsena ponovo zovemo timer skakanja*/
        if(jumping_animation)
            glutTimerFunc(TIMER_INTERVAL, on_timer, TIMER_JMP);
    }
    /*Timer za padanje*/
    if(value == TIMER_FALL) {
	/*Ako se igra zavrsila,  tj umrli smo onda se zavrsava padanje
	 *Animacija padanja mi je bila najzahtevnija, pokusao sam da bude sto realnije
	 *Zato sam pokusao da iskomentarisem sto vise*/
        if(game_started != 0) {
	  /*Prvo rotacija oko svoje ose*/
	  if(falling_rotate < 1) {
	      falling_rotate += 0.07; /*povecava se parametar*/
	      glutPostRedisplay(); /*iscrtava se ponovo*/
	      if(falling_animation)
		glutTimerFunc(TIMER_INTERVAL, on_timer, TIMER_FALL);
	  }
	  /*Kada se zavrsila rotacija oko ose krecemo da klizimo i padamo*/
	  else if(falling_rotate < 2.8) {
	      falling_translate += 0.023; /*transliramo ga u stranu*/
	      falling_rotate += 0.07; /*i nastavljmo da rotiramo*/
	      glutPostRedisplay(); /*ponovo iscrtavamo*/
	      if(falling_animation)
		  glutTimerFunc(TIMER_INTERVAL, on_timer, TIMER_FALL);
	  }
	  /*Kada smo skliznuli dovoljno sada zapravo ide animacija rotacije oko koordinatnog pocetka, iz nekog razloga je meni ovako izgledalo najrealnije,
	   *a deo rotacije se zapravo ni ne vidi od podijuma, tako da zapravo i izgleda kao da pada na dole*/
	  else if(falling_down < 2){
	      falling_translate += 0.01; /*Mora da nastavi da se translira inace izgleda kao da prodje kroz podijum*/
	      falling_down += 0.1; /*Povecavamo rotaciju oko koordinatnog pocetka, ovo je kao neka gravitacija zato je ovako "jaka"*/
	      falling_rotate += 0.07; /*slicno kao i za translaciju*/
	      glutPostRedisplay(); /*ponovo se iscrtava*/
	      if(falling_animation)
		  glutTimerFunc(TIMER_INTERVAL, on_timer, TIMER_FALL);
	      }
	  /*Napokon se sve zavrsilo, tj pali smo dovoljno i igra se zavrsava*/    
	  else {
	      game_started = 0;
	      falling_animation = 0;
	  }
      }
      /*Ispisujemo poruku u terminalu*/
      else {
          printf("You died!\nScore: %u\n", Score);
      }
    }
    /*Timer za prepreke*/
    if(value == TIMER_OBSTACLES) {
        obstacles_animation += 0.25 + speed;  /*pomeramo prereke*/
        obstacles_animation_2 += 0.25 + speed;
        obstacles_animation_3 += 0.25 + speed;
        obstacles_animation_4 += 0.25 + speed;
        obstacles_animation_5 += 0.25 + speed;
        speed += 0.0001; /*Ubrzanje*/
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
/*objekat*/
static void drawCube()
{
    /*Materijali za objekat*/
    /*Koeficijenti preuzeti sa http://devernay.free.fr/cours/opengl/materials.html
     *Nakon cega sam zapravo menjao malo sam koeficijente*/ 
    GLfloat ambient_coefs[] = {0.0215, 0.1745, 0.0215, 1};
    GLfloat diffuse_coefs[] = {0.27568,  0.81424,  0.27568,  1};
    GLfloat specular_coefs[] = {0.633,  0.727811,  0.633,  1};
    GLfloat shininess = 0.6 * 128.0;

    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_coefs);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coefs);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular_coefs);
    glMaterialf(GL_FRONT, GL_SHININESS, shininess);
    
    /*Moj objekat je kocka*/    
    glutSolidCube(1);
      
    /*Menjam materijale nazad na crno kako bih iscrtao crne ivice kocke*/
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

    /*Ivice kocke*/
    glLineWidth(4);    
    glutWireCube(1);
}
/*podijum*/
static void drawStage()
{
    /*Materijali za podijum*/
    GLfloat ambient_coefs[] = {0.2, 0.2, 0.2, 1};
    GLfloat diffuse_coefs[] = {0.3,  0.3,  0.3,  1};
    GLfloat specular_coefs[] = {0.1,  0.1,  0.1,  1};
    GLfloat shininess = 0;

    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_coefs);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coefs);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular_coefs);
    glMaterialf(GL_FRONT, GL_SHININESS, shininess);
    
    /*Crtam kvadrat koji se nalazi malo ispod objekta*/
    glBegin(GL_POLYGON);
        glVertex3f(-2, -0.51,   4);
        glVertex3f( 2, -0.51,   4);
        glVertex3f( 2, -0.51, -100);
        glVertex3f(-2, -0.51, -100);
    glEnd();
}
/*prepreke*/
void obstacles()
{
    /*Materijali za prepreke*/
    GLfloat ambient_coefs[] = {0, 0, 1, 1};
    GLfloat diffuse_coefs[] = {0.07568,  0.61424,  0.07568,  1};
    GLfloat specular_coefs[] = {0.633,  0.727811,  0.633,  1};
    GLfloat shininess = 0.6 * 128.0;

    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_coefs);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coefs);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular_coefs);
    glMaterialf(GL_FRONT, GL_SHININESS, shininess);
    
    /*Generisem random pozicije prepeka
     *isto kao i na pocetku pri startovanju igre s tim da pazim to radim samo kada prepreka treba ponovo da se crta*/
    if(obstacles_animation == 0) {
        srand(time(NULL));
        int r = rand() % 3;
    
        if(r == 0) 
            position = -1;
        else if(r == 1) 
            position = 1;
        else
            position = 0;
    }
    if(obstacles_animation_2 == 0) {
        srand(time(NULL));
        int r = rand() % 3;
        if(r == 0)
            position_2 = 1;
        else if(r == 1)
            position_2 = 0;
        else
            position_2 = -1;
    }
    if(obstacles_animation_3 == 0) {
        srand(time(NULL));
        int r = rand() % 3;
        if(r == 0)
            position_3 = -1;
        else if(r == 1)
            position_3 = 0;
        else
            position_3 = 1;
    }
    if(obstacles_animation_4 == 0) {
        srand(time(NULL));
        int r = rand() % 3;
        if(r == 0)
            position_4 = 0;
        else if(r == 1)
            position_4 = 1;
        else
            position_4 = -1;
    }
    if(obstacles_animation_5 == 0) {
        srand(time(NULL));
        int r = rand() % 3;
        if(r == 0)
            position_5 = 1;
        else if(r == 1)
            position_5 = 0;
        else
            position_5 = -1;
    }
    /*Prepreka je kocka*/
    glutSolidCube(1);
}

static void on_display(void)
{
    /*Proveravamo da li je objekat dosao do ivice i pokrecemo animaciju padanja*/
    if(moving_object > 1 || moving_object < -1) { 
        if(!falling_animation) {
            falling_animation = 1;
            glutTimerFunc(TIMER_INTERVAL, on_timer, TIMER_FALL);
        }
    }
    
    /*Proveravamo da li smo udarili u prepreku i ako jesmo zaustavljamo igru i ispisujemo poruku i rezultat*/
    if(game_started != 0) {
    if(jumping < 0.25 || jumping > 0.75) {
        if(obstacles_animation > 99.1 && (position == moving_object)&& obstacles_animation < 101) {
            game_started = 0;
            printf("You died!\nScore: %u\n", Score);
        }
        if(obstacles_animation_2 > 99.1 && (position_2 == moving_object)&& obstacles_animation_2 < 101) {
            game_started = 0;
            printf("You died!\nScore %u\n", Score);
        }
        if(obstacles_animation_3 > 99.1 && (position_3 == moving_object)&& obstacles_animation_3 < 101) {
            game_started = 0;
            printf("You died!\nScore: %u\n", Score);
        }
        if(obstacles_animation_4 > 99.1 && (position_4 == moving_object)&& obstacles_animation_4 < 101) {
            game_started = 0;
            printf("You died!\nScore: %u\n", Score);
        }
        if(obstacles_animation_5 > 99.1 && (position_5 == moving_object)&& obstacles_animation_5 < 101) {
            game_started = 0;
            printf("You died!\nScore: %u\n", Score);
        }
    } }
    
    /*Postavlja se boja svih piksela na zadatu boju pozadine*/
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    /*Postavlja se viewport*/
    glViewport(0, 0, window_width, window_height);
    /*Postavljaju se parametri projekcije*/
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, window_width/(float)window_height, 1, 100);
    
    /*Postavlja se kamera*/
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0, 1, 6, 0, 1, 0, 0, 1, 0);
    
    /*Iscrtavamo podijum*/
    drawStage();
    
    /*Iscrtavamo objekat*/
    glPushMatrix();
        /*Rotacija dole za padanje, 
	 *moving_object sluzi za detekciju da li padamo levo ili desno*/
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
    
    
    /*Prepreka 1*/
    /*Ako nam je prepreka dosla iza nas, premestamo je nazad na pocetak
     * i uvecavamo Score*/
    if(obstacles_animation >= 105) { 
        obstacles_animation = 0;
        Score += 10;
    }
    /*Iscrtava se prepreka*/
    glPushMatrix();
        glTranslatef(position, 0,obstacles_animation - 100);
        obstacles();
    glPopMatrix();
    
    /*Prepreka 2*/
    if(obstacles_animation_2 >= 105) {
        obstacles_animation_2 = 0;
        Score += 10;
    }
    glPushMatrix();
        glTranslatef(position_2, 0, obstacles_animation_2 - 100);
        obstacles();
    glPopMatrix();

    
    /*Prepreka 3*/
    if(obstacles_animation_3 >= 105) {
        obstacles_animation_3 = 0;
        Score += 10;
    }
    glPushMatrix();
        glTranslatef(position_3, 0, obstacles_animation_3 - 100);
        obstacles();
    glPopMatrix();

    
    /*Prepreka 4*/
    if(obstacles_animation_4 >= 105) {
        obstacles_animation_4 = 0;
        Score += 10;
    }
    glPushMatrix();
        glTranslatef(position_4, 0, obstacles_animation_4 - 100);
        obstacles();
    glPopMatrix();
    
    /*Prepreka 5*/
    if(obstacles_animation_5 >= 105) {
        obstacles_animation_5 = 0;
        Score += 10;
    }
    glPushMatrix();
        glTranslatef(position_5, 0, obstacles_animation_5 - 100);
        obstacles();
    glPopMatrix();

  /*Postavlja se nova slika u prozor*/
  glutSwapBuffers();
}
