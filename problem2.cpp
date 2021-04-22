#include<bits/stdc++.h>

#include <windows.h>
#include <GL/glut.h>

#define pi (2*acos(0.0))

using namespace std;

double cameraHeight;
double cameraAngle;
int drawgrid;
int drawaxes;
double angle;

double bigSquareLength;
double bigCircleRadius;
double prevSPEEDCONSTANT;
int togglecount = 0;
double SPEED_CONSTANT;

double bubbleRadius;
const int bubblesCount = 5;
int bubbleShowCount = 0;

int displayCounter = 0;
struct point
{
	double x,y,z;
};

struct Bubble
{
    struct point center;
    struct point velocity;
    int inBigCircle;
};

struct Bubble bubbleArray[bubblesCount];
void printPoint(point p)
{
    cout<<"("<<p.x<<" ,"<<p.y<<" ,"<<p.z<<")"<<endl;
}
void drawAxes()
{
	if(drawaxes==1)
	{
		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_LINES);{
			glVertex3f( 100,0,0);
			glVertex3f(-100,0,0);

			glVertex3f(0,-100,0);
			glVertex3f(0, 100,0);

			glVertex3f(0,0, 100);
			glVertex3f(0,0,-100);
		}glEnd();
	}
}

double calculateDistanceBetweenPoints(point p1,point p2)
{
    return sqrt(pow(p1.x-p2.x,2)+pow(p1.y-p2.y,2)+pow(p1.z-p2.z,2));
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
    glColor3f(1.0,0.0,0.0);
    glBegin(GL_QUADS);
    {
        glVertex3f( a, a,0);
		glVertex3f( a,-a,0);
		glVertex3f(-a,-a,0);
		glVertex3f(-a, a,0);
    }
    glEnd();
}

