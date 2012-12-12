#include "project4.h"

using namespace std;

static ObjReader objReader;

UINT texture_array[6];

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
bool toggle_run = false;
bool toggle_view = false;

int shader_state = -1;

char toon_vert[] = "toon.vert";
char toon_frag[] = "toon.frag";

Shader * shader;

float prevX, prevY, prevZ;
float xf, yf, zf;
bool pressedLeft, pressedMiddle, pressedRight;

float theta, axis;

GLfloat mat_diffuse_car[] = {1.0,1.0,1.0,1.0};
GLfloat mat_diffuse_character[] = {1.0,0.0,0.0,1.0};
GLfloat mat_specular[]  = {1.0,1.0,1.0,1.0};
GLfloat mat_shininess[] = {25.0};

GLfloat light0_diffuse[] = {1.0,0.827,0.608,1.0};
GLfloat light0_diffuse_character[] = {1.0,0.0,0.0,1.0};
GLfloat light0_specular[] = {1.0,1.0,1.0,1.0};
GLfloat light0_shininess[] = {25.0};
GLfloat light0_position[] = {0.0,50.0,0.0};
GLfloat light0_direction[] = {0.8,-1.0,0.0}; // GHOST CODE

GLfloat zero[] = {0.0,0.0,0.0,0.0};

int window::width  = 512;   // set window width in pixels here
int window::height = 512;   // set window height in pixels here

Group * g_world;

bool debug = false;
bool once_flag = true;
bool animate = true;
bool culling = false;

Vector3 p = Vector3(0,0,5);
Vector3 l = Vector3(0,0,0);
Vector3 up = Vector3(0,1,0);
Vector3 d = Vector3(0,0,1);

bool is_fullscreen = false;
const int MAX_PARTICLES = 500;

typedef struct
{
	float Xsrc;
	float Ysrc;
	float Zsrc;
	float Xpos;
	float Ypos;
	float Zpos;
	float Xmov;
	float Zmov;
	float Red;
	float Green;
	float Blue;
	float Direction;
	float Acceleration;
	float Deceleration;
	float Scalez;
	bool Visible;
}PARTICLES;

PARTICLES particle[MAX_PARTICLES];
int TERRAIN_ONE_ID;
int TERRAIN_TWO_ID;
int TERRAIN_THREE_ID;
int CURRENT_TERRAIN_ID;
int CURRENT_TERRAIN_HEIGHT = 2;
const float TERRAIN_ONE_HEIGHT = 1.0;
const float TERRAIN_TWO_HEIGHT = 1.5;
const float TERRAIN_THREE_HEIGHT = 2.0;

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

void initParticles(float xsrc, float ysrc, float zsrc)
{
	for (int i = 0; i < MAX_PARTICLES; ++i)
	{
		particle[i].Xsrc = xsrc;
		particle[i].Ysrc = ysrc;
		particle[i].Zsrc = zsrc;
		particle[i].Xpos = xsrc;
		particle[i].Ypos = ysrc + 5;
		particle[i].Zpos = zsrc;
		//Set the amount of movement on the X axis to a random number, we dont want
		//all our particles doing the same thing  
		particle[i].Xmov = (((((((2 - 1 + 1) * rand()%11) + 1) - 1 + 1) * rand()%11) + 1) * 0.005) - (((((((2 - 1 + 1) * rand()%11) + 1) - 1 + 1) * rand()%11) + 1) * 0.005);
		//Set the amount of movement on the Z axis to a random number, as above, we dont
		//want all our particles doing the same thing  
		particle[i].Zmov = (((((((2 - 1 + 1) * rand()%11) + 1) - 1 + 1) * rand()%11) + 1) * 0.005) - (((((((2 - 1 + 1) * rand()%11) + 1) - 1 + 1) * rand()%11) + 1) * 0.005);
		//Set the amount of Red to 1
		particle[i].Red = 1;
		//Set the amount of Green to 1
		particle[i].Green = 1;
		//Set the amount of Blue to 1
		particle[i].Blue = 1;
		//Scale the particle to 1 quarter of its original size
		particle[i].Scalez = 0.25;
		//Set the initial rotation angle to 0
		particle[i].Direction = 0;
		//Set the amount of acceleration to a random number so they climb to different
		//heights
		particle[i].Acceleration = ((((((8 - 5 + 2) * rand()%11) + 5) - 1 + 1) * rand()%11) + 1) * 0.02;
		//Decrease their acceleration by 0.0025. They will slow down at a constant
		//rate but you will not see a difference
		particle[i].Deceleration = 0.0025;
	}
}

