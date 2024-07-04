#include "glad.h"
#include <GLFW/glfw3.h>
#include <iostream>

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Left indicator", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) 
    {
        glfwTerminate();
        return -1;
    }

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        glColor3f(0.f, 1.f, 0.f);

        // hollow indicator
        glLineWidth(10.f);
        glBegin(GL_LINE_LOOP);
        glVertex2f(-0.5f, 0.0f);
        glVertex2f(0.0f, 0.5f);
        glVertex2f(0.0f, 0.25f);
        glVertex2f(0.5f, 0.25f);
        glVertex2f(0.5f, -0.25f);
        glVertex2f(0.0f, -0.25f);
        glVertex2f(0.0f, -0.5f);

        /* normal indicator
        glBegin(GL_TRIANGLES);
        glVertex2f(-0.5f, 0.0f); //part 1
        glVertex2f(0.0f, 0.5f);  //part 1
        glVertex2f(0.0f, -0.5f); //part 1
        glVertex2f(0.0f, 0.25f); //part 2.1
        glVertex2f(0.0f, -0.25f);//part 2.1
        glVertex2f(0.5f, 0.25f); //part 2.1
        glVertex2f(0.5f, -0.25f);//part 2.2
        glVertex2f(0.5f, 0.25f); //part 2.2
        glVertex2f(0.0f, -0.25f);//part 2.2*/

        glEnd();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}