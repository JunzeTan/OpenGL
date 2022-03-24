/*
Student: Eduardo Soto
Student Number: 1133693
Software Engineering 3GC3
Assignment 1
Particle System
October 9th, 2013
*/

#include <stdlib.h>
#include <stdio.h>
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

using namespace std;

const int MaxParticles = 200;		//Constant for Particles array maximum
int currentParticles = 50;			//Integer of current particles on screen

double mouseX;						//Mouse x and y coordinate
double mouseY;
double sizeChange=1.0;				//Size and Speed change variable
double speedChange=30.0;
int range=8000;						//Pull Push range
int pullPush;
int pause = 1;

typedef struct
{
double XCoor;
double YCoor;
double dX;				//dX and dY = velocity/direction
double dY;
double Red;
double Green;
double Blue;
double size;
bool affected;			//if the particle has been affected by pull or push
}PARTICLE;

PARTICLE Particles[MaxParticles];

void initParticles(void){
	int i;
	for(i=0; i<currentParticles; i++){			//for each particle we set random variables for all parameters
		Particles[i].XCoor = rand()%600*1.0;
		Particles[i].YCoor = rand()%600*1.0;
		Particles[i].Blue = rand()%2;
		Particles[i].Red = rand()%2;
		Particles[i].Green = rand()%2;
		Particles[i].dX = (rand()%10*0.006)* pow(-1.0,rand()%2);
		Particles[i].dY = (rand()%10*0.006)* pow(-1.0,rand()%2);
		Particles[i].size = (rand()%12 + 5);
		Particles[i].affected = false;
	}
	pause = 1;				//set pause to 1 in case of restart at pause
}

void display(void){
	if(pause == 0){
		glRasterPos2f(250.0, 500.0);			//When paused, write the following at the given coordinates with glRasterPos
		glColor3f(0.8,1.0,0.8);

		string str1("PAUSED");
		for (int i = 0; i < str1.size(); i++){
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,str1[i]);	//Write each character of the string as bitmapCharacter
		}
		glRasterPos2f(150.0, 400.0);
		string str2("Controls");
		for (int i = 0; i < str2.size(); i++){
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,str2[i]);
		}
		glRasterPos2f(200.0, 350.0);
		string str3("q-e = Increase/Decrease Speed");
		for (int i = 0; i < str3.size(); i++){
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,str3[i]);
		}
		glRasterPos2f(200.0, 300.0);
		string str4("a-d = Add/Delete Particle");
		for (int i = 0; i < str4.size(); i++){
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,str4[i]);
		}
		glRasterPos2f(200.0, 250.0);
		string str5("z-c Increase/Decrease Size");
		for (int i = 0; i < str5.size(); i++){
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,str5[i]);
		}
		glRasterPos2f(200.0, 200.0);
		string str6("w-x Increase/Decrease Range");
		for (int i = 0; i < str6.size(); i++){
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,str6[i]);
		}
		glRasterPos2f(200.0, 150.0);
		string str7("Left Click = Pull");
		for (int i = 0; i < str7.size(); i++){
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,str7[i]);
		}
		glRasterPos2f(200.0, 100.0);
		string str8("Right Click = Push");
		for (int i = 0; i < str8.size(); i++){
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,str8[i]);
		}
		glRasterPos2f(500.0, 50.0);
		string str9("Alt + Q = Quit");
		for (int i = 0; i < str9.size(); i++){
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12,str9[i]);
		}
		glRasterPos2f(500.0, 75.0);
		string str10("R = Reset");
		for (int i = 0; i < str10.size(); i++){
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12,str10[i]);
		}
	}
	else if(pause==1){
	
	int i;							//if not paused, run display function normally
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(0.15f, 0.15f, 0.3f, 0.0f);
	glRasterPos2f(535.0, 5.0);
	string str11("P - Pause");
	for (int j = 0; j < str11.size(); j++){
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12,str11[j]);
	}
	for(i=0;i<currentParticles;i++){

		Particles[i].XCoor += Particles[i].dX*speedChange;	//Logic for moving the particle
		Particles[i].YCoor += Particles[i].dY*speedChange;
		
		if((Particles[i].XCoor <= 1) | (Particles[i].XCoor >= 599)){ //Logic for bouncing off the wall
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
		if(Particles[i].affected == true){		//If the particle is affected by pull or push, make it move properly
			double temp1;						//temp variables for performing calculations
			double temp2;
			if(pullPush == 0){					//pullPush == 0 is pull, 1 is for push
				int i;
				for(i=0;i<currentParticles; i++){				//Calculate relative distance in both x and y between each particle and mouse
					temp1 = Particles[i].XCoor - mouseX;
					temp2 = (Particles[i].YCoor) - mouseY;
			
					if(pow(temp1,2) + pow(temp2,2) <= range){	//Relate the distance in between with the range
						Particles[i].dX = -temp1*0.0015;		//Change direction of particle
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
						Particles[i].dX = temp1*0.0015;
						Particles[i].dY = temp2*0.0015;
					}
				}
			}
		}
		glPointSize(Particles[i].size*sizeChange);	//Set size
		glBegin(GL_POINTS);							//Draw particle with openGL
		glColor3f(Particles[i].Red,Particles[i].Green,Particles[i].Blue);
		glVertex2f(Particles[i].XCoor, Particles[i].YCoor);
		glRasterPos2f(400.0, 400.0);
		glEnd();
		glColor3f(0.8,1.0,0.8);
	}
	}
	
	glutSwapBuffers();
}