void updateParticles()
{
	for (int i = 0; i < MAX_PARTICLES; ++i)
	{
		//Set the color of the current particle
		glColor3f (particle[i].Red, particle[i].Green, particle[i].Blue);

		//Move the particle on the Y axes, adding on the amount of acceleration
		//and then subtracting the rate of deceleration
		particle[i].Ypos = particle[i].Ypos + particle[i].Acceleration - particle[i].Deceleration;
		//Increase the deceleration rate so the particle falls gaining speed
		particle[i].Deceleration = particle[i].Deceleration + 0.0025;

		//Move the particle on the X axis
		particle[i].Xpos = particle[i].Xpos + particle[i].Xmov;
		//Move the particle on the Z axis
		particle[i].Zpos = particle[i].Zpos + particle[i].Zmov;

		//Rotate the particle
		particle[i].Direction = particle[i].Direction + ((((((int)(0.5 - 0.1 + 0.1) * rand()%11) + 1) - 1 + 1) * rand()%11) + 1);

		//Now here I am saying that if the particle goes beneath its initial height
		//which I set earlier to -5, then it will restart the particle changing some
		//of the variables.
		if (particle[i].Ypos < -5)
		{
			particle[i].Xpos = particle[i].Xsrc;
			particle[i].Ypos = particle[i].Ysrc;
			particle[i].Zpos = particle[i].Zsrc;
			particle[i].Red = 1;
			particle[i].Green = 1;
			particle[i].Blue = 1;
			//Set the angle of rotation
			particle[i].Direction = 0;
			//Adjust the Acceleration rate to another random number
			particle[i].Acceleration = ((((((8 - 5 + 2) * rand()%11) + 5) - 1 + 1) * rand()%11) + 1) * 0.02;
			//Reset the Deceleration rate
			particle[i].Deceleration = 0.0025;
		}
	}
}

void drawParticles()
{
	for (int i = 0; i < MAX_PARTICLES; ++i)
	{
		//Distinguish the start of our current particle, we do not wish for them
		//all to be affected by the ones prior
		glPushMatrix();

		//Translate the particle on the X, Y and Z axis accordingly
		glTranslatef (particle[i].Xpos, particle[i].Ypos, particle[i].Zpos);

		//Rotate the particle
		glRotatef (particle[i].Direction - 90, 0, 0, 1);
		//Scale the particle
		glScalef (particle[i].Scalez, particle[i].Scalez, particle[i].Scalez);

		glColor3f(particle[i].Red, particle[i].Green, particle[i].Blue);

		//Disable Depth Testing so our masking appears as one
		glDisable (GL_DEPTH_TEST);

		//Draw the shape

		glBegin (GL_QUADS);
		glVertex3f (-1, -1, 0);
		glVertex3f (1, -1, 0);
		glVertex3f (1, 1, 0);
		glVertex3f (-1, 1, 0);
		glEnd();

		//Re-enable Depth Testing
		glEnable(GL_DEPTH_TEST);

		//End the changes to the current object
		glPopMatrix();
	}
}

