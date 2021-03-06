#include <GL\glew.h>
#include <GL\freeglut.h>
#include <iostream>
#include <stdlib.h> 

using namespace std;

/* Proyecto 3 - Computaci�n Gr�fica
Abr-Jul 2016
Ultima modificaci�n: 29/05/16
autores:
Marisela Del Valle 11-10267
Nabil J. Marquez   11-10683
*/

//Inicializadores
#define DEF_floorGridScale	1.0f
#define DEF_floorGridXSteps	10.0f
#define DEF_floorGridZSteps	10.0f

GLfloat ctlpointsNurbsSurf[4][4][3];
GLfloat knotsSurfs[25] = {0};



GLUnurbsObj *theNurbSurf;
float t;

// Variables de la ecuaci�n de la ola
GLfloat ctlpoints[21][21][3] = {0};
GLfloat L[2] = {0};
GLfloat A[2] = {0};
GLfloat S[2] = {0};
GLfloat Dx[2] = {0};
GLfloat Dz[2] = {0};
GLfloat W[2] = {0};
GLfloat G[2] = {0};
int  waveID = 0;   //By default, it changes wave 1.
bool paused = true;

#define PI 3.1415926535897932384626433832795

// Puntos de control
void puntosNurb(){
	glColor3f(1.0,0.0,0.0);
	float xpoint = -10;
	float ypoint = 0;
	float zpoint = -10;

	glBegin(GL_LINES);
	for(int i = 0; i <=20; i++){
		for(int j = 0; j <=20; j++){
			ctlpoints[i][j][0]=xpoint++;
			ctlpoints[i][j][1]=0;
			ctlpoints[i][j][2]=zpoint;
		}
		xpoint = -10;
		ypoint = 0;
		zpoint++;
	}
		
	glEnd();

	glColor3f(0.0,1.0,0.0);

}

void init_surface(){

}

// Creamos la ecuaci�n con los valores dados
GLfloat newY(GLfloat X, GLfloat Z,GLfloat T){
	return (A[0]*sin((Dx[0]*X+Dz[0]*Z)*W[0]+T*G[0])) + (A[1]*sin((Dx[1]*X+Dz[1]*Z)*W[1]+T*G[1])) ;
}

// Actualizamos valores de la ecuacion y los puntos de control
void updateY(int value){
	if (!paused) {
		t += 0.1;
		for (int i = 0; i < 2; i++){
			W[i] = 2*PI / L[i];
			G[i] = S[i]*2*PI / L[i];
		}
			for (int i = 0; i <21; i++) {
				for (int j = 0; j < 21; j++) {
						ctlpoints[i][j][1] = newY(ctlpoints[i][j][0],ctlpoints[i][j][2],t);
				}
			}
	}
	glutTimerFunc(10,updateY,1);
	glutPostRedisplay();
}

// Se imprimen los valores que toman las olas
void imprimirVariables() {
	system("cls"); // Clear screen
	if (paused)
		printf("---Paused---\n\n\n");
	printf("%s\n\n", "Ola 1");
	printf("%s%f\n", "wL = ", L[0]);
	printf("%s%f\n", "aP = ", A[0]);
	printf("%s%f\n", "sP = ", S[0]);
	printf("%s%f\n", "dirX = ", Dx[0]);
	printf("%s%f\n\n", "dirY = ", Dz[0]);
	printf("\n\n===================\n\n");
	printf("%s\n\n", "Ola 2");
	printf("%s%f\n", "wL = ", L[1]);
	printf("%s%f\n", "aP = ", A[1]);
	printf("%s%f\n", "sP = ", S[1]);
	printf("%s%f\n", "dirX = ", Dx[1]);
	printf("%s%f\n\n", "dirY = ", Dz[1]);
}

// Valores iniciales para las olas
void initOlas(){
	// Ola 1
	L[0] = 8;
	A[0] = 0.4;
	S[0] = 2;
	Dx[0]= 0;
	Dz[0]= -1;
	// Ola 2
	L[1] = 4;
	A[1] = 0;
	S[1] = 0;
	Dx[1]= 1;
	Dz[1]= 1;
}

//Puntos de control
void init(){
	knotsSurfs[4]=0.058;
	for (int i = 0; i<4; i++){
		knotsSurfs[i]=0;
		knotsSurfs[24-i]=1;
	}
	for (int i = 5; i<21; i++){
		knotsSurfs[i]=knotsSurfs[i-1]+0.058;
	}
	for (int i = 0; i<25; i++){
		printf("knotsSurfs[%d] %f\n",i,knotsSurfs[i]);
	}
   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);
   glEnable(GL_DEPTH_TEST);
   glEnable(GL_AUTO_NORMAL);
   glEnable(GL_NORMALIZE);

   theNurbSurf = gluNewNurbsRenderer();
   gluNurbsProperty(theNurbSurf, GLU_SAMPLING_TOLERANCE, 15.0);
   gluNurbsProperty(theNurbSurf, GLU_DISPLAY_MODE, GLU_FILL);

   init_surface();

	puntosNurb();
	glutTimerFunc(10,updateY,1);
	t = 0.0;
	
	//Iniciamos las olas con los valores correspondientes
	initOlas();
	imprimirVariables();
}

