#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <algorithm>
#include <gl/glut.h>
#include <cmath>

int baseArray[5000][2];
int heightArray[5000][2];
int b = 0;
int h = 0;
int LEFT_BUTTON_PRESSED = 0;
int minHeight = 10000;
int maxHeight = -10000;

int minBoundary = 10000;
int maxBoundary = -10000;

static float BASE_PLANE = -30.0;

static int BASE_MODE = 0;
static int HEIGHT_MODE = 1;
static int GENERATE_PERLIN = 2;
int Mode;

float angle = 0.0;
float lx = 0.0f, ly = 1.0f, lz = -1.0f;
float ey = 1.0f;
float x = 0.0f, y = 1.0f, z = 5.0f;

float w = 0.0f, lw = 0.0f;

GLfloat gridLines = true;

static int DISPLACEMENT = 400;
static int Z_DISP = 290;

//static int topColor[3] = { 153, 119, 65 };
static int topColor[3] = { 204, 165, 106 };
//int bottomColor[3] = { 0, 255, 65 };
static int bottomColor[3] = { 94, 68, 26 };

#define N 4


void init(void)
{
	Mode = HEIGHT_MODE;

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	glClearColor(1.0, 1.0, 1.0, 1.0);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_TRUE);
	//glDepthFunc(GL_LESS);
	std::cout << "Current Mode: Base Mode";


	for (int i = 0; i < 5000; i++)
	{
		baseArray[i][0] = 0;
		baseArray[i][1] = 0;
		heightArray[i][0] = 0;
		heightArray[i][1] = 0;
	}
	glPointSize(2.0);
}

void reshape(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90, (float)w / (float)h, 10.0, 1000.0);
	glMatrixMode(GL_MODELVIEW);
}

float getWeight(int half, int j) {
	float d = float(float(half - j) / float(half));
	return std::pow(std::pow(d, 2) - 1, 2);
}

void renderHeightMap() {
	/*int H = 2 * h + 1;
	int** arr = new int*[H];
	for (int i = 0; i < H; i++)
		arr[i] = new int[H];*/

	float rColor;
	int half = h / 2;
	for (int i = 0; i < h-1; i++)
	{
		glBegin(GL_TRIANGLE_STRIP);

		float lastW = getWeight(half, 0);
		float nextW = 0;

		for (int j = 0; j < h - 1; j++) {
			//rColor = (abs((float)half - (float)j) / (float)half) * (float)(heightArray[j][1] - minHeight) / (float)(maxHeight - minHeight);

			rColor = float(((heightArray[j][1] + heightArray[i][1])/2-minHeight)) / float((maxHeight-minHeight));

			//std::cout << rColor << "\n";


			glColor4ub(bottomColor[0] + int(rColor*(topColor[0] - bottomColor[0])), 
				bottomColor[1] + int(rColor*(topColor[1] - bottomColor[1])), 
				bottomColor[2] + int(rColor*(topColor[2] - bottomColor[2])), 255);

			//glVertex3f(heightArray[i][0], BASE_PLANE + (abs((float)(half-j))/(float)half) * heightArray[i][1], -290 - heightArray[j][0]);
		 	//glVertex3f(heightArray[i + 1][0], BASE_PLANE + (abs((float)(half - j - 1)) / (float)half) * heightArray[i + 1][1], -290  - heightArray[j + 1][0]);

			glVertex3f(heightArray[i][0], BASE_PLANE + (lastW * heightArray[i][1]), - Z_DISP - heightArray[j][0]);
			nextW = getWeight(half, j + 1);
			glVertex3f(heightArray[i + 1][0], BASE_PLANE + (nextW * heightArray[i + 1][1]), - Z_DISP - heightArray[j + 1][0]);
			lastW = nextW;
		}
	}
	glEnd();

	/*for (int i = 0; i < H; i++)
		delete[] arr[i];
	delete[] arr;*/
}

