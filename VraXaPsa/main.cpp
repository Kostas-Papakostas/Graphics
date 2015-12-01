
#include <Windows.h>
#include <GL\glew.h>
#include <GL\freeglut.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include <GL/glu.h>

#include <iostream>
#include <math.h>
#include <time.h>
#include <fstream>
#include <string>
//#include <unistd.h>
//na chekarw aksona ton y sthn teleutaia sthlh CheckforTriplets

using namespace std;

int arr[180];
int colour[][3] = { { 1, 0, 0 }, { 0, 0, 1 } };
int a = -300;
bool start = false, Check = true, tripletsOnStart = true, restart = false;
GLfloat* texture;
bool swapColour = false;
int colour1, colour2, colour3, pixelx, pixely, square1, square2, pixelx2, pixely2;
int moves = 0, movesCounter = 0;
int score = 0;
int len;


ifstream textureFile;

void CheckForTriplets();

void textureInit(char const* TexName, int x, int y){

	string Line;

	int textureHeight, textureWidth, grayScale;
	char const* fileName = TexName;
	textureFile.open(fileName, ios::in);
	if (textureFile.fail()) {
		cout << "textureInit(): could not open file  " << fileName;
	}
	getline(textureFile, Line);
	getline(textureFile, Line);
	textureFile >> textureHeight;
	textureFile >> textureWidth;
	textureFile >> grayScale;

	texture = new GLfloat[textureHeight*textureWidth * 3];

	for (int i = 1; i < textureHeight*textureWidth * 3; i += 3){

		textureFile >> grayScale;
		texture[i + 0] = ((GLfloat)grayScale) / 255;
		texture[i + 1] = ((GLfloat)grayScale) / 255;
		texture[i + 2] = ((GLfloat)grayScale) / 255;

	}
	textureFile.close();

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight,
		0, GL_RGB, GL_FLOAT, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);


	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 1);
	glBegin(GL_QUADS);
	glColor3f(1, 1, 1);
	glTexCoord2f(0, 0);
	glVertex2f(x, y);
	glTexCoord2f(1, 0);
	glVertex2f(x + 40.0, y);
	glTexCoord2f(1, 1);
	glVertex2f(x + 40.0, y - 40.0);
	glTexCoord2f(0, 1);
	glVertex2f(x, y - 40.0);
	glEnd();

	glPopMatrix();
	glDisable(GL_TEXTURE_2D);

}

void output(char const* str1, int x, int y, int size)
{
	glColor3f(0, 0, 0);
	glRasterPos2f(x, y);

	len = (int)strlen(str1);
	if (size == 10){
		for (int i = 0; i < len; i++) {
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str1[i]);
		}
	}
	else if (size == 12){
		for (int i = 0; i < len; i++) {
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, str1[i]);
		}
	}
}

void init(){
	glClearColor(1.0, 1.0, 1.0, 0);
	glClear(GL_COLOR_BUFFER_BIT);



	for (int i = 0; i < 180; i++){

		int j = rand() % 6;
		if (rand() % 5 == 1 && j == 5){
			arr[i] = j;
		}
		else if (j != 5){
			arr[i] = j;
		}
	}

	//gluOrtho2D(-300, 300, -255, 255);
	gluOrtho2D(-300, 300, -255, 255);



}

