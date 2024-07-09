#include <glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <tiny_gltf.h>


GLuint loadShadersTriangle(tinygltf::Model& model, std::filesystem::path& gltfDirectory, unsigned int materialIndex)
{
    const char* defaultVertexShaderSource = 
        #include "vertexShader.glsl"
        ;

    const char* defaultFragmentShaderSource = 
        #include "fragmentShader.glsl"
        ;

    std::filesystem::path vertexShaderPath;
    std::filesystem::path fragmentShaderPath;
    std::string vertexShaderSource;
    std::string fragmentShaderSource;

    if (materialIndex < model.materials.size())
    {
        auto gltfShaderExtras = model.materials[materialIndex].extras;

        if (gltfShaderExtras.Has("shader"))
        {
            auto gltfShader = gltfShaderExtras.Get("shader");
            if (gltfShader.Has("vertex"))
            {
                std::string vertexShaderFile = gltfShader.Get("vertex").Get<std::string>();
                vertexShaderPath = gltfDirectory / vertexShaderFile;
            }
            if (gltfShader.Has("fragment"))
            {
                std::string fragmentShaderFile = gltfShader.Get("fragment").Get<std::string>();
                fragmentShaderPath = gltfDirectory / fragmentShaderFile;
            }
        }
    }
    else
    {
        vertexShaderSource = defaultVertexShaderSource;
        fragmentShaderSource = defaultFragmentShaderSource;
    }



    std::ifstream vertexShaderFile(vertexShaderPath);
    if(vertexShaderFile.is_open())
    {
        std::string vertexShaderSourceFile((std::istreambuf_iterator<char>(vertexShaderFile)), std::istreambuf_iterator<char>());
        vertexShaderSource = vertexShaderSourceFile;
        vertexShaderFile.close();
    }

    std::ifstream fragmentShaderFile(fragmentShaderPath);
    if(fragmentShaderFile.is_open())
    {
        std::string fragmentShaderSourceFile((std::istreambuf_iterator<char>(fragmentShaderFile)), std::istreambuf_iterator<char>());
        fragmentShaderSource = fragmentShaderSourceFile;
        fragmentShaderFile.close();
    }


    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    const GLchar *srcV = vertexShaderSource.c_str();
    glShaderSource(vertexShader, 1, &srcV, nullptr);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    const GLchar *srcF = fragmentShaderSource.c_str();
    glShaderSource(fragmentShader, 1, &srcF, nullptr);

    glCompileShader(vertexShader);
    GLint status;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);
    if(status == GL_FALSE)
    {
        GLint infoLogLength;
        glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &infoLogLength);
        std::vector<char> infoLog(infoLogLength);
        glGetShaderInfoLog(vertexShader, infoLogLength, nullptr, infoLog.data());
        std::cerr << "Failed to compile vertex shader: " << infoLog.data() << std::endl;
        return -1;
    }

    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &status);
    if(status == GL_FALSE)
    {
        GLint infoLogLength;
        glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &infoLogLength);
        std::vector<char> infoLog(infoLogLength);
        glGetShaderInfoLog(fragmentShader, infoLogLength, nullptr, infoLog.data());
        std::cerr << "Failed to compile fragment shader: " << infoLog.data() << std::endl;
        return -1;
    }
    

    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);

    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if(status == GL_FALSE)
    {
        GLint infoLogLength;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);
        std::vector<char> infoLog(infoLogLength);
        glGetProgramInfoLog(program, infoLogLength, nullptr, infoLog.data());
        std::cerr << "Failed to link program: " << infoLog.data() << std::endl;
        return -1;
    }

    glUseProgram(program);

    return program;
}

