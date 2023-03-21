#include "env_world.h"
#include "constants.h"

#include <GLFW/glfw3.h>
#include <iostream>
using namespace std;
int main(void) {
    env_world World;

    GLFWwindow* window;

    double lastTime = glfwGetTime();
    int nbFrames = 0;
    if (!glfwInit()) //initialisation de GL
        return -1;
    cout.precision(17);

    //Création de la fenètre
    window = glfwCreateWindow(640, 640, "TIPE", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    //Boucle principale
    while (!glfwWindowShouldClose(window)){
        double currentTime = glfwGetTime();
        nbFrames++;
        if (currentTime - lastTime >= 1.0) { // If last prinf() was more than 1 sec ago
            // printf and reset timer
            cout << 1000.0 / double(nbFrames) << endl;
            nbFrames = 0;
            lastTime += 1.0;
        }


        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        World.print_world(); //afficher la map
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}