void changeViewport(int w, int h) {
	
	t += 0.1;
	float aspectratio;

	if (h==0)
		h=1;

   glViewport (0, 0, (GLsizei) w, (GLsizei) h); 
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();
   gluPerspective(30, (GLfloat) w/(GLfloat) h, 1.0, 200.0);
   glMatrixMode (GL_MODELVIEW);
  
}

// Render principal
void render(){

	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	GLfloat zExtent, xExtent, xLocal, zLocal;
    int loopX, loopZ;

	glLoadIdentity ();                       
	gluLookAt (25.0, 12.0, 4.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	// Luz y material
	GLfloat mat_diffuse[] = { 0.6, 0.6, 0.9, 1.0 };
	GLfloat mat_specular[] = { 0.8, 0.8, 1.0, 1.0 };
	GLfloat mat_shininess[] = { 60.0 };
	
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

    GLfloat light_ambient[] = { 0.0, 0.0, 0.2, 1.0 };
	GLfloat light_diffuse[] = { 0.8, 0.8, 0.8, 1.0 };
	GLfloat light_specular[] = { 0.6, 0.6, 0.6, 1.0 };
	GLfloat light_position[] = { -10.0, 5.0, 0.0, 1.0 };

	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);   


	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_LINE_SMOOTH);

	glPushMatrix();

	// Inicializamos la superficie
	gluBeginSurface(theNurbSurf);
		gluNurbsSurface(theNurbSurf, 25, knotsSurfs, 25, knotsSurfs, 21*3, 3, &ctlpoints[0][0][0], 4, 4, GL_MAP2_VERTEX_3);
	gluEndSurface(theNurbSurf);

	glPopMatrix();
	
	glEnable(GL_LIGHTING);

	glDisable(GL_BLEND);
	glDisable(GL_LINE_SMOOTH);

	glutSwapBuffers();
}

// L�gica del teclado para cambiar valores de las olas
void Keyboard(unsigned char key, int x, int y)
{
  switch (key)
  {
	case 27:             
		exit (0);
		break;

	// Switch to wave 1
	case '1':
		waveID = 0;
		break;

	// Switch to wave 2
	case '2':
		waveID = 1;
		break;

	// Empezar la animacion de las olas
	case 'R': ; 
	case 'r':
		paused = false;
		imprimirVariables();
		break;
  
	// Detener la animacion de las olas
	case 'P':
	case 'p':
		paused = true;
		break;

	// L: Distancia entre cada ola
	case 'A':
	case 'a':
			L[waveID]-=0.1;
			imprimirVariables();
		break;

	case 'Z':
	case 'z':
			L[waveID]+=0.1;
			imprimirVariables();
		break;

	// A: altura de la ola
	case 'S':
	case 's':
			A[waveID]-=0.1;
			imprimirVariables();
		break;

	case 'X':
	case 'x':
			A[waveID]+=0.1;
			imprimirVariables();
		break;

	// S: velocidad de la ola
	case 'D':
	case 'd':
			S[waveID]-=0.1;
			imprimirVariables();
		break;

	case 'C':
	case 'c':
			S[waveID]+=0.1;
			imprimirVariables();
		break;

	// D: vector de dos coordenadas que determina la direcci�n de la ola - dirX1 y dirX2
	case 'F':
	case 'f':
			Dx[waveID]-=0.1;
			imprimirVariables();
		break;

	case 'V':
	case 'v':
			Dx[waveID]+=0.1;
			imprimirVariables();
		break;

	// D: vector de dos coordenadas que determina la direcci�n de la ola - dirY1 y dirY2
	case 'G':
	case 'g':
			Dz[waveID]-=0.1;
			imprimirVariables();
		break;

	case 'B':
	case 'b':
			Dz[waveID]+=0.1;
			imprimirVariables();
		break;
	}
}

int main (int argc, char** argv) {

	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);

	glutInitWindowSize(960,540);

	glutCreateWindow("Nurbs Proyecto - Ola");

	init();

	glutReshapeFunc(changeViewport);
	glutDisplayFunc(render);
	glutKeyboardFunc(Keyboard);

	GLenum err = glewInit();
	if (GLEW_OK != err) {
		fprintf(stderr, "GLEW error");
		return 1;
	}
	

	glutMainLoop();
	return 0;

}