void display(){
	//glClear(GL_COLOR_BUFFER_BIT);
	int x = -300, y = -225, z = 255;
	int i, j, b = 0;
	char intStr[20],movesStr[20];

	if (restart == true && movesCounter == 0){				//restart point works PERFECT
		for (int i = 0; i < 180; i++){

		int j = rand() % 6;
			if (rand() % 5 == 1 && j == 5){
				arr[i] = j;
			}
			else if (j != 5){
				arr[i] = j;
			}
		}

		restart = false;

		for (int i = 0; i < 180; i++){

			int j = rand() % 6;
			if (rand() % 5 == 1 && j == 5){
				arr[i] = j;
			}
			else if (j != 5){
				arr[i] = j;
			}
		}

		glutPostRedisplay();
		CheckForTriplets();
		score = 0;

	}

	if (start == false && Check == true){
		glColor3d(0.49, 0.51, 0.52);
		glBegin(GL_QUADS);
		glVertex2f(-150, 60);
		glVertex2f(150, 60);
		glVertex2f(150, -60);
		glVertex2f(-150, -60);
		glEnd();
		Check = false;
		output("Press <b> to start", -90, 0, 12);
		glFlush();
	}

	if (start==true && restart == false){
		glColor3d(0.49, 0.51, 0.52);
		glBegin(GL_QUADS);
		glVertex2f(-300, -225);
		glVertex2f(300, -225);
		glVertex2f(300, -285);
		glVertex2f(-300, -285);
		glEnd();
		glFlush();


		for (i = 1; i < 13; i++){
			a = -300;
			for (j = 1; j < 16; j++){
				if (arr[b] == 6){
					glColor3d(0, 0,0);

					glBegin(GL_QUADS);
					glVertex2f(a, z);
					glVertex2f(a + 40, z);
					glVertex2f(a + 40, z - 40);
					glVertex2f(a, z - 40);

					glEnd();
				}
				else if (arr[b] == 0 || arr[b] == 1){
					glColor3d(colour[arr[b]][0], colour[arr[b]][1], colour[arr[b]][2]); // i allagi poy ekana

					glBegin(GL_QUADS);
					glVertex2f(a, z);
					glVertex2f(a + 40, z);
					glVertex2f(a + 40, z - 40);
					glVertex2f(a, z - 40);

					glEnd();
				}
				else if (arr[b] == 2){
					textureInit("rock.pgm", a, z);
				}
				else if (arr[b] == 3){
					textureInit("paper.pgm", a, z);
				}
				else if (arr[b] == 4){
					textureInit("scissors.pgm", a, z);
				}
				else if (arr[b] == 5){
					textureInit("bomb.pgm", a, z);
				}
				b++;
				a += 40;
			}
			z -= 40;
		}
		b = 0;
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

	sprintf(intStr, "%d", score);
	output("Score:  ", 160, -250, 10);
	glRasterPos2f(225, -250);
	len = (int)strlen(intStr);
	for (int k = 0; k < len; k++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, intStr[k]);
	}

	sprintf(movesStr, "%d", moves-movesCounter);
	output("Moves:  ", 60, -250, 10);
	glRasterPos2f(120, -250);
	len = (int)strlen(intStr);
	for (int k = 0; k < len; k++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, movesStr[k]);
	}
	glFlush;

	if (movesCounter == moves){
		glColor3d(0.49, 0.51, 0.52);
		glBegin(GL_QUADS);
		glVertex2f(-150, 60);
		glVertex2f(150, 60);
		glVertex2f(150, -60);
		glVertex2f(-150, -60);
		glEnd();
		output("Game Over!!!", -90, 0, 12);
		glFlush();
		output("Press <b> to restart or <esc> to exit", -280, -245, 10);

		restart = true;
	}
	tripletsOnStart = true;
	glFlush();
}

void keyBoardInput(GLubyte key, GLint x, GLint y){

	switch (key){
	case 98:
		start = true;
		movesCounter = 0;
		glutPostRedisplay();
		break;
	case 27:
		exit(0);
	default:
		break;
	}
}




