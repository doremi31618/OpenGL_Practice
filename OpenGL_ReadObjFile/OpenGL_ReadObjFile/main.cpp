//
//  main.cpp
//  OpenGL_ReadObjFile
//
//  Created by 詹閔翔 on 2020/4/19.
//  Copyright © 2020 詹閔翔. All rights reserved.
//  程式參考：https://blog.csdn.net/qq_27161673/article/details/71915177
//

//需要包含的头文件
#include "model.hpp"

#define WINDOW_HEIGHT 300
#define WINDOW_WIDTH 500
#define PI 3.14159265

//摄像机离物体的距离
float G_fDistance = 0;
//物体的旋转角度
float G_fAngle_horizon = 0.0;
float G_fAngle_vertical = 0.0f;

vector<GLfloat > center(3);

GLfloat xx = 0.0f;
GLfloat yy = 0.0f;
GLfloat zz = 0.0f;

//设定读入的模型以及显示的方式
bool redraw = true;
bool axis = true;
bool boundingBox = true;
Objmodel *obj1 = new Objmodel();
string path = "/Users/zhanminxiang/Desktop/OpenGL/OpenGL_Course/OpenGL_ReadObjFile/OpenGL_ReadObjFile/obj/octahedron.obj";
GLint mode1 = GL_LINE;
GLint mode2 = GL_TRIANGLES;

//light0参数
GLfloat Vp0[] = { 0.0,0.0,0.0,1.0 };    //光源环境光位置
GLfloat Va0[] = { 0.8,0.8,0.8,1 };       //光源环境光强度数组
GLfloat Vd0[] = { 0.6,0.6,0.6,1 };       //光源散射光强度数组
GLfloat Vs0[] = { 0.5,0.5,0.5,1 };       //光源镜面反射光强度数组

GLfloat color[]  = { 1.0,1.0,1.0};

////////////////////////////////////////////////
void myinit(void);
void myReshape(GLsizei w, GLsizei h);
void display(void);

//menu functions
void GeneralMenu(int);
void FileMenu(int);
void RenderMenu(int);
void ColorMenu(int);
void createMenu(void);

//deal with keyboard input
void processSpecialKeys(int key, int x, int y);
void processNormalKeys(unsigned char key, int x, int y);

//custom functions
void RandomColor();
void ConstantColor();
void cube_generator(float x, float y, float z);
void line_generator(float x, float y,float z, float x2, float y2, float z2);
void settingCameraFrustrum(Objmodel* _model);

//transform matrix
void Translate(GLfloat x, GLfloat y, GLfloat z);
void RotateX(GLfloat x);
void RotateY(GLfloat y);
void RotateZ(GLfloat z);
void RotateArbitaryAxis(GLfloat x, GLfloat y, GLfloat z, GLfloat angle);
void Scale(GLfloat x, GLfloat y, GLfloat z);

int main(int argc, char* argv[])
{
    glutInit(&argc, argv);


    cout << "introduction：" << endl;
    cout << "小键盘1、2、3、4切换模型\n";
    cout << "键盘q(Q)、w(W)、e(E)切换模型显示方式\n";
    cout << "键盘↑、↓、←、→控制模型旋转\n";
    cout << "键盘a(A)控制视点的远近\n";



    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);


    //设定OPENGL窗口位置和大小
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("OpenGL");

    
    myinit();
    glutReshapeFunc(myReshape);
    glutSpecialFunc(processSpecialKeys);
    glutKeyboardFunc(processNormalKeys);
    glutDisplayFunc(display);
    createMenu();
    glutMainLoop();

    return 0;
}


void myinit(void)
{
    //your initialization code
    glEnable(GL_DEPTH_TEST);

    glShadeModel(GL_SMOOTH);
//    glShadeModel(GL_FLAT);
    


}

void myReshape(GLsizei w, GLsizei h)
{
    //设定视区
    glViewport(0, 0, w, h);

    //设定透视方式
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(80.0, 1.0*(GLfloat)w/(GLfloat)h, 0.1, 3000.0);

}


