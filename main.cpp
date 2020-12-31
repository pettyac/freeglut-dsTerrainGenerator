//File: main.cpp

#include <iostream>
#include <GL/freeglut.h>
#include "mygllib3d/gl3d.h"
#include <cmath>

GLboolean light_on;
GLfloat light_ambient[4];
GLfloat light_diffuse[4];
GLfloat light_specular[4];
GLfloat light_position[4];


/************************************************

used to construct the N by N heightmap
n = size of the grid
N = 2^n + 1
SIZE = 2^n

*************************************************/
const int n = 6;
const int N = pow(2, n) + 1;
const int SIZE = N - 1;
float heightmap[N][N];


void init()
{
    mygllib::View & view = *(mygllib::SingletonView::getInstance());
    
	view.set_projection();
	view.lookat();

    view.eyez() = -10.0f;
    view.eyey() = 5.0f;
    view.eyex() = -1.0f;
	glViewport(0, 0, 400, 400);
	glEnable(GL_DEPTH_TEST);	
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	glColor3f(0, 0, 1);


    heightmap[0][0] = 0;
	heightmap[0][N - 1] = 5;
	heightmap[N - 1][0] = 10;
	heightmap[N - 1][N - 1] = 44;

}



void diamond_square()
{
    for (int width = SIZE; width >= 2; width /= 2)      // for width w = 2^n, 2^(n - 1), ..., 2
    {
        int half = width / 2;
        
        //diamond step
        for (int x = 0; x < SIZE; x += width)
        {
            for (int z = 0; z < SIZE; z += width)
            {
                double avg = heightmap[x][z]            // averages the height from the 4 corners
                           + heightmap[x][z + width]
                           + heightmap[x + width][z]
                           + heightmap[x + width][z + width];
                avg /= 4.0;

                heightmap[x + half][z + half] = avg;    // center of square = avg of corners
            }
        }
        
        //square step
        for (int x = 0; x < SIZE; x += width)
        {
            for (int z = 0; z < SIZE; z += width)
            {
                double avg;
                int center_x = x + half;
                int center_z = z + half;
                int square_x, square_z = 0;
                int count = 4;          // tracks averaging 3 or 4 values
                                        // squares with 3 edges are divided by (count - 1)
                
                if(heightmap[center_x][center_z - half] == 0)
                {
                    avg = 0;
                    square_x = center_x;
                    square_z = center_z - half;
                    
                    avg = heightmap[square_x - half][square_z]
                        + heightmap[square_x + half][square_z]
                        + heightmap[square_x][square_z + half];

                    if(square_z - half >= 0)            // if upper edge is within bounds...
                    {
                        avg += heightmap[square_x][square_z - half];
                        avg /= count;                 // divide the avg by 4...
                    }
                    else avg /= (count - 1);            // or avg the 3 available edges
                    
                    heightmap[square_x][square_z] = avg;
                }
                
                if(heightmap[center_x - half][center_z] == 0)
                {
                    avg = 0;
                    square_x = center_x - half;
                    square_z = center_z;

                    avg = heightmap[square_x][square_z + half]
                        + heightmap[square_x][square_z - half]
                        + heightmap[square_x + half][square_z];

                    if(square_x - half >= 0)
                    {
                        avg += heightmap[square_x - half][square_z];
                        avg /= count;
                    }
                    else avg /= (count - 1);
                    heightmap[square_x][square_z] = avg;
                }
                
                if(heightmap[center_x + half][center_z] == 0)
                {
                    avg = 0;
                    square_x = center_x + half;
                    square_z = center_z;

                    avg = heightmap[square_x][square_z + half]
                        + heightmap[square_x][square_z - half]
                        + heightmap[square_x - half][square_z];

                    if(square_x + half <= SIZE)
                    {
                        avg += heightmap[square_x + half][square_z];
                        avg /= count;
                    }
                    else avg /= (count - 1);
                    heightmap[square_x][square_z] = avg;

                }
                
                if(heightmap[center_x][center_z + half] == 0)
                {
                    avg = 0;
                    square_x = center_x;
                    square_z = center_z + half;
                    
                    avg = heightmap[square_x - half][square_z]
                        + heightmap[square_x + half][square_z]
                        + heightmap[square_x][square_z - half];

                    if(square_z + half <= SIZE)            // if upper edge is within bounds...
                    {
                        avg += heightmap[square_x][square_z + half];
                        avg /= count;                 // divide the avg by 4...
                    }
                    else avg /= (count - 1);            // or avg the 3 available edges
                    
                    heightmap[square_x][square_z] = avg;
                }
            }
        }
    }
}





void draw_map()
{
    glColor3f(0.0, 0.0, 0.0);
    for (int x = 0; x < SIZE; ++x)
    {
        glBegin(GL_TRIANGLE_STRIP);
        {
            for (int z = 0; z < SIZE; ++z)
            {
                glVertex3f(x, heightmap[x][z], z);
                glVertex3f(x, heightmap[x][z + 1], z + 1);
                glVertex3f(x + 1, heightmap[x + 1][z], z);
                glVertex3f(x + 1, heightmap[x + 1][z + 1], z + 1);
            }
        }
        glEnd();
    }
}


void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    //mygllib::draw_xz_plane();
	mygllib::draw_axes(1);
    draw_map();
	glutSwapBuffers();
}

void map_vals()
{
    for (int i = 0; i < N; ++i)
    {
        std::cout << '\n' << '\n';
        for (int k = 0; k < N; ++k)
        {
            std::cout << heightmap[i][k] << '\t';
        }
    }
}
int main(int argc, char ** argv)
{
	mygllib::init3d();	
	init();
    diamond_square();
	map_vals();
    glutDisplayFunc(display);
	glutReshapeFunc(mygllib::Reshape::reshape);
	glutKeyboardFunc(mygllib::Keyboard::keyboard);
	glutMainLoop();

	return 0;
}
