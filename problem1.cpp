#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#include <windows.h>
#include <GL/glut.h>

#define pi (2*acos(0.0))
#define MOVE_CONSTANT 0.2
#define ROTATION_CONSTANT 0.025

double cameraHeight;
double cameraAngle;
int drawgrid;
int drawaxes;
double angle;
double angleSphere1;
double angleHalfCircle;
double yAxisAngle;
double rotationAngle;


struct point
{
	double x,y,z;
};
//global variable of vectors
//u -- up vector
//r -- right vector
//l - look vector
point pos, u, r, l;

void drawAxes()
{
	if(drawaxes == 1)
	{

		glBegin(GL_LINES);{
		    //X - axis -- RED
            glColor3f(1.0, 0, 0);
			glVertex3f( 150,0,0);
			glVertex3f(-150,0,0);

			//Y - axis --GREEN
            glColor3f(0, 1, 0);
			glVertex3f(0,-150,0);
			glVertex3f(0, 150,0);

			//Z - axis -- BLUE
            glColor3f(0, 0, 1);
			glVertex3f(0,0, 150);
			glVertex3f(0,0,-150);
		}glEnd();
	}
}


void drawGrid()
{
	int i;
	if(drawgrid==1)
	{
		glColor3f(0.6, 0.6, 0.6);	//grey
		glBegin(GL_LINES);{
			for(i=-8;i<=8;i++){

				if(i==0)
					continue;	//SKIP the MAIN axes

				//lines parallel to Y-axis
				glVertex3f(i*10, -90, 0);
				glVertex3f(i*10,  90, 0);

				//lines parallel to X-axis
				glVertex3f(-90, i*10, 0);
				glVertex3f( 90, i*10, 0);
			}
		}glEnd();
	}
}

void drawSquare(double a)
{
    //glColor3f(1.0,0.0,0.0);
	glBegin(GL_QUADS);{
		glVertex3f( a, a,2);
		glVertex3f( a,-a,2);
		glVertex3f(-a,-a,2);
		glVertex3f(-a, a,2);
	}glEnd();
}


void drawCircle(double radius,int segments)
{
    int i;
    struct point points[100];
    glColor3f(0.7,0.7,0.7);
    //generate points
    for(i=0;i<=segments;i++)
    {
        points[i].x=radius*cos(((double)i/(double)segments)*2*pi);
        points[i].y=radius*sin(((double)i/(double)segments)*2*pi);
    }
    //draw segments using generated points
    for(i=0;i<segments;i++)
    {
        glBegin(GL_LINES);
        {
			glVertex3f(points[i].x,points[i].y,0);
			glVertex3f(points[i+1].x,points[i+1].y,0);
        }
        glEnd();
    }
}

void drawCone(double radius,double height,int segments)
{
    int i;
    double shade;
    struct point points[100];
    //generate points
    for(i=0;i<=segments;i++)
    {
        points[i].x=radius*cos(((double)i/(double)segments)*2*pi);
        points[i].y=radius*sin(((double)i/(double)segments)*2*pi);
    }
    //draw triangles using generated points
    for(i=0;i<segments;i++)
    {
        //create shading effect
        if(i<segments/2)shade=2*(double)i/(double)segments;
        else shade=2*(1.0-(double)i/(double)segments);
        glColor3f(shade,shade,shade);

        glBegin(GL_TRIANGLES);
        {
            glVertex3f(0,0,height);
			glVertex3f(points[i].x,points[i].y,0);
			glVertex3f(points[i+1].x,points[i+1].y,0);
        }
        glEnd();
    }
}