void renderWater() {
	if (w == 0.0f)
		return;

	std::cout << h << "\n";

	//float xMin = heightArray[0][0], xMax = heightArray[h - 1][0];
	//float zMin = -Z_DISP - heightArray[0][0], zMax = -Z_DISP - heightArray[h - 1][0];

	float X[2] = { minBoundary, maxBoundary };
	float Y[2] = { 0 + BASE_PLANE, w + BASE_PLANE };
	float Z[2] = { -Z_DISP - minBoundary, - Z_DISP - maxBoundary };
	glColor4ub(133, 179, 252, 255);

	glBegin(GL_POLYGON);

	glVertex3f(X[0], Y[0], Z[0]);
	glVertex3f(X[0], Y[0], Z[1]);
	glVertex3f(X[0], Y[1], Z[1]);
	glVertex3f(X[0], Y[1], Z[0]);

	glEnd();
	glBegin(GL_POLYGON);

	glVertex3f(X[1], Y[0], Z[0]);
	glVertex3f(X[1], Y[0], Z[1]);
	glVertex3f(X[1], Y[1], Z[1]);
	glVertex3f(X[1], Y[1], Z[0]);

	glEnd();
	glBegin(GL_POLYGON);

	glVertex3f(X[0], Y[0], Z[0]);
	glVertex3f(X[0], Y[0], Z[1]);
	glVertex3f(X[1], Y[0], Z[1]);
	glVertex3f(X[1], Y[0], Z[0]);

	glEnd();
	glBegin(GL_POLYGON);

	glVertex3f(X[0], Y[1], Z[0]);
	glVertex3f(X[0], Y[1], Z[1]);
	glVertex3f(X[1], Y[1], Z[1]);
	glVertex3f(X[1], Y[1], Z[0]);

	glEnd();
	glBegin(GL_POLYGON);

	glVertex3f(X[0], Y[0], Z[0]);
	glVertex3f(X[0], Y[1], Z[0]);
	glVertex3f(X[1], Y[1], Z[0]);
	glVertex3f(X[1], Y[0], Z[0]);

	glEnd();
	glBegin(GL_POLYGON);

	glVertex3f(X[0], Y[0], Z[1]);
	glVertex3f(X[0], Y[1], Z[1]);
	glVertex3f(X[1], Y[1], Z[1]);
	glVertex3f(X[1], Y[0], Z[1]);

	glEnd();
}

void renderGridLines() {
	glBegin(GL_LINES);
	int i, j, minB = -1000, maxB = 1000, gap = 20;
	glColor4ub(100, 100, 255, 100);
	for (i = minB; i <= maxB; i += gap)
	{
		glVertex3f(i, BASE_PLANE, minB);
		glVertex3f(i, BASE_PLANE, maxB);
	}

	for (i = minB; i <= maxB; i += gap)
	{
		glVertex3f(minB, BASE_PLANE, i);
		glVertex3f(maxB, BASE_PLANE, i);
	}
	glEnd();
}

void drawPoints()
{
	if (Mode == GENERATE_PERLIN) {
		renderWater();
		renderHeightMap();
	}
	else if (Mode == BASE_MODE) {
		glBegin(GL_POINTS);
		glColor3f(0.0, 0.0, 1.0);
		for (int i = 0; i < b; i++)
		{
			glVertex3f(baseArray[i][0], baseArray[i][1], -1 * Z_DISP);
		}
		glEnd();
	}
	else {
		glBegin(GL_POINTS);
		for (int i = 0; i < h; i++)
		{
			if (heightArray[i][1] > BASE_PLANE)
				glColor3ub(0, 255, 0);
			else
				glColor3ub(255, 0, 0);
			glVertex3f(heightArray[i][0], heightArray[i][1], -1 * Z_DISP);

		}
		glEnd();
		//for (int i = 0; i < b; i++)
		//{
		//	glVertex3f(baseArray[i][0], BASE_PLANE, baseArray[i][1] - Z_DISP);
		//}
	}

	if (gridLines) {
		renderGridLines();
	}
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	// Set the camera
	gluLookAt(x, y + ey, z,
		x + lx, y + ly, z + lz,
		0.0f, 1.0f, 0.0f);

	//glLoadIdentity();
	glPushMatrix();

	drawPoints();

	glPopMatrix();

	glutSwapBuffers();
}


