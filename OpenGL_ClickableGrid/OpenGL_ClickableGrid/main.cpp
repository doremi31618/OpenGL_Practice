//
//  main.cpp
//  OpenGL_ClickableGrid
//
//  Created by 詹閔翔 on 2020/5/6.
//  Copyright © 2020 詹閔翔. All rights reserved.
//
#include <GLUT/glut.h>
#include <iostream>
#include <vector>
#include <math.h>
//#include <cstdlib>
using namespace std;

#define SCREEN 125
#define REFRESH 30
struct vertex_data{
    public:
    vector<GLfloat> v_color;
    GLfloat x,y;
};

enum mode
{
    midpoint = 0,
    antialiasing = 1
};

const int GRID = 5;
float quadSize =0;
int quadCount = 0;
float strokeWeight = 0.01;


float mousePosition[2];
vector<vertex_data>  mousePositionQueue;
vector<vertex_data>  drawPositionQueue;


mode show_mode;

//callback func
void init(void);
void display(void);
void resize(int x, int y);
void mouse(int button, int states, int x, int y);
void menu(int index);
void motionFunc(int x, int y);

//custom func
void createMenu(void);
void clearWindow(void);
void midpointAlgorithm(void);
void antialiasingAlgorithm(void);
void savePoint(int, int, vector<float>);
int normalizeSpazeToGridSpace(float input, float quadsize);

int main(int argc, char *argv[]) {
    std::cout << "操作方式 ： \n" ;
    std::cout << "點擊右鍵叫出選單 \n" ;
    std::cout << "size(畫布解析度) : 支援 5*5、15*15、25*25\n" ;
    std::cout << "line algorithm（畫線演算法） :\n" ;
    std::cout << "midpoint algorithm (Bresenham's alogorithm)\n  ";
    std::cout << "anti-aliasing algorithm (Bresenham's Algorithm & Antialiasing Line)\n" ;
    std::cout << "clear（清空畫布）\n" ;
    std::cout << "exit（離開）\n" ;
    
    //glut init
    glutInit(&argc, argv);
    glutCreateWindow("OpenGL Setup Test");
    glutInitWindowSize(SCREEN, SCREEN);
    glutInitWindowPosition(0, 0);
    glutInitDisplayMode(GLUT_DOUBLE);
    glutMouseFunc(mouse);
    glutPassiveMotionFunc(motionFunc);
    createMenu();
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(resize);
      
    //enter event loop processing
    glutMainLoop();
    return 0;
}

void init(void)
{
    quadCount = 5;
    quadSize = 2.0f / static_cast<float>(quadCount) + strokeWeight ;
    mousePosition[0]=-1;
    mousePosition[1]=10;
    
}

void display(void){
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black and opaque
    glClear(GL_COLOR_BUFFER_BIT);         // Clear the color buffer (background)
    glMatrixMode(GL_MODELVIEW);     // To operate on Model-View matrix
    glLoadIdentity();
    //std::cout << (GLfloat)1/x << " : " << (GLfloat)1/y << std::endl;
    
    
    float mouseX = mousePosition[0];
    float mouseY = mousePosition[1];
    
    
    
    for(int x=0; x < SCREEN; x++)
    {
        float x_pos =  -1.0 + x * quadSize ;
        
        for(int y=0; y < SCREEN; y++)
        {
            float y_pos =  -1.0 + y * quadSize;
            
            
            float _color[3] = {1.0,1.0,1.0};
            if(x_pos < mouseX && x_pos  + quadSize >= mouseX ){
                if(y_pos < mouseY && y_pos  + quadSize >= mouseY){
                    _color[0] = 1.0;
                    _color[1]=_color[2]=0.0;
                }
            }
            
            //detect if the mouse(X,Y) is on the line
            float grid_center[2] = {(2 * x_pos + quadSize)/2, (2 * y_pos + quadSize)/2};
            

            glBegin(GL_QUADS);              // Each set of 4 vertices form a quad
                glColor3f(_color[0], _color[1], _color[2]); // Red
                glVertex2f(x_pos + strokeWeight   , y_pos  + strokeWeight );    // x, y
                glVertex2f(x_pos + quadSize  - strokeWeight , y_pos + strokeWeight  );
                glVertex2f(x_pos + quadSize - strokeWeight , y_pos + quadSize - strokeWeight );
                glVertex2f(x_pos  + strokeWeight, y_pos + quadSize - strokeWeight);
            glEnd();

        }
    }
    
    for(int i=0; i<drawPositionQueue.size(); i++)
    {
        float x_pos = drawPositionQueue[i].x;
        float y_pos = drawPositionQueue[i].y;
        
        glBegin(GL_QUADS);
            glColor4f(drawPositionQueue[i].v_color[0],
                      drawPositionQueue[i].v_color[1],
                      drawPositionQueue[i].v_color[2],
                      drawPositionQueue[i].v_color[3]);
            glVertex2f(x_pos + strokeWeight   , y_pos  + strokeWeight );    // x, y
                           glVertex2f(x_pos + quadSize  - strokeWeight , y_pos + strokeWeight  );
                           glVertex2f(x_pos + quadSize - strokeWeight , y_pos + quadSize - strokeWeight );
                           glVertex2f(x_pos  + strokeWeight, y_pos + quadSize - strokeWeight);
        glEnd();
    }
    
    if(mousePositionQueue.size() > 0)
    {
       
        glBegin(GL_LINES);
            glColor4f(0,0,0,1);
            glVertex2f(mousePositionQueue[0].x,mousePositionQueue[0].y);
            glVertex2f(mouseX,mouseY);
        glEnd();
    }
    glFlush();  // Render now

}