void Draw_Skybox(float x, float y, float z, float width, float height, float length)
{
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);
    
	// Center the Skybox around the given x,y,z position
	x = x - width  / 2;
	y = y - height / 2;
	z = z - length / 2;

	glColor4f(1,1,1,1);
	// Draw Front side
	glBindTexture(GL_TEXTURE_2D, texture_array[SKYFRONT]);
	glBegin(GL_QUADS);	
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x,		  y,		z+length);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x,		  y+height, z+length);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x+width, y+height, z+length); 
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x+width, y,		z+length);
	glEnd();

	// Draw Back side
	glBindTexture(GL_TEXTURE_2D, texture_array[SKYBACK]);
	glBegin(GL_QUADS);		
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x+width, y,		z);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x+width, y+height, z); 
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x,		  y+height,	z);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x,		  y,		z);
	glEnd();

	// Draw Left side
	glBindTexture(GL_TEXTURE_2D, texture_array[SKYLEFT]);
	glBegin(GL_QUADS);		
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x,		  y+height,	z);	
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x,		  y+height,	z+length); 
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x,		  y,		z+length);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x,		  y,		z);		
	glEnd();

	// Draw Right side
	glBindTexture(GL_TEXTURE_2D, texture_array[SKYRIGHT]);
	glBegin(GL_QUADS);		
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x+width, y,		z);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x+width, y,		z+length);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x+width, y+height,	z+length); 
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x+width, y+height,	z);
	glEnd();

	// Draw Up side
	glBindTexture(GL_TEXTURE_2D, texture_array[SKYUP]);
	glBegin(GL_QUADS);		
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x+width, y+height, z);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x+width, y+height, z+length); 
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x,		  y+height,	z+length);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x,		  y+height,	z);
	glEnd();

	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
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
	printw (9.5, 18.5, 0, "FPS: %4.2f", fps);
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
bool jump = false;
float j_start = -1.5;
float j_x;
int rage_count = 0;

int rage_path = 0;

void selectRandomPath()
{
	if (rage_count == 0) // select random path
	{
		rage_path = rand()%5;
		rage_count += 1;
	}
	else // continue executing code
	{
		rage_count++;
		if (rage_count < 30)
		{
			if (rage_path == 0)
			{
				
				rotation.getMatrix().setMatrix(world.getMatrix().rotateY(-0.2));
				world.getMatrix().setMatrix(world.getMatrix().multiply(rotation.getMatrix()));
				world.getMatrix().setMatrix(world.getMatrix().translate(0.0,0,2.1));
				rage_count+=5;
			}
			else if (rage_path == 1)
			{
				world.getMatrix().setMatrix(world.getMatrix().translate(0.0,0,2.1));
				rotation.getMatrix().setMatrix(world.getMatrix().rotateY(0.2));
				world.getMatrix().setMatrix(world.getMatrix().multiply(rotation.getMatrix()));
				rage_count+=5;
			}
			else
			{
				world.getMatrix().setMatrix(world.getMatrix().translate(0.0,0,2.1));
			}
		}
		else
		{
			rage_count = 0;
		}
	}
}

