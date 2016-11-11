#define GL_CLAMP_TO_EDGE 0x812F
#include "SOIL.h"
#include <stdio.h>
//#include <Windows.h>
//#include <filesystem>
#include <vector>
#include <GL/gl.h>
#include <GL/glut.h>
#include <GL/glu.h>
#include <fstream>


#include <iostream>
#include <math.h>

using namespace std;

float ver[8][3] =
{
	{ -1.0, -1.0, 1.0 },
	{ -1.0, 1.0, 1.0 },
	{ 1.0, 1.0, 1.0 },
	{ 1.0, -1.0, 1.0 },
	{ -1.0, -1.0, -1.0 },
	{ -1.0, 1.0, -1.0 },
	{ 1.0, 1.0, -1.0 },
	{ 1.0, -1.0, -1.0 },
};

int num_of_hammers, num_of_levels, N, lost_hammers = 0;
int ***map;
float x_cam = 0.0f, y_cam = 0.0f, z_cam = -65.f, forz = -65.f, forx = 0.f, movements=0;

float lastx, lasty;

GLuint texture, texture2, texture3;

bool rotating = false,finish=false;
bool out = false;
int len;

// angle of rotation for the camera direction
float angle = 0.0, angle2 = 0.0;

// actual vector representing the camera's direction
float lx = 1.f, lz = 1.f;

// XZ position of the camera
float lookUp = 30.f;
int currentLvL = 30, currentLvL2 = 0;
bool begingame = false;
int counter = 0, counter2 = 0;
bool jumping = false, falling = false, starter = true;
float jumpHeight, jumpLanding, jumpSpeed = 0.5, jumpUp = 0;
float currentPlayerPos[11];

double rotate_z = 1;
double rotate_y = 0;
double rotate_x = 0;

int walkleft = 0, walkright = 0;
float x_limit = 0, z_limit = 0;
GLfloat Ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
GLfloat Diffuse[] = { 0.1f, 0.6f, 0.4f, 1.0f };
GLfloat Pos[] = { x_cam, y_cam + currentLvL - 10, z_cam, 1.0f };
GLfloat sun[] = { -1, 15, -1, 1.f };
int theta, phi;

struct cubes{
	int x_pos;
	int y_pos;
	int type;
	int lvl;
};
vector<cubes> boxeOfCubes;

void quad(int a, int b, int c, int d);
void texturedcube();
void coloredcube(GLfloat r, GLfloat g, GLfloat b);
GLuint LoadTexture(const char * filename, int width, int height);
void texturedcube(GLuint texted);
void FreeTexture(GLuint texture);

void drawGrid(){
	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_LINES);
	for (int i = 0; i <= N; i++) {

		glVertex3f(i, 0, 0);
		glVertex3f(i, 0, N);

		glVertex3f(0, 0, i);
		glVertex3f(N, 0, i);
	}
	glEnd();
}

void makeFloor(GLuint text){
	glBindTexture(GL_TEXTURE_2D, text);
	glEnable(GL_TEXTURE_2D);
	quad(0, 4, 7, 3);
}


void reshape(int w, int h) {
	glViewport(0, 0, (GLsizei)w, (GLsizei)h); //set the viewport to the current window specifications
	glMatrixMode(GL_PROJECTION); //set the matrix to projection

	glLoadIdentity();
	gluPerspective(60, (GLfloat)w / (GLfloat)h, 1.0, 100.0); //set the perspective (angle of sight, width, height, , depth)
	glMatrixMode(GL_MODELVIEW); //set the matrix back to model

}

void player(float x, float y, float z, int state){

	if (state == 1){
		
		glPushMatrix();
		glTranslatef(x, y, z);
		
		glRotatef(90, 1.0, 0.0, 0.0);
		glColor4f(1, 0, 0,1);
		
		glBlendFunc(GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA);
		glEnable(GL_BLEND);

		glBegin(GL_POLYGON);
		GLUquadricObj *obj = gluNewQuadric();
		gluCylinder(obj, 1.f, 1.f, 1.f, 30, 30);
		glPopMatrix();
	}

}