void display(void)
{

   
    
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    
    //绘制物体
    if (redraw) {       //只读一遍
        obj1->readFile(path);//讀取物體
        center = obj1->getCenter();//取得模型中心
        G_fDistance = 0;//每次換模型都重設相機距離、角度
        G_fAngle_horizon = 0;
        G_fAngle_vertical = 0;
        redraw = false;
    }
    
    //reset camera frustum base on model bouding box
    //p.s. reset matrix mode to projection
    settingCameraFrustrum(obj1);
    
    //reset matrix mode to GL_model view
    glMatrixMode(GL_MODELVIEW);// GLmatrix mode : GL_PROJECTION GL_MODELVIEW GL_TEXTURE
    glLoadIdentity();
    Translate(-center[0],-center[1], -center[2]);
    
    GLfloat zOffset = ((obj1->one0fcatercorner[2])-(obj1->other0fcatercorner[2]));//zOffset +
    gluLookAt(0, 0,  zOffset , 0, 0, 0, 0, 1, 0);

    if(axis)
    {
        line_generator(500.0,0.0,0.0,-500.0,0.0,0.0);
        line_generator(0.0,500.0, 0, 0, -500.0,0);
        line_generator(0.0,0.0,500.0, 0.0,0.0,-500.0);
    }
    Translate(0,0,G_fDistance );
    RotateY(G_fAngle_horizon);
    RotateX(G_fAngle_vertical);
    obj1->showObj(mode1,mode2, color); //参数为模型的显示方式  GL_POINTS GL_LINES    GL_TRIANGLES ,glColor
    if(boundingBox)
        obj1->drawBox();
    
    
    glFlush();
    glutSwapBuffers();//交换前后缓冲区
    
}

void settingCameraFrustrum(Objmodel* _model)
{
   
    GLfloat maximumValue = 1;
    for(int i=0; i<_model->one0fcatercorner.size(); i++){
        if(_model->one0fcatercorner[i] > maximumValue){
            maximumValue = (_model->one0fcatercorner[i] - (_model)->other0fcatercorner[i])/2;
            }
    }
    
   GLfloat zOffset = ((obj1->one0fcatercorner[2])-(obj1->other0fcatercorner[2]));//zOffset +
   gluLookAt(0, 0,  maximumValue , 0, 0, 0, 0, 1, 0);


    
}
void GeneralMenu(int index)
{
    switch(index)
    {
        case 1://bounding box
            //open bounding box
            boundingBox = !boundingBox;
            break;
            
        case 2://axis
            //open axis
            axis = !axis;
            break;
    }
    glutPostRedisplay();

    
}

//定義按鈕功能
void FileMenu(int index)
{
    switch(index)
    {
            case 1://lamp
                path = "/Users/zhanminxiang/Desktop/OpenGL/OpenGL_Course/OpenGL_ReadObjFile/OpenGL_ReadObjFile/obj/lamp.obj";
                    redraw = true; obj1->~Objmodel(); obj1 = new Objmodel(); break;
            case 2://gourd
                    path = "/Users/zhanminxiang/Desktop/OpenGL/OpenGL_Course/OpenGL_ReadObjFile/OpenGL_ReadObjFile/obj/gourd.obj";
                    redraw = true; obj1->~Objmodel(); obj1 = new Objmodel(); break;
            case 3://octahedron
                path = "/Users/zhanminxiang/Desktop/OpenGL/OpenGL_Course/OpenGL_ReadObjFile/OpenGL_ReadObjFile/obj/octahedron.obj";
                    redraw = true; obj1->~Objmodel(); obj1 = new Objmodel(); break;
            case 4://teapot
                path = "/Users/zhanminxiang/Desktop/OpenGL/OpenGL_Course/OpenGL_ReadObjFile/OpenGL_ReadObjFile/obj/teapot.obj";
                    redraw = true; obj1->~Objmodel(); obj1 = new Objmodel(); break;
            case 5://teddy
                path = "/Users/zhanminxiang/Desktop/OpenGL/OpenGL_Course/OpenGL_ReadObjFile/OpenGL_ReadObjFile/obj/teddy.obj";
                redraw = true; obj1->~Objmodel(); obj1 = new Objmodel(); break;
        }
    std::cout << " choose file : " << path <<" \n";
    glutPostRedisplay();
}

