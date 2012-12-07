#include "project4.h"

using namespace std;

static ObjReader objReader;

UINT SkyboxTexture[6];		// We need 6 textures for our Skybox

int nVerts;
float * vertices;
float * normals;
float * texcoords;
int nIndices;
int * indices;

float running = 0.0;
float angle = 0.0;

static Camera camera;
static Cube terrain;
static Cube character;
static Cube all;			// Final matrix
static Cube cube;			// Object matrix
static Cube cube1;			// Object matrix
static Cube cube2;			// Object matrix
static Cube light0;			// Light_0 matrix
static Cube light1;			// Light_1 matrix
static Cube base;			// Initial matrix before mouse event
static Cube temp;			// Evanescent matrix during mouse event
static Cube world;			// Accumulation matrix for base and temp
static Cube base_light0;
static Cube temp_light0;
static Cube world_light0;
static Cube base_light1;
static Cube temp_light1;
static Cube world_light1;
static Cube rotation;

static Cube dragon;
static bool sphere = true;

bool light0Toggle = false;
bool light1Toggle = false;
bool toggle_shader = false;
bool toggle_texture = false;
bool toggle_view = false;

int shader_state = -1;

char toon_vert[] = "toon.vert";
char toon_frag[] = "toon.frag";

Shader * shader;

float prevX, prevY, prevZ;
float xf, yf, zf;
bool pressedLeft, pressedMiddle, pressedRight;

float theta, axis;

GLfloat mat_diffuse[] = {1.0,1.0,1.0,1.0};
GLfloat mat_specular[]  = {1.0,1.0,1.0,1.0};
GLfloat mat_shininess[] = {25.0};

float light_pos[] = {-10.0,0.0,-50.0};

GLfloat light0_diffuse[] = { 0.0, 1.0, 0.0, 1.0 };
GLfloat light0_specular[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat light0_shininess[] = {50.0};
GLfloat light0_position[] = {light_pos[0],light_pos[1],light_pos[2],1.0};
GLfloat light0_direction[] = {0.0,10.0,-1.0};

GLfloat zero[] = {0.0,0.0,0.0,0.0};

int window::width  = 512;   // set window width in pixels here
int window::height = 512;   // set window height in pixels here

Group * g_world;
Group * g_world_ec;
Matrix4 m_id = Matrix4();

double r_torso = 0.0;
double r_head = 0.0;
double r_shoulder_left = 0.0;
double r_shoulder_right = 0.0;
double r_leg_left = 0.0;
double r_leg_right = 0.0;
double r_arm_left = 0.0;
double r_arm_right = 0.0;
double r_foot_left = 0.0;
double r_foot_right = 0.0;

bool b_torso = true;
bool b_head_forward = true;
bool b_shoulder_left_forward = true;
bool b_shoulder_right_forward = false;
bool b_leg_left_forward = true;
bool b_leg_right_forward = false;
bool b_arm_left_forward = true;
bool b_arm_right_forward = false;
bool b_foot_left_forward = true;
bool b_foot_right_forward = false;

bool debug = false;
bool once_flag = true;
bool animate = true;
bool culling = true;

int army_size = 20;

Vector3 p = Vector3(0,0,5);
Vector3 l = Vector3(0,0,0);
Vector3 up = Vector3(0,1,0);
Vector3 d = Vector3(0,0,1);

GLdouble fovy = 45.0;
GLdouble aspect = float(window::width)/float(window::height);
GLdouble zNear = 1.0;
GLdouble zFar = 1000.0;

GLdouble vf_hnear;
GLdouble vf_wnear;
GLdouble vf_hfar;
GLdouble vf_wfar;

float frustum[6][4];
Vector4 frustum_points[8];

float interval = 0.02;

bool is_fullscreen = false;
TerrainHelper * terrain_helper = new TerrainHelper();
int terrainListID;

// FPS
GLvoid *font_style = GLUT_BITMAP_8_BY_13;
void printw (float x, float y, float z, char* format, ...);
int frameCount = 0;
float fps = 0;
int currentTime = 0, previousTime = 0;
void calculateFPS();
void drawFPS();

Cube::Cube()
{
}

Matrix4& Cube::getMatrix()
{
	return matrix;
}

void Draw_Skybox(float x, float y, float z, float width, float height, float length)
{
	//glPushAttrib(GL_ENABLE_BIT);
	glDisable(GL_LIGHT0);
    glEnable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);
    //glDisable(GL_BLEND);
	// Center the Skybox around the given x,y,z position
	x = x - width  / 2;
	y = y - height / 2;
	z = z - length / 2;

	glColor4f(1,1,1,1);
	// Draw Front side
	glBindTexture(GL_TEXTURE_2D, SkyboxTexture[SKYFRONT]);
	glBegin(GL_QUADS);	
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x,		  y,		z+length);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x,		  y+height, z+length);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x+width, y+height, z+length); 
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x+width, y,		z+length);
	glEnd();

	// Draw Back side
	glBindTexture(GL_TEXTURE_2D, SkyboxTexture[SKYBACK]);
	glBegin(GL_QUADS);		
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x+width, y,		z);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x+width, y+height, z); 
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x,		  y+height,	z);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x,		  y,		z);
	glEnd();

	// Draw Left side
	glBindTexture(GL_TEXTURE_2D, SkyboxTexture[SKYLEFT]);
	glBegin(GL_QUADS);		
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x,		  y+height,	z);	
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x,		  y+height,	z+length); 
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x,		  y,		z+length);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x,		  y,		z);		
	glEnd();

	// Draw Right side
	glBindTexture(GL_TEXTURE_2D, SkyboxTexture[SKYRIGHT]);
	glBegin(GL_QUADS);		
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x+width, y,		z);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x+width, y,		z+length);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x+width, y+height,	z+length); 
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x+width, y+height,	z);
	glEnd();

	// Draw Up side
	glBindTexture(GL_TEXTURE_2D, SkyboxTexture[SKYUP]);
	glBegin(GL_QUADS);		
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x+width, y+height, z);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x+width, y+height, z+length); 
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x,		  y+height,	z+length);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x,		  y+height,	z);
	glEnd();

	// Draw Down side
	/*glBindTexture(GL_TEXTURE_2D, SkyboxTexture[SKYDOWN]);
	glBegin(GL_QUADS);		
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x,		  y,		z);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x,		  y,		z+length);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x+width, y,		z+length); 
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x+width, y,		z);
	glEnd();*/
    //glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
}