void drawLeftHemisphere(double radius,int slices,int stacks)
{
    struct point points[100][100];
	int i,j;
	double h,r;
	//generate points
	for(i=0;i<=stacks;i++)
	{
		h=radius*sin(((double)i/(double)stacks)*(pi/2));
		r=radius*cos(((double)i/(double)stacks)*(pi/2));
		for(j=0;j<=slices;j++)
		{
			points[i][j].x=r*sin(((double)j/(double)slices)*2*pi);
			points[i][j].y=h;
			points[i][j].z=r*cos(((double)j/(double)slices)*2*pi);
		}
	}
	//draw quads using generated points
	for(i=0;i<stacks;i++)
	{
		for(j=0;j<slices;j++)
		{
		    if(j % 2 == 0){
                glColor3f(1,1,1);
            }
            else{
                glColor3f(0,0,0);
            }
			glBegin(GL_QUADS);{
			    //upper hemisphere
				glVertex3f(points[i][j].x,-points[i][j].y,points[i][j].z);
				glVertex3f(points[i][j+1].x,-points[i][j+1].y,points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,-points[i+1][j+1].y,points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,-points[i+1][j].y,points[i+1][j].z);
			}glEnd();
		}
	}
}

void drawRightHemisphere(double radius,int slices,int stacks)
{
    struct point points[100][100];
	int i,j;
	double h,r;
	//generate points
	for(i=0;i<=stacks;i++)
	{
		h=radius*sin(((double)i/(double)stacks)*(pi/2));
		r=radius*cos(((double)i/(double)stacks)*(pi/2));
		for(j=0;j<=slices;j++)
		{
			points[i][j].x=r*sin(((double)j/(double)slices)*2*pi);
			points[i][j].y=h;
			points[i][j].z=r*cos(((double)j/(double)slices)*2*pi);
		}
	}
	//draw quads using generated points
	for(i=0;i<stacks;i++)
	{
		for(j=0;j<slices;j++)
		{
		    if(j % 2 == 0)
        {
            glColor3f(1,1,1);
        }
        else
        {
            glColor3f(0,0,0);
        }
			glBegin(GL_QUADS);{
			    //upper hemisphere
				glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
			}glEnd();
		}
	}
}

void drawCylinder(double radius,int slices,int height)
{
    struct point points[100][100];
	int i,j;
	double h,r;
	//generate points
	for(i=0;i<=height;i++)
	{
		h=height;
		r=radius;
		for(j=0;j<=slices;j++)
		{
			points[i][j].x=r*sin(((double)j/(double)slices)*2*pi);
			points[i][j].y=i;
			points[i][j].z=r*cos(((double)j/(double)slices)*2*pi);
		}
	}
	//draw quads using generated points
	for(i=0;i<height;i++)
	{
		for(j=0;j<slices;j++)
		{
            if(j % 2 == 0){
                glColor3f(1,1,1);
            }
            else{
                glColor3f(0,0,0);
            }

			glBegin(GL_QUADS);{
			    //upper hemisphere
				glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
                //lower hemisphere

			}glEnd();
		}
	}
}



void drawSphere(double radius,int slices,int stacks)
{
	struct point points[100][100];
	int i,j;
	double h,r;
	//generate points
	for(i=0;i<=stacks;i++)
	{
		h=radius*sin(((double)i/(double)stacks)*(pi/2));
		r=radius*cos(((double)i/(double)stacks)*(pi/2));
		//r = 2*radius - r;
		for(j=0;j<=slices;j++)
		{
			points[i][j].x=r*sin(((double)j/(double)slices)*2*pi);
			points[i][j].y=h;
			points[i][j].z=r*cos(((double)j/(double)slices)*2*pi);
		}
	}
	//draw quads using generated points
	for(i=0;i<stacks;i++)
	{
		for(j=0;j<slices;j++)
		{
		    if(j % 2 == 0)
            {
                glColor3f(1,1,1);
            }
            else
            {
                glColor3f(0,0,0);
            }
			glBegin(GL_QUADS);{
			    //upper hemisphere
				glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
                //lower hemisphere
                //updateAngle();
                glRotatef(angleSphere1,1,0,0);
                glVertex3f(points[i][j].x,-points[i][j].y,points[i][j].z);
				glVertex3f(points[i][j+1].x,-points[i][j+1].y,points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,-points[i+1][j+1].y,points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,-points[i+1][j].y,points[i+1][j].z);
			}glEnd();
		}
	}
}

