#include <windows.h>	//Inclusion de variables de windows
#include <glut.h>	//Inclusion de GLUT
#include <GL/gl.h>		//Inclusion de GL		 	
#include <GL/glu.h>	
#include <stdio.h>
#include <stdlib.h>
#include <math.h>	//Inclusion de librerias auxiliares	
#include "SOIL.h"


#define PI 3.1416
int W_WIDTH = 500;	 //Ancho de la ventana
int W_HEIGHT = 500;		//Alto de la ventana
int perspectiva = 0;
//camaras
int cam=0;

int cubos[20];

GLuint indice;//cubo lista
int error;
boolean poli;
//texturas
GLuint texture[10];
GLuint waterTexture[25];
int a;
// Ángulos de rotación
static GLfloat yRot = 0.0f;
int cont=0;
int ya;
float aux[10], aux2[10];
//menu
int dir = 0;
int modo=0;

//cronometro
double tiempo = 0;
int t = 0;

//colisiones
int col=0;

extern void esfera(double);
extern void myTeclasespeciales(int, int, int);

enum {
	MENU_MODO,
	MENU_CAMBIAR,
	MENU_POLYMODE,
	MENU_EXIT
};

BOOL g_bFillPolygons = TRUE;
//posicion luz
GLfloat lightPosA[] = { 0,200,0,1.0f };


typedef struct {//struct objetos
	float px, py, pz; // posicion
	float angulo_trans; // angulo giro y
	float radio;
	GLuint tex; // lista de render
	float velocidad_t;
	float vy;
} objeto;

objeto cubo1;
objeto cubo2;
objeto cubo3;
objeto cubo4;
objeto cubo5;
objeto cubo6;
objeto cubo7;
objeto cubo8;
objeto cubo9;
objeto cubo0;
objeto *bola;