void destroy(int i, char position, int chroma){
	int j, row, collumn, x, k, z = 0;
	int enemy, counterAttack;
	int bomb = 5;
	if (chroma == 2){
		enemy = 3;
		counterAttack = 4;
	}
	else if (chroma == 3){
		enemy = 4;
		counterAttack = 2;
	}
	else if (chroma == 4){
		enemy = 2;
		counterAttack = 3;
	}
	if (position == 'x'){
		if (chroma == 1){
			collumn = i / 15;
			for (k = collumn; k >= 1; k--){

				arr[i] = 6;
				arr[i + 1] = 6;
				arr[i+2] = 6;
				display();

				arr[i] = arr[i - 15];
				arr[i + 1] = arr[i + 1 - 15];
				arr[i + 2] = arr[i + 2 - 15];
				i -= 15;
			}
			arr[i] = rand() % 5;
			arr[i + 1] = rand() % 5;
			arr[i + 2] = rand() % 5;

		}
		else{
			x = i - 3;
			while (x < (int)((i / 12) * 15)){
				x++;
				z++;
			}

			x = x - 45;
			while (x < 0){
				x += 15;
			}
			for (int l = x; l <= x + 90; l += 15){     //katevainei kathe fora mia grammi kai elegxei ta stoixeia
				if (l > 179){
					break;
				}
				if (arr[l] == counterAttack){		//ama trwei to tetragwnno tote katevainoyn ola ta apo panw
					for (k = l; k > 14; k -= 15){
						arr[k] = 6;
						display();
						arr[k] = arr[k - 15];
						score += 3;
					}
					arr[k] = 6;
					display();
					arr[k] = rand() % 5;
				}
				if (arr[l + 8 - z] == counterAttack){
					for (k = l + 8 - z; k > 14; k -= 15){
						arr[k] = 6;
						display();
						arr[k] = arr[k - 15];
						score += 3;
					}
					arr[k] = 6;
					display();
					arr[k] = rand() % 5;
				}
				if (arr[l + 1] == counterAttack || arr[l + 1] == bomb){		//ama trwei to tetragwnno tote katevainoyn ola ta apo panw (2i stili)
					if (arr[l + 1] == bomb){
						score -= 30;
					}
					else{
						score += 3;
					}

					for (k = l + 1; k > 14; k -= 15){
						arr[k] = 6;
						display();
						arr[k] = arr[k - 15];
						//score += 3;
					}
					arr[k] = 6;
					display();
					arr[k] = rand() % 6;

				}
				if (arr[l + 7 - z] == counterAttack || arr[l + 7 - z]==bomb){
					if (arr[l + 7 - z] == bomb){
						score -= 30;
					}
					else{
						score += 3;
					}
					for (k = l + 7 - z; k > 14; k -= 15){
						arr[k] = 6;
						display();
						arr[k] = arr[k - 15];
						//						score += 3;
					}
					arr[k] = 6;
					display();
					arr[k] = rand() % 6;
				}
				if (arr[l + 2] == counterAttack || arr[l + 2] == bomb){		//ama trwei to tetragwnno tote katevainoyn ola ta apo panw (3i stili)
					for (k = l + 2; k > 14; k -= 15){
						if (abs(l - i) == 16 || abs(l - i) == 1){			//an exei apostasi ena apo ta tetragwnna tis triadas
							if (arr[k] != enemy && arr[k] != chroma&&arr[k] != bomb){
								arr[k] = 6;
								display();
								arr[k] = arr[k - 15];
								score += 2;
							}
							else if (arr[k] == bomb){
								arr[k] = 6;
								display();
								arr[k] = arr[k - 15];
								score -= 30;
							}
							else{
								score--;
							}
						}
						else{
							if (arr[l + 2] == bomb){
								arr[k] = 6;
								display();
								arr[k] = arr[k - 15];
								score -= 30;
							}
							else{
								arr[k] = 6;
								display();
								arr[k] = arr[k - 15];
								score += 3;
							}
						}
					}
					arr[k] = rand() % 6;
				}
				if (arr[l + 6 - z] == counterAttack || arr[l + 6 - z] == bomb){
					for (k = l - z + 6; k > 14; k -= 15){
						if (abs(l - z - i) == 12 || abs(l - z - i) == 3){

							if (arr[k] != enemy && arr[k] != chroma&&arr[k] != bomb){
								arr[k] = 6;
								display();
								arr[k] = arr[k - 15];
								score += 2;
							}
							else if (arr[k] == bomb){
								arr[k] = 6;
								display();
								arr[k] = arr[k - 15];
								score -= 30;
							}
							else{
								score--;
							}
						}
						else{
							if (arr[l + 6 - z] == bomb){
								arr[k] = 6;
								display();
								arr[k] = arr[k - 15];
								score -= 30;
							}
							else{
								arr[k] = 6;
								display();
								arr[k] = arr[k - 15];
								score += 3;
							}
						}
					}
					arr[k] = 6;
					display();
					arr[k] = rand() % 6;

				}
				if (arr[l + 3] == counterAttack || arr[l + 3] == bomb){		//4i stili
					for (k = l + 3; k > 14; k -= 15){
						if (abs(l - i) == 15){
							if (arr[k] != enemy && arr[k] != chroma&&arr[k] != bomb){
								arr[k] = 6;
								display();
								arr[k] = arr[k - 15];
								score += 2;
							}
							else if (arr[k] == bomb){
								arr[k] = 6;
								display();
								arr[k] = arr[k - 15];
								score -= 30;
							}
							else{
								score--;
							}
						}
						else if (k != i){
							if (arr[l + 6 - z] == bomb){
								arr[k] = 6;
								display();
								arr[k] = arr[k - 15];
								score -= 30;
							}
							else{
								arr[k] = 6;
								display();
								arr[k] = arr[k - 15];
								score += 3;
							}
						}
					}
					arr[k] = 6;
					display();
					arr[k] = rand() % 6;
				}
				if (arr[l + 4] == counterAttack || arr[l + 4]==bomb){		//5i stili
					for (k = l + 4; k > 14; k -= 15){
						if (abs(l - i + 1) == 15){
							if (arr[k] != enemy && arr[k] != chroma&&arr[k] != bomb){
								arr[k] = 6;
								display();
								arr[k] = arr[k - 15];
								score += 2;
							}
							else if (arr[k] == bomb){
								arr[k] = 6;
								display();
								arr[k] = arr[k - 15];
								score -= 30;
							}
							else{
								score--;
							}
						}
						else if (k != i + 1){
							if (arr[l + 6 - z] == bomb){
								arr[k] = 6;
								display();
								arr[k] = arr[k - 15];
								score -= 30;
							}
							else{
								arr[k] = 6;
								display();
								arr[k] = arr[k - 15];
								score += 3;
							}
						}
					}
					arr[k] = 6;
					display();
					arr[k] = rand() % 6;
				}
				if (arr[l + 5] == counterAttack||arr[l+5]==bomb){		//6i stili
					for (k = l + 5; k > 14; k -= 15){
						if (abs(l - i + 2) == 15){
							if (arr[k] != enemy && arr[k] != chroma && arr[k] != bomb){
								arr[k] = 6;
								display();
								arr[k] = arr[k - 15];
								score += 2;
							}
							else if (arr[k] == bomb){
								arr[k] = 6;
								display();
								arr[k] = arr[k - 15];
								score -= 30;
							}
							else{
								score--;
							}
						}
						else if (k != i + 2){
							if (arr[l + 6 - z] == bomb){
								arr[k] = 6;
								display();
								arr[k] = arr[k - 15];
								score -= 30;
							}
							else{
								arr[k] = 6;
								display();
								arr[k] = arr[k - 15];
								score += 3;
							}
						}
					}
					arr[k] = 6;
					display();
					arr[k] = rand() % 6;
				}
			}
			collumn = i / 15;
			for (k = collumn; k >= 1; k--){
				arr[i] = 6;
				arr[i + 1] = 6;
				arr[i + 2] = 6;
				display();
				arr[i] = arr[i - 15];
				arr[i + 1] = arr[i + 1 - 15];
				arr[i + 2] = arr[i + 2 - 15];
				i -= 15;
			}
			if (rand() % 2 == 1){
				arr[i] = 6;
				arr[i + 1] = 6;
				arr[i + 2] = 6;
				display();
				arr[i] = rand() % 6;
				arr[i + 1] = rand() % 6;
				arr[i + 2] = rand() % 6;
			}
			else{
				arr[i] = 6;
				arr[i + 1] = 6;
				arr[i + 2] = 6;
				display();
				arr[i] = rand() % 6;
				arr[i + 1] = rand() % 6;
				arr[i + 2] = rand() % 6;
			}
			score += 10;
		}
	}
	else if (position == 'y'){
		if (chroma == 1){
			if (i >= 0 && i <= 14){
				arr[i] = rand() % 5;
				arr[i + 15] = rand() % 5;
				arr[i + 30] = rand() % 5;
			}
			else if (i >= 15 && i <= 29){
				arr[i] = 6;
				arr[i + 15] = 6;
				arr[i + 30] = 6;
				display();
				arr[i] = rand() % 5;
				arr[i + 15] = rand() % 5;
				arr[i + 30] = arr[i - 15];
			}
			else if (i >= 30 && i <= 44){
				arr[i] = 6;
				arr[i + 15] = 6;
				arr[i + 30] = 6;
				display();
				arr[i] = rand() % 5;
				arr[i + 15] = arr[i - 30];
				arr[i + 30] = arr[i - 15];
			}
			else{
				arr[i] = 6;
				arr[i + 15] = 6;
				arr[i + 30] = 6;
				display();
				arr[i] = arr[i - 45];
				arr[i + 15] = arr[i - 30];
				arr[i + 30] = arr[i - 15];
				i = i - 15;
				row = i / 12;
				for (int k = row; k > 4; k--){
					arr[i] = 6;
					display();
					arr[i] = arr[i - 15];
					i -= 15;
				}
				arr[i] = 6;
				arr[i -15] = 6;
				arr[i - 30] = 6;
				display();
				arr[i] = rand() % 5;
				arr[i - 15] = rand() % 5;
				arr[i - 30] = rand() % 5;
			}
		}
		else{
			x = i - 3;
			while (x < (int)((i / 12) * 15)){
				x++;
				z++;
			}

			x = x - 45;
			while (x < 0){
				x += 15;
			}
			for (int l = x; l <= x + 120; l += 15){
				if (l > 179){
					break;
				}
				if (arr[l] == counterAttack){		//1i stili
					for (k = l; k > 14; k -= 15){
						arr[k] = 6;
						display();
						arr[k] = arr[k - 15];
						score += 3;
					}
					arr[k] = 6;
					display();
					arr[k] = rand() % 5;
				}
				if (arr[l + 1] == counterAttack){		//2i
					for (k = l + 1; k > 14; k -= 15){
						arr[k] = 6;
						display();
						arr[k] = arr[k - 15];
						score += 3;
					}
					arr[k] = 6;
					display();
					arr[k] = rand() % 5;
				}
				if (arr[l + 5 - z] == counterAttack){		//6i
					for (k = l + 5 - z; k > 14; k -= 15){
						arr[k] = 6;
						display();
						arr[k] = arr[k - 15];
						score += 3;
					}
					arr[k] = 6;
					display();
					arr[k] = rand() % 5;
				}
				if (arr[l + 6 - z] == counterAttack){		//7i
					for (k = l + 6 - z; k > 14; k -= 15){
						arr[k] = 6;
						display();
						arr[k] = arr[k - 15];
						score += 3;
					}
					arr[k] = 6;
					display();
					arr[k] = rand() % 5;
				}
				if (arr[l + 2] == counterAttack||arr[l+2]==bomb){		//3i stili
					for (k = l + 2; k > 14; k -= 15){
						if (abs(l - i) == 16 || abs(l - i) == 1 || abs(l - i) == 31 || abs(l - i) == 46){
							if (arr[k] != enemy && arr[k] != chroma&&arr[k] != bomb){
								arr[k] = 6;
								display();
								arr[k] = arr[k - 15];
								score += 2;
							}
							else if (arr[k] == bomb){
								arr[k] = 6;
								display();
								arr[k] = arr[k - 15];
								score -= 30;
							}
							else{
								score--;
							}
						}
						else{
							if (arr[k] == counterAttack){
								arr[k] = 6;
								display();
								arr[k] = arr[k - 15];
								score += 3;
							}
						}
					}
					arr[k] = 6;
					display();
					arr[k] = rand() % 5;
				}
				if (arr[l + 4] == counterAttack||arr[l+4]==bomb){		//5i stili
					for (k = l + 4; k > 14; k -= 15){
						if (abs(l - i) == 14 || abs(l - i) == 1 || abs(l - i) == 29 || abs(l - i) == 44){
							if (arr[k] != enemy && arr[k] != chroma && arr[k] != bomb){
								arr[k] = 6;
								display();
								arr[k] = arr[k - 15];
								score += 2;
							}
							else if (arr[k] == bomb){
								arr[k] = 6;
								display();
								arr[k] = arr[k - 15];
								score -= 30;
							}
							else{
								score--;
							}
						}
						else{
							if (arr[k] == counterAttack){
								arr[k] = 6;
								display();
								arr[k] = arr[k - 15];
								score += 3;
							}
						}
					}
					arr[k] = 6;
					display();
					arr[k] = rand() % 5;
				}
				if (arr[l + 3] == counterAttack||arr[l+3]==bomb){		//4i stili
					for (k = l + 3; k > 14; k -= 15){
						if (abs(l - i) == 15 || abs(l - i) == 45){
							if (arr[k] != enemy && arr[k] != chroma&&arr[k] != bomb){
								arr[k] = 6;
								display();
								arr[k] = arr[k - 15];
								score += 2;
							}
							else if (arr[k] == bomb){
								arr[k] = 6;
								display();
								arr[k] = arr[k - 15];
								score -= 30;
							}
							else{
								score--;
							}
						}
						else if (k != i + 15 || k != i + 30 || k != i){
							arr[k] = 6;
							display();
							arr[k] = arr[k - 15];
							score += 3;
						}
					}
					arr[k] = 6;
					display();
					arr[k] = rand() % 5;
				}
			}
			if (i >= 0 && i <= 14){
				arr[i] = rand() % 5;
				arr[i + 15] = rand() % 5;
				arr[i + 30] = rand() % 5;
			}
			else if (i >= 15 && i <= 29){
				arr[i] = 6;
				arr[i + 15] = 6;
				arr[i + 30] = 6;
				display();
				arr[i] = rand() % 5;
				arr[i + 15] = rand() % 5;
				arr[i + 30] = arr[i - 15];
			}
			else if (i >= 30 && i <= 44){
				arr[i] = 6;
				arr[i + 15] = 6;
				arr[i + 30] = 6;
				display();
				arr[i] = rand() % 5;
				arr[i + 15] = arr[i - 30];
				arr[i + 30] = arr[i - 15];
			}
			else{
				arr[i] = 6;
				arr[i + 15] = 6;
				arr[i + 30] = 6;
				display();
				arr[i] = arr[i - 45];
				arr[i + 15] = arr[i - 30];
				arr[i + 30] = arr[i - 15];
				i = i - 15;
				row = i / 12;
				for (int k = row; k > 4; k--){
					arr[i] = 6;
					display();
					arr[i] = arr[i - 15];
					i -= 15;
				}
				arr[i] = 6;
				arr[i - 15] = 6;
				arr[i - 30] = 6;
				display();
				//i=i+15;
				arr[i] = rand() % 5;
				arr[i - 15] = rand() % 5;
				arr[i - 30] = rand() % 5;
			}
			score += 10;
		}
	}
}