void kbd(unsigned char key, int x, int y){
	y = 600 - y;		//600 - y = fixed coordinate system
	
	//Keyboard logic for all keys used
	if(glutGetModifiers() == GLUT_ACTIVE_ALT && (key == 'q' || key == 'Q') )
	{
		exit(0);	//Exit Program
	}

	if(key == 'p' || key =='P'){	//Set pause variable to active or inactive
		if(pause == 1){
		pause = 0;		
		}
		else if(pause == 0){
			pause = 1;
		}
	}

	if(key == 'w' || key == 'W'){		//Be able to change the pull push range between 6500 and 30000
		range += 500;
		if(range >=30500){
			range = 30000;
		}
	}
	if(key == 'x' || key == 'X'){
		range -= 500;
		if(range <=6000){
			range = 6500;
		}
	}

	if(key == 'q' || key == 'Q'){		//q and e Change the speed
		speedChange += 1;
		if(speedChange >=500){
			speedChange = 499;
		}
	}

	if(key == 'e' || key == 'E'){
		speedChange -= 1;
		if(speedChange <=-500){
			speedChange = -499;
		}
	}

	if(key == 'z' || key == 'Z'){	//z and c change the particle size
		sizeChange += 0.2;
		if(sizeChange >=5.2){
			sizeChange = 5;
		}
	}

	if(key == 'r' || key == 'R'){	//r = restart button
		currentParticles = 50;
		initParticles();
	}

	if(key == 'c' || key == 'C'){
		sizeChange -= 0.2;
		if(sizeChange <=0.0){
			sizeChange = 0.2;
		}
	}
	if(key == 'a' || key == 'A' ){	//Add a particle
		glBegin(GL_POINTS);			//Draws a particle with the next element in the array by setting its parameters to randomly
		glColor3f(Particles[currentParticles].Red = rand()%2,Particles[currentParticles].Green = rand()%2,Particles[currentParticles].Blue = rand()%2);
		Particles[currentParticles].dX = (rand()%10*0.01)* pow(-1.0,rand()%2)*pause;
		Particles[currentParticles].dY = (rand()%10*0.01)* pow(-1.0,rand()%2)*pause;
		Particles[currentParticles].size = rand()%12 + 5;
		//location is not random so we set it next to the mouse coordinates
		glVertex2i(Particles[currentParticles].XCoor = x, Particles[currentParticles].YCoor = y);	
		glEnd();
		currentParticles += 1;		//We add 1 to current particles to keep track of how many particles in the array to draw
		if(currentParticles > MaxParticles){	//Set max limit on particles
			currentParticles = MaxParticles;
		}
	}
	if(key == 'd' || key == 'D' ){	//Delete nearest particle
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
	}
	
}


void idle(void){
	glutPostRedisplay();
}
void mouse(int btn, int state, int x, int y){

	y = 600 - y; //Y Coordinate fixed

	if(btn == GLUT_LEFT_BUTTON && state==GLUT_DOWN)
	{
		//When LMB is pushed set mouseX\Y for (-1,-1) to mouse coordinates
		pullPush = 0;
		mouseX = x;
		mouseY = y;

		int i;
		double temp1;
		double temp2;
		//Logic for moving particle towards mouse
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
		//Set mouseX\Y back to their default (-1,-1)
		mouseX = -1;
		mouseY = -1;

		//When LMB depressed randomized particles speed and direction
		int i;
		for(i=0;i<currentParticles; i++){
			if(Particles[i].affected == true){
			Particles[i].dX = (rand()%10*0.005)* pow(-1.0,rand()%2)*pause;
			Particles[i].dY = (rand()%10*0.005)* pow(-1.0,rand()%2)*pause;
			Particles[i].affected = false;
			}
		}
	}
	
	if(btn == GLUT_RIGHT_BUTTON && state==GLUT_DOWN)
	{
		//Viceversa to LMB
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

	if(btn == GLUT_RIGHT_BUTTON && state==GLUT_UP)
	{
		mouseX = -1;
		mouseY = -1;
		int i;
		for(i=0;i<currentParticles; i++){
			if(Particles[i].affected == true){
			//Particles[i].dX = (rand()%10*0.005)* pow(-1.0,rand()%2)*pause;
			//Particles[i].dY = (rand()%10*0.005)* pow(-1.0,rand()%2)*pause;
			Particles[i].affected = false;
			}
		}
	}
}

void motion(int x, int y)
{
	y = 600 - y;
	mouseX = x;
	mouseY = y;
	int i;
	double temp1;
	double temp2;
	if(pullPush == 0){		//When pullPush = Pull (0), move particles towards mouse when mouse in motion if particle in range
		
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

	if(pullPush == 1){		//Move particles away from mouse if in range, then it is affected
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



int main(int argc, char** argv)
{	
	
	glutInit(&argc, argv);		//starts up GLUT
	
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(600, 600);
	initParticles();
	glutCreateWindow("Particle System");	//creates the window
	glutInitDisplayMode(GLUT_DOUBLE);
	glutDisplayFunc(display);		
	glutKeyboardFunc(kbd);
	glutMotionFunc(motion);
	glutMouseFunc(mouse);
	glutReshapeWindow(600, 600);
	gluOrtho2D(0, 600, 0, 600);
	glutIdleFunc(idle);
	glutMainLoop();				//starts the event loop

	return(0);					
}