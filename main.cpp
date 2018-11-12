#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <cmath>
using namespace std;

#define ERROR 1 
#define NORM 500.0 


/*****************************************************************************/
/* Classes                            		                             */
/*****************************************************************************/
 
class Vertex {
	public:
		double x;   
		double y;  
		double z; 
	        double normX;
		double normY; 
		double normZ;
		Vertex(double x, double y, double z, double normX, double normY, double normZ); 	
};

class Triangle {
	public: 
		int vertexA;
		int vertexB; 
		int vertexC;
		Triangle(int vertexA, int vertexB, int vertexC); 
};

class Normal {
        public:
                int VID;
                double xVal;
                double yVal;
                double zVal;
		Normal(int VID, double xVal, double yVal, double zVal); 
};

class Polyhedron {
	public: 
		vector<Vertex> vArr; 
		vector<Triangle> triArr; 
		vector<Normal> normArr; 
};	

/*****************************************************************************/
/* Globals                                                                   */
/*****************************************************************************/

int windowID, windowXY, windowXZ, windowYZ;
char* fileName;
vector<Polyhedron> polyArr; 

/*****************************************************************************/
/* Function Definitions                                                      */
/*****************************************************************************/

//Constructors
Vertex::Vertex(double x, double y, double z, double normX, double normY, double normZ)
{
	this->x = x; 
	this->y = y;
	this->z = z; 
	this->normX = normX;
	this->normY = normY; 
	this->normZ = normZ; 
}

Triangle::Triangle(int vertexA, int vertexB, int vertexC)
{
	this->vertexA = vertexA;
	this->vertexB = vertexB; 
	this->vertexC = vertexC; 
}
Normal::Normal(int VID, double xVal, double yVal, double zVal)
{
	this->VID = VID;
	this->xVal = xVal; 
	this->yVal = yVal;
	this->zVal = zVal; 
}
//Fill appropriate classes with input file specs
void populatePolyhedronInfo(vector<double> v)
{
        auto vpoint = v.begin();

        int polyTotal = (int)*(vpoint);

        for (int i = 0; i < polyTotal; i++) {
                Polyhedron poly;
                polyArr.push_back(poly);
                int vertices = (int)*(++vpoint);

                for (int j = 0; j < vertices; j++) {
                        double x = *(++vpoint)/NORM;
                        double y = *(++vpoint)/NORM;
                        double z = *(++vpoint)/NORM;
                        Vertex vertex(x, y, z, 0, 0, 0);
                        polyArr.at(i).vArr.push_back(vertex);
                }

                int numTri = (int)*(++vpoint);

                for (int k = 0; k < numTri; k++) {
                        int first = (int)*(++vpoint);
                        int second = (int)*(++vpoint);
                        int third = (int)*(++vpoint);
                        Triangle triangle(first, second, third);
                        polyArr.at(i).triArr.push_back(triangle);
                }
        }
}
void shiftOver(int* first, int* second, int* third) 
{
	int temp = *first; 
	*first = *second;
        *second = *third; 
	*third = temp; 	

}
void crossProduct(Polyhedron currPoly, int A, int B, int C)
{
			double Ux = currPoly.vArr.at(B).x - currPoly.vArr.at(A).x;
                        double Uy = currPoly.vArr.at(B).y - currPoly.vArr.at(A).y;
                        double Uz = currPoly.vArr.at(B).z - currPoly.vArr.at(A).z;

                        double Vx = currPoly.vArr.at(C).x - currPoly.vArr.at(B).x;
                        double Vy = currPoly.vArr.at(C).y - currPoly.vArr.at(B).y;
                        double Vz = currPoly.vArr.at(C).z - currPoly.vArr.at(B).z;

                        Vertex U(Ux, Uy, Uz, 0, 0, 0);
                        Vertex V(Vx, Vy, Vz, 0, 0, 0);
			
			double Nx = U.y*V.z - U.z*V.y;
			double Ny = U.z*V.x - U.x*V.z;
			double Nz = U.x*V.y - U.y*V.x;
			double mag = sqrt(pow(Nx, 2) + pow(Ny, 2) + pow(Nz, 2)); 

                        Normal surfNormal(B, Nx/mag, Ny/mag, Nz/mag);
                        currPoly.normArr.push_back(surfNormal);

}
void calculateNormals() 
{
	for (int i = 0; i < polyArr.size(); i++) {
		Polyhedron currPoly = polyArr.at(i);
		for (int j = 0; j < currPoly.triArr.size(); j++) {
			Triangle currTri = currPoly.triArr.at(j); 

			int A = currTri.vertexA;
			int B = currTri.vertexB; 
			int C = currTri.vertexC;
			
			crossProduct(currPoly, A, B, C);  

			shiftOver(&A, &B, &C); 
			crossProduct(currPoly, A, B, C); 
			
			shiftOver(&A, &B, &C); 
			crossProduct(currPoly, A, B, B);
		}
	}	
}
void setNormals()
{
        for (int i = 0; i < polyArr.size(); i++) {
                for (int j = 0; j < polyArr.at(i).vArr.size(); j++) {
                        int count = 0;
                        for (int k = 0; k < polyArr.at(i).normArr.size(); k++) {
                                if (j == polyArr.at(i).normArr.at(k).VID) {
                                        polyArr.at(i).vArr.at(j).normX += polyArr.at(i).normArr.at(k).xVal;
                                        polyArr.at(i).vArr.at(j).normY += polyArr.at(i).normArr.at(k).yVal;
                                        polyArr.at(i).vArr.at(j).normY += polyArr.at(i).normArr.at(k).zVal;
                                        count++;
                                }
                        }
                        polyArr.at(i).vArr.at(j).normX /= count; 
                        polyArr.at(i).vArr.at(j).normY /= count;
                        polyArr.at(i).vArr.at(j).normZ /= count; 
                }
        }
}
//Inline function for mainMenu delegates all functionality to sub menus 
inline void mainMenu(int pid) {;}