void output(char const* str1, int x, int y, int size)
{
	glColor3f(0, 0, 0);
	glDisable(GL_COLOR_MATERIAL);
	glEnable(GL_COLOR);
	glRasterPos3f(x, y,z_cam);

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

void startJump(float height, float land){
	jumpHeight = height;
	jumpLanding = land;
	if (!falling && !jumping){
		jumping = true;
	}
}

void jump(){ // called every frame
	if (falling){
		if (jumpUp >= -jumpHeight){
			jumpUp -= jumpSpeed;
		}
		else{
			jumping = false;
			falling = false;
		}
	}

	else if (jumping){
		if (jumpUp <= -jumpLanding){
			jumpUp += jumpSpeed;
		}
		else{
			jumping = false;
			falling = true;
		}
	}
}

void sunLighting(){
	glNormal3f(10, 6, 16);
	glLightfv(GL_LIGHT0, GL_POSITION, sun);

	glLightfv(GL_LIGHT0, GL_AMBIENT, Ambient);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

}

void headLight(){

	Pos[0] = 8;
	Pos[1] = 15;
	Pos[2] = 3;
	glNormal3f(abs(rotate_x), y_cam, abs(rotate_z));
	glLightfv(GL_LIGHT7, GL_POSITION, Pos);

	glLightfv(GL_LIGHT7, GL_DIFFUSE, Diffuse);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT7);
}

void keyboard(unsigned char key, int x, int y) {
	float fraction = 0.5f;

	if (key == 'r' || key == 'R'){
		if (rotating == false){
			angle += 0.1;
			x_cam = (N * 2) / 2 + num_of_levels * 20 * sin(angle);
			z_cam = (N * 2) / 2 + num_of_levels * 20 * (-cos(angle));
			y_cam = num_of_levels;
			rotate_x = (N * 2) / 2;
			rotate_z = (N * 2) / 2;
			forx = 0;
			forz = 0;

			rotating = true;
		}
		else{
			rotating = false;
		}
	}
	if (key == 'v' || key == 'V') {
		if (begingame == false){
			begingame = true;
			counter = 1;
		}
		else if (begingame == true){
			counter2 = 1;
		}
	}
	if (key == 32){
		if (currentLvL == num_of_levels){
			finish = true;
		}
		else if (boxeOfCubes[map[(int)((x_cam / 2) +rotate_x)][(int)((z_cam / 2) +rotate_z)][(int)(currentLvL / 2)]].type == 0){
			boxeOfCubes[map[(int)((x_cam / 2) )][(int)((z_cam / 2) )][(int)(currentLvL / 2)]].type = 5;
			currentLvL += 2;
			
			currentLvL2 = currentLvL;
		}
	}
	if (key == 'h' || key == 'H') {
		if (boxeOfCubes[map[((int)abs(x_cam / 2 + rotate_x))][((int)abs(z_cam / 2 + rotate_z))][currentLvL / 2]].type != 0 && num_of_hammers > 0){
			boxeOfCubes[map[((int)abs(x_cam / 2 + rotate_x))][((int)abs(z_cam / 2 + rotate_z))][currentLvL / 2]].type = 0;
			num_of_hammers--;
			lost_hammers++;
		}
	}
	if (key == 'w' || key == 'W') {
		if ((currentLvL/2) >=1){
			if (boxeOfCubes[map[((int)(x_cam / 2))][((int)(z_cam / 2))][(int)(currentLvL / 2)]].type == 0){
				currentLvL -= 2;
				currentLvL2 = currentLvL;
			}
		}
		if (boxeOfCubes[map[((int)((x_cam / 2)))][((int)((z_cam / 2)))][currentLvL / 2]].type == 7){
			for (int i = 0; i<N; i++){
				for (int j = 0; j<N; j++){
					if ((i != (int)(x_cam / 2)) && (j != (int)(z_cam / 2))
						&& (boxeOfCubes[map[i][j][currentLvL / 2]].type == 7)){
						x_cam = 2 * i;
						z_cam = 2 * j;
					}
				}
			}
		}
		if (begingame == false && counter == 0){
			x_cam += rotate_x * fraction;
			z_cam += rotate_z * fraction;
			forx = x_cam;
			forz = z_cam;
		}
		else{
			if (boxeOfCubes[map[((int)((x_cam / 2)))][((int)((z_cam / 2) + abs(rotate_z)))][currentLvL]].y_pos - (z_cam / 2) > 0.001 &&
				boxeOfCubes[map[((int)((x_cam / 2)))][((int)((z_cam / 2) + abs(rotate_z)))][currentLvL]].type == 0){
				z_cam += rotate_z * fraction;
				movements += 0.01;
				forz = z_cam;
			}
			if (boxeOfCubes[map[((int)((x_cam / 2) + abs(rotate_x)))][((int)((z_cam / 2)))][currentLvL]].x_pos - (x_cam / 2) > 0.001 &&
				boxeOfCubes[map[((int)((x_cam / 2) + abs(rotate_x)))][((int)((z_cam / 2)))][currentLvL]].type == 0){
				x_cam += rotate_x * fraction;
				movements += 0.01;
				forx = x_cam;
			}
		}
	}
	if (key == 's' || key == 'S') {
		x_cam -= rotate_x * fraction;
		z_cam -= rotate_z * fraction;		// moving backwards
		movements += 0.01;
		forx = x_cam;
		forz = z_cam;
	}
	if (key == 'e' || key == 'E'){
		finish = true;
	}
	glutPostRedisplay();
}


