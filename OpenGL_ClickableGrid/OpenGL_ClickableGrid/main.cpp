//
//  main.cpp
//  OpenGL_ClickableGrid
//
//  Created by 詹閔翔 on 2020/5/6.
//  Copyright © 2020 詹閔翔. All rights reserved.
//
#include <GLUT/glut.h>
#include <iostream>

#define SCREEN 125
#define REFRESH 30

const int GRID = 5;
float quadSize =0;
int quadCount = 0;
float strokeWeight = 0.01;


int mousePosition[2];

void init(void);
void display(void);
void resize(int x, int y);
void mouse(int button, int states, int x, int y);

int main(int argc, char *argv[]) {
    
    //glut init
    glutInit(&argc, argv);
    glutCreateWindow("OpenGL Setup Test");
    glutInitWindowSize(SCREEN, SCREEN);
    glutInitWindowPosition(100, 100);
    glutInitDisplayMode(GLUT_DOUBLE);
    glutMouseFunc(mouse);
    
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(resize);
      
    //enter event loop processing
    glutMainLoop();
    return 0;
}

void init(void)
{
    quadCount = SCREEN / GRID;
    quadSize = 2.0f / static_cast<float>(quadCount) + strokeWeight ;
    mousePosition[0]=-10;
    mousePosition[1]=10;
}

void display(void){
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black and opaque
    glClear(GL_COLOR_BUFFER_BIT);         // Clear the color buffer (background)
    glMatrixMode(GL_MODELVIEW);     // To operate on Model-View matrix
    glLoadIdentity();

    for(int x=0; x < SCREEN; x++)
    {
        float x_pos =  -1.0 + x * quadSize + strokeWeight;
        
        for(int y=0; y < SCREEN; y++)
        {
            float y_pos =  -1.0 + y * quadSize + strokeWeight;
            
            //std::cout << (GLfloat)1/x << " : " << (GLfloat)1/y << std::endl;
            float _color[3] = {1.0,1.0,1.0};
            
            float mouseX = float(mousePosition[0])/float(quadCount/2);
            float mouseY = float(mousePosition[1])/float(quadCount/2);
            
            if(x_pos < mouseX && x_pos  + quadSize >= mouseX ){
                if(y_pos < mouseY && y_pos  + quadSize >= mouseY){
                    _color[0] = 1.0;
                    _color[1]=_color[2]=0.0;
                }
            }

            glBegin(GL_QUADS);              // Each set of 4 vertices form a quad
                glColor3f(_color[0], _color[1], _color[2]); // Red
                glVertex2f(x_pos , y_pos );    // x, y
                glVertex2f(x_pos + quadSize - strokeWeight, y_pos );
                glVertex2f(x_pos + quadSize - strokeWeight, y_pos + quadSize - strokeWeight);
                glVertex2f(x_pos  , y_pos + quadSize - strokeWeight);
            glEnd();

        }
    }
    glFlush();  // Render now
    
}

void resize(int x, int y)
{
    
}

void mouse(int button, int states, int x, int y)
{
    
    if(button == GLUT_LEFT_BUTTON && states == GLUT_DOWN)
    {
        float half_width = glutGet(GLUT_WINDOW_WIDTH)/2;
        float half_height= glutGet(GLUT_WINDOW_HEIGHT)/2;
        
        mousePosition[0]= quadCount/2 * (x-half_width)/half_width;
        mousePosition[1]= quadCount/2 * (half_height-y)/half_width;

        std::cout << "( mouseX, mouseY) : " << mousePosition[0]<<", " << mousePosition[1] <<" ; quadSize : " << quadSize << "\n";
        
        display();
        
    }
    
}