void drawSphereOutward(double radius,int slices,int stacks)
{
	struct point points[100][100];
	int i,j;
	double h,r;
	//generate points
	for(i=0;i<=stacks;i++)
	{
		h=radius*sin(((double)i/(double)stacks)*(pi/2));
		r=radius*cos(((double)i/(double)stacks)*(pi/2));
		r = 2*radius - r;
		for(j=0;j<=slices;j++)
		{
			points[i][j].x=r*sin(((double)j/(double)slices)*2*pi);
			points[i][j].y=h;
			points[i][j].z=r*cos(((double)j/(double)slices)*2*pi);
		}
	}
	//draw quads using generated points
	for(i=0;i<stacks;i++)
	{
		for(j=0;j<slices;j++)
		{
		    if(j % 2 == 0){
                glColor3f(1,1,1);
            }
            else{
                glColor3f(0,0,0);
            }
			glBegin(GL_QUADS);{
			    //upper hemisphere
				glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);

			}glEnd();
		}
	}
}



void drawSS()
{
    glColor3f(1,0,0);
    drawSquare(20);

    glRotatef(angle,0,0,1);
    glTranslatef(110,0,0);
    glRotatef(2*angle,0,0,1);
    glColor3f(0,1,0);
    drawSquare(15);

    glPushMatrix();
    {
        glRotatef(angle,0,0,1);
        glTranslatef(60,0,0);
        glRotatef(2*angle,0,0,1);
        glColor3f(0,0,1);
        drawSquare(10);
    }
    glPopMatrix();

    glRotatef(3*angle,0,0,1);
    glTranslatef(40,0,0);
    glRotatef(4*angle,0,0,1);
    glColor3f(1,1,0);
    drawSquare(5);
}

void draw_gun()
{

    //glRotatef(90,0,0,1);
    //drawSphere(25,30,30);
    int r1 = 35;
    int r2 = 10;
    int h = 70;
    int _slices = 80;
    int _stacks = 80;

    glRotatef(yAxisAngle,0,0,1);
    drawLeftHemisphere(r1, _slices, _stacks);

    glRotatef(angleSphere1,1,0,0);
    drawRightHemisphere(r1, _slices, _stacks);
    glTranslatef(0,r1,0);

    glRotatef(angleHalfCircle,1,0,0);
    glRotatef(rotationAngle,0,1,0);
    glTranslatef(0,r2,0);
    drawLeftHemisphere(r2, _slices, _stacks);

    drawCylinder(r2, _slices, h);
    glTranslatef(0,h,0);
    drawSphereOutward(r2, _slices, _stacks);


}




void look_left(double angle)
{
    //up fixed
    point temp = l ;
    l.x = temp.x * cos((double)angle) + (u.y * temp.z - u.z * temp.y) * sin((double)angle);
    l.y = temp.y * cos((double)angle) + (u.z * temp.x - u.x * temp.z) * sin((double)angle);
    l.z = temp.z * cos((double)angle) + (u.x * temp.y - u.y * temp.x) * sin((double)angle);

    temp = r ;
    r.x = temp.x * cos((double)angle) + (u.y * temp.z - u.z * temp.y) * sin((double)angle);
    r.y = temp.y * cos((double)angle) + (u.z * temp.x - u.x * temp.z) * sin((double)angle);
    r.z = temp.z * cos((double)angle) + (u.x * temp.y - u.y * temp.x) * sin((double)angle);

}

void look_right(double angle)
{
    angle = (-1)*angle;
    look_left(angle);
}
void look_down(double angle)
{
    //r fixed
    point temp = u ;
    u.x = temp.x * cos((double)angle) + (r.y * temp.z - r.z * temp.y) * sin((double)angle);
    u.y = temp.y * cos((double)angle) + (r.z * temp.x - r.x * temp.z) * sin((double)angle);
    u.z = temp.z * cos((double)angle) + (r.x * temp.y - r.y * temp.x) * sin((double)angle);

    temp = l ;
    l.x = temp.x * cos((double)angle) + (r.y * temp.z - r.z * temp.y) * sin((double)angle);
    l.y = temp.y * cos((double)angle) + (r.z * temp.x - r.x * temp.z) * sin((double)angle);
    l.z = temp.z * cos((double)angle) + (r.x * temp.y - r.y * temp.x) * sin((double)angle);
}
void look_up(double angle)
{
    angle = (-1)*angle;
    look_down(angle);
}

