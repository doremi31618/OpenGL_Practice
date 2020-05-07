//
//  main.cpp
//  OpenGL_ViewFrustumAdjustment
//
//  Created by 詹閔翔 on 2020/5/5.
//  Copyright © 2020 詹閔翔. All rights reserved.
//

#include <iostream>
#include <GLUT/glut.h>

#define WINDOW_WIDTH 250
#define WINDOW_HEIGHT 250

void display(void);
void init(void);

int main(int argc, char * argv[]) {
    //init the glut
    glutInit(&argc, argv);
    
    //setting display mode
    glutInitDisplayMode(GLUT_DEPTH | GLUT_SINGLE | GLUT_RGB);
    
    //setting the window size
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    
    //set the window position
    glutInitWindowPosition(100,100);
    
    //create the window
    glutCreateWindow("First window");
   
    //call init
    init();
    
    //call display function
    glutDisplayFunc(display);
    
    //Enter the GLUT event loop
    glutMainLoop();
    
    return 0;
}

//like processing setting
void init(){
    //setting background color
    glClearColor(0, 0, 0, 0);

}

//like processing draw
void display(){
    //background color(0,0,0,0);
    glClearColor(0, 0, 0, 0);
    
    
    glFlush();
    
}
