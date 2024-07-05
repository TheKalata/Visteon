#include <glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <tiny_gltf.h>

//void loadShaders(WindowContext WindowContext)
//{
//
//}

int main(void)
{
    GLFWwindow* window;

    std::string gltfFilename = "../examples/gltf/01_triangle/export/triangle.gltf";

    tinygltf::Model model;
    tinygltf::TinyGLTF loader;
    std::string err, warn;

    bool success = loader.LoadASCIIFromFile(&model, &err, &warn, gltfFilename);
    if(!success)
    {
        std::cerr << "Failed to load glTF file: " << gltfFilename << std::endl;
        std::cerr << "Error: " << err << std::endl;
        std::cerr << "Warning: " << warn << std::endl;
        return 1;
    }

    //GLfloat vertices[] = {
    //    -0.5f, -0.5f, 0.0f, //1. coordinate
    //    0.5f, -0.5f, 0.0f, //2. coordinate
    //    0.0f,  0.5f, 0.0f //3. coordinate
    //};

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

    //GLfloat vertexData[] = {
    //    -0.5f, -0.5f, 0.0f, // positions[0]​
    //    0.5f, -0.5f, 0.0f, // positions[1]​
    //    0.0f, 0.5f, 0.0f, // positions[2]
    //    0.0f, 0.0f, 1.0f, // normals[0]​
    //    0.0f, 0.0f, 1.0f, // normals[1]
    //    0.0f, 0.0f, 1.0f, // normals[2]
    //    0.25f, 0.25f, // texture coordinates[0]​
    //    0.75f, 0.25f, // texture coordinates[1]​
    //    0.5f, 0.75f // texture coordinates[2]​
    //};

    GLfloat vertexPositionData[] = {
        -0.5f, -0.5f, 0.0f, // positions[0]​
        0.5f, -0.5f, 0.0f, // positions[1]​
        0.0f, 0.5f, 0.0f, // positions[2]​
    };

    GLfloat vertexNormalData[] = {
        0.0f, 0.0f, 1.0f, // normals[0]​
        0.0f, 0.0f, 1.0f, // normals[1]​
        0.0f, 0.0f, 1.0f, // normals[2]​
    };

    GLfloat vertexTexCoordData[] = {
        0.25f, 0.25f, // texture coordinates[0]​
        0.75f, 0.25f, // texture coordinates[1]​
        0.5f, 0.75f // texture coordinates[2]​
    };

    //uint32_t gltfPositionIndex = model.meshes[0].primitives[0].attributes["POSITION"];
    //uint32_t gltfBufferIndex = model.bufferViews[gltfPositionIndex].buffer;
    //unsigned char* gltfBufferData = model.buffers[gltfBufferIndex].data.data();

    //float* vertices = (float*)gltfBufferData;

    //for(int i = 0; i < 9; i++)
    //{
    //    std::cout << vertices[i] << std::endl;
    //}

    GLuint vertexBuffer0 = 0;
    glGenBuffers(1, &vertexBuffer0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer0);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositionData), vertexPositionData, GL_STATIC_DRAW);

    GLuint vertexBuffer1 = 0;
    glGenBuffers(1, &vertexBuffer1);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer1);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexNormalData), vertexNormalData, GL_STATIC_DRAW);

    GLuint vertexBuffer2 = 0;
    glGenBuffers(1, &vertexBuffer2);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexTexCoordData), vertexTexCoordData, GL_STATIC_DRAW);
    

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        //Binding buffers
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), vertexPositionData); //vertexPositionData or 0 because thats the startpoint - const void *pointer

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), vertexNormalData); //vertexNormalData or (void*)(3 * 3 * sizeof(GLfloat))

        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), vertexTexCoordData); //vertexTexCoordData or (void*)(6 * 3 * sizeof(GLfloat))
        //Binding buffers

        glDrawArrays(GL_TRIANGLES, 0, 3);

        // hollow indicator
        //glLineWidth(10.f);
        //glBegin(GL_LINE_LOOP);
        //glVertex2f(-0.5f, 0.0f);
        //glVertex2f(0.0f, 0.5f);
        //glVertex2f(0.0f, 0.25f);
        //glVertex2f(0.5f, 0.25f);
        //glVertex2f(0.5f, -0.25f);
        //glVertex2f(0.0f, -0.25f);
        //glVertex2f(0.0f, -0.5f);

        //normal indicator
        //glBegin(GL_TRIANGLES);
        //glVertex2f(-0.5f, 0.0f); //part 1
        //glVertex2f(0.0f, 0.5f);  //part 1
        //glVertex2f(0.0f, -0.5f); //part 1
        //glVertex2f(0.0f, 0.25f); //part 2.1
        //glVertex2f(0.0f, -0.25f);//part 2.1
        //glVertex2f(0.5f, 0.25f); //part 2.1
        //glVertex2f(0.5f, -0.25f);//part 2.2
        //glVertex2f(0.5f, 0.25f); //part 2.2
        //glVertex2f(0.0f, -0.25f);//part 2.2

        glEnd();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}