void keyDown(unsigned char key, int x, int y) {
	Vector3 v;
	switch (key)
	{
		case ' ':
			if (!jump)
			{
				j_x = j_start;
				jump = true;
			}
			break;
		case 'w':
			keys[W] = true;
			break;
		case 's':
			keys[S] = true;
			break;
		case 'a':
			keys[A] = true;
			break;
		case 'd':
			keys[D] = true;
			break;
		case 'v':
			toggle_view = !toggle_view;
			if (toggle_view)
			{
				v = Vector3(0,10,5);
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
		/*case '1':
			light0Toggle = !light0Toggle;
			if (light0Toggle)
				glEnable(GL_LIGHT0);
			else
				glDisable(GL_LIGHT0);
			break;*/
		case 'r':
			toggle_run = !toggle_run;
			rage_count = 0;
			break;
		case 'p':
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
		case 'u':
			debug = !debug;
			break;
		case '1':
			CURRENT_TERRAIN_ID = TERRAIN_ONE_ID;
			//CURRENT_TERRAIN_HEIGHT = 2;
			break;
		case '2':
			CURRENT_TERRAIN_ID = TERRAIN_TWO_ID;
			//CURRENT_TERRAIN_HEIGHT = 2;
			break;
		case '3':
			CURRENT_TERRAIN_ID = TERRAIN_THREE_ID;
			//CURRENT_TERRAIN_HEIGHT = 2;
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
	float y;
	if (!toggle_run)
	{
		if (keys[W])
		{
			world.getMatrix().setMatrix(world.getMatrix().translate(0.0,0,1.1));
		}
		if (keys[S])
		{
			world.getMatrix().setMatrix(world.getMatrix().translate(0.0,0,-1.1));
		}
	}
	else
	{
		if (keys[W])
		{
			world.getMatrix().setMatrix(world.getMatrix().translate(0.0,0,1.1));
			toggle_run = false;
		}
		if (keys[S])
		{
			world.getMatrix().setMatrix(world.getMatrix().translate(0.0,0,-1.1));
			toggle_run = false;
		}
		else {
			//world.getMatrix().setMatrix(world.getMatrix().translate(0.0,0,1.1));
			selectRandomPath();
		}
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
	if (jump)
	{
		j_x += 0.1;
		y = -(j_x*j_x);
		if (j_x >= 0) y = y*-1;

		if (j_x > -j_start)
			jump = false;
		else
			world.getMatrix().setMatrix(world.getMatrix().translate(0.0,y,0.0));
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

void drawCharacter()
{
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse_car);
	glColor3f(1.0,1.0,1.0);
	glBegin(GL_TRIANGLES);
	for (int i=0; i<nIndices; i+=3)
	{
		glVertex3f(vertices[indices[i]*3],vertices[indices[i]*3+1],vertices[indices[i]*3+2]);
		glVertex3f(vertices[indices[i+1]*3],vertices[indices[i+1]*3+1],vertices[indices[i+1]*3+2]);
		glVertex3f(vertices[indices[i+2]*3],vertices[indices[i+2]*3+1],vertices[indices[i+2]*3+2]);

		if (normals != NULL)
		{
			glNormal3f(normals[indices[i]*3],normals[indices[i]*3+1],normals[indices[i]*3+2]);
			glNormal3f(normals[indices[i+1]*3],normals[indices[i+1]*3+1],normals[indices[i+1]*3+2]);
			glNormal3f(normals[indices[i+2]*3],normals[indices[i+2]*3+1],normals[indices[i+2]*3+2]);
		}
		if (texcoords != NULL)
		{
			glTexCoord3f(texcoords[indices[i]*3],texcoords[indices[i]*3+1],texcoords[indices[i]*3+2]);
			glTexCoord3f(texcoords[indices[i+1]*3],texcoords[indices[i+1]*3+1],texcoords[indices[i+1]*3+2]);
			glTexCoord3f(texcoords[indices[i+2]*3],texcoords[indices[i+2]*3+1],texcoords[indices[i+2]*3+2]);
		}
	}
	glEnd();
}

bool insideWorld(Matrix4 m)
{
	cout << *(m.getPointer() + 12) << " and " << *(m.getPointer() + 13) << " and " << *(m.getPointer() + 14) << endl;

	if (*(m.getPointer() + 12) < 240 && *(m.getPointer() + 12) > -240 &&
		*(m.getPointer() + 13) < 240 && *(m.getPointer() + 13) > -240 &&
		*(m.getPointer() + 14) < 240 && *(m.getPointer() + 14) > -240)
		return true;
	return false;
}

//----------------------------------------------------------------------------
// Callback method called when window readraw is necessary or
// when glutPostRedisplay() was called.
void window::displayCallback(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // clear color and depth buffers

	processMovement();

	camera.inverseCamera();
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse_character);

	// CHARACTER
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	cube.getMatrix().identity();
	all.getMatrix().setMatrix(cube.getMatrix().rotateY(PI/2));
	all.getMatrix().setMatrix(all.getMatrix().multiply(camera.getMatrix())); // YOU
	glLoadMatrixf(all.getMatrix().getPointer());
	glColor3f(0.0,0.0,1.0);
	if (toggle_shader) shader->bind();
	glutSolidSphere(2,30,30);
	if (toggle_shader) shader->unbind();
	if (debug)
	{
		glColor3f(1.0,0.0,0.0);
		glutWireCube(3.6);
	}
	drawFPS();
	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);

	glLoadMatrixf(world.getMatrix().getPointer());
	Draw_Skybox(0,150,0,800,800,800);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
	glPushMatrix();
	glTranslatef(0, -CURRENT_TERRAIN_HEIGHT, 0);
	glCallList(CURRENT_TERRAIN_ID);
	glPopMatrix();

	glDisable(GL_LIGHTING);
	light0.getMatrix().identity();
	light0.getMatrix().setMatrix(light0.getMatrix().translate(light0_position[0],light0_position[1],light0_position[2]));
	all.getMatrix().setMatrix(light0.getMatrix());
	all.getMatrix().setMatrix(all.getMatrix().multiply(world.getMatrix()));
	glLoadMatrixf(all.getMatrix().getPointer());
	glColor3f(1.0,1.0,1.0);
	glutSolidSphere(1.0,20,20);
	glEnable(GL_LIGHTING);

	// WORLD
	glEnable(GL_LIGHTING);
	if (toggle_shader) shader->bind();
	g_world->draw(world.getMatrix());
	if (toggle_shader) shader->unbind();
	glDisable(GL_LIGHTING);

	glutSwapBuffers();
}

void createWorld()
{
	Matrix4 m_world = Matrix4();
	m_world.identity();
	g_world = new MatrixTransform(id_world, m_world);
	int id = 0;
	int type;
	srand(1);
	float c_x,c_y,c_z;
	while (id < 20) {
		type = rand()%4;
		
		Matrix4 m_obj = Matrix4();
		m_obj.setMatrix(m_obj.rotateX((float)rand()/((float)RAND_MAX/(2*PI))));
		m_obj.setMatrix(m_obj.rotateY((float)rand()/((float)RAND_MAX/(2*PI))));
		m_obj.setMatrix(m_obj.rotateZ((float)rand()/((float)RAND_MAX/(2*PI))));
		
		Geode* mesh_obj;
		switch (type)
		{
			case SIZE1:
				mesh_obj = new Sphere(SIZE1);
				m_obj.setMatrix(m_obj.scale(1.0,1.0,1.0));
				break;
			case SIZE2:
				mesh_obj = new Sphere(SIZE2);
				m_obj.setMatrix(m_obj.scale(3.0,3.0,3.0));
				break;
			case SIZE3:
				mesh_obj = new Sphere(SIZE3);
				m_obj.setMatrix(m_obj.scale(5.0,5.0,5.0));
				break;
			case SIZE4:
				mesh_obj = new Sphere(SIZE4);
				m_obj.setMatrix(m_obj.scale(7.0,7.0,7.0));
				break;
		}
		
		c_x = rand()%300-rand()%300;
		c_y = 0;
		c_z = rand()%300-rand()%300;
		mesh_obj->setPos(c_x,c_y,c_z);
		m_obj.setMatrix(m_obj.translate(c_x,c_y,c_z));
		
		char id_obj[] = " ";
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
	camera.lookAt(p,l,up);

	cout << "Loading .obj files" << endl;
	objReader.readObj("cow.obj", nVerts, &vertices, &normals, &texcoords, nIndices, &indices);

	cout << "Loading texture files" << endl;
	loadTexture(texture_array,"desert_front.ppm", SKYFRONT);
	loadTexture(texture_array,"desert_back.ppm", SKYBACK);
	loadTexture(texture_array,"desert_left.ppm", SKYLEFT);
	loadTexture(texture_array,"desert_right.ppm", SKYRIGHT);
	loadTexture(texture_array,"desert_top.ppm", SKYUP);
	loadTexture(texture_array,"rust.ppm", 5);

	cout << "Generating terrain1" << endl;
	TerrainHelper th1;
	th1.terrainLoad(800,800,1);
	th1.terrainScale(0,TERRAIN_ONE_HEIGHT);
	TERRAIN_ONE_ID = th1.terrainCreateDL(0,0,0);

	cout << "Generating terrain2" << endl;
	TerrainHelper th2;
	th2.terrainLoad(800,800,1);
	th2.terrainScale(0, TERRAIN_TWO_HEIGHT);
	TERRAIN_TWO_ID = th2.terrainCreateDL(0,0,0);

	cout << "Generating terrain3" << endl;
	TerrainHelper th3;
	th3.terrainLoad(800, 800,1);
	th3.terrainScale(0,TERRAIN_THREE_HEIGHT);
	TERRAIN_THREE_ID = th3.terrainCreateDL(0,0,0);

	CURRENT_TERRAIN_ID = TERRAIN_ONE_ID;

	cout << "Creating world" << endl;
	createWorld();

	//cout << "initializing particles" << endl;
	//initParticles(0.0,0.0,0.0);

	glutMainLoop();
	return 0;
}