void RenderMenu(int index)
{
    
    switch(index)
    {
            case 1://Point
                mode2 = GL_POINTS; break;
            case 2://Line
                mode2 = GL_TRIANGLES; mode1 = GL_LINE; break;
            case 3://Face
                mode1 = GL_FILL; mode2 = GL_TRIANGLES; break;
    }
    glutPostRedisplay();
}

void ColorMenu(int index)
{
    std::cout << "color index " <<  index <<"\n";
    switch(index)
    {
            case 0://white
                ConstantColor();
                break;
            case 1://random color
                RandomColor();
                break;
            default:
                ConstantColor();
                break;
    }
    glutPostRedisplay();
}

void createMenu()
{
    ///setting all the submenus
    //choose model file
    int File = glutCreateMenu(FileMenu);
    glutAddMenuEntry("lamp", 1);
    glutAddMenuEntry("gourd", 2);
    glutAddMenuEntry("octahedron", 3);
    glutAddMenuEntry("teapot", 4);
    glutAddMenuEntry("teddy", 5);
    
    //choose color mode
    int ColorMode = glutCreateMenu(ColorMenu);
    glutAddMenuEntry("White", 0);
    glutAddMenuEntry("Random Color", 1);
    
    //choose render mode
    int _RenderType = glutCreateMenu(RenderMenu);
    glutAddMenuEntry("point", 1);
    glutAddMenuEntry("line", 2);
    glutAddMenuEntry("face", 3);
    
    ///setting main menu
    //create sub menu
    int menu = glutCreateMenu(GeneralMenu);
    glutAddSubMenu("File", File);
    glutAddSubMenu("Color", ColorMode);
    glutAddSubMenu("Render Type", _RenderType);
    glutAddMenuEntry("Bounding Box", 1);//if use Bounding Box
    glutAddMenuEntry("Axis", 2);//is use Axis
    
    glutAttachMenu(GLUT_RIGHT_BUTTON);

}

void processSpecialKeys(int key, int x, int y)
{
    switch (key) {
    case GLUT_KEY_LEFT:
        G_fAngle_horizon -= 10.0f;
        break;
    case GLUT_KEY_RIGHT:
        G_fAngle_horizon += 10.0f;
        break;
    case GLUT_KEY_UP:
        G_fAngle_vertical -= 10.0f;
        break;
    case GLUT_KEY_DOWN:
        G_fAngle_vertical += 10.0f;
        break;
    }
    glutPostRedisplay();
}

void processNormalKeys(unsigned char key, int x, int y)
{
    switch (key) {
    case 97:    //"a"
        G_fDistance -= 2.0f;
        break;
    case 65:        //"A"
        G_fDistance += 2.0f;
        break;
    case 27:    //"esc"
        exit(0);
    case '1':
        path = "/Users/zhanminxiang/Desktop/OpenGL/OpenGL_Course/OpenGL_ReadObjFile/OpenGL_ReadObjFile/obj/lamp.obj";
        redraw = true; obj1->~Objmodel(); obj1 = new Objmodel(); break;
    case '2':
        path = "/Users/zhanminxiang/Desktop/OpenGL/OpenGL_Course/OpenGL_ReadObjFile/OpenGL_ReadObjFile/obj/octahedron.obj";
        redraw = true; obj1->~Objmodel(); obj1 = new Objmodel(); break;
    case '3':
        path = "/Users/zhanminxiang/Desktop/OpenGL/OpenGL_Course/OpenGL_ReadObjFile/OpenGL_ReadObjFile/obj/gourd.obj";
        redraw = true; obj1->~Objmodel(); obj1 = new Objmodel(); break;
    case '4':
        path = "/Users/zhanminxiang/Desktop/OpenGL/OpenGL_Course/OpenGL_ReadObjFile/OpenGL_ReadObjFile/obj/teddy.obj";
        redraw = true; obj1->~Objmodel(); obj1 = new Objmodel(); break;
    case '5':
        path = "/Users/zhanminxiang/Desktop/OpenGL/OpenGL_Course/OpenGL_ReadObjFile/OpenGL_ReadObjFile/obj/teapot.obj";
        redraw = true; obj1->~Objmodel(); obj1 = new Objmodel(); break;
    case 'q':
    case 'Q':
        mode2 = GL_POINTS; break;
    case 'w':
    case 'W':
        mode2 = GL_TRIANGLES; mode1 = GL_LINE; break;
    case 'e':
    case 'E':
        mode1 = GL_FILL; mode2 = GL_TRIANGLES; break;
    }
    glutPostRedisplay();
}
void ConstantColor()
{
    color[0] = color[1] =  color[2] = 1.0;
}

