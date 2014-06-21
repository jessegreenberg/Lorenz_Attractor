/*
 *  Homework_1
 *  Jesse Greenberg
 *  CSCI 4229
 *  Plotting the Lorenz Attractor in 3D
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#define GL_GLEXT_PROTOTYPES
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

// Global Variables
int th = 0; // Azimuthal angle
int ph = 0; // Elevation of view
double w = 1; // W variable for fourth dimension
double dim = 2; // Dimension of orthogonal box
double s = 10.0; // Lorenz parameter sigma
double b = 2.666; // Lorenz parameter beta
double r = 28.0; // Lorenz parameter rho
int animate = 0; // State variable for particle animation
//double xi = 1; // Initial coordinate for the particle
//double yi = 1; // Intitial coordinate for the particle
//double zi = 1; // Initial coordinate for the particle
double xsolution[50000]; // array holding x solution coordinates
double ysolution[50000]; // array holding y solution coordinates
double zsolution[50000]; // array holding z solution coordinates
int num = 0; // Location of animation particle;

/* Convenience routine to output raster text
 * Taken from example six in lecture material
 **/
#define LEN 8192 // Maximum length of text string
void Print(const char* format , ...)
{
	char buf[LEN];
	char* ch = buf;
	va_list args;
	va_start(args, format);
	vsnprintf(buf, LEN, format, args);
	va_end(args);
	// Display the characters at the raster position
	while (*ch)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *ch++);
}

/* Function to generate numerical coordinates of 
 * the Lorenz Attractor.
 * */
void draw_attractor()
{
	int i; // iteration variable
	/* Initial conditions */
	double x = 1.0;
	double y = 1.0;
	double z = 1.0;
	
	/* Time step */
	double dt = 0.001;
	
	glPointSize(1); // Set size of vertices.
	
	glPushMatrix(); // Push the matrix onto the stack
	glScaled(0.045, 0.045, 0.045); // Scale the vertex placement
	
	/*
	 * Integrate 50,000 steps (50 time units with dt = 0.001)
	 * Solving with explicit Euler integration
	 * */

	for (i = 0; i < 50000; i++)
	{
		double dx = s*(y-x);
		double dy = x *(r-z)-y;
		double dz = x*y - b*z;
		x += dt*dx;
		y += dt*dy;
		z += dt*dz;
		/* Store the solution values for animation*/
		xsolution[i] = x;
		ysolution[i] = y;
		zsolution[i] = z;
		// Draw the vertices
		glBegin(GL_POINTS);
		glVertex4d(x, y, z, w);
		glEnd();
	}
		
	/* Pop transformation matrix off the stack */
	glPopMatrix();
	// Tell GLUT to update the screen
	glutPostRedisplay();
}

/* Animation function to show trajectory of particle.
 * Shows the velocity of the particlle as well.
 * */
void animate_particle()
{

	glPushMatrix();
	glScaled(0.045, 0.045, 0.045); // Scale the particle to the plot
	
	double x = xsolution[num];
	double y = ysolution[num];
	double z = zsolution[num];
	num++;
	
	/* Draw the particle */
	glPointSize(5);
	glColor3d(0, 1, 1);
	glBegin(GL_POINTS);
	glVertex4d(x, y, z, w);
	glEnd();
	/* glut function to add delay to callback for animation */
	glutTimerFunc(1000, animate_particle, 0);
	glPopMatrix();
}


// Print the lorenz parameters
void print_params(){
	glWindowPos2i(5, 80);
	Print("rho = %.2f", r);
	glWindowPos2i(5, 60);
	Print("beta = %.2f", b);
	glWindowPos2i(5, 40);
	Print("sigma = %.2f", s);
}

// Draw the cartesian axes
void draw_axes()
{
	// Draw the axes
   glColor3f(1, 1, 1);
   glBegin(GL_LINES);
   glVertex3d(0,0, 0);
   glVertex3d(1.5, 0, 0);
   glVertex3d(0, 0, 0);
   glVertex3d(0, 1.5, 0);
   glVertex3d(0, 0, 0);
   glVertex3d(0, 0, 1.5);
   glEnd();
   // Label the axes
   glRasterPos3d(1.5, 0.0, 0.0);
   Print("X");
   glRasterPos3d(0.0, 1.5, 0);
   Print("Y");
   glRasterPos3d(0.0, 0.0, 1.5);
   Print("Z");
   // Update the view
   glutPostRedisplay();
}