int cargaBMP() {

	bola->tex = SOIL_load_OGL_texture("Azul.JPG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
	if (bola->tex == 0) {
		return 5;
	}
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	return 0;
}

void initBola() {
	myEsfera();
	bola = (objeto *)malloc(sizeof(objeto));
	bola->px = 0;
	bola->py = 5;
	bola->pz = 0;
	bola->angulo_trans = 90;
	bola->velocidad_t = 0;
	bola->radio = 3;
}

void myTeclado(unsigned char tras){
	if (modo == 0){
		switch (tras) {
		case 'w':
			if (bola->velocidad_t >= 2.0f) {
				bola->velocidad_t = 2.0f;

			}
			else bola->velocidad_t -= 0.5f;

			break;
		default:
			break;
		case 's':
			if (bola->velocidad_t <= 0.0f) {
				bola->velocidad_t = 0.0f;
				break;
			}
			else {
				bola->velocidad_t += 1.0f;
				break;
			}
		case 'a':
			if (bola->velocidad_t == 0.0f) {
				break;
			}
			else {
				if (bola->angulo_trans <= 5.0f || bola->angulo_trans >= -5.0f) {
					bola->angulo_trans -= 1.0;
				}
				else bola->angulo_trans += 1;
				break;
			}
		case 'd':
			if (bola->velocidad_t == 0.0f) {
				break;
			}
			else {
				if (bola->angulo_trans <= 5.0f || bola->angulo_trans >= -5.0f) {
					bola->angulo_trans += 1.0;
				}
				else bola->angulo_trans -= 1;
				break;
			}
		}
	}
	else {
		switch (tras) {
		case 'w':
			if (bola->velocidad_t >= 3.0f) {
				bola->velocidad_t = 3.0f;

			}
			else bola->velocidad_t += 0.1f;
			if (dir == 0) {
				bola->angulo_trans += 90.0f;
			}
			else bola->angulo_trans -= 90.0f;
			break;
		default:
			break;

		
		}
	}
	// Se se modificou algo redebúxase
	glutPostRedisplay();
}

void actualizarCamara1() {
	glViewport(0, 0, W_WIDTH, W_HEIGHT);
	//Configuración de la matriz de proyeccion
	glMatrixMode(GL_PROJECTION);
	//La ponemos auno
	glLoadIdentity();
	gluPerspective(60, (GLfloat)W_WIDTH / (GLfloat)W_HEIGHT, 0.1, 800);
	gluLookAt(bola->px, bola->py + 10, bola->pz + 50, bola->px, bola->py + 10, bola->pz - 100, 0, 1, 0);
	glutPostRedisplay();
}

void actualizarCamara2() {
	glViewport(0, 0, W_WIDTH, W_HEIGHT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (float)W_WIDTH / (float)W_HEIGHT, 1.0, 800.0);
	gluLookAt(bola->px *cos(-bola->angulo_trans*PI / 180), 15, bola->pz+50, bola->px, 0, bola->pz, 0, 1, 0);
	glutPostRedisplay();
}

void iluminacion() {

	//Variables de iluminacion
	GLfloat Ambient1[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	GLfloat Diffuse1[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	// Detectar profundidad de los objetos y no pintar caras ocultas.

	// Normaliza las normales
	glEnable(GL_NORMALIZE);
	// Definimos las iluminaciones
	glLightfv(GL_LIGHT0, GL_AMBIENT, Ambient1);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, Diffuse1);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosA);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glEnable(GL_COLOR_MATERIAL);
}


void colocaBola(double size) {

	glTranslatef(bola->px, bola->py, bola->pz);
	glRotatef(bola->angulo_trans, 0.0f, 1.0f, 0.0f);
	//glBindTexture(GL_TEXTURE_2D, bola.tex);
	esfera(size);
	//glBindTexture(GL_TEXTURE_2D, 0);
	glutPostRedisplay();
}

void colocaSuelo() {//preguntar al profesor
	float x, z;


		glBegin(GL_QUADS);
		for (x = -40.0f; x <= 30.0f; x += 10.0f) {
			for (z = 50.0f; z >= -400; z -= 10.0f) {//haceer random	
				glNormal3f(0.0f, 1.0f, 0.0f);
				glTexCoord2f(0.0f, 0.0f); glVertex3f(x, 0.0f, z);
				glTexCoord2f(1.0f, 0.0f); glVertex3f(x + 10.0f, 0.0f, z);
				glTexCoord2f(1.0f, 1.0f); glVertex3f(x + 10.0f, 0.0f, z - 10.0f);
				glTexCoord2f(0.0f, 1.0f); glVertex3f(x, 0.0f, z - 10.0f);
			}
		}
		glEnd();

	
	glutPostRedisplay();
}

void colocaPared() {
	float x, z;

	for (x = -40.0f; x <= 30.0f; x += 10.0f) {
		for (z = 10.0f; z >= -400; z -= 10.0f) {//haceer random	
			if (x == -40 || x == 30) {
				glPushMatrix();
				glTranslatef(x, 0, z);
				glCallList(indice);
				glPopMatrix();
			}
		}
	}
	glEnd();


	glutPostRedisplay();
}

void colocaMeta() {
	glPushMatrix();
	glTranslatef(0, 0, -400);
	glutSolidTorus(20, 50, 20, 30);
	glTranslatef(0, 40, 32);
	glScalef(4, 1, 1);
	glColor3f(1, 1, 1);
	glCallList(indice);
	glPopMatrix();
	glutPostRedisplay();
}

void colocaSalida() {
	glPushMatrix();
	glTranslatef(0, 10, 0);
	glutSolidTorus(20, 50, 20, 30);

	glPopMatrix();
	glutPostRedisplay();
}


void colocaEstrellas() {

	glColor3f(1, 1, 1);
	for (int j = 1; j <= 100; ++j) {
		for (int i = 0; i < 20; i++) {
			glPushMatrix();
			glTranslatef((i*aux[i] / 100) - aux2[i] - 10*i*j+10, (i*(-aux2[i]) / 100) + aux[i]*j+i, -500 - i + (aux[i] / aux[2])+i*j+i);
			glutSolidSphere(1, 20, 20);
			glPopMatrix();

		}
	}

	glutPostRedisplay();
}

void situaCubos() {
	glColor3f(0.0f, 1.0f, 0.0f);
	for (int i = 0; i < 10 ; i++) {
		glPushMatrix();
		glTranslatef(aux[i], 0, aux2[i]);
		glCallList(indice);
		glPopMatrix();
	}
}

void dibujaCubo(int x, int z){
	
	indice = glGenLists(1);

	glNewList(indice, GL_COMPILE);
	//glBindTexture(GL_TEXTURE_2D, texture[2]);
	glBegin(GL_QUADS);
	glNormal3f(1.0f, 0.0f, 0.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x+5.0f, 0.02f, z+5.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x+5.0f, 0.02f, z-5.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x+5.0f, 10.02f, z-5.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x+5.0f, 10.02f, z+5.0f);
	glEnd();
	//glBindTexture(GL_TEXTURE_2D, texture[3]);
	glBegin(GL_QUADS);
	glNormal3f(1.0f, 0.0f, 0.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x-5.0f, 0.02f, z-5.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x-5.0f, 0.02f, z+5.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x-5.0f, 10.02f, z+5.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x-5.0f, 10.02f, z-5.0f);
	glEnd();
	//glBindTexture(GL_TEXTURE_2D, texture[4]);
	glBegin(GL_QUADS);
	glNormal3f(1.0f, 0.0f, 0.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x-5.0f, 0.02f, z+5.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x+5.0f, 0.02f, z+5.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x+5.0f, 10.02f, z+5.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x-5.0f, 10.02f, z+5.0f);
	glEnd();
	//glBindTexture(GL_TEXTURE_2D, texture[4]);
	glBegin(GL_QUADS);
	glNormal3f(1.0f, 0.0f, 0.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x+5.0f, 0.02f, z-5.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x-5.0f, 0.02f, z-5.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x-5.0f, 10.02f, z-5.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x+5.0f, 10.02f, z-5.0f);
	glEnd();
	//glBindTexture(GL_TEXTURE_2D, texture[5]);
	glBegin(GL_QUADS);
		glNormal3f(0.0f, 1.0f, 0.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x+5.0f, 10.02f, z+5.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x+5.0f, 10.02f, z-5.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x-5.0f, 10.02f, z-5.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x-5.0f, 10.02f, z+5.0f);
	glEnd();
	//glBindTexture(GL_TEXTURE_2D, 0);
	glEndList();

}

int colisiones() {
	//if ((bola->.px - bola.radio < cubo0.px +5 && bola.px + bola.radio > cubo0.px -5) && (bola.pz - bola.radio > cubo0.pz +5 && bola.pz + bola.radio < cubo0.pz-5)) {
		//return 1;
//	}
/* 
	else if ((bola.px - bola.radio < cubo1.px + 5 && bola.px + bola.radio > cubo1.px - 5) && (bola.pz - bola.radio < cubo1.pz + 5 || bola.pz + bola.radio > cubo1.px - 5)) {
		return 1;
	}
	else if ((bola.px - bola.radio < cubo2.px + 5 || bola.px + bola.radio > cubo2.px - 5) && (bola.pz - bola.radio < cubo2.pz + 5 || bola.pz + bola.radio > cubo2.px - 5)) {
		return 1;
	}
	else if ((bola.px - bola.radio < cubo3.px + 5 || bola.px + bola.radio > cubo3.px - 5) && (bola.pz - bola.radio < cubo3.pz + 5 || bola.pz + bola.radio > cubo3.px - 5)) {
		return 1;
	}
	else if ((bola.px - bola.radio < cubo4.px + 5 || bola.px + bola.radio > cubo4.px - 5) && (bola.pz - bola.radio < cubo4.pz + 5 || bola.pz + bola.radio > cubo4.px - 5)) {
		return 1;
	}
	else if ((bola.px - bola.radio < cubo5.px + 5 || bola.px + bola.radio > cubo5.px - 5) && (bola.pz - bola.radio < cubo5.pz + 5 || bola.pz + bola.radio > cubo5.px - 5)) {
		return 1;
	}
	else if ((bola.px - bola.radio < cubo6.px + 5 || bola.px + bola.radio > cubo6.px - 5) && (bola.pz - bola.radio < cubo6.pz + 5 || bola.pz + bola.radio > cubo6.px - 5)) {
		return 1;
	}
	else if ((bola.px - bola.radio < cubo7.px + 5 || bola.px + bola.radio > cubo7.px - 5) && (bola.pz - bola.radio < cubo7.pz + 5 || bola.pz + bola.radio > cubo7.px - 5)) {
		return 1;
	}
	else if ((bola.px - bola.radio < cubo8.px + 5 || bola.px + bola.radio > cubo8.px - 5) && (bola.pz - bola.radio < cubo8.pz + 5 || bola.pz + bola.radio > cubo8.px - 5)) {
		return 1;
	}
	else if ((bola.px - bola.radio < cubo9.px + 5 || bola.px + bola.radio > cubo9.px - 5) && (bola.pz - bola.radio < cubo9.pz + 5 || bola.pz + bola.radio > cubo9.px - 5)) {
		return 1;
	}*/
	//else return 0;
}

void victoria() {
	
}

void controlFinal() {
	if (tiempo != 0.0f || bola->pz>-400) {
		tiempo += 0.0001;
	}
	if (bola->pz <= -400 && t == 0) {
		printf("Tiempo transcurrido: %f\n", tiempo);
		if (modo == 0) {
			printf("Modo facil\n\n");
		}
		else printf("Modo dificil\n\n");
		t = 1;
		tiempo = 0;
		victoria();
	}
	if (bola->pz <= -450) {
		bola->angulo_trans += 20;
		if (modo == 0)	bola->velocidad_t = 0.3f;
		else bola->velocidad_t = 0;
	}
}

// Funcion de dibukop

void myDisplay (void) {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClearDepth(1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	iluminacion();

	switch (cam)
	{
	case 0:
		actualizarCamara2();
		break;
	case 1:
		actualizarCamara1();
		break;
	default:
		break;
	}



	// Incluye una matriz de rotacion en todos los puntos que se dibujen
	glMatrixMode(GL_MODELVIEW);
	// Inicializamos la matriz del modelo a la identidad
	glLoadIdentity();



	glColor3f(1.0f, 1.0f, 1.0f);
	
	colocaSuelo();//dibuja suelo
	colocaSalida();
	colocaPared();
	glDisable(GL_LIGHTING); // Deshabilitamos las luces
	colocaEstrellas();
	glEnable(GL_LIGHTING);//hABILITAMOS LUCES


	situaCubos();
	colocaMeta();

	glColor3f(1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, bola->tex);
	colocaBola(bola->radio);
	//glBindTexture(GL_TEXTURE_2D, 0);
	printf("%f %f\t", bola->px, bola->pz);

	col = colisiones();
	if (col == 1) {
		exit(0);
	}

	controlFinal();
	
	glFlush();
	glutSwapBuffers();
		
}	

void actualizar() {
	bola->px = bola->px + bola->velocidad_t*cos(bola->angulo_trans*PI / 180);
	bola->pz = bola->pz + bola->velocidad_t*sin(bola->angulo_trans*PI / 180);
	glutPostRedisplay();
	glutTimerFunc(40, actualizar, 0);
}


void SelectFromMenu(int idCommand)
{
	switch (idCommand)
	{
	case MENU_MODO:
		bola->px = 0;
		bola->pz = 0;
		bola->velocidad_t = 0;
		tiempo = 0;
		t = 0;
		if (modo==0) {
			modo = 1;
			cam = 1;
			bola->angulo_trans = 0;
		}
		else {
			modo = 0;
			cam = 0;
			bola->angulo_trans = 90;
		}
		break;
	case MENU_CAMBIAR:
		bola->px = 0;
		bola->pz = 0;
		bola->velocidad_t = 0;
		tiempo = 0;
		t = 0;
		if (modo == 1) {
			if (dir == 0) {
				dir = 1;
				bola->angulo_trans = 0;
			}
			else {
				dir = 0;
				bola->angulo_trans = 180;
			}
		}	
		break;

	case MENU_POLYMODE:
		g_bFillPolygons = !g_bFillPolygons;
		glPolygonMode(GL_FRONT_AND_BACK, g_bFillPolygons ? GL_FILL : GL_LINE);
		break;

	case MENU_EXIT:
		exit(0);
		break;
	}
	glutPostRedisplay();
}


int ConstruirPopUpMenu(void)
{
	int menu;

	menu = glutCreateMenu(SelectFromMenu);
	glutAddMenuEntry("Cambiar Modo", MENU_MODO);
	glutAddMenuEntry("Cambiar Giro", MENU_CAMBIAR);
	glutAddMenuEntry("Polygon Mode", MENU_POLYMODE);
	glutAddMenuEntry("Exit", MENU_EXIT);

	return menu;
}


void changeSizec(GLint newWidth, GLint newHeight) {
	W_WIDTH = newWidth;
	W_HEIGHT = newHeight;
	glutPostRedisplay();
}

int main(int argc, char **argv) {
	int r = 0;
	glutInit(&argc, argv);
	//Inicializa la ventana en una determinada posicion
	glutInitWindowPosition(0,0);  
	 //Inicializa el tamano de la funcion
	glutInitWindowSize (W_WIDTH, W_HEIGHT);
	//Inicaliza el modeo de display, RGBA y Doble buffer
	glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE);
	// Nombre de la ventana
	glutCreateWindow("BolaRun");
	// llamada a la funcion de dibujo
	//Incializaciones varias
	dibujaCubo(0, 0);
	//Eventos
	srand(time(NULL));
	for (int i=0; i < 10; i++) {
		aux[i] = 1 + rand() % 4;
		aux2[i] =1 + rand() % 400;
		aux[i] = 40-(20 * aux[i] - 10);
		aux2[i] = 1 - aux2[i];
		printf("%f %f\n", aux[i], aux2[i]);
		if (i == 0) {
			cubo0.px = aux[i];
			cubo0.pz = aux2[i];
			printf("%f %f\n", cubo0.px, cubo0.pz);
		}
		else if (i == 1) {
			cubo1.px = aux[i];
			cubo1.pz = aux2[i];
		}
		else if (i == 2) {
			cubo2.px = aux[i];
			cubo2.pz = aux2[i];
		}
		else if (i == 3) {
			cubo3.px = aux[i];
			cubo3.pz = aux2[i];
		}
		else if (i == 4) {
			cubo4.px = aux[i];
			cubo4.pz = aux2[i];
		}
		else if (i == 5) {
			cubo5.px = aux[i];
			cubo5.pz = aux2[i];
		}
		else if (i == 6) {
			cubo6.px = aux[i];
			cubo6.pz = aux2[i];
			printf("%f %f\n", cubo6.px, cubo6.pz);
		}
		else if (i == 7) {
			cubo7.px = aux[i];
			cubo7.pz = aux2[i];
		}
		else if (i == 8) {
			cubo8.px = aux[i];
			cubo8.pz = aux2[i];
		}
		else if (i == 9) {
			cubo9.px = aux[i];
			cubo9.pz = aux2[i];
		}
	}

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_NORMALIZE);
	glEnable(GL_TEXTURE_2D);
	glShadeModel(GL_SMOOTH);

	r = 0;
	r = cargaBMP();
	printf("Erro: %d", r);

	glutKeyboardFunc(myTeclado);
	glutDisplayFunc(myDisplay);
	glutTimerFunc(40, actualizar, 0);
	

	
	tiempo = 0.0f;
	cam = 0;
	modo = 0;
	t = 0;
	//col = 0;
	ya = 200 + rand() % (401-200);
	

	ConstruirPopUpMenu();
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	glutReshapeFunc(changeSizec);


	// Empieza en bucle principal
	glutMainLoop();
	return 0;
}