void StageInit(const char * filename){
	string line;
	int tmp;
	ifstream file(filename, std::ifstream::in);

	if (!file)
	{
		cout << "An error occured while loading file with name : " << filename << endl;
		exit(0);
	}

	if (file.is_open()){

		getline(file, line);
		tmp = line.find("=");
		line = line.substr(tmp + 1);
		num_of_levels = atoi(line.c_str());

		getline(file, line);
		tmp = line.find("=");
		line = line.substr(tmp + 1);
		N = atoi(line.c_str());

		getline(file, line);
		tmp = line.find("=");
		line = line.substr(tmp + 1);
		num_of_hammers = atoi(line.c_str());
		boxeOfCubes.reserve(num_of_levels*N*N);

		map = new int**[N];
		for (int i = 0; i < N; ++i) {
			map[i] = new int*[N];

			for (int j = 0; j < N; ++j)
				map[i][j] = new int[num_of_levels];
		}

		for (int i = 0; i < num_of_levels; i++){
			if (line == "NEW LEVEL"){
				getline(file, line);
			}
			getline(file, line);
			for (int j = 0; j < N; j++){
				for (int k = 0; k < N; k++){
					string Value;
					file >> Value;
					struct cubes cur;
					if (Value == "R")
						cur.type = 1;
					else if (Value == "G")
						cur.type = 2;
					else if (Value == "B")
						cur.type = 3;
					else if (Value == "T1")
						cur.type = 4;
					else if (Value == "T2")
						cur.type = 5;
					else if (Value == "T3")
						cur.type = 6;
					else if (Value == "E"){
						cur.type = 0;
					}
					else if (Value == "W"){
						cur.type = 7;
					}
					cur.x_pos = k;
					cur.y_pos = j;
					cur.lvl = i;
					boxeOfCubes.push_back(cur);
				}
			}
		}
	}
}

