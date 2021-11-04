#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#ifdef __APPLE__
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#  include <GLUT/glut.h>
#else
#  include <GL/gl.h>
#  include <GL/glu.h>
#  include <GL/freeglut.h>
#endif

//Naming variables and initialization
using namespace std;

const int MaxParticles = 500;	
int currentParticles = 200;			
double mouseX;						
double mouseY;
double sizeChange=1.0;				
double speedChange=5.0;
int range=5000;						
int pullPush;
int pause = 1;
int Display_Width = 600;
int Display_Height = 600;
int Display_Offset_Width;
int Display_Offset_Height;


typedef struct{
	double XCoor;
	double YCoor;
	double dX;
	double dY;
	double Red;
	double Green;
	double Blue;
	double size;
	bool affected;
}PARTICLE;

PARTICLE Particles[MaxParticles];

//initialized parameters
void initParticles(void){
	int i;
	//Set random variables for all parameters
	for(i=0; i<currentParticles; i++){			
		Particles[i].XCoor = rand()%600*1.0;
		Particles[i].YCoor = rand()%600*1.0;
		Particles[i].Blue = rand()%2;
		Particles[i].Red = rand()%2;
		Particles[i].Green = rand()%2;
		Particles[i].dX = (rand()%10*0.006)* pow(-1.0,rand()%2);
		Particles[i].dY = (rand()%10*0.006)* pow(-1.0,rand()%2);
		Particles[i].size = 5;
		Particles[i].affected = false;
	}
	pause = 1;				
}
//Display function 
void display(void){
	//when the program is paused dispaly the instruction page
	if(pause == 1){
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(0, 0, 0, 0);
		glRasterPos2f(150.0, 500.0);	
		glColor3f(1.0,1.0,1.0);
		string str1("PAUSED Press space to start");
		for (int i = 0; i < str1.size(); i++){
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,str1[i]);
		}
		glRasterPos2f(240.0, 450.0);
		string str2("Instructions");
		for (int i = 0; i < str2.size(); i++){
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,str2[i]);
		}
		glRasterPos2f(150.0, 400.0);
		string str3("Press w or s to Increase/Decrease Speed");
		for (int i = 0; i < str3.size(); i++){
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,str3[i]);
		}
		glRasterPos2f(150.0, 350.0);
		string str4("Press a or d to Add/Delete a Particle");
		for (int i = 0; i < str4.size(); i++){
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,str4[i]);
		}
		glRasterPos2f(150.0, 300.0);
		string str5("Press 1-5 to adjust Size");
		for (int i = 0; i < str5.size(); i++){
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,str5[i]);
		}
		glRasterPos2f(150.0, 250.0);
		string str6("Press + or - for Increase/Decrease Range");
		for (int i = 0; i < str6.size(); i++){
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,str6[i]);
		}
		glRasterPos2f(150.0, 200.0);
		string str7("Left Click for attracting particles");
		for (int i = 0; i < str7.size(); i++){
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,str7[i]);
		}
		glRasterPos2f(150.0, 150.0);
		string str8("Right Click for deflecting particles");
		for (int i = 0; i < str8.size(); i++){
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,str8[i]);
		}
		glRasterPos2f(150.0, 100.0);
		string str9("R or r to Reset");
		for (int i = 0; i < str9.size(); i++){
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,str9[i]);
		}
		glRasterPos2f(150.0, 50.0);
		string str10("press q, Q or ESC to Quit");
		for (int i = 0; i < str10.size(); i++){
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,str10[i]);
		}
	}
	else if(pause==0){
	
	int i;						
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(0, 0, 0, 0);
	glRasterPos2f(0, 590.0);
	string str11("Press space to pause");
	for (int j = 0; j < str11.size(); j++){
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12,str11[j]);
	}

	for(i=0;i<currentParticles;i++){

		//Logic for moving the particle
		Particles[i].XCoor += Particles[i].dX*speedChange;	
		Particles[i].YCoor += Particles[i].dY*speedChange;
		
		//Logic for bouncing off the wall
		if((Particles[i].XCoor <= 1) | (Particles[i].XCoor >= 599)){ 
			Particles[i].dX = Particles[i].dX*(1);
		}
		if((Particles[i].YCoor <= 1) | (Particles[i].YCoor >= 599)){
			Particles[i].dY = Particles[i].dY*(-1);
		}

		/*mouseX/Y is set to the mouse location in x and y and used here, it is then reversed to mouseX\Y = (-1, -1)
		for no interference, this is used when the LMB is pressed and the particle stops at the cursor instead of keep going*/
		if(Particles[i].XCoor + 1>= mouseX & Particles[i].XCoor -1 <= mouseX & Particles[i].YCoor + 1>= mouseY & Particles[i].YCoor -1 <= mouseY){
			Particles[i].dX = 0;
			Particles[i].dY = 0;
		}
        
        for(int j = 0; j<= currentParticles; j++)
        {
            if((Particles[i].YCoor > 601)||(Particles[i].YCoor < -1)){
                
                Particles[i].YCoor = 599;
            }
            if(Particles[i].XCoor < 1){
                
                Particles[i].XCoor = 599;
                
                
            }else if(Particles[i].XCoor > 599){
                Particles[i].XCoor = 1;
                
            }
        }
		if(Particles[i].affected == true){		
			double temp1;						
			double temp2;
			//pullPush == 0 is pull, 1 is for push
			if(pullPush == 0){					
				int i;
				for(i=0;i<currentParticles; i++){				
					temp1 = Particles[i].XCoor - mouseX;
					temp2 = (Particles[i].YCoor) - mouseY;
			
					if(pow(temp1,2) + pow(temp2,2) <= range){	
						Particles[i].dX = -temp1*0.0015;		
						Particles[i].dY = -temp2*0.0015;
					}
				}
			}
			if(pullPush == 1){
				int i;
		
				for(i=0;i<currentParticles; i++){
					temp1 = Particles[i].XCoor - mouseX;
					temp2 = Particles[i].YCoor - mouseY;
			
					if(pow(temp1,2) + pow(temp2,2) <= range){
						Particles[i].dX = temp1*0.0025;
						Particles[i].dY = temp2*0.0025;
					}
				}
			}
		}
		glPointSize(Particles[i].size*sizeChange);
		glBegin(GL_POINTS);							
		glColor3f(Particles[i].Red,Particles[i].Green,Particles[i].Blue);
		glVertex2f(Particles[i].XCoor, Particles[i].YCoor);
		glRasterPos2f(400.0, 400.0);
		glEnd();
		glColor3f(0.8,1.0,0.8);
	}
	}
	
	glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y){
	y = Display_Height - y;

	switch (key){
		//exit
		case 'q':
		case 'Q':
		case 27:

			exit(0);
			break;
		//press space bar to pause
		case ' ':
			if (pause == 1){
				pause = 0;
			}	
			else if(pause == 0){
				pause = 1;
			}
			break;
		
		//press w or Wto increase the speed of particle movement
		case 'w':
		case 'W':
			speedChange += 1;
			if(speedChange >=500){
				speedChange = 499;
			}
			break;
		//press s or S to decrease the speed of particle movement
		case 'S':
		case 's':
			speedChange -= 1;
			if(speedChange <=-500){
				speedChange = -499;
			}
			break;
		//Press r or R to reset
		case 'R':
		case 'r':
			currentParticles = 50;
			initParticles();
			break;
		//Press + to increase the range of affect
		case '+':
			range += 500;
			if(range >=30500){
				range = 30000;
			}
			break;
		//Press - to decrease the range of affect
		case '-':
			range -= 500;
			if(range <=6000){
				range = 6500;
			}
			break;
		//Press 1-5 to adjust the size of particles
		case '1':
			sizeChange = 1.2;
			break;
		case '2':
			sizeChange = 1.4;
			break;
		case '3':
			sizeChange = 1.8;
			break;
		case '4':
			sizeChange = 2;
			break;
		case '5':
			sizeChange = 2.2;
			break;
		
		//press a or A to generate particle at current cusor position
		case 'a':
		case 'A':
			glBegin(GL_POINTS);			
			glColor3f(Particles[currentParticles].Red = rand()%2,Particles[currentParticles].Green = rand()%2,Particles[currentParticles].Blue = rand()%2);
			Particles[currentParticles].dX = (rand()%10*0.01)* pow(-1.0,rand()%2)*pause;
			Particles[currentParticles].dY = (rand()%10*0.01)* pow(-1.0,rand()%2)*pause;
			Particles[currentParticles].size = rand()%3 + 5;
			//Generate particle at current cusor postion
			glVertex2i(Particles[currentParticles].XCoor = x, Particles[currentParticles].YCoor = y);	
			glEnd();
			currentParticles += 1;
			//In case the current particles exceed the max of particles		
			if(currentParticles > MaxParticles){	
			currentParticles = MaxParticles;
			}
			break;


		//Press d or D to delete particle
		case 'd':
		case 'D':
			int i;
			double temp1;
			double temp2;
			double temp3=1000;		//Temp3 set to 1000 as buffer
			int slot = 0;
			/*This loop goes through all current particles, calculates distance between particle and cursor, then it compares the result
			with the previous particles result, if the distance is lower, then record its slot number in slot*/
			for(i=0;i<currentParticles; i++){
				temp1 = Particles[i].XCoor - x;
				temp2 = Particles[i].YCoor - y;
				if((fabs(temp1) + fabs(temp2)) < temp3){
					slot = i;
					temp3 = fabs(temp1) + fabs(temp2);
				}
			}
			for(i = slot; i <currentParticles;i++){		//Starting from the particle assigned to Particles[slot], bring one element forward,
				Particles[i] = Particles[i+1];			//rewriting the one we wish to delete and reorganizing the array
			}
			currentParticles -= 1;		//Decrease array by one and check lower limit
			if(currentParticles < 0){
				currentParticles = 0;
			}
			break;
	

	}
		
}