void loadShadersHollowIndicator()
{
    const char* vertexShaderSource = R"(
        #version 300 es

        layout(location = 0) in vec3 position;
        void main()
        {
            gl_Position = vec4(position, 1.0);
        }
    )";

    const char* fragmentShaderSource = R"(
        #version 300 es

        precision mediump float;
        out mediump vec4 fragColor;
        void main()
        {
            fragColor = vec4(0.0, 0.0, 0.0, 1.0);
        }
    )";

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);

    glCompileShader(vertexShader);
    GLint status;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);
    if(status == GL_FALSE)
    {
        GLint infoLogLength;
        glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &infoLogLength);
        std::vector<char> infoLog(infoLogLength);
        glGetShaderInfoLog(vertexShader, infoLogLength, nullptr, infoLog.data());
        std::cerr << "Failed to compile vertex shader: " << infoLog.data() << std::endl;
        return;
    }

    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &status);
    if(status == GL_FALSE)
    {
        GLint infoLogLength;
        glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &infoLogLength);
        std::vector<char> infoLog(infoLogLength);
        glGetShaderInfoLog(fragmentShader, infoLogLength, nullptr, infoLog.data());
        std::cerr << "Failed to compile fragment shader: " << infoLog.data() << std::endl;
        return;
    }
    

    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);

    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if(status == GL_FALSE)
    {
        GLint infoLogLength;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);
        std::vector<char> infoLog(infoLogLength);
        glGetProgramInfoLog(program, infoLogLength, nullptr, infoLog.data());
        std::cerr << "Failed to link program: " << infoLog.data() << std::endl;
        return;
    }

    glUseProgram(program);
}