void getCofactor(float A[N][N], float temp[N][N], int p, int q, int n)
{
	int i = 0, j = 0;
	for (int row = 0; row < n; row++)
	{
		for (int col = 0; col < n; col++)
		{
			if (row != p && col != q)
			{
				temp[i][j++] = A[row][col];
				if (j == n - 1)
				{
					j = 0;
					i++;
				}
			}
		}
	}
}

int determinant(float A[N][N], int n)
{
	int D = 0;
	if (n == 1)
		return A[0][0];

	float temp[N][N];

	int sign = 1;
	for (int f = 0; f < n; f++)
	{
		getCofactor(A, temp, 0, f, n);
		D += sign * A[0][f] * determinant(temp, n - 1);
		sign = -sign;
	}
	return D;
}

void adjoint(float A[N][N], int adj[N][N])
{
	if (N == 1)
	{
		adj[0][0] = 1;
		return;
	}

	int sign = 1;
	float temp[N][N];

	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			getCofactor(A, temp, i, j, N);
			sign = ((i + j) % 2 == 0) ? 1 : -1; 
			adj[j][i] = (sign)*(determinant(temp, N - 1));
		}
	}
}

bool inverse(float A[N][N], float inverse[N][N])
{
	int det = determinant(A, N);
	if (det == 0)
	{
		return false;
	}
	int adj[N][N];
	adjoint(A, adj);
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			inverse[i][j] = adj[i][j] / float(det);
	return true;
}

void aToM(GLfloat A[N*N], float M[N][N]) {
	for (int i = 0; i < N; ++i) {
		for (int j = 0; j < N; ++j) {
			M[i][j] = A[i*N + j];
		}
	}
}

void multiply(float A[N][N], float B[N][N], float Result[N][N])
{
	for (int I = 0; I < N; ++I)
	{
		for (int J = 0; J < N; ++J)
		{
			float s = 0.0f;
			for (int K = 0; K < N; ++K)
			{
				s += A[I][K] * B[K][J];
			}
			Result[I][J] = s;
		}
	}
}

void getTransformationMatrix(float inv[N][N]) {
	GLfloat m[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, m);
	float M[N][N];
	aToM(m, inv);
	//inverse(M, inv);
}


void myMouseMove(int xx, int yy)
{
	if (LEFT_BUTTON_PRESSED == 1) {

		//float n1[N][N];
		//getTransformationMatrix(n1);

		//float n[N][N];
		//inverse(n1, n);

		//gluLookAt(x, y + ly, z,
		//	x + lx, y + ly, z + lz,
		//	0.0f, 1.0f, 0.0f);

		//glPushMatrix();

		//float m[N][N];
		//getTransformationMatrix(m);

		//float r[N][N];
		//multiply(n1, m, r);

		///*for (int i = 0; i < 16; ++i) {
		//	if (i % 4 == 0)
		//		std::cout << "\n";
		//	std:: cout << m[i] << "\t";
		//}
		//std::cout << "\n";*/

		//int x1 = xx;
		//int y1 = yy;

		//xx = x1 * r[0][0] + y1 * r[1][0] /*+ z * m[8]*/ + r[3][0];
		//yy = x1 * r[0][1] + y1 * r[1][1] /*+ z * m[9]*/ + r[3][1];


		/*glm::matrix;

		typedef double *vec3_t;
		vec3_t vec3_create(vec3_t vec);
		vec3_t vec = vec3_create(NULL); 
		vec[0] = xx;
		vec[1] = yy;
		vec[2] = 0;
		vec[3] = 1;

		vec = matrix * vec;*/

		if (Mode == BASE_MODE) {
			//std::cout << "x:" << x << " y:" << y;
			baseArray[b][0] = xx - DISPLACEMENT;
			baseArray[b++][1] = DISPLACEMENT - yy;
		}
		else if (Mode == HEIGHT_MODE) {
			//std::cout << "x:" << x << " y:" << y;
			heightArray[h][0] = xx - DISPLACEMENT;
			heightArray[h++][1] = DISPLACEMENT - yy;
			minHeight = std::min(DISPLACEMENT - yy, minHeight);
			maxHeight = std::max(DISPLACEMENT - yy, maxHeight);

			minBoundary = std::min(minBoundary, xx - DISPLACEMENT);
			maxBoundary = std::max(maxBoundary, xx - DISPLACEMENT);
			//std::cout << maxHeight << " and " << minHeight;

			lw = float(h) / 200.0f;
		}
		//glPopMatrix();
	}
}