void Cube::drawObj()
{
	int i;
	glBegin(GL_TRIANGLES);
	for (i=0; i<nIndices; i+=3)
	{
		srand(i);
		double r = rand() % 100 / 100.0;
		double b = rand() % 100 / 100.0;
		double g = rand() % 100 / 100.0;
		glVertex3f(vertices[indices[i]*3],vertices[indices[i]*3+1],vertices[indices[i]*3+2]);
		glVertex3f(vertices[indices[i+1]*3],vertices[indices[i+1]*3+1],vertices[indices[i+1]*3+2]);
		glVertex3f(vertices[indices[i+2]*3],vertices[indices[i+2]*3+1],vertices[indices[i+2]*3+2]);

		if (normals != NULL)
		{
			glNormal3f(normals[indices[i]*3],normals[indices[i]*3+1],normals[indices[i]*3+2]);
			glNormal3f(normals[indices[i+1]*3],normals[indices[i+1]*3+1],normals[indices[i+1]*3+2]);
			glNormal3f(normals[indices[i+2]*3],normals[indices[i+2]*3+1],normals[indices[i+2]*3+2]);
		}
	}
	glEnd();
}

//-------------------------------------------------------------------------
//  Draw FPS
//-------------------------------------------------------------------------
void drawFPS()
{
    //  Load the identity matrix so that FPS string being drawn
    //  won't get animates
	glDisable(GL_LIGHTING);
	glLoadIdentity ();
	glColor3d(1.0,1.0,1.0);
	//  Print the FPS to the window
	printw (-18, -18, 0, "FPS: %4.2f", fps);
	glEnable(GL_LIGHTING);
}