//Initializes each of the subwindoes as well as the primary window 
void init()
{	
	if (glutGetWindow() == windowID) 
		glClearColor(1.0, 1.0, 1.0, 0.0); //Set color to white
	else 
        	glClearColor(0.0, 0.0, 0.0, 0.0); //Set color to black

        glMatrixMode(GL_PROJECTION);
}

//Draws the lines as specified by the lines via vertex pairs in the input file; XY: All Z values are ignored 
void drawSceneXY()
{
	glClear(GL_COLOR_BUFFER_BIT); 
        glLoadIdentity();
	glBegin(GL_LINES);
		
		for (int i = 0; i < polyArr.size(); i++) { //For each polyhedron
			for (int j = 0; j < polyArr.at(i).triArr.size(); j++) { //For each each triangle in the polyhedron 
				int A = polyArr.at(i).triArr.at(j).vertexA;
				int B = polyArr.at(i).triArr.at(j).vertexB; 
				int C = polyArr.at(i).triArr.at(j).vertexC;
			
				
				float x1 = polyArr.at(i).vArr.at(A).x;
				float y1 = polyArr.at(i).vArr.at(A).y;

                                float x2 = polyArr.at(i).vArr.at(B).x;
                                float y2 = polyArr.at(i).vArr.at(B).y;

				float x3 = polyArr.at(i).vArr.at(C).x;
                                float y3 = polyArr.at(i).vArr.at(C).y;


				glColor3f(1.0, 1.0, 1.0); 
				glVertex2f(x1, y1);
				glVertex2f(x2, y2); 
                                glVertex2f(x2, y2);
                                glVertex2f(x3, y3);
                                glVertex2f(x3, y3);
                                glVertex2f(x1, y1);

			}
		}

	glEnd(); 
	glFlush(); 
}
//Draws the lines as specified by the lines via vertex pairs in the input file; XZ: All Y values are ignored
void drawSceneXZ()
{
        glClear(GL_COLOR_BUFFER_BIT);
        glLoadIdentity();
	glBegin(GL_LINES);
		 
                for (int i = 0; i < polyArr.size(); i++) { //For each polyhedron
                        for (int j = 0; j < polyArr.at(i).triArr.size(); j++) { //For each each triangle in the polyhedron 
                                int A = polyArr.at(i).triArr.at(j).vertexA;
                                int B = polyArr.at(i).triArr.at(j).vertexB;
                                int C = polyArr.at(i).triArr.at(j).vertexC;


                                float x1 = polyArr.at(i).vArr.at(A).x;
                                float z1 = polyArr.at(i).vArr.at(A).z;

                                float x2 = polyArr.at(i).vArr.at(B).x;
                                float z2 = polyArr.at(i).vArr.at(B).z;

                                float x3 = polyArr.at(i).vArr.at(C).x;
                                float z3 = polyArr.at(i).vArr.at(C).z;


                                glColor3f(1.0, 1.0, 1.0);
                                glVertex2f(x1, z1);
                                glVertex2f(x2, z2);
                                glVertex2f(x2, z2);
                                glVertex2f(x3, z3);
                                glVertex2f(x3, z3);
                                glVertex2f(x1, z1);

                        }
                }


	glEnd(); 
	glFlush(); 
}
//Draws the lines as specified by the lines via vertex pairs in the input file; XZ: All X values are ignored
void drawSceneYZ()
{
        glClear(GL_COLOR_BUFFER_BIT);
        glLoadIdentity();
	glBegin(GL_LINES);

                for (int i = 0; i < polyArr.size(); i++) { //For each polyhedron
                        for (int j = 0; j < polyArr.at(i).triArr.size(); j++) { //For each each triangle in the polyhedron 
                                int A = polyArr.at(i).triArr.at(j).vertexA;
                                int B = polyArr.at(i).triArr.at(j).vertexB;
                                int C = polyArr.at(i).triArr.at(j).vertexC;


                                float y1 = polyArr.at(i).vArr.at(A).y;
                                float z1 = polyArr.at(i).vArr.at(A).z;

                                float y2 = polyArr.at(i).vArr.at(B).y;
                                float z2 = polyArr.at(i).vArr.at(B).z;

                                float y3 = polyArr.at(i).vArr.at(C).y;
                                float z3 = polyArr.at(i).vArr.at(C).z;


                                glColor3f(1.0, 1.0, 1.0);
                                glVertex2f(y1, z1);
                                glVertex2f(y2, z2);
                                glVertex2f(y2, z2);
                                glVertex2f(y3, z3);
                                glVertex2f(y3, z3);
                                glVertex2f(y1, z1);

                        }
                }


	glEnd(); 
	glFlush(); 
}
//Set up main display 
void background()
{
        glClear(GL_COLOR_BUFFER_BIT);
        glLoadIdentity();
        glFlush();
}
//Master display function
void display()
{	
	glutSetWindow(windowID);
	background(); 
        glutSetWindow(windowXY);
        drawSceneXY();
        glutSetWindow(windowXZ);
        drawSceneXZ();
        glutSetWindow(windowYZ);
        drawSceneYZ();
}
//Provide current vertex information for specified polyhedron 
void vertexMenu(int pid)
{
	;
}
//Translation 
void translateMenu(int pid)
{
	glutPostRedisplay(); 
 	;	
}
//Scaling
void scaleMenu(int pid) 
{

        //glutPostRedisplay();
	;
}
void rotateMenu(int pid)
{
	
        //glutPostRedisplay();
	;

}
/*****************************************************************************/
/* Main                                                                      */
/*****************************************************************************/