void handleMouseClicks(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON) {
		if (state == GLUT_DOWN) {
			LEFT_BUTTON_PRESSED = 1;
		}
		else if (state == GLUT_UP) {
			LEFT_BUTTON_PRESSED = 0;
		}
	}

	/*if (button == GLUT_RIGHT_BUTTON and state == GLUT_UP) {
		if (Mode == BASE_MODE) {
			std::cout << "Current Mode: Height Mode";
			Mode = HEIGHT_MODE;
		} else if (Mode == HEIGHT_MODE) {
			std::cout << "Current Mode: Height Mode";
			Mode = GENERATE_PERLIN;
		}
	}*/
}

void generateLandformVector() {
	int minX = 10000;
	int maxX = -10000;
	int minZ = 10000;
	int maxZ = -10000;

	for (int i = 0; i < b; ++i) {
		minX = std::min(baseArray[i][0], minX);
		maxX = std::max(baseArray[i][0], maxX);
		minZ = std::min(baseArray[i][1], minZ);
		maxZ = std::max(baseArray[i][1], maxZ);
	}
}

void handleKeyClicks(unsigned char Key, int xx, int yy) {
	float fr = 1.0f;

	switch (Key)
	{
	case 'm':
		if (Mode == BASE_MODE) {
			std::cout << "Current Mode: Height Mode";
			Mode = HEIGHT_MODE;
			//generateLandformVector();
		}
		else if (Mode == HEIGHT_MODE) {
			std::cout << "Current Mode: Height Mode";
			Mode = GENERATE_PERLIN;
		}
		else if (Mode == GENERATE_PERLIN) {
			std::cout << "Current Mode: Perlin";
			Mode = HEIGHT_MODE;
		}

	case 'a':
		angle -= 0.01f;
		lx = sin(angle);
		lz = -cos(angle);
		break;
	case 'd':
		angle += 0.01f;
		lx = sin(angle);
		lz = -cos(angle);
		break;
	case 'w':
		x += lx * fr;
		z += lz * fr;
		break;
	case 's':
		x -= lx * fr;
		z -= lz * fr;
		break;
	case 'r':
		y += ly * fr;
		break;
	case 'f':
		y -= ly * fr;
		break;
	case 'o':
		if (Mode == GENERATE_PERLIN)
			w += lw;
		break;
	case 'l':
		if (Mode == GENERATE_PERLIN)
			w = std::max(w-lw, 0.0f);
		break;
	case 'g':
		gridLines = !gridLines;
	default:
		break;
	}
}

void handleCameraRotation(int key, int xx, int yy) {
	float fr = 5.0f;

	switch (key) {
	case GLUT_KEY_LEFT:
		angle -= 0.1f;
		lx = sin(angle);
		lz = -cos(angle);
		break;
	case GLUT_KEY_RIGHT:
		angle += 0.1f;
		lx = sin(angle);
		lz = -cos(angle);
		break;
	case GLUT_KEY_UP:
		x += lx * fr;
		z += lz * fr;
		break;
	case GLUT_KEY_DOWN:
		x -= lx * fr;
		z -= lz * fr;
		break;
	case GLUT_KEY_PAGE_UP:
		y += ly * fr;
		break;
	case GLUT_KEY_PAGE_DOWN:
		y -= ly * fr;
		break;
	}
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(800, 800);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Sketch Terrain");

	init();

	glutDisplayFunc(display);
	glutIdleFunc(display);
	glutReshapeFunc(reshape);
	glutMotionFunc(myMouseMove);
	glutMouseFunc(handleMouseClicks);
	//glutPassiveMotionFunc(myMouseMove);
	glutKeyboardFunc(handleKeyClicks);
	glutSpecialFunc(handleCameraRotation);
	glutMainLoop();
	return 0;
}