void RandomColor()
{
    
    srand( time(NULL) );
    color[0] = rand() / (RAND_MAX + 1.0);
    color[1]= rand() / (RAND_MAX + 1.0);
    color[2] = rand() / (RAND_MAX + 1.0);
    std::cout << "random color : (" << color[0] <<", " <<  color[1] <<", " <<color[2] << " )\n" ;

}

void line_generator(float x, float y,float z, float x2, float y2, float z2)
{
    glBegin(GL_LINES);
        
        float r = x*x2 !=0 ? 1 : 0;
        float g = y*y2 !=0 ? 1 : 0;
        float b = z*z2 !=0 ? 1 : 0;
    
        glColor3f(r,g,b);
        glVertex3f( x, y,z);
        glVertex3f(x2,y2,z2);
    
    glEnd();
}

void Translate(GLfloat x, GLfloat y, GLfloat z){
    GLfloat translate_matrix[16];
    translate_matrix[0]=1;translate_matrix[4]=0;translate_matrix[8]=0;translate_matrix[12]=x;
    translate_matrix[1]=0;translate_matrix[5]=1;translate_matrix[9]=0;translate_matrix[13]=y;
    translate_matrix[2]=0;translate_matrix[6]=0;translate_matrix[10]=1;translate_matrix[14]=z;
    translate_matrix[3]=0;translate_matrix[7]=0;translate_matrix[11]=0;translate_matrix[15]=1;
    glMultMatrixf(translate_matrix);
}

void RotateX(GLfloat x){
    GLfloat rotateX_matrix[16];
    rotateX_matrix[0]=1;rotateX_matrix[4]=0;rotateX_matrix[8]=0;rotateX_matrix[12]=0;
    rotateX_matrix[1]=0;rotateX_matrix[5]=cos(x*PI/180);rotateX_matrix[9]=-sin(x*PI/180);rotateX_matrix[13]=0;
    rotateX_matrix[2]=0;rotateX_matrix[6]=sin(x*PI/180);rotateX_matrix[10]=cos(x*PI/180);rotateX_matrix[14]=0;
    rotateX_matrix[3]=0;rotateX_matrix[7]=0;rotateX_matrix[11]=0;rotateX_matrix[15]=1;
    
    glMultMatrixf(rotateX_matrix);
}