//-------------------------------------------------------------------------
// Calculates the frames per second
//-------------------------------------------------------------------------
void calculateFPS()
{
    //  Increase frame count
    frameCount++;

    //  Get the number of milliseconds since glutInit called 
    //  (or first call to glutGet(GLUT ELAPSED TIME)).
    currentTime = glutGet(GLUT_ELAPSED_TIME);

    //  Calculate time passed
    int timeInterval = currentTime - previousTime;

    if(timeInterval > 1000)
    {
        //  calculate the number of frames per second
        fps = frameCount / (timeInterval / 1000.0f);

        //  Set time
        previousTime = currentTime;

        //  Reset frame count
        frameCount = 0;
    }
}

//-------------------------------------------------------------------------
//  Draws a string at the specified coordinates.
//-------------------------------------------------------------------------
void printw (float x, float y, float z, char* format, ...)
{
	va_list args;	//  Variable argument list
	int len;		//	String length
	int i;			//  Iterator
	char * text;	//	Text

	//  Initialize a variable argument list
	va_start(args, format);

	//  Return the number of characters in the string referenced the list of arguments.
	//  _vscprintf doesn't count terminating '\0' (that's why +1)
	len = _vscprintf(format, args) + 1; 

	//  Allocate memory for a string of the specified size
	text = (char *)malloc(len * sizeof(char));

	//  Write formatted output using a pointer to the list of arguments
	vsprintf_s(text, len, format, args);

	//  End using variable argument list 
	va_end(args);

	//  Specify the raster position for pixel operations.
	glRasterPos3f (x, y, z);

	//  Draw the characters one by one
    for (i = 0; text[i] != '\0'; i++)
        glutBitmapCharacter(font_style, text[i]);

	//  Free the allocated memory for the string
	free(text);
}

float findZ(float x, float y)
{
	float z = 1 - x*x - y*y;

	if (z < 0)
	{
		z = 0;
	}
	else
	{
		z = sqrt(z);
	}
	return z;
}

float getTheta(Vector3 v, Vector3 w)
{
	return acos(v.dot(w) / (v.magnitude() * w.magnitude()));
}

void processMouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		xf = (float)x/((float)window::width/2.0) - 1;
		yf = 1 - (float)y/((float)window::height/2.0);
		pressedLeft = true;
		prevX = xf;
		prevY = yf;
		prevZ = findZ(prevX,prevY);
		base.getMatrix().setMatrix(world.getMatrix());
	}
	else if (button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN)
	{
		xf = (float)x/((float)window::width/2.0) - 1;
		yf = 1 - (float)y/((float)window::height/2.0);
		pressedMiddle = true;
		prevX = xf;
		prevY = yf;
		prevZ = findZ(prevX,prevY);
		base.getMatrix().setMatrix(world.getMatrix());
	}
	else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		xf = (float)x/((float)window::width/2.0) - 1;
		yf = 1 - (float)y/((float)window::height/2.0);
		pressedRight = true;
		prevX = xf;
		prevY = yf;
		prevZ = findZ(prevX,prevY);
		base.getMatrix().setMatrix(world.getMatrix());
	}
	else
	{
		base.getMatrix().setMatrix(world.getMatrix());
		temp.getMatrix().identity();
		pressedLeft = false;
		pressedMiddle = false;
		pressedRight = false;
	}

}

void processMotion(int x, int y)
{
	xf = (float)x/((float)window::width/2.0) - 1;
	yf = 1 - (float)y/((float)window::height/2.0);
	if (pressedLeft)
	{
		zf = findZ(xf,yf);
		Vector3 prev = Vector3(prevX,prevY,prevZ);
		Vector3 curr = Vector3(xf,yf,zf);
		
		Vector3 p = prev.normalize();
		Vector3 c = curr.normalize();
		prev.set(p.get(0), p.get(1), p.get(2));
		curr.set(c.get(0), c.get(1), c.get(2));

		theta = getTheta(prev, curr);
		Vector3 axis3 = prev.cross(prev, curr);
		Vector3 a = axis3.normalize();
		axis3.set(a.get(0), a.get(1), a.get(2));

		Vector4 axis4 = Vector4(axis3.get(0), axis3.get(1), axis3.get(2), 0);

		temp.getMatrix().setMatrix(temp.getMatrix().rotateAA(axis4,theta));
	}
	else if (pressedMiddle)
	{
		temp.getMatrix().identity();
		temp.getMatrix().setMatrix(temp.getMatrix().translate((xf-prevX)*15,(yf-prevY)*15,0));
	}
	else if (pressedRight)
	{
		float s;
		if (prevY < yf)
		{
			s = 1-(yf-prevY)*-2;
			temp.getMatrix().identity();
			temp.getMatrix().setMatrix(temp.getMatrix().scale(s,s,s));
		}
		else
		{
			float s = 1+(yf-prevY)*0.5;
			if (s < 0)
			{
				s = 0.0001;
			}
			temp.getMatrix().identity();
			temp.getMatrix().setMatrix(temp.getMatrix().scale(s,s,s));
		}
	}
}