int main(void)
{
    GLFWwindow* window;

    std::string gltfFilename = "../examples/gltf/04_suzanne/export/suzanne.gltf";

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
    window = glfwCreateWindow(640, 480, "MishMash", NULL, NULL);
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

    GLuint positionBuffer = 0; //positionBuffer = vertexBuffer
    GLuint normalBuffer = 0;
    GLuint texcoordBuffer = 0;
    GLuint indexBuffer = 0;

    uint32_t gltfAccessorPositionIndex = model.meshes[0].primitives[0].attributes["POSITION"];
    uint32_t gltfAccessorNormalIndex = model.meshes[0].primitives[0].attributes["NORMAL"];
    uint32_t gltfAccessorTexCoordIndex = model.meshes[0].primitives[0].attributes["TEXCOORD_0"];
    uint32_t gltfAccessorIndicesIndex = model.meshes[0].primitives[0].indices;

    uint32_t gltfBufferViewPositionIndex = model.accessors[gltfAccessorPositionIndex].bufferView;
    uint32_t gltfBufferViewNormalIndex = model.accessors[gltfAccessorNormalIndex].bufferView;
    uint32_t gltfBufferViewTexCoordIndex = model.accessors[gltfAccessorTexCoordIndex].bufferView;
    uint32_t gltfBufferViewIndicesIndex = model.accessors[gltfAccessorIndicesIndex].bufferView;

    uint32_t gltfBufferIndexPosition = model.bufferViews[gltfBufferViewPositionIndex].buffer;
    uint32_t gltfBufferIndexNormal = model.bufferViews[gltfBufferViewNormalIndex].buffer;
    uint32_t gltfBufferIndexTexCoord = model.bufferViews[gltfBufferViewTexCoordIndex].buffer;
    uint32_t gltfBufferIndexIndices = model.bufferViews[gltfBufferViewIndicesIndex].buffer;

    unsigned char* gltfBufferDataPosition = model.buffers[gltfBufferIndexPosition].data.data();
    unsigned char* gltfBufferDataNormal = model.buffers[gltfBufferIndexNormal].data.data();
    unsigned char* gltfBufferDataTexCoord = model.buffers[gltfBufferIndexTexCoord].data.data();
    unsigned char* gltfBufferDataIndices = model.buffers[gltfBufferIndexIndices].data.data();

    uint32_t gltfPositionByteOffset = model.bufferViews[gltfBufferViewPositionIndex].byteOffset;
    uint32_t gltfNormalByteOffset = model.bufferViews[gltfBufferViewNormalIndex].byteOffset;
    uint32_t gltfTexCoordByteOffset = model.bufferViews[gltfBufferViewTexCoordIndex].byteOffset;
    uint32_t gltfIndicesByteOffset = model.bufferViews[gltfBufferViewIndicesIndex].byteOffset;

    uint32_t gltfPositionByteLength = model.bufferViews[gltfBufferViewPositionIndex].byteLength;
    uint32_t gltfNormalByteLength = model.bufferViews[gltfBufferViewNormalIndex].byteLength;
    uint32_t gltfTexCoordByteLength = model.bufferViews[gltfBufferViewTexCoordIndex].byteLength;
    uint32_t gltfIndicesByteLength = model.bufferViews[gltfBufferViewIndicesIndex].byteLength;

    GLint indicesCount = gltfIndicesByteLength / sizeof(GLushort);
    
    glGenBuffers(1, &positionBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
    glBufferData(GL_ARRAY_BUFFER, gltfPositionByteLength, gltfBufferDataPosition + gltfPositionByteOffset, GL_STATIC_DRAW);

    glGenBuffers(1, &normalBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
    glBufferData(GL_ARRAY_BUFFER, gltfNormalByteLength, gltfBufferDataNormal + gltfNormalByteOffset, GL_STATIC_DRAW);

    glGenBuffers(1, &texcoordBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, texcoordBuffer);
    glBufferData(GL_ARRAY_BUFFER, gltfTexCoordByteLength, gltfBufferDataTexCoord + gltfTexCoordByteOffset, GL_STATIC_DRAW);

    glGenBuffers(1, &indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, gltfIndicesByteLength, gltfBufferDataIndices + gltfIndicesByteOffset, GL_STATIC_DRAW);

    // GLfloat colorData[] = {
    //     1.0f, 0.0f, 0.0f, // red
    //     0.0f, 1.0f, 0.0f, // green
    //     0.0f, 0.0f, 1.0f, // blue
    //     // rainbow
    // };

    // GLuint vertexBufferRainbow = 0;
    // glGenBuffers(1, &vertexBufferRainbow);
    // glBindBuffer(GL_ARRAY_BUFFER, vertexBufferRainbow);
    // glBufferData(GL_ARRAY_BUFFER, sizeof(colorData), colorData, GL_STATIC_DRAW);


    GLuint vertexArrayObject1 = 0;
    glGenVertexArrays(1, &vertexArrayObject1);
    glBindVertexArray(vertexArrayObject1);

    GLuint vertexArrayObject2 = 0;
    glGenVertexArrays(1, &vertexArrayObject2);
    glBindVertexArray(vertexArrayObject2);

    glBindBuffer(GL_ARRAY_BUFFER,0);

    std::filesystem::path gltfPath = gltfFilename;
    std::filesystem::path gltfDirectory = gltfPath.parent_path();

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClearColor(0.75f, 0.75f, 0.75f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        GLuint program = loadShadersTriangle(model, gltfDirectory,0);

        glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

        glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

        glBindBuffer(GL_ARRAY_BUFFER, texcoordBuffer);
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);

        // glBindBuffer(GL_ARRAY_BUFFER, vertexBufferRainbow);
        // glEnableVertexAttribArray(3);
        // glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, 0);

        glBindVertexArray(vertexArrayObject1);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);

        GLint uniformLocationColor = glGetUniformLocation(program, "color");
        if(uniformLocationColor != -1)
        {
            glUniform3f(uniformLocationColor, 1.0f, 0.0f, 0.0f);
        }

        glDrawElements(GL_TRIANGLES, indicesCount, GL_UNSIGNED_SHORT, nullptr);

        //glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
        //glEnableVertexAttribArray(2);
        //glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

        //glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
        //glEnableVertexAttribArray(1);
        //glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

        //glBindBuffer(GL_ARRAY_BUFFER, texcoordBuffer);
        //glEnableVertexAttribArray(0);
        //glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE,0, 0);

        //glBindVertexArray(vertexArrayObject2);
        //glDrawArrays(GL_TRIANGLES, 0, 3);  
        

        // hollow indicator
        glUseProgram(0);
        loadShadersHollowIndicator();
        glLineWidth(10.f);
        glBegin(GL_LINE_LOOP);
        glVertex2f(0.0f, 0.5f);
        glVertex2f(0.5f, 1.0f);
        glVertex2f(0.5f, 0.75f);
        glVertex2f(1.0f, 0.75f);
        glVertex2f(1.0f, 0.25f);
        glVertex2f(0.5f, 0.25f);
        glVertex2f(0.5f, 0.0f);
        glEnd();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}