void RotateY(GLfloat y){
    GLfloat rotateY_matrix[16];
    rotateY_matrix[0]= cos(y*PI/180);rotateY_matrix[4]=0;rotateY_matrix[8]=sin(y*PI/180);rotateY_matrix[12]=0;
    rotateY_matrix[1]=0;rotateY_matrix[5]=1;rotateY_matrix[9]=0;rotateY_matrix[13]=0;
    rotateY_matrix[2]=-sin(y*PI/180);rotateY_matrix[6]=0;rotateY_matrix[10]=cos(y*PI/180);rotateY_matrix[14]=0;
    rotateY_matrix[3]=0;rotateY_matrix[7]=0;rotateY_matrix[11]=0;rotateY_matrix[15]=1;
    glMultMatrixf(rotateY_matrix);
}
void RotateZ(GLfloat z){
    GLfloat rotateZ_matrix[16];
    rotateZ_matrix[0]=cos(z*PI/180);rotateZ_matrix[4]=-sin(z*PI/180);rotateZ_matrix[8]=0;rotateZ_matrix[12]=0;
    rotateZ_matrix[1]=sin(z*PI/180);rotateZ_matrix[5]= cos(z*PI/180);rotateZ_matrix[9]=0;rotateZ_matrix[13]=0;
    rotateZ_matrix[2]=0;rotateZ_matrix[6]=0;rotateZ_matrix[10]=1;rotateZ_matrix[14]=0;
    rotateZ_matrix[3]=0;rotateZ_matrix[7]=0;rotateZ_matrix[11]=0;rotateZ_matrix[15]=1;
     glMultMatrixf(rotateZ_matrix);
}
void RotateArbitaryAxis(GLfloat x, GLfloat y, GLfloat z, GLfloat angle){
    //perform normalize
    GLfloat length = sqrt(pow(x,2) + pow(y,2) + pow(z,2));
    x = x/length;
    y = y/length;
    z = z/length;
    
    //declare matrix & cos & sin
    GLfloat arbitary_axis_rotate_matrix[16];
    GLfloat Cos = cos(angle*PI/180);
    GLfloat Sin = sin(angle*PI/180);
    
    float x_pow_2 = pow(x,2);
    float y_pow_2 = pow(y,2);
    float z_pow_2 = pow(z,2);
    
    float one_minus_cos = 1 - Cos;
    
    //matrix content start
    
    //row 1
    arbitary_axis_rotate_matrix[0] = Cos + one_minus_cos*x_pow_2;
    arbitary_axis_rotate_matrix[4] = one_minus_cos*x*y - Sin*z;
    arbitary_axis_rotate_matrix[8] = one_minus_cos*x*z + Sin*y;
    arbitary_axis_rotate_matrix[12] = 0;
    
    //row2
    arbitary_axis_rotate_matrix[1] = one_minus_cos*x*y + Sin*z;
    arbitary_axis_rotate_matrix[5] = Cos + one_minus_cos*y_pow_2;;
    arbitary_axis_rotate_matrix[9] = one_minus_cos*y*z - Sin*x;
    arbitary_axis_rotate_matrix[13] = 0;
    
    //row3
    arbitary_axis_rotate_matrix[2] = one_minus_cos*z*x - Sin*y;
    arbitary_axis_rotate_matrix[6] = one_minus_cos*z*y + Sin*x;
    arbitary_axis_rotate_matrix[10] = Cos + one_minus_cos*z_pow_2;
    arbitary_axis_rotate_matrix[14] = 0;
    
    //row4
    arbitary_axis_rotate_matrix[3] = 0;
    arbitary_axis_rotate_matrix[7] = 0;
    arbitary_axis_rotate_matrix[11] = 0;
    arbitary_axis_rotate_matrix[15] = 1;
    
    //matrix content end
    glMultMatrixf(arbitary_axis_rotate_matrix);
}
void Scale(GLfloat x, GLfloat y, GLfloat z){
    GLfloat scale_matrix[16];
    scale_matrix[0]=x;scale_matrix[4]=0;scale_matrix[8]=0;scale_matrix[12]=0;
    scale_matrix[1]=0;scale_matrix[5]=y;scale_matrix[9]=0;scale_matrix[13]=0;
    scale_matrix[2]=0;scale_matrix[6]=0;scale_matrix[10]=z;scale_matrix[14]=0;
    scale_matrix[3]=0;scale_matrix[7]=0;scale_matrix[11]=0;scale_matrix[15]=1;
    glMultMatrixf(scale_matrix);
}

void cube_generator(float x, float y, float z){
    glBegin(GL_TRIANGLES);
         float width=4;
         float height=4;
         float depth=4;
    
        //    v6------v5
        //   / |      /|
        //  v1-------v0|
        //  |  |     | |
        //  |  v7----|-v4
        //  | /      |/
        //  v2-------v3
    
         
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