// Function called by GLUT to display the scene
void display()
{
   //  Clear the screen
   glClear(GL_COLOR_BUFFER_BIT);
   // Load the identity matrix
   glLoadIdentity();
   // Set the view angle
   glRotated(ph, 1, 0, 0);
   glRotated(th, 0, 1, 0);
   // Draw the Lorenz Attractor
   glColor3f(0, 0, 1);
   draw_attractor();
   // Draw the axes
   draw_axes();
   glColor3f(0, 0, 1);
   if(animate == 1) {
	   // Refresh the Attractor
	   draw_attractor();
	   // Draw the animation
	   animate_particle();
	}
   // Print the Lorenz Parameters
   print_params();
   // Make the scene visible
   glFlush();
   // Remove Clipping
   glutSwapBuffers();
}

// Handles key presses
void key(unsigned char ch, int x, int y)
{
	// Conveniently exit on escape
	if (ch == 27) {
		exit(0);
	}
	// Easily reset view angle
	else if (ch == '0') {
		th = ph = 0;
	}
	// Decrease w by 0.1 (zoom in)
	else if (ch == '+') {
		w -= 0.1;
	}
	// Increase w by 0.1 (zoom out)
	else if (ch == '-') {
		w += 0.1;
	}
	
	/* CHANGE THE LORENZ PARAMETERS */
	// Increase rho by 0.1
	else if (ch == 'a' || ch == 'A') {
		r += 0.1;
	}
	// Decrease rho by 0.1
	else if (ch == 'z' || ch == 'Z') {
		r -= 0.1;
	}
	// Increase sigma by 0.1
	else if (ch == 's' || ch == 'S') {
		s += 0.1;
	}
	// Decrease sigma by 0.1 {
	else if (ch == 'x' || ch == 'X') {
		s -= 0.1;
	}
	// Increase beta by 0.1
	else if (ch == 'd' || ch == 'D') {
		b += 0.1;
	}
	// Decrease beta by 0.1
	else if (ch == 'c' || ch == 'C') {
		b -= 0.1;
	}
	// Reset system to initial state of chaos
	else if (ch == 'r' || ch == 'R') {
		r = 28;
		s = 10;
		b = 2.666;
	}
	else if (ch == 32) {
		if (animate == 0) {
			num = 0;
			draw_attractor();
			animate = 1;
		}
		else {
			num = 0;
			animate = 0;
		}
	}
	
	// Tell GLUT it is necessary to redisplay the scene
	glutPostRedisplay();
}

// Handles arrow presses to change the view angle
// Mostly taken from example 6 in course material
void special(int key, int x, int y)
{
	// Right arrow key - increase azimuth by 5 degrees
	if (key == GLUT_KEY_RIGHT){
		th += 5;
	}
	else if(key == GLUT_KEY_LEFT) {
		th -= 5;
	}
	else if(key == GLUT_KEY_UP) {
		ph += 5;
	}
	else if (key == GLUT_KEY_DOWN) {
		ph -= 5;
	}
	// Keep angles bounded by 360 in magnitude
	th %= 360;
	ph %= 360;
	// Tell GLUT to update the screen
	glutPostRedisplay();
}

/* Cleanly handle window resizing
 * Taken from example 6 in course material
 ****/

void reshape(int width, int height)
{
	// Ratio of width to height of the window
	double w2h = (height>0) ? (double)width/height : 1;
	// Set the viewport to the entire window
	glViewport(0, 0, width, height);
	// Tell OpenGL we want to manipulate the projection matrix
	glMatrixMode(GL_PROJECTION);
	// Load the identity matrix
	glLoadIdentity();
	// Orthogonal projection box adjusted for
	// aspect ratio of the window
	glOrtho(-dim*w2h, +dim*w2h, -dim, +dim, -dim, +dim);
	// Switch back to manipulating the model matrix
	glMatrixMode(GL_MODELVIEW);
	// Re-load the identity matrix
	glLoadIdentity();
}
	
/*
 * Use GLUT to display
 */
int main(int argc, char* argv[])
{
   // Initialize GLUT
   glutInit(&argc,argv);
   // Request a double buffered, true color window
   glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
   // Request a specific window size
   glutInitWindowSize(800, 800);
   // Create the window
   glutCreateWindow("Homework 1");
   // Register function used to display scene
   glutDisplayFunc(display);
   // Tell GLUT to handle window resizing with our function
   glutReshapeFunc(reshape);
   // Tell GLUT to handle user input from keys
   glutKeyboardFunc(key);
   // Tell GLUT to handle input from special keys
   glutSpecialFunc(special);
   // Pass control to GLUT for events
   glutMainLoop();
   // Return to OS
   return 0;
}
