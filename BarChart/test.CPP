/*
    Code to display a bar chart
    Maintainer: Shivanvitha Ambati, 19MCME24
    Done on: 20th February, 2022

    ********** Coded on Linux machine ********** 
    g++ <filename>.cpp -o <executable> -lGL -lGLU -lglut
    ./<executable> data.txt

*/

// Import necessary libraries
#include <GL/glut.h>
#include <iostream>
#include <fstream>
#include <string.h>
#define M 50
#define N 70

using namespace std;

// Variables to hold number of observations, maximum dataValues
int nObs = 0, maximum, minimum = 50;
// Randomly initialised width and height of window which will be changed dynamically later
GLsizei winWidth = 900, winHeight = 500; 
// Raster coordinates
GLint xRaster = 25, yRaster = 150; 
// Variables to hold labels, dataValues
char label [200];
GLint dataValue [M];
char xLabel[M], yLabel[M];


void init (void)
{
    // Black display window
    glClearColor(0.0, 0.0, 0.0, 0.0); 
    glMatrixMode (GL_PROJECTION);
    gluOrtho2D (0.0, winWidth, 0.0, winHeight);
}

// Finds maximum value in data values
void maximumValue () {
    maximum = dataValue[0];
    for ( int i = 1; i < nObs; i++ ) {
        if ( dataValue[i] > maximum )
            maximum = dataValue[i];
    }
}

// Finds minimum value in data values
// void minimumValue () {
//     minimum = dataValue[0];
//     for ( int i = 1; i < nObs; i++ ) {
//         if ( dataValue[i] < minimum )
//             minimum = dataValue[i];
//     }
// }

// Bar Chart display function
void barChart (void) {
    // Intermediate variables
    GLint month, k;
    char sdataValue[M];
    glClear (GL_COLOR_BUFFER_BIT); 
    // Set colour to white
    glColor3f (1.0, 1.0, 1.0);
    // Changing abscissa raster
    xRaster = 40;
    // X-Axis and Y-Axis lines
    glBegin(GL_LINES);
		glVertex2i (winWidth - xRaster, minimum);
		glVertex2i (xRaster, minimum);
    glEnd( );

    glBegin(GL_LINES);
		glVertex2i (xRaster, winHeight - xRaster);
		glVertex2i (xRaster, minimum);
    glEnd( );

    // Rectangles and dataValues displayed on top of them
    for (k = 0; k < nObs; k++) {
        // Pink colour
        glColor3f (255.0, 0.0, 1.0);

        if ( dataValue[k] == 0 ) {
            glRectf (xRaster + 10 + k*50, 0, 2*xRaster + k*50, dataValue[k]/GLfloat(maximum) * winHeight/2 );

            glColor3f (1.0, 1.0, 1.0);

            glRasterPos2i ( 15 + xRaster + k*50, 2*xRaster);

        }
        else {
            glRectf (xRaster + 10 + k*50, minimum, 2*xRaster + k*50, dataValue[k]/GLfloat(maximum) * winHeight/2 + minimum);

            glColor3f (1.0, 1.0, 1.0);

            glRasterPos2i ( 15 + xRaster + k*50, dataValue[k]/GLfloat(maximum) * winHeight/2 + minimum + 10);

        }

        // Changing int to string to print on window
        sprintf ( sdataValue, "%d", dataValue[k] );
        for (int j = 0; sdataValue[j] != '\0'; j++)
            glutBitmapCharacter (GLUT_BITMAP_HELVETICA_12, sdataValue[j]); // printing values
        
    }
    glColor3f (1.0, 1.0, 1.0); // Set colour to black.

    xRaster += 10;
    int l = -1;
    //Printing X-Axis labels
    for (month = 0; month < nObs; month++) {
        glRasterPos2i (xRaster, minimum - 20);
        l++;
        while ( label[l] != ',' && label[l] != '\0' ) {
            glutBitmapCharacter (GLUT_BITMAP_HELVETICA_10, label [l]);
            l++;
        }
        xRaster += 50;
    }

    // Displaying X Label
    glRasterPos2f (winWidth/2 - 60, minimum - 44);
    for ( int i = 0; xLabel[i] != '\0'; i++ )
        glutBitmapCharacter( GLUT_BITMAP_HELVETICA_12, xLabel[i] );

    // Displaying Y Label
    GLfloat yPos = winHeight/2;        
    glRasterPos2f (15, winHeight - 20);

    for ( int i = 0; yLabel[i] != '\0'; i++ ) 
        glutBitmapCharacter( GLUT_BITMAP_HELVETICA_12, yLabel[i] );

    glFlush ( );
}

int main (int argc, char** argv) {
    // Error handling for wrong cmdline arguments
    if ( argc != 2 ) {
        cout << "Error: File name has to passed as cmdline arg" << endl;
        cout << "./<executable> <file_name>.txt" << endl;
        return 0;
    }
    // Intermediate varibles
    string mydata;
    char nsObs[10], data[M];
    int labelCharCount = 0, dataValueCount = 0;
    // Reading from file
    ifstream fr (argv[1]);

    // If no such file found, display error and exit
    if ( fr.fail() ) {
        cout << "File not found" << endl;
        return 0;
    }
    // Get X Label
    getline ( fr, mydata );
    strcpy(xLabel, mydata.c_str());

    // Get Y Label
    getline ( fr, mydata );
    strcpy(yLabel, mydata.c_str());

    // Get num of obs
    getline ( fr, mydata );
    strcpy(nsObs, mydata.c_str());
    // Convert to integer
    sscanf(nsObs, "%d", &nObs);

    // Adding X values and Y values to arrays
    for ( int i = 0; i < nObs; i++ ) {
        getline ( fr, mydata );
        strcpy(data, mydata.c_str());
        // splitting data with space separator
        char *token = strtok(data, " ");
   
        for ( int j = 0; token[j] != '\0'; j++ ) {
            label[labelCharCount] = token[j];
            labelCharCount++;
        }
        label[labelCharCount] = ',';
        labelCharCount++;

            
        token = strtok(NULL, " ");
        sscanf(token, "%d", &dataValue[dataValueCount]);

        dataValueCount++;
    }
    label[labelCharCount - 1] = '\0';

    fr.close();

    // Compute max value
    maximumValue ();

    // minimum = 50;

    
    winHeight = 600;
    winWidth = nObs * 60;

    // Initialise glu 
    glutInit (&argc, argv);
    // Set display mode
    glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
    // Set window position
    glutInitWindowPosition (100, 100);
    // Set window size
    glutInitWindowSize (winWidth, winHeight);
    // Set window title
    glutCreateWindow ("Bar Plot - 19MCME24");
    // Call init
    init ( );
    // Display bar chart
    glutDisplayFunc (barChart);
    // The process will be looped
    glutMainLoop ( );


    return 0;
}