void createMap(){
	int k = 0;
	int l = 0;
	int u = 0;
	for (int i = 0; i < boxeOfCubes.size(); ++i){
		int x = boxeOfCubes[i].x_pos;
		int y = boxeOfCubes[i].y_pos;
		int lvl = boxeOfCubes[i].lvl;
		int type = boxeOfCubes[i].type;
		GLuint text;
		glPushMatrix();

		glTranslatef(x * 2.001, lvl*2.001, y*2.001);
		if (boxeOfCubes[i].type == 4){
			glDisable(GL_TEXTURE_2D);
			glColor4f(1.0, 1.0, 1.0, 0);
			texturedcube(texture);
			map[x][y][lvl] = i;
			if (boxeOfCubes[i].lvl == 0){
				makeFloor(texture3);
			}
		}
		else if (boxeOfCubes[i].type == 5){
			glDisable(GL_TEXTURE_2D);
			glColor4f(1.0, 1.0, 1.0, 0);
			texturedcube(texture2);
			map[x][y][lvl] = i;
			if (boxeOfCubes[i].lvl == 0){
				makeFloor(texture3);
			}
		}
		else if (boxeOfCubes[i].type == 6){
			glDisable(GL_TEXTURE_2D);
			glColor4f(1.0, 1.0, 1.0, 0);
			texturedcube(texture3);
			map[x][y][lvl] = i;
			if (boxeOfCubes[i].lvl == 0){
				makeFloor(texture3);
			}
		}
		else if (boxeOfCubes[i].type == 0){
			glDisable(GL_TEXTURE_2D);
			glColor4f(1.0, 1.0, 1.0, 0);
			map[x][y][lvl] = i;
			if (boxeOfCubes[i].lvl == 0){
				makeFloor(texture3);
			}
		}
		else if (boxeOfCubes[i].type == 1){
			glDisable(GL_TEXTURE_2D);
			glColor4f(1.0, 1.0, 1.0, 0);
			coloredcube(1, 0, 0);
			map[x][y][lvl] = i;
			if (boxeOfCubes[i].lvl == 0){
				makeFloor(texture3);
			}
		}
		else if (boxeOfCubes[i].type == 2){
			glDisable(GL_TEXTURE_2D);
			glColor4f(1.0, 1.0, 1.0, 0);
			coloredcube(0, 1, 0);
			map[x][y][lvl] = i;
			if (boxeOfCubes[i].lvl == 0){
				makeFloor(texture3);
			}
		}
		else if (boxeOfCubes[i].type == 3){
			glDisable(GL_TEXTURE_2D);
			glColor4f(1.0, 1.0, 1.0, 0);
			coloredcube(0, 0, 1);
			map[x][y][lvl] = i;
			if (boxeOfCubes[i].lvl == 0){
				makeFloor(texture3);
			}
		}
		else if (boxeOfCubes[i].type == 7){
			glDisable(GL_TEXTURE_2D);
			glColor4f(1.0, 1.0, 1.0, 0);
			coloredcube(0, 0, 0);
			map[x][y][lvl] = i;
			if (boxeOfCubes[i].lvl == 0){
				makeFloor(texture3);
			}
		}

		glPopMatrix();
	}
}

GLuint LoadTexture(const char * filename, int width, int height)
{

	GLuint texture_id;
	unsigned char *data;
	FILE *file;

	// open texture data
	file = fopen(filename, "r");
	if (file == NULL) return 0;

	// allocate buffer
	data = (unsigned char*)malloc(width * height * 4);

	//read texture data
	fread(data, width * height * 4, 1, file);
	fclose(file);


	texture_id = SOIL_load_OGL_texture
		(filename,
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
		);


	// check for an error during the load process 
	if (texture_id == 0)
	{
		printf("SOIL loading error: '%s'\n", SOIL_last_result());
	}

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_DECAL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_DECAL);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	free(data);  // free buffer

	return texture_id;
}





void init(){
	glClearColor(0.0, 0.0, 0.0, 0.0);
	StageInit("file.maz");
}

void quad(int a, int b, int c, int d)
{

	glBegin(GL_POLYGON);

	glTexCoord2f(0, 0);
	glVertex3fv(ver[a]);


	glTexCoord2f(0, 1);
	glVertex3fv(ver[b]);

	glTexCoord2f(1, 1);
	glVertex3fv(ver[c]);

	glTexCoord2f(1, 0);
	glVertex3fv(ver[d]);
	glEnd();

}

void texturedcube(GLuint texted)
{
	glBindTexture(GL_TEXTURE_2D, texted);
	glColor4f(1, 1, 1, 0.6);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_COLOR_MATERIAL);
	quad(0, 1, 2, 3);
	quad(2, 3, 7, 6);
	quad(0, 4, 7, 3);
	quad(1, 2, 6, 5);
	quad(4, 5, 6, 7);
	quad(0, 1, 5, 4);
}

void coloredcube(GLfloat r, GLfloat g, GLfloat b)
{
	glColor4f(r, g, b, 0.4);
	glBlendFunc(GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA);
	glEnable(GL_BLEND);
	quad(0, 1, 2, 3);
	quad(2, 3, 7, 6);
	quad(0, 4, 7, 3);
	quad(1, 2, 6, 5);
	quad(4, 5, 6, 7);
	quad(0, 1, 5, 4);
}

void specialKeys(int key, int x, int y)
{
	float fraction = 0.5f;

	if (key == GLUT_KEY_UP){
		x_cam += lx * fraction;
		z_cam += lz * fraction;		// moving forward
		forx = x_cam;
		forz = z_cam;
	}
	else if (key == GLUT_KEY_DOWN){
		x_cam -= lx * fraction;
		z_cam -= lz * fraction;		// moving backwards
		forx = x_cam;
		forz = z_cam;
	}
	glutPostRedisplay();
}


