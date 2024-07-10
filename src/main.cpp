#include <glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <tiny_gltf.h>
#include "basic_types.hpp"

std::unordered_map<std::string, float> materialUniformFloats;
std::unordered_map<std::string, Vector4> materialUniformVector4;
std::unordered_map<std::string, Vector3> materialUniformVector3;
std::unordered_map<std::string, Vector2> materialUniformVector2;
std::unordered_map<std::string, int> materialUniformInts;


static void materialSetProperty(std::string uniformName, float value)
{
    if (materialUniformFloats.find(uniformName) != materialUniformFloats.end())
    {
        materialUniformFloats[uniformName] = value;
    }
}

static void materialSetProperty(std::string uniformName, Vector4 value)
{
    if (materialUniformVector4.find(uniformName) != materialUniformVector4.end())
    {
        materialUniformVector4[uniformName] = value;
    }
}

static void materialUpdateProperties(GLuint program)
{
    for (auto& uniform : materialUniformFloats)
    {
        GLint location = glGetUniformLocation(program, uniform.first.c_str());
        if(location != -1)
        {
            glUniform1f(location, uniform.second);
        }
        std::cout << "Uniform " << uniform.first << " = " << uniform.second << std::endl;
    }

    for (auto& uniform : materialUniformVector4)
    {
        GLint location = glGetUniformLocation(program, uniform.first.c_str());
        if(location != -1)
        {
            glUniform4f(location, uniform.second.x, uniform.second.y, uniform.second.z, uniform.second.w);
        }
        std::cout << "Uniform " << uniform.first << " = " << uniform.second.x << ", " << uniform.second.y << ", " << uniform.second.z << ", " << uniform.second.w << std::endl;
    }
    
    for (auto& uniform : materialUniformVector3)
    {
        GLint location = glGetUniformLocation(program, uniform.first.c_str());
        if(location != -1)
        {
            glUniform3f(location, uniform.second.x, uniform.second.y, uniform.second.z);
        }
        std::cout << "Uniform " << uniform.first << " = " << uniform.second.x << ", " << uniform.second.y << ", " << uniform.second.z << std::endl;
    }
    
    for (auto& uniform : materialUniformVector2)
    {
        GLint location = glGetUniformLocation(program, uniform.first.c_str());
        if(location != -1)
        {
            glUniform2f(location, uniform.second.x, uniform.second.y);
        }
        std::cout << "Uniform " << uniform.first << " = " << uniform.second.x << ", " << uniform.second.y << std::endl;
    }
    
    for (auto& uniform : materialUniformInts)
    {
        GLint location = glGetUniformLocation(program, uniform.first.c_str());
        if(location != -1)
        {
            glUniform1i(location, uniform.second);
        }
        std::cout << "Uniform " << uniform.first << " = " << uniform.second << std::endl;
    }
}

static float getCurretnTime()
{
    return (float)glfwGetTime();
}

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

            if (gltfShader.Has("uniforms"))
            {
                auto gltfUniforms = gltfShader.Get("uniforms");

                for (int uniformIndex = 0; uniformIndex < gltfUniforms.ArrayLen(); uniformIndex++)
                {
                    auto uniform = gltfUniforms.Get(uniformIndex);
                    std::string uniformName;

                    if (uniform.Has("name"))
                    {
                        uniformName = uniform.Get("name").Get<std::string>();
                    }

                    if (uniform.Has("type"))
                    {
                        std::string type = uniform.Get("type").Get<std::string>();
                        auto uniformValue = uniform.Get("value");
                        if (type == "Float")
                        {
                            double uniformValueFloat = uniformValue.Get(0).Get<double>();
                            materialUniformFloats[uniformName] = uniformValueFloat;
                            std::cout << "uniform " << uniformName << " = " << uniformValueFloat << std::endl;
                        }
                        else if (type == "Vector4")
                        {
                            double uniformValueFloatX = uniformValue.Get(0).Get<double>();
                            double uniformValueFloatY = uniformValue.Get(1).Get<double>();
                            double uniformValueFloatZ = uniformValue.Get(2).Get<double>();
                            double uniformValueFloatW = uniformValue.Get(3).Get<double>();
                            materialUniformVector4[uniformName] = Vector4(uniformValueFloatX, uniformValueFloatY, uniformValueFloatZ, uniformValueFloatW);
                            std::cout << "uniform " << uniformName << " = " << uniformValueFloatX << ", " << uniformValueFloatY << ", " << uniformValueFloatZ << ", " << uniformValueFloatW << std::endl;
                        }
                        else if (type == "Vector3")
                        {
                            double uniformValueFloatX = uniformValue.Get(0).Get<double>();
                            double uniformValueFloatY = uniformValue.Get(1).Get<double>();
                            double uniformValueFloatZ = uniformValue.Get(2).Get<double>();
                            materialUniformVector3[uniformName] = Vector3(uniformValueFloatX, uniformValueFloatY, uniformValueFloatZ);
                            std::cout << "uniform " << uniformName << " = " << uniformValueFloatX << ", " << uniformValueFloatY << ", " << uniformValueFloatZ << std::endl;
                        }
                        else if (type == "Vector2")
                        {
                            double uniformValueFloatX = uniformValue.Get(0).Get<double>();
                            double uniformValueFloatY = uniformValue.Get(1).Get<double>();
                            materialUniformVector2[uniformName] = Vector2(uniformValueFloatX, uniformValueFloatY);
                            std::cout << "uniform " << uniformName << " = " << uniformValueFloatX << ", " << uniformValueFloatY << std::endl;
                        }
                        else if (type == "Int")
                        {
                            int uniformValueInt = uniformValue.Get(0).Get<int>();
                            materialUniformInts[uniformName] = uniformValueInt;
                            std::cout << "uniform " << uniformName << " = " << uniformValueInt << std::endl;
                        }
                    }
                }
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


int main(void)
{
    GLFWwindow* window;

    std::string gltfFilename = "../examples/gltf/05_suzanne_uniforms/export/suzanne.gltf";

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


    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(820, 480, "GLTFLoader", NULL, NULL);
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


    GLuint vertexArrayObject1 = 0;
    glGenVertexArrays(1, &vertexArrayObject1);
    glBindVertexArray(vertexArrayObject1);

    glBindBuffer(GL_ARRAY_BUFFER,0);

    std::filesystem::path gltfPath = gltfFilename;
    std::filesystem::path gltfDirectory = gltfPath.parent_path();

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        // glClearColor(0.75f, 0.75f, 0.75f, 1.0f);
        // glClear(GL_COLOR_BUFFER_BIT);

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

        glBindVertexArray(vertexArrayObject1);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);

        materialSetProperty("iTime", getCurretnTime());
        materialUpdateProperties(program);

        glDrawElements(GL_TRIANGLES, indicesCount, GL_UNSIGNED_SHORT, nullptr);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}