void tilt_clockwise(double angle)
{
    //l fixed
    angle = (-1)*angle;
    point temp = u ;
    u.x = temp.x * cos((double)angle) + (l.y * temp.z - l.z * temp.y) * sin((double)angle);
    u.y = temp.y * cos((double)angle) + (l.z * temp.x - l.x * temp.z) * sin((double)angle);
    u.z = temp.z * cos((double)angle) + (l.x * temp.y - l.y * temp.x) * sin((double)angle);

    temp = r ;
    r.x = temp.x * cos((double)angle) + (l.y * temp.z - l.z * temp.y) * sin((double)angle);
    r.y = temp.y * cos((double)angle) + (l.z * temp.x - l.x * temp.z) * sin((double)angle);
    r.z = temp.z * cos((double)angle) + (l.x * temp.y - l.y * temp.x) * sin((double)angle);

}
void tilt_anticlockwise(double angle)
{
    angle = (-1)*angle;
    tilt_clockwise(angle);
}
void keyboardListener(unsigned char key, int x,int y){
	switch(key){
		case '1':
            look_left(pi/18*ROTATION_CONSTANT);
			break;
		case '2':
		    look_right(pi/18*ROTATION_CONSTANT);
			break;
		case '3':
		    look_up(pi/18*ROTATION_CONSTANT);
			break;
		case '4':
            look_down(pi/18*ROTATION_CONSTANT);
			break;
        case '5':
            tilt_clockwise(pi/18*ROTATION_CONSTANT);
			break;
        case '6':
            tilt_anticlockwise(pi/18*ROTATION_CONSTANT);
			break;
        case 'w':
            yAxisAngle -= 0.3;
            if(yAxisAngle <= -45)
                yAxisAngle = -45;
            break;
        case 'q':
            yAxisAngle += 0.3;
            if(yAxisAngle >= 45)
                yAxisAngle = 45;
            break;
        case 'e':
            angleSphere1 += 0.3;
            if(angleSphere1>=60)
                angleSphere1 =60;
            break;
        case 'r':
            angleSphere1-=0.3;
            if(angleSphere1 <= -60)
                angleSphere1 = -60;
            break;
        case 'a':
            angleHalfCircle += 0.3;
            if(angleHalfCircle>=45)
                angleHalfCircle =45;
            break;
        case 's':
            angleHalfCircle-=0.3;
            if(angleHalfCircle <= -45)
                angleHalfCircle = -45;
            break;
        case 'd':
            rotationAngle += 0.3;
            if(rotationAngle>=45)
                rotationAngle =45;
            break;
        case 'f':
            rotationAngle-=0.3;
            if(rotationAngle <= -45)
                rotationAngle = -45;
		default:
			break;
	}
}


void specialKeyListener(int key, int x,int y){
	switch(key){
		case GLUT_KEY_DOWN:		//down arrow key
			cameraHeight -= 3.0;
			pos.x -= MOVE_CONSTANT*l.x;
			pos.y -= MOVE_CONSTANT*l.y;
			pos.z -= MOVE_CONSTANT*l.z;
			break;
		case GLUT_KEY_UP:		// up arrow key
			cameraHeight += 3.0;
			pos.x += MOVE_CONSTANT*l.x;
			pos.y += MOVE_CONSTANT*l.y;
			pos.z += MOVE_CONSTANT*l.z;

			break;

		case GLUT_KEY_RIGHT:
			//cameraAngle += 0.03;
			pos.x += MOVE_CONSTANT*r.x;
			pos.y += MOVE_CONSTANT*r.y;
			pos.z += MOVE_CONSTANT*r.z;
			break;
		case GLUT_KEY_LEFT:
			//cameraAngle -= 0.03;
			pos.x -= MOVE_CONSTANT*r.x;
			pos.y -= MOVE_CONSTANT*r.y;
			pos.z -= MOVE_CONSTANT*r.z;
			break;

		case GLUT_KEY_PAGE_UP:
		    pos.x += MOVE_CONSTANT*u.x;
			pos.y += MOVE_CONSTANT*u.y;
			pos.z += MOVE_CONSTANT*u.z;
			break;
		case GLUT_KEY_PAGE_DOWN:
		    pos.x -= MOVE_CONSTANT*u.x;
			pos.y -= MOVE_CONSTANT*u.y;
			pos.z -= MOVE_CONSTANT*u.z;
			break;

		case GLUT_KEY_INSERT:
			break;

		case GLUT_KEY_HOME:
			break;
		case GLUT_KEY_END:
			break;

		default:
			break;
	}
}


