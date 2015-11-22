
#include <Windows.h>
#include <GL\glew.h>
#include <GL\freeglut.h>
#include <iostream>
#include <math.h>
#include <time.h>
#include <fstream>
#include <string>

using namespace std;
 
GLuint texName;
int arr[180][3];
int colour[][3] = { { 1, 0, 0 }, { 0, 1, 0 }, { 0, 0, 1 }, { 1, 1, 0 }, {0,1,1} };
int a = -300;
bool start = false;

bool swapColour = false;
int colour1, colour2, colour3, pixelx, pixely, square1, square2,pixelx2,pixely2;

void textureInit(){
	ifstream textureFile;
	string Line;
	GLfloat* texture;
	int textureHeight, textureWidth, grayScale;
	char* fileName = "rock.pgm";
	textureFile.open(fileName, ios::in);
	if (textureFile.fail()) {
		cout << "textureInit(): could not open file  " << fileName;
	}
	getline(textureFile, Line);
	getline(textureFile, Line);
	textureFile >> textureHeight;
	textureFile >> textureWidth;
	textureFile >> grayScale;

	texture = new GLfloat[textureHeight*textureWidth*3];

	for (int i = 1; i < textureHeight*textureWidth*3; i+=3){
		//for (int j = 1; j < textureWidth; j+=3){
			textureFile >> grayScale;
			texture[i + 0] = ((GLfloat)grayScale) / 255;
			texture[i+ 1] = ((GLfloat)grayScale) / 255;
			texture[i+ 2] = ((GLfloat)grayScale) / 255;
		//}
	}
	textureFile.close();

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight,
	0, GL_RGB, GL_FLOAT, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);


	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

	/*
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 1);
	glBegin(GL_QUADS);
	glColor3f(1, 1, 1);
	glTexCoord2f(0, 1);
	glVertex2f(0.0, 0.0);
	glTexCoord2f(1, 1);
	glVertex2f(40.0, 0.0);
	glTexCoord2f(1, 0);
	glVertex2f(40.0, 40.0);
	glTexCoord2f(0, 0);
	glVertex2f(0.0, 40.0);
	glEnd();

	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
	*/
}

void output(char *string)
{
	glColor3f(0, 0, 0);
	glRasterPos2f(-90, 0);
	int len;
	len = (int)strlen(string);
	for (int i = 0; i < len; i++) {
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, string[i]);
	}
}

void init(){
	glClearColor(1.0, 1.0, 1.0, 0);
	glMatrixMode(GL_PROJECTION|GL_BUFFER);
	textureInit();
	for (int i = 0; i < 182; i++){	
			int j = rand() % 5;
			arr[i][0] = colour[j][0];
			arr[i][1] = colour[j][1];
			arr[i][2] = colour[j][2];

	}

	gluOrtho2D(-300, 300, -255, 255);
}

void display(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	int i, j;

	
	int x = -300, y = -225, z = 255;

	glColor3d(0.49, 0.51, 0.52);
	glBegin(GL_QUADS);
	glVertex2f(-150, 60);
	glVertex2f(150, 60);
	glVertex2f(150, -60);
	glVertex2f(-150, -60);
	glEnd();
	output("Press <b> to start");
	glFlush();
	
	if (start){
		
		glColor3d(0.49, 0.51, 0.52);
		glBegin(GL_QUADS);
		glVertex2f(-300, -225);
		glVertex2f(300, -225);
		glVertex2f(300, -255);
		glVertex2f(-300, -255);
		glEnd();
		glFlush();
		
		for (i = 1; i < 13; i++){
			a = -300;
			for (j = 1; j < 16; j++){
				glColor3d(arr[(i*j)-1][0], arr[(i*j)-1][1], arr[(i*j)-1][2]); // i allagi poy ekana
				glBegin(GL_QUADS);
				glVertex2f(a, z);
				glVertex2f(a + 40, z);
				glVertex2f(a + 40, z - 40);
				glVertex2f(a, z - 40);

				glEnd();
				a += 40;
			}
			z -= 40;
		}
		for (int k = 1; k < 16; k++){

			glColor3d(0, 0, 0);
			glBegin(GL_LINES);
			glVertex2i(x, -225);
			glVertex2i(x, 255);
			glEnd();
			x += 40;
		}

		for (int k = 1; k < 13; k++){
			y += 40;
			glColor3d(0, 0, 0);
			glBegin(GL_LINES);
			glVertex2i(-300, y);
			glVertex2i(300, y);

			glEnd();
		}
		
	}
	textureInit();
	glFlush();
}