void CheckForTriplets(){
	while (tripletsOnStart){
		tripletsOnStart = false;
		for (int i = 0; i < 180; i++){
			if ((i % 15) < 13 && arr[i] == arr[i + 1]){  //an to deksia mou einai idio
				if (arr[i] == arr[i + 2]){ //chekarw to amesws pio deksia
					if (arr[i] == 0 || arr[i] == 1){
						tripletsOnStart = true;
						destroy(i, 'x', 1);
					}
					else if (arr[i] == 2 || arr[i] == 3 || arr[i] == 4){									//if rock,paper,scissors
						tripletsOnStart = true;
						destroy(i, 'x', arr[i]);
					}
				}
			}
		}
		for (int i = 0; i < 180; i++){
			if ((i % 12) < 11 && arr[i] == arr[i + 15]){
				if (arr[i] == arr[i + 30]){
					if (arr[i] == 0 || arr[i] == 1){
						tripletsOnStart = true;
						destroy(i, 'y', 1);
					}
					else if (arr[i] == 2 || arr[i] == 3 || arr[i] == 4){					//if rock,paper,scissors
						tripletsOnStart = true;
						destroy(i, 'y', arr[i]);
					}
				}
			}
		}
	}
}



void OnMouseClick(int button, int state, int x, int y){
	switch (button){
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN){
			if (!swapColour){
				pixelx = x / 40 + 1;
				pixely = y / 40 + 1;

				if (pixely == 1){
					square1 = (pixely)*(pixelx);
				}
				else if (pixely >1 && pixelx == 1){
					square1 = (pixely)*(pixelx)+(14 * (pixely - 1));
				}
				else if (pixely>1 && pixelx != 1){
					square1 = pixelx*pixely + ((15 - pixelx) *(pixely - 1));
				}

				square1 = abs(square1) - 1;
				swapColour = true;
			}
			else{

				pixelx2 = (x / 40 + 1);
				pixely2 = (y / 40 + 1);

				if (pixely2 == 1){
					square2 = (pixely2)*(pixelx2);
				}
				else if (pixely2 >1 && pixelx2 == 1){
					square2 = (pixely2)*(pixelx2)+(14 * (pixely2 - 1));
				}
				else if (pixely2>1 && pixelx2 != 1){
					square2 = pixelx2*pixely2 + ((15 - pixelx2) *(pixely2 - 1));
				}

				square2 = abs(square2) - 1;
				if (abs(square1 - square2) == 1 || abs(square1 - square2) == 15){
					colour1 = arr[square1];
					arr[square1] = arr[square2];
					arr[square2] = colour1;
					movesCounter++;
					CheckForTriplets();
					glutPostRedisplay();
				}

				swapColour = false;
			}
		}
		break;
	}
}

int main(int argc, char** argv)
{
	cout << "Plhthos kinhsewn :    ";
	cin >> moves;
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(150, 150);
	glutInitWindowSize(600, 510);
	glutCreateWindow("VraXaPsa II");
	srand(time(NULL));
	init();

	CheckForTriplets();
	glutDisplayFunc(display);
	score = 0;
	glutKeyboardFunc(keyBoardInput);
	glutMouseFunc(OnMouseClick);


	glutMainLoop();

	return 0;
}