void mouseListener(int button, int state, int x, int y){	//x, y is the x-y of the screen (2D)
	switch(button){
		case GLUT_LEFT_BUTTON:
			if(state == GLUT_DOWN){		// 2 times?? in ONE click? -- solution is checking DOWN or UP
				drawaxes=1-drawaxes;
			}
			break;

		case GLUT_RIGHT_BUTTON:
			//........
			break;

		case GLUT_MIDDLE_BUTTON:
			//........
			break;

		default:
			break;
	}
}




void display(){

	//clear the display
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0,0,0,0);	//color black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/********************
	/ set-up camera here
	********************/
	//load the correct matrix -- MODEL-VIEW matrix
	glMatrixMode(GL_MODELVIEW);

	//initialize the matrix
	glLoadIdentity();

	//now give three info
	//1. where is the camera (viewer)?
	//2. where is the camera looking?
	//3. Which direction is the camera's UP direction?

	//gluLookAt(100,100,100,	0,0,0,	0,0,1);
	//gluLookAt(200*cos(cameraAngle), 200*sin(cameraAngle), cameraHeight,		0,0,0,		0,0,1);
	//gluLookAt(0,0,200,	0,0,0,	0,1,0);
	gluLookAt(pos.x, pos.y, pos.z,      pos.x+l.x, pos.y+l.y,pos.z+l.z,    u.x,u.y,u.z);


	//again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);


	/****************************
	/ Add your objects from here
	****************************/
	//add objects

	drawAxes();
	drawGrid();

	draw_gun();




	//ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
	glutSwapBuffers();
}


void animate(){
	angle+=0.05;
	//codes for any changes in Models, Camera
	glutPostRedisplay();
}

void init(){
	//codes for initialization
	drawgrid=0;
	drawaxes=1;
	cameraHeight=150.0;
	cameraAngle=1.0;
	angle=0;
	angleSphere1 = yAxisAngle = angleHalfCircle = rotationAngle = 0;

//	//initialization of pos, u, r, l vectors
	pos.x = pos.y = 120;
	pos.z = 20;

	u.x = u.y = 0;
	u.z = 1;

	r.x = (double)-1/sqrt(2);
	r.y = (double)1/sqrt(2);
	r.z = 0;

	l.x = (double)-1/sqrt(2);
	l.y = (double)-1/sqrt(2);
	l.z = 0;




	//clear the screen
	glClearColor(0,0,0,0);

	/************************
	/ set-up projection here
	************************/
	//load the PROJECTION matrix
	glMatrixMode(GL_PROJECTION);

	//initialize the matrix
	glLoadIdentity();

	//give PERSPECTIVE parameters
	gluPerspective(80,	1,	1,	1000.0);
	//field of view in the Y (vertically)
	//aspect ratio that determines the field of view in the X direction (horizontally)
	//near distance
	//far distance
}

int main(int argc, char **argv){
	glutInit(&argc,argv);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color

	glutCreateWindow("My OpenGL Program");

	init();

	glEnable(GL_DEPTH_TEST);	//enable Depth Testing

	glutDisplayFunc(display);	//display callback function
	glutIdleFunc(animate);		//what you want to do in the idle time (when no drawing is occuring)

	glutKeyboardFunc(keyboardListener);
	glutSpecialFunc(specialKeyListener);
	glutMouseFunc(mouseListener);

	glutMainLoop();		//The main loop of OpenGL

	return 0;
}



