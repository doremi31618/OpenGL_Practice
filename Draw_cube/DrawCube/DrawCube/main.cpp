/***C/C++  http://bbs.csdn.net/topics/100091584***/
#include <stdio.h>
#include <stdlib.h>
/***freeglut***/
//#include <OpenGL/gl.h>
//#include <GLUT/glut.h>


using namespace std;
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GLFW/glfw3.h>
#include <iostream>

void ChangeSize(int , int );
void RenderScene(void);
void buildPopupMenu();
void cube_generator(float, float, float);
void line_generator(float, float, float, float, float, float);
void keyboardFunc(unsigned char , int , int );
float _x =0;
float _y=0;
float _z=0;

int main(int argc, char** argv)
{
    //create a new glut
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB );
    glutInitWindowSize(400,400);
    glutInitWindowPosition(600,80);
    glutCreateWindow("Simple Rectangle");
    glutKeyboardFunc(keyboardFunc);
    
    buildPopupMenu();
    
    //register callbacks function
    glutReshapeFunc(ChangeSize);  //reshape window size (每次調整視窗大小都會呼叫）
    glutDisplayFunc(RenderScene); //render 資訊寫在這裡
    
    
    glutMainLoop(); //啟動所有call back function
   return 0;
}

void menuSelect(int option)
{
    switch(option)
    {
    case 0:
        glShadeModel(GL_FLAT);
        glutPostRedisplay();
        break;
    case 1:
        glShadeModel(GL_SMOOTH);
        glutPostRedisplay();
        break;
    default:
        break;
    }
}
void keyboardFunc(unsigned char key, int x, int y)
{
    std::cout << "pressed button :" << key << " " << _x << " "<< _y  << " " << _z << "\n";
    
    switch (key) {
        case 'a':
            _x -= 1;
            break;
        case 'd':
            _x += 1;
            break;
            
        case 'w':
            _y+=1;
            break;
            
        case 's':
            _y-=1;
            break;
            
        case 'z':
            _z+=1;
            break;
        case 'x':
            _z-=1;
            break;
        default:
            _x = _y = _z=0;
            break;
    }
    RenderScene();
}
void buildPopupMenu()
{
    int menu_id;
    menu_id = glutCreateMenu(menuSelect);
    glutAddMenuEntry("Flat",0);
    glutAddMenuEntry("Smooth",1);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

}