bool keys[4]; // wasd

void keyDown(unsigned char key, int x, int y) {
	Vector3 v;
	switch (key)
	{
		case 'w':
			//world.getMatrix().setMatrix(world.getMatrix().translate(0.0,0,1.1));
			keys[W] = true;
			break;
		case 's':
			//world.getMatrix().setMatrix(world.getMatrix().translate(0.0,0,-1.1));
			keys[S] = true;
			break;
		case 'a':
			//rotation.getMatrix().setMatrix(world.getMatrix().rotateY(-0.05));
			//world.getMatrix().setMatrix(world.getMatrix().multiply(rotation.getMatrix()));
			keys[A] = true;
			break;
		case 'd':
			//rotation.getMatrix().setMatrix(world.getMatrix().rotateY(0.05));
			//world.getMatrix().setMatrix(world.getMatrix().multiply(rotation.getMatrix()));
			keys[D] = true;
			break;
		case 'v':
			toggle_view = !toggle_view;
			if (toggle_view)
			{
				v = Vector3(0,0,-5);
				camera.lookAt(v,l,up);
				camera.getE().print();
				camera.getMatrix().print();
			}
			else
			{
				camera.lookAt(p,l,up);
				camera.getE().print();
				camera.getMatrix().print();
			}
			break;
		case '1':
			light0Toggle = !light0Toggle;
			if (light0Toggle)
				glEnable(GL_LIGHT0);
			else
				glDisable(GL_LIGHT0);
			break;
		case 'r':
			//base.getMatrix().identity();
			//temp.getMatrix().identity();
			world.getMatrix().identity();
			break;
		case 'h':
			toggle_shader = !toggle_shader;
			break;
		case 't':
			toggle_texture = !toggle_texture;
			if (toggle_texture)
				glEnable(GL_TEXTURE_2D);
			else
				glDisable(GL_TEXTURE_2D);
			break;
		case 'f':
			is_fullscreen = !is_fullscreen;
			if (is_fullscreen)
				glutFullScreen();
			else
				glutReshapeWindow(512, 512);
			break;
		case 27:
			//if (shader)
			//	delete shader;
			//glDeleteLists(terrainListID, 1);
			exit(0);
			break;
	}
}

void keyUp(unsigned char key, int x, int y)
{
	switch (key)
	{
		case 'w':
			keys[W] = false;
			break;
		case 's':
			
			keys[S] = false;
			break;
		case 'a':
			
			keys[A] = false;
			break;
		case 'd':
			
			keys[D] = false;
			break;
	}
}

void processMovement()
{
	if (keys[W])
	{
		world.getMatrix().setMatrix(world.getMatrix().translate(0.0,0,1.1));
	}
	if (keys[S])
	{
		world.getMatrix().setMatrix(world.getMatrix().translate(0.0,0,-1.1));
	}
	if (keys[A])
	{
		rotation.getMatrix().setMatrix(world.getMatrix().rotateY(-0.05));
		world.getMatrix().setMatrix(world.getMatrix().multiply(rotation.getMatrix()));
	}
	if (keys[D])
	{
		rotation.getMatrix().setMatrix(world.getMatrix().rotateY(0.05));
		world.getMatrix().setMatrix(world.getMatrix().multiply(rotation.getMatrix()));
	}
}