void resize(int x, int y)
{
    
}

void mouse(int button, int states, int x, int y)
{
    float half_width = glutGet(GLUT_WINDOW_WIDTH)/2;
    float half_height= glutGet(GLUT_WINDOW_HEIGHT)/2;
   
    mousePosition[0]=  (x-half_width)/half_width;
    mousePosition[1]=  (half_height-y)/half_width;
    
    if(button == GLUT_LEFT_BUTTON && states == GLUT_DOWN)
    {
        //compute deviation of mouse coordinate
        float deviation_x =0;
        float deviation_y =0;
        
        if( mousePosition[0] < 0) deviation_x=-quadSize/2;
        else deviation_x+=quadSize/2;
        
        if( mousePosition[1] < 0) deviation_y=-quadSize/2;
        else deviation_y=quadSize/2;
        
        float transfer_x =  int( (mousePosition[0] + deviation_x)/quadSize );
        float transfer_y = int( (mousePosition[1] + deviation_y)/quadSize) ;
        
        std::cout << " transfer ( mouseX, mouseY) : " <<transfer_x<<", " <<transfer_y << "\n";
       
        //check if add mouse poition
        int num_mouse = mousePositionQueue.size();
        
        if(num_mouse < 1){
            struct vertex_data new_mousePosition;
            new_mousePosition.x = mousePosition[0];
            new_mousePosition.y = mousePosition[1];
            mousePositionQueue.push_back(new_mousePosition);
        } else {
            struct vertex_data new_mousePosition;
            new_mousePosition.x = mousePosition[0];
            new_mousePosition.y = mousePosition[1];
            mousePositionQueue.push_back(new_mousePosition);
            
            
            if(show_mode == antialiasing)
            {
                //use midpoint algorithm with antialiasing to calculate
                antialiasingAlgorithm();
            }else{
                midpointAlgorithm();
            }
            //clear positions of queue
            mousePositionQueue.clear();
        }
    }
    display();
    
}
void menu(int index)
{
    switch(index)
    {
        //main menu
        case 0:
            exit (0);
            break;
        
        case 1://clear window
            clearWindow();
            break;
            
        //size menu
        case 20:
            quadCount = 5;
            clearWindow();
            break;
        case 21:
            quadCount = 15;
            clearWindow();
            break;
        case 22:
            quadCount = 25;
            clearWindow();
            break;
            
        //mode menu
        case 30:
            show_mode = midpoint;
            clearWindow();
            break;
        case 31:
            show_mode = antialiasing;
            clearWindow();
            break;
    }
     quadSize = 2.0f / static_cast<float>(quadCount);
    display();
}
void createMenu()
{
    int sizeMenu = glutCreateMenu(menu);
    glutAddMenuEntry("5x5", 20);
    glutAddMenuEntry("15x15", 21);
    glutAddMenuEntry("25x25", 22);
    
    int lineAlgrithm =glutCreateMenu(menu);
    glutAddMenuEntry("midpoint algrithm", 30);
    glutAddMenuEntry("antialiasing algrithm", 31);
    
    int mainMenu = glutCreateMenu(menu);
    glutAddSubMenu("size", sizeMenu);
    glutAddSubMenu("line algrithm", lineAlgrithm);
    glutAddMenuEntry("clear window", 1);
    glutAddMenuEntry("exit", 0);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
    
}

void clearWindow(void)
{
    drawPositionQueue.clear();
    display();
}

