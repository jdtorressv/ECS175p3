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

//global variables

/* vArr and lArr are vectors of vectors of the vertices and lines for each 
 * polygon build from the input file and are referenced throughout. */
vector<vector<double>> vArr;
vector<vector<double>> lArr;

int windowID, windowXY, windowXZ, windowYZ; 
char* fileName;  
bool rotate = false; 
float rx1, ry1, rz1, rx2, ry2, rz2; //Used in Axis of Rotation Definition  


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
		
		for (int i = 0; i < lArr.size(); i++) { //For each polyhedron
			for (int j = 1; j < lArr.at(i).size(); j = j+2) { //For each each line in the polyhedron 
				//First vArr entry appears as [6,0,100,200,200,100,200,0,300,200,200,300,200,100,200,400,100,200,0]
				//First lArr entry appears as [12,1,2,1,3,2,4,3,4,1,5,2,5,3,5,4,5,1,6,2,6,3,6,4,6]
				float x1 = vArr.at(i).at((lArr.at(i).at(j) - 1)*3+1);    //X of first point
                        	float y1 = vArr.at(i).at((lArr.at(i).at(j) - 1)*3+2);    //Y of first point

                        	float x2 = vArr.at(i).at((lArr.at(i).at(j+1) - 1)*3+1);   //X of second point
                        	float y2 = vArr.at(i).at((lArr.at(i).at(j+1) - 1)*3+2);   //Y of second point

				glColor3f(1.0, 1.0, 1.0); 
				glVertex2f(x1, y1);
				glVertex2f(x2, y2); 
			}
		}

		//If called by rotate function, draw the rotation of axis as well 
                if (rotate) {
                        glColor3f(1.0, 1.0, 1.0);
                        glVertex2f(rx1, ry1);
                        glVertex2f(rx2, ry2);
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
		 
        	for (int i = 0; i < lArr.size(); i++) { //For each polyhedron
                	for (int j = 1; j < lArr.at(i).size(); j = j+2) { //For each each line in the polyhedron 
                        	//First vArr entry appears as [6,0,100,200,200,100,200,0,300,200,200,300,200,100,200,400,100,200,0]
                        	//First lArr entry appears as [12,1,2,1,3,2,4,3,4,1,5,2,5,3,5,4,5,1,6,2,6,3,6,4,6]
                        	float x1 = vArr.at(i).at((lArr.at(i).at(j) - 1)*3+1);    //X of first point
                        	float z1 = vArr.at(i).at((lArr.at(i).at(j) - 1)*3+3);    //Z of first point

                        	float x2 = vArr.at(i).at((lArr.at(i).at(j+1) - 1)*3+1);   //X of second point
                        	float z2 = vArr.at(i).at((lArr.at(i).at(j+1) - 1)*3+3);   //Z of second point

                                glColor3f(1.0, 1.0, 1.0);
                                glVertex2f(x1, z1);
                                glVertex2f(x2, z2);
                	}
        	}

                //If called by rotate function, draw the rotation of axis as well 
                if (rotate) {
                        glColor3f(1.0, 1.0, 1.0);
                        glVertex2f(rx1, rz1);
                        glVertex2f(rx2, rz2);
                }

	glEnd(); 
	glFlush(); 
}
//Draws the lines as specified by the lines via vertex pairs in the input file; XZ: All Y values are ignored
void drawSceneYZ()
{
        glClear(GL_COLOR_BUFFER_BIT);
        glLoadIdentity();
	glBegin(GL_LINES);
		//YZ: All X values are ignored
        	for (int i = 0; i < lArr.size(); i++) { //For each polyhedron
                	for (int j = 1; j < lArr.at(i).size(); j = j+2) { //For each each line in the polyhedron 
				//First vArr entry appears as [6,0,100,200,200,100,200,0,300,200,200,300,200,100,200,400,100,200,0]
                        	//First lArr entry appears as [12,1,2,1,3,2,4,3,4,1,5,2,5,3,5,4,5,1,6,2,6,3,6,4,6]
                        	float y1 = vArr.at(i).at((lArr.at(i).at(j) - 1)*3+2);    //Y of first point
                        	float z1 = vArr.at(i).at((lArr.at(i).at(j) - 1)*3+3);    //Z of first point

                        	float y2 = vArr.at(i).at((lArr.at(i).at(j+1) - 1)*3+2);   //Y of second point
                        	float z2 = vArr.at(i).at((lArr.at(i).at(j+1) - 1)*3+3);   //Z of second point
			
				glColor3f(1.0, 1.0, 1.0);
                                glVertex2f(y1, z1);
                                glVertex2f(y2, z2);
                	}
        	}
		
                //If called by rotate function, draw the rotation of axis as well 
		if (rotate) {
                        glColor3f(1.0, 1.0, 1.0);
			glVertex2f(ry1, rz1); 
			glVertex2f(ry2, rz2);
		        rotate = false; //YZ scene drawn last; if end of rotate call, reset rotate flag
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
        switch (pid)
        {
                case 0: cout << "Present vertices of the octahedron or polhedron 0:\n";
                        break;
                case 1: cout << "Present vertices of the tetrahedron or polyhedron 1:\n";
                        break;
                case 2: cout << "Present vertices of the hexahedron or polyhedron 2:\n";
                        break;
        }
        for (int i = 1; i < vArr.at(pid).size(); i+=3)
                cout << "(" << vArr.at(pid).at(i)*NORM << ", " << vArr.at(pid).at(i+1)*NORM << ", " << vArr.at(pid).at(i+2)*NORM << ")\n";

}
//Translation 
void translateMenu(int pid)
{
        float x, y, z;
        int vertices = vArr.at(pid).at(0);
	cout << "Number of vertices is " << vertices << endl; 
        cout << "Please enter the x, y, and z translation values:\n";
        cin >> x >> y >> z; 
	//Norm is used to normalize world coordinates 
	cout << "After normalization, you entered " << x/NORM << ", " << y/NORM << ", " << z/NORM << endl; 
        for (int i = 0; i < vertices; i++) {
                vArr.at(pid).at(1+3*i) += x/NORM;
                vArr.at(pid).at(2+3*i) += y/NORM;
		vArr.at(pid).at(3+3*i) += z/NORM;
	       	
        }

	glutPostRedisplay(); 
 	
	//Write changes back to file 
	ofstream file;
        file.open(fileName, std::ofstream::out | std::ofstream::trunc);
        if (!file) {
                cerr << "Unable to open file\n";
                exit(ERROR);   // call system to stop
        }

        file << vArr.size() << '\n'; 
        for (int i = 0; i < vArr.size(); i++) {
		file << '\n' << vArr.at(i).at(0) << '\n';  
                for (int j = 1; j < vArr.at(i).size(); j+=3)
                        file << vArr.at(i).at(j)*NORM << " " << vArr.at(i).at(j+1)*NORM << " " << vArr.at(i).at(j+2)*NORM << '\n';
		file << lArr.at(i).at(0) << '\n'; 
		for (int k = 1; k < lArr.at(i).size(); k+=2)
			file << lArr.at(i).at(k) << " " << lArr.at(i).at(k+1) << '\n'; 	
        }
}
//Scaling
void scaleMenu(int pid) 
{
        double scale;
        int vertices = vArr.at(pid).at(0);
        double xSum = 0;
        double ySum = 0;
	double zSum = 0; 
        double centX, centY, centZ;

        cout << "Please enter the magnitude you'd like to scale by:\n";
        cin >> scale;

	// Calculate centroids 
        for (int i = 0; i < vertices; i++) 
                xSum += vArr.at(pid).at(1+i*3);
        
        for (int i = 0; i < vertices; i++) 
                ySum += vArr.at(pid).at(2+i*3);
        
	for (int i = 0; i < vertices; i++) 
		zSum += vArr.at(pid).at(3+i*3);

	centX = xSum / (double)vertices;
	centY = ySum / (double)vertices;
	centZ = zSum / (double)vertices; 
	
        for (int i = 0; i < vertices; i++) {
                vArr.at(pid).at(1+i*3) = scale*(vArr.at(pid).at(1+i*3) - centX) + centX;
                vArr.at(pid).at(2+i*3) = scale*(vArr.at(pid).at(2+i*3) - centY) + centY;
		vArr.at(pid).at(3+i*3) = scale*(vArr.at(pid).at(3+i*3) - centZ) + centZ; 
        }

        glutPostRedisplay();

	//Write changes back to file 
        ofstream file;
        file.open(fileName, std::ofstream::out | std::ofstream::trunc);
        if (!file) {
                cerr << "Unable to open file\n";
                exit(ERROR);   // call system to stop
        }

        file << vArr.size() << '\n';
        for (int i = 0; i < vArr.size(); i++) {
                file << '\n' << vArr.at(i).at(0) << '\n';
                for (int j = 1; j < vArr.at(i).size(); j+=3)
                        file << vArr.at(i).at(j)*NORM << " " << vArr.at(i).at(j+1)*NORM << " " << vArr.at(i).at(j+2)*NORM << '\n';
                file << lArr.at(i).at(0) << '\n';
                for (int k = 1; k < lArr.at(i).size(); k+=2)
                        file << lArr.at(i).at(k) << " " << lArr.at(i).at(k+1) << '\n';
        }
}
void rotateMenu(int pid)
{
        float alpha; 
	rotate = true; //Set rotate flag to true 
        int vertices = vArr.at(pid).at(0);
	cout << "Please enter the x1, y1, z1, x2, y2, and z2 values to define an axis of rotation, followed by the angle of rotation in radians\n"; 
	cin >> rx1 >> ry1 >> rz1 >> rx2 >> ry2 >> rz2 >> alpha; 
	rx1 /= NORM;
	ry1 /= NORM;
	rz1 /= NORM;
	rx2 /= NORM;
	ry2 /= NORM;
        rz2 /= NORM; 	
	float mag = sqrt(pow((rx2 - rx1), 2) + pow((ry2 -ry1), 2) + pow((rz2 - rz1), 2)); 
	float a = (rx2 - rx1) / mag; 
	float b = (ry2 -ry1) / mag; 
	float c = (rz2 - rz1) / mag; 
	float d = sqrt(pow(b, 2) + pow(c, 2));
	
	// Phases are as outlined in the text, i.e. R(theta) = T^-1 * Rx^-1(alpha) * Ry^-1(beta) * Rz(theta) * Ry(beta) * Rx(alpha) * T
	for (int i = 0; i < vertices; i++) {
		float oldX, oldY, oldZ, tempX, tempY, tempZ; 
		//Phase 1/7
		oldX = vArr.at(pid).at(1+i*3) - rx1; 
		oldY = vArr.at(pid).at(2+i*3) - ry1; 
		oldZ = vArr.at(pid).at(3+i*3) - rz1; 

		//Phase 2/7
		tempX = oldX; 
		tempY = (c/d)*oldY + (-1*b/d)*oldZ;
		tempZ = (b/d)*oldY + (c/d)*oldZ; 

		//Phase 3/7
		oldX = d*tempX - a*tempZ; 
		oldY = tempY; 
		oldZ = a*tempX + d*tempZ; 

		//Phase 4/7
		tempX = oldX*cos(alpha) - oldY*sin(alpha);
		tempY = oldX*sin(alpha) + oldY*cos(alpha);
		tempZ = oldZ; 

		//Phase 5/7
		oldX = d*tempX + a*tempZ; 
		oldY = tempY;
		oldZ = d*tempZ - a*tempX;

		//Phase 6/7
		tempX = oldX;
		tempY = (c/d)*oldY + (b/d)*oldZ; 
		tempZ = (c/d)*oldZ - (b/d)*oldY; 

		//Phase 7/7
                vArr.at(pid).at(1+i*3) = tempX + rx1; 
                vArr.at(pid).at(2+i*3) = tempY + ry1;
                vArr.at(pid).at(3+i*3) = tempZ + rz1; 
	}
	
        glutPostRedisplay();

	//Write changes back to file 
        ofstream file;
        file.open(fileName, std::ofstream::out | std::ofstream::trunc);
        if (!file) {
                cerr << "Unable to open file\n";
                exit(ERROR);   // call system to stop
        }

        file << vArr.size() << '\n';
        for (int i = 0; i < vArr.size(); i++) {
                file << '\n' << vArr.at(i).at(0) << '\n';
                for (int j = 1; j < vArr.at(i).size(); j+=3)
                        file << vArr.at(i).at(j)*NORM << " " << vArr.at(i).at(j+1)*NORM << " " << vArr.at(i).at(j+2)*NORM << '\n';
                file << lArr.at(i).at(0) << '\n';
                for (int k = 1; k < lArr.at(i).size(); k+=2)
                        file << lArr.at(i).at(k) << " " << lArr.at(i).at(k+1) << '\n';
        }
}

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

	auto vpoint = v.begin(); 
	int polyTotal = (int)*vpoint;
	
	for (int i = 0; i < polyTotal; i++) {
        	vector<double> Va;
		vector<double> Vb; 
                vArr.push_back(Va);
		lArr.push_back(Vb); 
                int vertices = (int)*(++vpoint);
                vArr.at(i).push_back(vertices);
                for (int j = 0; j < vertices; j++) {
                        vArr.at(i).push_back(*(++vpoint) / NORM);
                        vArr.at(i).push_back(*(++vpoint) / NORM);
			vArr.at(i).push_back(*(++vpoint) / NORM); 
                }
		int lines = (int)*(++vpoint); 
		lArr.at(i).push_back(lines);
	       	for (int k = 0; k < lines; k++) {
			lArr.at(i).push_back(*(++vpoint));
			lArr.at(i).push_back(*(++vpoint));
		}
        }

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