/** Load a ppm file from disk.
 @input filename The location of the PPM file.  If the file is not found, an error message
		will be printed and this function will return 0
 @input width This will be modified to contain the width of the loaded image, or 0 if file not found
 @input height This will be modified to contain the height of the loaded image, or 0 if file not found
 @return Returns the RGB pixel data as interleaved unsigned chars (R0 G0 B0 R1 G1 B1 R2 G2 B2 .... etc) or 0 if an error ocured
**/
unsigned char* loadPPM(const char* filename, int& width, int& height)
{
	const int BUFSIZE = 128;
	FILE* fp;
	unsigned int read;
	unsigned char* rawData;
	char buf[3][BUFSIZE];
	char* retval_fgets;
	size_t retval_sscanf;

	if ( (fp=fopen(filename, "rb")) == NULL)
	{
		std::cerr << "error reading ppm file, could not locate " << filename << std::endl;
		width = 0;
		height = 0;
		return NULL;
	}

	// Read magic number:
	retval_fgets = fgets(buf[0], BUFSIZE, fp);

	// Read width and height:
	do
	{
		retval_fgets=fgets(buf[0], BUFSIZE, fp);
	} while (buf[0][0] == '#');
	retval_sscanf=sscanf(buf[0], "%s %s", buf[1], buf[2]);
	width  = atoi(buf[1]);
	height = atoi(buf[2]);

	// Read maxval:
	do
	{
	  retval_fgets=fgets(buf[0], BUFSIZE, fp);
	} while (buf[0][0] == '#');

	// Read image data:
	rawData = new unsigned char[width * height * 3];
	read = fread(rawData, width * height * 3, 1, fp);
	fclose(fp);
	if (read != 1)
	{
		std::cerr << "error parsing ppm file, incomplete data" << std::endl;
		delete[] rawData;
		width = 0;
		height = 0;
		return NULL;
	}

	return rawData;
}

// load image file into texture object
void loadTexture(UINT textureArray[], char* strFileName, int ID)
{
	int twidth, theight;   // texture width/height [pixels]
	unsigned char* tdata;  // texture pixel data

	// Load image file
	tdata = loadPPM(strFileName, twidth, theight);
	if (tdata==NULL) return;

	// Create ID for texture
	glGenTextures(1, &textureArray[ID]);   

	// Set this texture to be the one we are working with
	glBindTexture(GL_TEXTURE_2D, textureArray[ID]);

	// Generate the texture
	glTexImage2D(GL_TEXTURE_2D, 0, 3, twidth, theight, 0, GL_RGB, GL_UNSIGNED_BYTE, tdata);

	// Set bi-linear filtering for both minification and magnification
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Select GL_MODULATE to mix texture with color for shading:
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	// Use bilinear filtering for higher visual quality:
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Set the wrap mode (but it doesn't make a difference in this project):
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	delete tdata;
}

//----------------------------------------------------------------------------
// Callback method called when system is idle.
void window::idleCallback(void)
{
	displayCallback();    // call display routine to redraw cube
	calculateFPS();
}

//----------------------------------------------------------------------------
// Callback method called when window is resized.
void window::reshapeCallback(int w, int h)
{	
	width = w;
	height = h;
	glViewport(0, 0, w, h);  // set new viewport size
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90.0, w/(float)h, 1, 1000);
	glTranslatef(0, -5, -15);
	glMatrixMode(GL_MODELVIEW);
}

//----------------------------------------------------------------------------
// Callback method called when window readraw is necessary or
// when glutPostRedisplay() was called.
void window::displayCallback(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // clear color and depth buffers

	//world.getMatrix().setMatrix(base.getMatrix().multiply(temp.getMatrix()));
	processMovement();
	camera.inverseCamera();

	// CHARACTER
	glDisable(GL_LIGHTING);
	cube.getMatrix().identity();
	all.getMatrix().setMatrix(cube.getMatrix().rotateY(PI/2));
	all.getMatrix().setMatrix(all.getMatrix().multiply(camera.getMatrix())); // YOU
	glLoadMatrixf(all.getMatrix().getPointer());
	glColor3f(1.0,1.0,1.0);
	//glutSolidSphere(2.0,20,20);
	if (toggle_shader) shader->bind();
	cube.drawObj();
	if (toggle_shader) shader->unbind();
	glEnable(GL_LIGHTING);

	cube.getMatrix().identity();
	all.getMatrix().setMatrix(cube.getMatrix());
	all.getMatrix().setMatrix(all.getMatrix().multiply(world.getMatrix()));
	glLoadMatrixf(all.getMatrix().getPointer());
	Draw_Skybox(0,150,0,1000,1000,1000);
	glCallList(terrainListID);
	
	// WORLD
	glDisable(GL_LIGHTING);
	g_world->draw(world.getMatrix());
	glEnable(GL_LIGHTING);
	
	glutSwapBuffers();
	
}

