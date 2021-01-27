//File: main.cpp

#include <iostream>
#include <GL/freeglut.h>
#include "mygllib3d/gl3d.h"
#include <cmath>
#include <ctime>
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
const int n = 4;
const int N = pow(2, n) + 1;
const int SIZE = N - 1;

float heightmap[N][N];
float M = 0.25 * N; //Max rand range
float R = 1.5;      //roughness


void init()
{
    mygllib::View & view = *(mygllib::SingletonView::getInstance());
    
	view.set_projection();
	view.lookat();
	glViewport(0, 0, 400, 400);
	glEnable(GL_DEPTH_TEST);	
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	glColor3f(0.9, 0.9, 0.9);


    heightmap[0][0] = 1;
	heightmap[0][N - 1] = 2;
	heightmap[N - 1][0] = 2;
	heightmap[N - 1][N - 1] = 1;

}


float avg(int div, float w, float x, float y, float z = 0)
{
    float val = (w + x + y + z) / div;
    float r = float(rand() % 2) * 2 * M - M;
    return val + r;
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
                heightmap[x + half][z + half]= avg(4, heightmap[x][z],            // averages the height from the 4 corners
                                                   heightmap[x][z + width],
                                                   heightmap[x + width][z],
                                                   heightmap[x + width][z + width]);
            }
        }
        
        //square step
        for (int x = 0; x < SIZE; x += width)
        {
            for (int z = 0; z < SIZE; z += width)
            {
                int center_x = x + half;
                int center_z = z + half;
                
                int square_x = center_x;
                int square_z = center_z - half;
                
                if (square_z - half >= 0)
                {
                    heightmap[square_x][square_z] = avg(4, heightmap[square_x - half][square_z],
                                                        heightmap[square_x + half][square_z],
                                                        heightmap[square_x][square_z + half],
                                                        heightmap[square_x][square_z - half]);
                }
                else
                {
                    heightmap[square_x][square_z] = avg(3, heightmap[square_x - half][square_z],
                                                        heightmap[square_x + half][square_z],
                                                        heightmap[square_x][square_z + half]);
                }
                

                
                square_x = center_x;
                square_z = center_z + half;
                
                if (square_z + half <= SIZE)
                {
                    heightmap[square_x][square_z] = avg(4, heightmap[square_x - half][square_z],
                                                        heightmap[square_x + half][square_z],
                                                        heightmap[square_x][square_z - half],
                                                        heightmap[square_x][square_x + half]);
                }
                else
                {
                    heightmap[square_x][square_z] = avg(3, heightmap[square_x - half][square_z],
                                                        heightmap[square_x + half][square_z],
                                                        heightmap[square_x][square_z - half]);
                }
                
               
                square_x = center_x - half;
                square_z = center_z;
                if (square_x - half >= 0)
                {
                    heightmap[square_x][square_z] = avg(4, heightmap[square_x - half][square_z],
                                                        heightmap[square_x + half][square_z],
                                                        heightmap[square_x][square_z - half],
                                                        heightmap[square_x][square_z + half]);
                }
                else
                {
                    heightmap[square_x][square_z] = avg(3, heightmap[square_x + half][square_z],
                                                        heightmap[square_x][square_z - half],
                                                        heightmap[square_x][square_z + half]);
                }
                
                
                square_x = center_x + half;
                square_z = center_z; 
                if (square_x + half <= SIZE)
                {
                    heightmap[square_x][square_z] = avg(4, heightmap[square_x - half][square_z],
                                                        heightmap[square_x + half][square_z],
                                                        heightmap[square_x][square_z - half],
                                                        heightmap[square_x][square_z + half]);
                }
                else
                {
                    heightmap[square_x][square_z] = avg(3, heightmap[square_x - half][square_z],
                                                        heightmap[square_x][square_z - half],
                                                        heightmap[square_x][square_z + half]);
                }
            }
        }

        M = M * pow(2, -R);
        //std::cout << M << std::endl;
    }
}





void draw_map()
{
    glColor3f(0.0, 0.0, 0.0);
    for (int x = 0; x < SIZE; ++x)
    {
        //glBegin(GL_TRIANGLE_STRIP);
        glBegin(GL_TRIANGLES);
        {
            for (int z = 0; z < SIZE; ++z)
            {   
                /*
                glVertex3f(x, heightmap[x][z], z);
                glVertex3f(x, heightmap[x][z + 1], z + 1);
                glVertex3f(x + 1, heightmap[x + 1][z], z);
                glVertex3f(x + 1, heightmap[x + 1][z + 1], z + 1);
                */
            }
        }
        glEnd();
    }
}


void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glFrontFace(GL_CCW);
    //mygllib::draw_xz_plane();
	mygllib::draw_axes(1);
    draw_map();
	glutSwapBuffers();
}

void map_vals()
{
    for (int i = 0; i <= SIZE; ++i)
    {
        std::cout << '\n';
        for (int k = 0; k <= SIZE; ++k)
        {
            std::cout << heightmap[i][k] << '\t';
        }
        std::cout << '\n';
    }
}
int main(int argc, char ** argv)
{
    srand(time(NULL));
	mygllib::init3d();	
	init();
    diamond_square();
	//map_vals();
    glutDisplayFunc(display);
	glutReshapeFunc(mygllib::Reshape::reshape);
	glutKeyboardFunc(mygllib::Keyboard::keyboard);
	glutMainLoop();

	return 0;
}