void idle(void){
	glutPostRedisplay();
}

//Mouse function
void mouse(int btn, int state, int x, int y){

	y = Display_Height - y;
	//Logic for attracting particles by left mouse button
	if(btn == GLUT_LEFT_BUTTON && state==GLUT_DOWN)
	{
		pullPush = 0;
		mouseX = x;
		mouseY = y;
		int i;
		double temp1;
		double temp2;
		for(i=0;i<currentParticles; i++){
			temp1 = Particles[i].XCoor - x;
			temp2 = Particles[i].YCoor - y;
			
			if(pow(temp1,2) + pow(temp2,2) <= range){
				Particles[i].dX = -temp1*0.0015*pause;
				Particles[i].dY = -temp2*0.0015*pause;
				Particles[i].affected = true;
			}
			
		}
		
	}

	if(btn == GLUT_LEFT_BUTTON && state==GLUT_UP)
	{
		mouseX = -1;
		mouseY = -1;
		int i;
		for(i=0;i<currentParticles; i++){
			if(Particles[i].affected == true){
			Particles[i].affected = false;
			}
		}
	}
	
	//Logic for deflecting particles by right mouse button
	if(btn == GLUT_RIGHT_BUTTON && state==GLUT_DOWN)
	{

		pullPush = 1;
		mouseX = x;
		mouseY = y;
		int i;
		double temp1;
		double temp2;
		
		for(i=0;i<currentParticles; i++){
			temp1 = Particles[i].XCoor - x;
			temp2 = Particles[i].YCoor - y;
			
			if(pow(temp1,2) + pow(temp2,2) <= range){
				Particles[i].dX = temp1*0.00125*pause;
				Particles[i].dY = temp2*0.00125*pause;
				Particles[i].affected = true;
			}
		}
		
	}
	//When the right button is released
	if(btn == GLUT_RIGHT_BUTTON && state==GLUT_UP)
	{
		mouseX = -1;
		mouseY = -1;
		int i;
		for(i=0;i<currentParticles; i++){
			if(Particles[i].affected == true){
			Particles[i].affected = false;
			}
		}
	}
}
//Motion function of particles
void motion(int x, int y)
{
	y = Display_Height - y;
	mouseX = x;
	mouseY = y;
	int i;
	double temp1;
	double temp2;
	//Pull particles towards mouse when mouse in motion if particle in range
	if(pullPush == 0){		
		
		for(i=0;i<currentParticles; i++){
			temp1 = Particles[i].XCoor - x;
			temp2 = Particles[i].YCoor - y;
			
			if(pow(temp1,2) + pow(temp2,2) <= range){
				Particles[i].dX = -temp1*0.0015*pause;
				Particles[i].dY = -temp2*0.0015*pause;
				Particles[i].affected = true;
			}
		}
	}
	//Push particles away from mouse if in range, then it is affected
	if(pullPush == 1){		
		for(i=0;i<currentParticles; i++){
			temp1 = Particles[i].XCoor - x;
			temp2 = Particles[i].YCoor - y;
			
			if(pow(temp1,2) + pow(temp2,2) <= range){
				Particles[i].dX = temp1*0.0015*pause;
				Particles[i].dY = temp2*0.0015*pause;
				Particles[i].affected = true;
			}
		}
	}
}

void reshape(int w, int h)
{
	// Update global constants for window dimensions
	Display_Width = w;
	Display_Height = h;

	// Reset pixel matrix with new dimensions
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, Display_Width, 0, Display_Height);

	// Reconstruct view of 2D projection
	glMatrixMode(GL_MODELVIEW);
	glViewport(0, 0, Display_Width, Display_Height);
}

//Main function
int main(int argc, char** argv)
{	
	
	glutInit(&argc, argv);
	
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(Display_Width, Display_Height);
	glutInitWindowPosition(Display_Offset_Width,Display_Offset_Height);
	initParticles();
	glutCreateWindow("Particle System");
	glutInitDisplayMode(GLUT_DOUBLE);
	glutDisplayFunc(display);		
	glutKeyboardFunc(keyboard);
	glutMotionFunc(motion);
	glutMouseFunc(mouse);
	glutReshapeWindow(600, 600);
	gluOrtho2D(0, 600, 0, 600);
	glutIdleFunc(idle);
	glutMainLoop();	

	return(0);					
}