void keyBoardInput(GLubyte key,GLint x,GLint y){

	switch (key){
	case 98:
		start = true;
		glutPostRedisplay();
		//glutSwapBuffers();
		break;
	case 27:
		exit(0);
	default:
		break;
	}

}

void OnMouseClick(int button, int state, int x, int y){
	switch (button){
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN){                                                             //x 1-600           y 1-480
			// std::cout << x << " "<<y << " epomeno ";

			if (!swapColour){
				pixelx = x / 40 +1;
				pixely = y / 40 +1;

				if (pixely == 1){
					square1 = (pixely)*(pixelx);
				}
				else if (pixely >1 && pixelx==1){
					square1 = (pixely)*(pixelx)+(14*(pixely-1));			//i prwti stili kai grammi einai entaksei
				}
				else if(pixely>1 && pixelx!=1){
					square1 = pixelx*pixely + ((15-pixelx) *(pixely - 1));
																			//Thelw ta noymera twn koytiwn na ayksanontai kathe 
																			//fora analoga me ti grammi dld se kathe mia na prstithete to 
																			// +1 dld thelw to pixely + 14-(pixelx) (prospathise na piaseis to skeptiko kai vgainei)
				}

				square1 = abs(square1);
				std::cout << "x" << " " << x << "y" << "  " << y<<"square   " << square1 << "::  "<< endl;
				swapColour = true;
			}
			else{

				pixelx2 = (x / 40 + 1);
				pixely2 = (y / 40 + 1);
				
				if (pixely2 == 1){
					square2 = (pixely2)*(pixelx2);
				}
				else if (pixely2 >1 && pixelx2 == 1){
					square2 = (pixely2)*(pixelx2)+(14 * (pixely2 - 1));			//i prwti stili kai grammi einai entaksei
				}
				else if (pixely2>1 && pixelx2 != 1){
					square2 = pixelx2*pixely2 + ((15 - pixelx2) *(pixely2 - 1));
					//Thelw ta noymera twn koytiwn na ayksanontai kathe 
					//fora analoga me ti grammi dld se kathe mia na prstithete to 
					// +1 dld thelw to pixely + 14-(pixelx) (prospathise na piaseis to skeptiko kai vgainei)
				}

				square2 = abs(square2);
				std::cout << "x" << " " << x << "y" << "  " << y << "square2   " << square2 << "::  " << endl;
				if (abs(square1 - square2) == 1 || abs(square1 - square2) == 15){		//Change only the neighbours
					colour1 = arr[square1-1][0];
					colour2 = arr[square1-1][1];
					colour3 = arr[square1-1][2];
					arr[square1-1][0] = arr[square2-1][0];
					arr[square1-1][1] = arr[square2-1][1];
					arr[square1-1][2] = arr[square2-1][2];
					arr[square2-1][0] = colour1;
					arr[square2-1][1] = colour2;
					arr[square2-1][2] = colour3;
					
					//glutSwapBuffers();
					//glutPostRedisplay();
					display();
					
				}
				swapColour = false;
			}
		}
		break;
	}

}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE| GLUT_RGB); // GLUT_DOUBLE --> μας παρέχει δύο buffers και μπορούμε να τους εναλλάσουμε ώστε να ανανεώνουμε την οθόνη
	glutInitWindowPosition(150, 150);
	glutInitWindowSize(600, 510);
	glutCreateWindow("ΒραΧαΨα II");
	srand(time(NULL));
	init();
	
	glutDisplayFunc(display);
	glutKeyboardFunc(keyBoardInput);
	glutMouseFunc(OnMouseClick);

	glutMainLoop();
	return 0;
}