void ChangeSize(int w, int h)
{
   printf("Window Size= %d X %d\n",w,h);
    //viewport = canvas
   glViewport(0, 0, w, h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(-10,10,-10,10,-10,10);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}
void RenderScene(void)
{
   
   glClearColor(1.0, 1.0, 1.0, 1.0);
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glDepthFunc(GL_LESS);
   glEnable(GL_DEPTH_TEST);
   
   
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   
    
//    glm::mat4 CameraMatrix = glm::LookAt(
//        cameraPosition, // the position of your camera, in world space
//        cameraTarget,   // where you want to look at, in world space
//        upVector        // probably glm::vec3(0,1,0), but (0,-1,0) would make you looking upside-down, which can be great too
//    );
    
    gluLookAt(4,3,4,0,0,0,0,1,0);

//   gluLookAt(_x + 4,_y +3,_z + 4,_x,_y,_z,0,1,0);
    //create shape
    line_generator(10.0,0.0,0.0,-10.0,0.0,0.0);
    line_generator(0.0,10.0, 0, 0, -10.0,0);
    line_generator(0.0,0.0,10.0, 0.0,0.0,-10.0);
    cube_generator(_x,_y,_z);
  
    
    glutSwapBuffers();
}

void line_generator(float x, float y,float z, float x2, float y2, float z2)
{
    glBegin(GL_LINES);
        
        glVertex3f( x, y,z);
        glVertex3f(x2,y2,z2);
    glEnd();
}

void cube_generator(float x, float y, float z){
    glBegin(GL_TRIANGLES);
         float width=4;
         float height=4;
         float depth=4;
         //point 1 glVertex3f( x+width/2, y+height/2, z+depth/2 );
         //point 2 glVertex3f( x+width/2, y+height/2, z-depth/2 );
         //point 3 glVertex3f( x+width/2, y-height/2, z+depth/2 );
         //point 4 glVertex3f( x+width/2, y-height/2, z-depth/2 );
         //point 5 glVertex3f( x-width/2, y+height/2, z+depth/2 );
         //point 6 glVertex3f( x-width/2, y+height/2, z-depth/2 );
         //point 7 glVertex3f( x-width/2, y-height/2, z+depth/2 );
         //point 8 glVertex3f( x-width/2, y-height/2, z-depth/2 );
        //face 6
        glColor3f( 0, 1, 0);glVertex3f( x-width/2, y-height/2, z+depth/2 );
        glColor3f( 0, 1, 0);glVertex3f( x-width/2, y-height/2, z-depth/2 );
        glColor3f( 0, 1, 0);glVertex3f( x+width/2, y-height/2, z-depth/2 );
        
        glColor3f( 0, 1, 0);glVertex3f( x-width/2, y-height/2, z+depth/2 );
        glColor3f( 0, 1, 0);glVertex3f( x+width/2, y-height/2, z-depth/2 );
        glColor3f( 0, 1, 0);glVertex3f( x+width/2, y-height/2, z+depth/2 );
        
         
    
        //face 2
        glColor3f( 1, 1, 0);glVertex3f( x+width/2, y-height/2, z-depth/2 );
        glColor3f( 1, 1, 0);glVertex3f( x-width/2, y-height/2, z-depth/2 );
        glColor3f( 1, 1, 0);glVertex3f( x+width/2, y+height/2, z-depth/2 );

        glColor3f( 1, 1, 0);glVertex3f( x-width/2, y+height/2, z-depth/2 );
        glColor3f( 1, 1, 0);glVertex3f( x+width/2, y+height/2, z-depth/2 );
        glColor3f( 1, 1, 0);glVertex3f( x-width/2, y-height/2, z-depth/2 );
        
        //face 3
        glColor3f( 0, 1, 1);glVertex3f( x-width/2, y+height/2, z+depth/2 );
        glColor3f( 0, 1, 1);glVertex3f( x-width/2, y+height/2, z-depth/2 );
        glColor3f( 0, 1, 1);glVertex3f( x-width/2, y-height/2, z+depth/2 );

        glColor3f( 0, 1, 1);glVertex3f( x-width/2, y-height/2, z+depth/2 );
        glColor3f( 0, 1, 1);glVertex3f( x-width/2, y+height/2, z-depth/2 );
        glColor3f( 0, 1, 1);glVertex3f( x-width/2, y-height/2, z-depth/2 );
    
        //face 4
        glColor3f( 0, 0, 1);glVertex3f( x+width/2, y+height/2, z+depth/2 );
        glColor3f( 0, 0, 1);glVertex3f( x-width/2, y+height/2, z+depth/2 );
        glColor3f( 0, 0, 1);glVertex3f( x-width/2, y-height/2, z+depth/2 );

        glColor3f( 0, 0, 1);glVertex3f( x+width/2, y+height/2, z+depth/2 );
        glColor3f( 0, 0, 1);glVertex3f( x-width/2, y-height/2, z+depth/2 );
        glColor3f( 0,0, 1);glVertex3f( x+width/2, y-height/2, z+depth/2 );
    
        //face 5
        glColor3f( 1, 0, 0);glVertex3f( x+width/2, y+height/2, z-depth/2 );
        glColor3f( 1, 0, 0);glVertex3f( x-width/2, y+height/2, z+depth/2 );
        glColor3f( 1, 0, 0);glVertex3f( x+width/2, y+height/2, z+depth/2 );
        
        glColor3f( 1, 0, 0);glVertex3f( x+width/2, y+height/2, z-depth/2 );
        glColor3f( 1, 0, 0);glVertex3f( x-width/2, y+height/2, z-depth/2 );
        glColor3f( 1, 0, 0);glVertex3f( x-width/2, y+height/2, z+depth/2 );
        
        //face 1
         glColor3f( 1, 0, 1);glVertex3f( x+width/2, y+height/2, z-depth/2 );
         glColor3f( 1, 0, 1);glVertex3f( x+width/2, y+height/2, z+depth/2 );
         glColor3f( 1, 0, 1);glVertex3f( x+width/2, y-height/2, z+depth/2 );

        glColor3f( 1, 0, 1);glVertex3f( x+width/2, y+height/2, z-depth/2 );
        glColor3f( 1, 0, 1);glVertex3f( x+width/2, y-height/2, z+depth/2 );
        glColor3f( 1, 0, 1);glVertex3f( x+width/2, y-height/2, z-depth/2 );
        
    
        
      glEnd();
    
}
