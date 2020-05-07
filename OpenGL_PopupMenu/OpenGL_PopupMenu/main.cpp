//
//  main.cpp
//  OpenGL_PopupMenu
//
//  Created by 詹閔翔 on 2020/5/6.
//  Copyright © 2020 詹閔翔. All rights reserved.
//
#include <GLUT/glut.h>
#include <iostream>

#define WIDTH 640
#define HEIGHT 480

void init(void);
void display(void);


void createMenu(void);
void mainMenu(int index);
void subMenu1(int index);
void subMenu2(int index);
void subMenu3(int index);

int main(int argc, char * argv[]) {
    
    //init window
    glutInit(&argc, argv);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutCreateWindow("Pop-up Menu");
    
    //callback event
    createMenu();
    
    init();
    glutDisplayFunc(display);
    
    //enter main loop
    glutMainLoop();
    
    return 0;
    
}
void init(){}
void menu(int index){}

void mainMenu(int index)
{
    switch(index)
    {
            case 1:
                break;
            case 2:
                break;
        }
}

//定義按鈕功能
void subMenu1(int index)
{
    switch(index)
    {
            case 1:
                break;
            case 2:
                break;
            case 3:
                break;
        }
}

void subMenu2(int index)
{
    
    switch(index)
    {
            case 1:
                break;
            case 2:
                break;
            case 3:
                break;
    }
}

void subMenu3(int index)
{
    switch(index)
    {
            case 0:
                break;
            case 1:
                break;
            default:
                break;
    }
}


void createMenu()
{
     ///setting all the submenus
       //create sub-menu 1
       int submenu1 = glutCreateMenu(subMenu1);
       glutAddMenuEntry("sub1_1", 1);//add option of sub manu 1
       glutAddMenuEntry("sub1_2", 2);
       glutAddMenuEntry("sub1_3", 3);
       
       //create sub-menu 2
       int submenu2 = glutCreateMenu(subMenu2);
       glutAddMenuEntry("sub2_1", 1);//add option of sub manu 2
       glutAddMenuEntry("sub2_2", 2);
       glutAddMenuEntry("sub2_3", 3);
       
       //create sub-menu 3
       int submenu3 = glutCreateMenu(subMenu3);
       glutAddMenuEntry("sub3_1", 1);//add option of sub manu 3
       glutAddMenuEntry("sub3_2", 2);
       glutAddMenuEntry("sub3_3", 3);
       
       ///create main menu
       int menu = glutCreateMenu(mainMenu);
       glutAddSubMenu("sub1", submenu1);//add submenu to main menu
       glutAddSubMenu("sub2", submenu2);
       glutAddSubMenu("sub3", submenu3);
       glutAddMenuEntry("other1", 1);
       glutAddMenuEntry("other2", 2);
       
       glutAttachMenu(GLUT_RIGHT_BUTTON);
    
}

void display()
{
    glClearColor(0.0,0.0, 0.0, 0.0);
    
    glFlush();
    
}