int main(int argc, char** argv) 
{
	if (argc != 2) {
		cout << "Usage: p1 <input_file_name> \n";
		exit(ERROR); 
	}	

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(100, 100); 
	glutInitWindowSize(800, 800); 

	windowID = glutCreateWindow("Polyhedron Orthographic Projections: XY, XZ, YZ from left to right and top down");  
	init();

	int scale_menu, rotate_menu, translate_menu, vertex_menu; //For use in graphical menu

        vector<double> v;
        double num;
	fileName = argv[1]; 
        fstream file;
        file.open(fileName);

        if (!file) {
                cerr << "Unable to open file\n";
                exit(ERROR);
        }
        while (file >> num)
                v.push_back(num); //Initial vector for all polygons
        file.close();
	
	populatePolyhedronInfo(v); 

	calculateNormals(); 
	
	setNormals(); 

	//XY
	windowXY = glutCreateSubWindow(windowID, 25, 50, 320, 320);
	init();

	//XZ
	windowXZ = glutCreateSubWindow(windowID, 25, 450, 320, 320); 
	init(); 

	//YZ
	windowYZ = glutCreateSubWindow(windowID, 425, 450, 320, 320); 
	init();
	

	
	glutSetWindow(windowID); 

        // Offer the user opportunities to 3D transform! 
        translate_menu = glutCreateMenu(translateMenu);
                glutAddMenuEntry("Octahedron/Polyhedron 0", 0);
                glutAddMenuEntry("Tetrahedron/Polyhedron 1", 1);
                glutAddMenuEntry("Hexahedron/Polyhedron 2", 2);

        scale_menu = glutCreateMenu(scaleMenu);
                glutAddMenuEntry("Octahedron/Polyhedron 0", 0);
                glutAddMenuEntry("Tetrahedron/Polyhedron 1", 1);
                glutAddMenuEntry("Hexahedron/Polyhedron 2", 2);

        rotate_menu = glutCreateMenu(rotateMenu);
                glutAddMenuEntry("Octahedron/Polyhedron 0", 0);      
      		glutAddMenuEntry("Tetrahedron/Polyhedron 1", 1);
                glutAddMenuEntry("Hexahedron/Polyhedron 2", 2);
	
	vertex_menu = glutCreateMenu(vertexMenu);
                glutAddMenuEntry("Octahedron/Polyhedron 0", 0);
                glutAddMenuEntry("Tetrahedron/Polyhedron 1", 1);
                glutAddMenuEntry("Hexahedron/Polyhedron 2", 2);

        glutCreateMenu(mainMenu);
                glutAddSubMenu("Translate", translate_menu);
                glutAddSubMenu("Scale", scale_menu);
                glutAddSubMenu("Rotate", rotate_menu);
		glutAddSubMenu("Vertex Dump", vertex_menu); 
        glutAttachMenu(GLUT_RIGHT_BUTTON);


        glutDisplayFunc(display);


	glutMainLoop(); 

	return 0; 
}