void createWorld()
{
	Matrix4 m_world = Matrix4();
	m_world.identity();
	g_world = new MatrixTransform(id_world, m_world);
	int i,j;
	int id = 0;
	srand(1);
	while (id < 200) {
		Geode* mesh_obj = new Sphere(TORSO);
		Matrix4 m_obj = Matrix4();
		m_obj.setMatrix(m_obj.translate(rand()%300-rand()%300,2,rand()%300-rand()%300));
		char id_obj[] = "hi";
		Group* mt_obj = new MatrixTransform(id_obj, m_obj);
		g_world->addNode(mt_obj);
		mt_obj->addNode(mesh_obj);
		id+=1;
	}
}


int main(int argc, char *argv[])
{
	GLfloat ambient[]         = {0.5,0.5,0.5,1.0};					// default ambient color for all light sources

	glutInit(&argc, argv);											// initialize GLUT
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);		// open an OpenGL context with double buffering, RGB colors, and depth buffering
	glutInitWindowSize(512, 512);									// set initial window size
	glutCreateWindow("Final Project");								// open window and set window title
	if (is_fullscreen)
		glutFullScreen();

	glEnable(GL_NORMALIZE);
	glEnable(GL_DEPTH_TEST);										// enable z-buffer
	glClearColor(0.0, 0.0, 0.0, 0.0);								// set clear color to black

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient);				// set the default ambient color
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);		// allow glColor to set ambient and diffuse colors of geometry

	// Generate light source:
	glEnable(GL_LIGHTING);											// enables lighting; this changes the behavior of glColor
	glEnable(GL_LIGHT0);											// enable a light source; otherwise you will only see ambient light
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glDisable(GL_TEXTURE_2D);

	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);
	glLightfv(GL_LIGHT0, GL_SHININESS, light0_shininess);
	//glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, light0_direction);
	glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
	
	// Install callback functions:
	glutDisplayFunc(window::displayCallback);
	glutReshapeFunc(window::reshapeCallback);
	glutIdleFunc(window::idleCallback);

	glutMouseFunc(processMouse);
	glutMotionFunc(processMotion);
	glutKeyboardFunc(keyDown);
	glutKeyboardUpFunc(keyUp);

	// Initialize cube matrix:
	m_id.identity();
	character.getMatrix().identity();
	all.getMatrix().identity();
	cube.getMatrix().identity();
	cube1.getMatrix().identity();
	cube2.getMatrix().identity();
	dragon.getMatrix().identity();
	base.getMatrix().identity();
	temp.getMatrix().identity();
	world.getMatrix().identity();
	terrain.getMatrix().identity();
	rotation.getMatrix().identity();

	// Shaders
	shader = new Shader(toon_vert,toon_frag,true);

	cout << "Loading .obj files" << endl;
	objReader.readObj("cow.obj", nVerts, &vertices, &normals, &texcoords, nIndices, &indices);

	camera.lookAt(p,l,up);

	vf_hnear = 2*tan(fovy/2)*zNear;
	vf_wnear = vf_hnear*aspect;
	vf_hfar = 2*tan(fovy/2)*zFar;
	vf_wfar = vf_hfar*aspect;

	createWorld();

	loadTexture(SkyboxTexture,"desert_front.ppm", SKYFRONT);
	loadTexture(SkyboxTexture,"desert_back.ppm", SKYBACK);
	loadTexture(SkyboxTexture,"desert_left.ppm", SKYLEFT);
	loadTexture(SkyboxTexture,"desert_right.ppm", SKYRIGHT);
	loadTexture(SkyboxTexture,"desert_top.ppm", SKYUP);

	terrain_helper->terrainLoad(500,500,1);
	terrain_helper->terrainScale(0, 1);
	terrainListID = terrain_helper->terrainCreateDL(0,0,0);

	//loadTexture(SkyboxTexture,"desert_down.ppm.jpg", SKYDOWN);

	delete(terrain_helper);

	glutMainLoop();
	return 0;
}

