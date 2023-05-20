#pragma once

#include <GLFW/glfw3.h>
#include <assert.h>
#include <cmath>
#include <iostream>

#define WINDOW_SIZE 640

GLFWwindow* graphic_init(){
    GLFWwindow* window;
    if(!glfwInit())
        exit(-1);
    window = glfwCreateWindow(WINDOW_SIZE, WINDOW_SIZE, "TIPE", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(-1);
    }
    glfwMakeContextCurrent(window);

    return window;
}

void graphic_exit(){
    glfwTerminate();
}

void drawFilledCircle(GLfloat x, GLfloat y, GLfloat radius){
	int i;
	int triangleAmount = 20; //# of triangles used to draw circle
	
	//GLfloat radius = 0.8f; //radius
	GLfloat twicePi = 2.0f * M_PI;
	
	glBegin(GL_TRIANGLE_FAN);
		glVertex2f(x, y); // center of circle
		for(i = 0; i <= triangleAmount;i++) { 
			glVertex2f(
		            x + (radius * cos(i *  twicePi / triangleAmount)), 
			    y + (radius * sin(i * twicePi / triangleAmount))
			);
		}
	glEnd();
}

void draw_screen(state &s, GLFWwindow *window){
    vector<vector<unit>> list_of_U_list = s.unitList_get();
    double x,y;
    glClear(GL_COLOR_BUFFER_BIT);
    for(auto U_list: list_of_U_list){
        for(auto u:U_list){
            x = u.getX()*100/WINDOW_SIZE;
            y = u.getY()*100/WINDOW_SIZE;
            cout << x << " " << y << endl;
            drawFilledCircle(x,y,0.01);
        }
    }
    glfwSwapBuffers(window);
    glfwPollEvents();
}