/* For reference see https://docs.gl/*/
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <string>
/*#include "src/IndexBuffer.h"
#include "src/VertexBuffer.h"*/

/*FILE(GLOB_RECURSE sources ${CMAKE_SOURCE_DIR}/src/*.cpp)

add_executable(${PROJECT_NAME} ${sources})
add_executable(tutorial main.cpp)*/

/*To read the shader file*/
static std::string ParseShader(const std::string& filepath){
    std::ifstream stream(filepath);

    /*Error handling for file loading*/
    if(stream.fail()){
        std::cout << "file not able to load" << std::endl;
    }

    std::string line;
    std::stringstream ss;

    while(getline(stream, line)){
        ss << line << '\n';
    }

    return ss.str();
}

/**/
static unsigned int CompileShader(unsigned int type, const std::string& source){
    unsigned int id = glCreateShader(type);

    /*Ptr to the beginning of our data*/
    const char* src = source.c_str();

    /**/
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    /*Testing*/
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);

    if (result == GL_FALSE){
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);

        /*alloca allocates directly on the stack*/
        char* message = (char*)alloca(length * sizeof(char));

        glGetShaderInfoLog(id, length, & length, message);
        std::cout << "Failed to compile" <<(type == GL_VERTEX_SHADER ? "vertex":"fragment")<< "sharder" << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id);

        return 0;
    }

    return  id;
}

/*The strings taken as inputs by this function are the actual shader source code*/
static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader){
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    /*Since the shaders are linked at this point we can delete them*/
    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

int main(void){   
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    glfwSwapInterval(5);

    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        // Problem: glewInit failed
        fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
        return 1;
    }
    fprintf(stdout, "GLEW version: %s\n", glewGetString(GLEW_VERSION));
    fprintf(stdout, "GL version: %s\n", glGetString(GL_VERSION));

    /* Each line represents a vertex position*/
    float positions[] = {
        -0.5f, -0.5f, // 1
         0.5f, -0.5f, // 2
         0.5f,  0.5f, // 3
        -0.5f,  0.5f, // 4

    };

    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    unsigned int buffer;
    /* Calls our external VerrtexBuffer function */    /*Index: 0 | Number of floats representing a vertex: 2 | Type of data: float |Stride (amount of bites between each vertex): size of two floats |
    Pointer to first attribute (we only have one vertex attribute: position): 0 */    /* Tells that this buffer is supposed to be drawn */
    /* Pass data to buffer*/
    /*VertexBuffer vb(positions, 4 * 2 * sizeof(float));*/
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, 4*2*sizeof(float), positions, GL_STATIC_DRAW);


    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);

    /* Calls our external IndexBuffer function */
    /*IndexBuffer ib(indices, 6);*/
    unsigned int ibo;
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6* sizeof(unsigned int), indices, GL_STATIC_DRAW);


    /*Index: 0 | Number of floats representing a vertex: 2 | Type of data: float |Stride (amount of bites between each vertex): size of two floats
    Pointer to first attribute (we only have one vertex attribute: position): 0 */
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
        
    std::string VertexSource = ParseShader("../../src/vertex.shader");
    std::string FragmentSource = ParseShader("../../src/fragment.shader");

    unsigned int shader = CreateShader(VertexSource, FragmentSource);
    glUseProgram(shader);

    /*Retreive location of u_Color*/
    int location = glGetUniformLocation(shader, "u_Color");
    
    /* Loop until the user closes the window */
    float r = 0.0f;
    float increment = 0.05f;
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);
        
        /* Set color*/
        glUniform4f(location, r, 0.3f, 0.8f, 1.0f);

        /* Draws the triangle absent an idx buffer*/
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

        /*Dynamicaly Change Color*/
        if (r > 1.0f)
            increment = -0.05f;
        else if (r < 0.0f)
            increment = 0.05f;

        r += increment;

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glDeleteProgram(shader);
    glfwTerminate();
    return 0;
}