void midpointAlgorithm(void)
{
    
    int x0,y0,x1,y1;
    x0 =  normalizeSpazeToGridSpace(mousePositionQueue[0].x, quadSize);
    y0 =  normalizeSpazeToGridSpace(mousePositionQueue[0].y, quadSize);
    x1 =  normalizeSpazeToGridSpace(mousePositionQueue[1].x, quadSize);
    y1 =  normalizeSpazeToGridSpace(mousePositionQueue[1].y, quadSize);
    
    //do the midpoint algorithm
    bool steep = abs(y1-y0)> abs(x1-x0);
    if(steep)
    {
        swap(x0,y0);
        swap(x1,y1);
    }
    
    if(x0>x1)
    {
        swap(x0,x1);
        swap(y0,y1);
    }
    
    int dx = abs(x1-x0);
    int dy = abs(y1-y0);
    int p = 2*dy - dx;
    int twoDy = 2*dy;
    int twoDyMinusTwoDx =twoDy-2*dx;
    int x=x0;
    int y=y0;
    int ystep = (y0<y1) ? 1 : -1;
    
    //
    vector<float> _color;
    _color.push_back(0);_color.push_back(0);_color.push_back(0);_color.push_back(1);
    
    
   while(x<=x1)
   {
       if(steep)savePoint(y,x,_color);
       else savePoint(x,y,_color);
       x++;
       if(p<0)
       {
           p+=twoDy;
       }else{
           y+=ystep;
           p+=twoDyMinusTwoDx;
       }
       
       
   }
    
   
}

void savePoint(int x, int y, vector<float> _color)
{
    
    vertex_data new_pos;
    new_pos.v_color = _color;
    new_pos.x = x/(float(quadCount)/2)-quadSize/2;
    new_pos.y = y/(float(quadCount)/2)-quadSize/2;
//    std::cout << "new_pos : " << new_pos.x <<" ," << new_pos.y << " quadSize : " << quadCount <<"\n";
    drawPositionQueue.push_back(new_pos);
}

void antialiasingAlgorithm(void)
{
    int x0,y0,x1,y1;
     x0 =  normalizeSpazeToGridSpace(mousePositionQueue[0].x, quadSize);
     y0 =  normalizeSpazeToGridSpace(mousePositionQueue[0].y, quadSize);
     x1 =  normalizeSpazeToGridSpace(mousePositionQueue[1].x, quadSize);
     y1 =  normalizeSpazeToGridSpace(mousePositionQueue[1].y, quadSize);
     
     //do the midpoint algorithm
     bool steep = abs(y1-y0)> abs(x1-x0);
     if(steep)
     {
         swap(x0,y0);
         swap(x1,y1);
     }
     
     if(x0>x1)
     {
         swap(x0,x1);
         swap(y0,y1);
     }
     
     int dx = abs(x1-x0);
     int dy = abs(y1-y0);
     float m = float(dy)/float(dx);
     int p = 2*dy - dx;
     int twoDy = 2*dy;
     int twoDyMinusTwoDx =twoDy-2*dx;
     int x=x0;
     int y=y0;
     int ystep = (y0<y1) ? 1 : -1;
     
    while(x<=x1)
    {
        
        float yf = x * m;
        float yi = floor(yf);
        float f = abs(yf-yi);
        
        vector<float> color1;
        vector<float> color2;
        if(color1.size() * color2.size() == 0){
            for(int i=0; i<4; i++)
            {
                color1.push_back(f);
                color2.push_back(1-f);
            }
        }else{
            for(int i=0; i<4; i++)
            {
                color1[i] = f;
                color2[i] = 1-f;
            }
        }
        
        
        if(steep){
            savePoint(y,x,color1);
            savePoint(y,x+1,color2);
        }
        else {
            savePoint(x,y,color1);
            savePoint(x,y+1,color2);
        }
        
        x++;
        if(p<0)
        {
            p+=twoDy;
        }else{
            y+=ystep;
            p+=twoDyMinusTwoDx;
        }
        
    }
     
}

void motionFunc(int x, int y)
{
    float half_width = glutGet(GLUT_WINDOW_WIDTH)/2;
    float half_height= glutGet(GLUT_WINDOW_HEIGHT)/2;
    
    mousePosition[0]=  (x-half_width)/half_width;
    mousePosition[1]=  (half_height-y)/half_width;
    display();
}
int normalizeSpazeToGridSpace(float input, float quadsize){
    float deviation =( input < 0) ? -quadsize/2 : quadsize/2;
    return int(( input + deviation)/quadsize);
}