void display()
{
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_TEXTURE_2D);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	int w = glutGet(GLUT_WINDOW_WIDTH);
	int h = glutGet(GLUT_WINDOW_HEIGHT);
	gluPerspective(60, w / h, 0.1, 100);

	glMatrixMode(GL_MODELVIEW);
	
	glLoadIdentity();
	glutSetCursor(GLUT_CURSOR_NONE);

	if (finish == false){

		headLight();

		if (begingame == true && counter == 1){
			rotating = false;
			glTranslatef(0, 0, 0);

			x_cam = currentPlayerPos[0];
			y_cam = currentPlayerPos[1];
			z_cam = currentPlayerPos[2];
			forx = x_cam;
			forz = z_cam;
			player(x_cam, y_cam, z_cam, 0);
			currentLvL = 0;
			rotate_x = 0;
			rotate_z = 0;
			rotate_y = 0;
			lookUp = 0;
			counter = 0;
			out = false;
			glutPostRedisplay();
		}
		else if (begingame == true && counter2 == 1){
			currentPlayerPos[0] = x_cam;
			currentPlayerPos[1] = y_cam;
			currentPlayerPos[2] = z_cam;
			x_cam = 0.0f;
			y_cam = 0.0f;
			z_cam = -65.f;
			forx = x_cam;
			forz = z_cam;
			rotate_z = 1;
			rotate_y = 0;
			rotate_x = 0;
			currentLvL = 30;
			currentLvL2 = 0;
			lookUp = 30;
			counter2 = 0;
			begingame = false;
			out = true;
			glutPostRedisplay();
		}
		/****camera rotation******/
		gluLookAt(x_cam, y_cam + currentLvL, z_cam,
			forx + rotate_x, y_cam + lookUp + rotate_y + currentLvL2, forz + rotate_z,
			0.0f, 0.001f, 0.0f);

		if (out == true){
			player(currentPlayerPos[0], currentPlayerPos[1], currentPlayerPos[2], 1);
		}

		sunLighting();
		createMap();

		if (starter == true){
			player(0.f, 0.f, 0.f, 0);
			starter = false;
		}

		jump();
		glutSwapBuffers();
	}
	if (finish == true){
		gluOrtho2D(0, 640, 0, 480);
		output("SCORE   :", 10, 10, 10);
		cout << "SCORE!!!!     " << N*N - movements * 10 - lost_hammers * 50 << endl;
	}
	/*if (rotating == true){
	angle += 0.01;
	x_cam = (N*2)/2+num_of_levels*20*sin(angle);
	z_cam = (N * 2) / 2 + num_of_levels * 20 * (-cos(angle));
	y_cam = num_of_levels;
	rotate_x = (N*2)/2;
	rotate_z = (N * 2) / 2;
	forx = 0;
	forz = 0;
	glutPostRedisplay();
	}*/
	glFlush();
}

void FreeTexture(GLuint texture)
{
	glDeleteTextures(1, &texture);
}

/*********all the transforamtions are here*******/
void mouseMovement(int x, int y) {
	int diffx = x - lastx; //check the difference between the 
	int diffy = lasty - y; //check the difference between the 
	lastx = x;
	lasty = y; //set lasty to the current y position
	if (diffx > 0){
		angle += 0.05;
	}
	else if (diffx < 0){
		angle -= 0.05;
	}
	if (diffy>0){
		angle2 += 0.05;
	}
	else if (diffy < 0){
		angle2 -= 0.05;
	}
	/*******trnsform to polar coords******/
	rotate_x = sin(angle); //set the xrot to xrot with the addition
	rotate_z = -cos(angle);    //set the xrot to yrot with the addition
	rotate_y = sin(angle2);
	glutPostRedisplay();
}

int main(int argc, char **argv)
{

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(640, 480);
	glutCreateWindow("CUBE");
	glEnable(GL_DEPTH_TEST);
	texture = LoadTexture("T1.png", 256, 256);
	texture2 = LoadTexture("T2.png", 256, 256);
	texture3 = LoadTexture("T3.png", 256, 256);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(specialKeys);
	StageInit("file.maz");
	glutPassiveMotionFunc(mouseMovement);

	glutMainLoop();
	FreeTexture(texture);
	FreeTexture(texture2);
	FreeTexture(texture3);
	free(map);

	return 0;
}