void drawRectangle(double a)
{
    glColor3f(0, 1.0, 0);
    glBegin(GL_LINES);
    {
        glVertex3f(-a,-a,0);
        glVertex3f(a,-a,0);

        glVertex3f(-a,-a,0);
        glVertex3f(-a,a,0);

        glVertex3f(-a,a,0);
        glVertex3f(a,a,0);

        glVertex3f(a,-a,0);
        glVertex3f(a,a,0);
    }
    glEnd();

}
void drawCircle(double radius,int segments)
{
    int i;
    struct point points[100];
    if(radius == bigCircleRadius)
        glColor3f(1.0,0,0);
    else if(radius == bubbleRadius)
        glColor3f(0,0,1.0);
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


void animateBubble()
{
    int seg = 50;
    drawRectangle(bigSquareLength);
    drawCircle(bigCircleRadius,seg);


    if(displayCounter <= bubblesCount*1500)
    {
        bubbleShowCount = displayCounter/1500;
    }
    int i;
    for(i=0;i<bubbleShowCount;i++)
    {
        glPushMatrix();
        {
            glTranslatef(-bigSquareLength,-bigSquareLength,0);
            glTranslatef(bubbleArray[i].center.x,bubbleArray[i].center.y,0);
            drawCircle(bubbleRadius,seg);
        }
        glPopMatrix();
    }
}
void keyboardListener(unsigned char key, int x,int y){
	switch(key){

		case '1':
			drawgrid=1-drawgrid;
			break;
        case 'p':
            togglecount++;
            if(togglecount % 2 != 0){
              prevSPEEDCONSTANT = SPEED_CONSTANT;
              SPEED_CONSTANT = 0;
            }
            else
                SPEED_CONSTANT = prevSPEEDCONSTANT;

            break;
		default:
			break;
	}
}


void specialKeyListener(int key, int x,int y){
	switch(key){
		case GLUT_KEY_DOWN:		//down arrow key
		    //range -- (0.02--0.25)
			if(SPEED_CONSTANT-0.01 > 0.01){
                SPEED_CONSTANT -= 0.01;
			}
			//printf("down -  %f\n",SPEED_CONSTANT);
			break;
		case GLUT_KEY_UP:		// up arrow key
			//cameraHeight += 3.0;

			if(SPEED_CONSTANT+0.01 < 0.26)
                SPEED_CONSTANT += 0.01;
            //printf("up -- %f\n",SPEED_CONSTANT);
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
    displayCounter++;
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
	gluLookAt(0,0,200,	0,0,0,	0,1,0);


	//again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);


	/****************************
	/ Add your objects from here
	****************************/
	//add objects

	//drawAxes();
	drawGrid();

    //mY FUNCTION
    animateBubble();

	//ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
	glutSwapBuffers();
}

double generateRandomDouble(double lower, double upper)
{
    double f = (double)rand()/RAND_MAX;
    return lower + f*(upper-lower);
}
point createPoint(double x, double y, double z)
{
    point p;
    p.x = x;
    p.y = y;
    p.z = z;

    return p;
}

double dotProduct(point p1, point p2)
{
    return p1.x * p2.x + p1.y * p2.y + p1.z * p2.z;
}
point normalizePoint(point p)
{
    point newp;

    newp.x = p.x/(sqrt(pow(p.x, 2)+pow(p.y, 2))+pow(p.z, 2));
    newp.y = p.y/(sqrt(pow(p.x, 2)+pow(p.y, 2))+pow(p.z, 2));
    newp.z = p.z/(sqrt(pow(p.x, 2)+pow(p.y, 2))+pow(p.z, 2));

    return newp;
}

void updateBubbleCenter(int bubbleIndex)
{
    bubbleArray[bubbleIndex].center.x += bubbleArray[bubbleIndex].velocity.x * SPEED_CONSTANT;
    bubbleArray[bubbleIndex].center.y += bubbleArray[bubbleIndex].velocity.y * SPEED_CONSTANT;
    bubbleArray[bubbleIndex].center.z += 0;
}

void updateBubbleCenter(int bubbleIndex, double newx, double newy, double newz)
{
    bubbleArray[bubbleIndex].center.x = newx;
    bubbleArray[bubbleIndex].center.y = newy;
    bubbleArray[bubbleIndex].center.z = newz;
}


void animate(){
	angle += 0.05;
	//codes for any changes in Models, Camera
	int i = 0;
    for(i=0;i<bubbleShowCount;i++)
    {
        double newx = bubbleArray[i].center.x + bubbleArray[i].velocity.x * SPEED_CONSTANT;
        double newy = bubbleArray[i].center.y + bubbleArray[i].velocity.y * SPEED_CONSTANT;

        //wall collision check
        if((double) newx + bubbleRadius > 2 * bigSquareLength || newx < bubbleRadius)
        {
            bubbleArray[i].velocity.x = (-1)* bubbleArray[i].velocity.x;
            newx = bubbleArray[i].center.x + bubbleArray[i].velocity.x  *SPEED_CONSTANT;
        }
        else if((double) newy + bubbleRadius > 2 * bigSquareLength || newy < bubbleRadius)
        {
            bubbleArray[i].velocity.y = (-1)* bubbleArray[i].velocity.y;
            newy = bubbleArray[i].center.y + bubbleArray[i].velocity.y * SPEED_CONSTANT;
        }


        //Circle bubble collision check
        //Big circle center(squareLength,squareLength)
        point bigCircleCenter = createPoint(bigSquareLength, bigSquareLength, 0);
        point newPos = createPoint(newx, newy, 0);


        double currentPosDist = calculateDistanceBetweenPoints(bubbleArray[i].center, bigCircleCenter);
        double newPosDist = calculateDistanceBetweenPoints(bigCircleCenter, newPos);


        //condition if bubble is inside the circle
        if((double)currentPosDist <= bigCircleRadius - bubbleRadius)
        {
            bubbleArray[i].inBigCircle = 1;
        }

        //if the current bubble center is outside the circle and new center is outside the circle then collision will happen inside the circle
        if((double)currentPosDist <= bigCircleRadius - bubbleRadius && newPosDist > bigCircleRadius - bubbleRadius)
        {

            point normalVector = createPoint(bubbleArray[i].center.x - bigCircleCenter.x, bubbleArray[i].center.y - bigCircleCenter.y, 0 );
            //r = d - 2(d dot n)n      #r = updated velocity #d=current velocity(direction) vector #n = normal vector on the circle(current center - origin)
            double dotMul = dotProduct(bubbleArray[i].velocity, normalVector);

            point newVelocity = createPoint(bubbleArray[i].velocity.x - 2*dotMul*normalVector.x, bubbleArray[i].velocity.y - 2*dotMul*normalVector.y,0);
            //normalize and update direction
            bubbleArray[i].velocity = normalizePoint(newVelocity);
            updateBubbleCenter(i);
        }
        else
        {
            updateBubbleCenter(i,newx,newy,0);
        }

        //bubble bubble collision check
        int j ;
        for(j=0;j<bubbleShowCount;j++)
        {
            if(i!=j && bubbleArray[i].inBigCircle == 1 && bubbleArray[j].inBigCircle == 1)
            {
                point newPosi = createPoint(bubbleArray[i].center.x + bubbleArray[i].velocity.x*SPEED_CONSTANT, bubbleArray[i].center.y + bubbleArray[i].velocity.y*SPEED_CONSTANT, 0);
                point newPosj = createPoint(bubbleArray[j].center.x + bubbleArray[j].velocity.x*SPEED_CONSTANT, bubbleArray[j].center.y + bubbleArray[j].velocity.y*SPEED_CONSTANT, 0);

                double currentDistanceBetweenCenters = calculateDistanceBetweenPoints(bubbleArray[i].center, bubbleArray[j].center);
                double nextDistanceBetweenCenters = calculateDistanceBetweenPoints(newPosi,newPosj);


                if((double)currentDistanceBetweenCenters >= 2*bubbleRadius && nextDistanceBetweenCenters <= 2*bubbleRadius)
                {
                    //printf("hello overlap\n");
                    point bubbleNormal = createPoint(bubbleArray[i].center.x - bubbleArray[j].center.x, bubbleArray[i].center.y - bubbleArray[j].center.y, 0);
                    point newVelocityOfi = createPoint(bubbleArray[i].velocity.x + bubbleNormal.x, bubbleArray[i].velocity.y + bubbleNormal.y, 0);
                    bubbleArray[i].velocity = normalizePoint(newVelocityOfi);
                    updateBubbleCenter(i);

                    point newVelocityOfj = createPoint(bubbleArray[j].velocity.x - bubbleNormal.x, bubbleArray[j].velocity.y - bubbleNormal.y, 0);
                    bubbleArray[j].velocity = normalizePoint(newVelocityOfj);
                    updateBubbleCenter(j);
                }
            }
        }
    }

	glutPostRedisplay();
}

void init(){
	//codes for initialization
	drawgrid=0;
	drawaxes=1;
	cameraHeight=150.0;
	cameraAngle=1.0;
	angle=0;

	SPEED_CONSTANT = 0.01;
	bubbleRadius = 10;
	bigCircleRadius = 80;
	bigSquareLength = 120;



	int i = 0;
	for(i=0;i<bubblesCount;i++)
    {
        bubbleArray[i].center.x = bubbleRadius;
        bubbleArray[i].center.y = bubbleRadius;
        bubbleArray[i].center.z = 0;

        double rand1 = generateRandomDouble(0.01,0.99);
        double rand2 = generateRandomDouble(0.01,0.99);

        bubbleArray[i].velocity.x = rand1;
        bubbleArray[i].velocity.y = rand2;
        bubbleArray[i].velocity.z = 0;

        //normalization
        bubbleArray[i].velocity = normalizePoint(bubbleArray[i].velocity);
        bubbleArray[i].inBigCircle = 0;

    }
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
    srand(time(0));
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
