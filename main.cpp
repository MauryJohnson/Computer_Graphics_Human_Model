#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "SOIL.h"

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"

GLfloat mix_value=0.2f;

void key_callback(GLFWwindow* window,int key,int scancode,int action,int mode)
{
    if(key==GLFW_KEY_ESCAPE && action==GLFW_PRESS)
        glfwSetWindowShouldClose(window,GL_TRUE);

    if(key==GLFW_KEY_UP && action==GLFW_PRESS)
    {
        mix_value+=0.1f;
        if(mix_value>1.0f) mix_value=1.0f;
    }

    if(key==GLFW_KEY_DOWN && action==GLFW_PRESS)
    {
        mix_value-=0.1f;
        if(mix_value<0.0f) mix_value=0.0f;
    }
}

int main()
{
    glfwInit();
#if __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,GL_TRUE);
#endif
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE,GL_FALSE);

    GLFWwindow *window=glfwCreateWindow(800,600,"Learn OpenGL",nullptr,nullptr);
    if(window==nullptr)
    {
        std::cout<<"Failed to create GLFW window!"<<std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    glewExperimental=GL_TRUE;
    if(glewInit()!=GLEW_OK)
    {
        std::cout<<"Failed to initialize GLEW!"<<std::endl;
        return -1;
    }

    int width,height;
    glfwGetFramebufferSize(window,&width,&height);
    glViewport(0,0,width,height);

    // projection matrix
    glm::mat4 projection(1.0f);
    projection=glm::perspective(glm::radians(45.0f),(float)width/height,0.1f,100.0f);

    glfwSetKeyCallback(window,key_callback);

    Shader our_shader("shader.vs","shader.frag");

    GLuint texture1,texture2;

    // generate texture 1
    glGenTextures(1,&texture1);
    glBindTexture(GL_TEXTURE_2D,texture1);
    // set texture parameters
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
    // set texture filtering
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    // load, create and generate mipmaps
    unsigned char* image=SOIL_load_image("A.jpg",&width,&height,0,SOIL_LOAD_RGB);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,width,height,0,GL_RGB,GL_UNSIGNED_BYTE,image);
    glGenerateMipmap(GL_TEXTURE_2D);
    // free image data
    SOIL_free_image_data(image);
    glBindTexture(GL_TEXTURE_2D,0);

    // generate texture 2
    glGenTextures(1,&texture2);
    glBindTexture(GL_TEXTURE_2D,texture2);
    // set texture parameters
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
    // set texture filtering
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    // load, create and generate mipmaps
    unsigned char* image2=SOIL_load_image("B.jpg",&width,&height,0,SOIL_LOAD_RGB);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,width,height,0,GL_RGB,GL_UNSIGNED_BYTE,image2);
    glGenerateMipmap(GL_TEXTURE_2D);
    // free image data
    SOIL_free_image_data(image2);
    glBindTexture(GL_TEXTURE_2D,0);

    
    GLfloat vertices[]={
        // positions        // textures
         0.0f,0.0f,0.0f,  0.0f,0.0f,
         0.0f,0.5f,0.0f,  1.0f,0.0f,
         0.5f, 0.0f,-0.0f,  1.0f,1.0f,
         //0.5f, 0.5f,-0.5f,  1.0f,1.0f,
        //-0.5f, 0.5f,-0.5f,  0.0f,1.0f,
        //-0.5f,-0.5f,-0.5f,  0.0f,0.0f,

        -0.5f,-0.5f, 0.5f,  0.0f,0.0f,
         0.5f,-0.5f, 0.5f,  1.0f,0.0f,
         0.5f, 0.5f, 0.5f,  1.0f,1.0f,
         //0.5f, 0.5f, 0.5f,  1.0f,1.0f,
        //-0.5f, 0.5f, 0.5f,  0.0f,1.0f,
        //-0.5f,-0.5f, 0.5f,  0.0f,0.0f,

        -0.5f, 0.5f, 0.5f,  1.0f,0.0f,
        -0.5f, 0.5f,-0.5f,  1.0f,1.0f,
        -0.5f,-0.5f,-0.5f,  0.0f,1.0f,
        //-0.5f,-0.5f,-0.5f,  0.0f,1.0f,
        //-0.5f,-0.5f, 0.5f,  0.0f,0.0f,
        //-0.5f, 0.5f, 0.5f,  1.0f,0.0f,

         0.5f, 0.5f, 0.5f,  1.0f,0.0f,
         0.5f, 0.5f,-0.5f,  1.0f,1.0f,
         0.5f,-0.5f,-0.5f,  0.0f,1.0f,
         //0.5f,-0.5f,-0.5f,  0.0f,1.0f,
         //0.5f,-0.5f, 0.5f,  0.0f,0.0f,
         //0.5f, 0.5f, 0.5f,  1.0f,0.0f,
	/*
        -0.5f,-0.5f,-0.5f,  0.0f,1.0f,
         0.5f,-0.5f,-0.5f,  1.0f,1.0f,
         0.5f,-0.5f, 0.5f,  1.0f,0.0f,
         //0.5f,-0.5f, 0.5f,  1.0f,0.0f,
        //-0.5f,-0.5f, 0.5f,  0.0f,0.0f,
        //-0.5f,-0.5f,-0.5f,  0.0f,1.0f,

        -0.5f, 0.5f,-0.5f,  0.0f,1.0f,
         0.5f, 0.5f,-0.5f,  1.0f,1.0f,
         0.5f, 0.5f, 0.5f,  1.0f,0.0f,
        //0.5f, 0.5f, 0.5f,  1.0f,0.0f,
        //-0.5f, 0.5f, 0.5f,  0.0f,0.0f,
        //-0.5f, 0.5f,-0.5f,  0.0f,1.0f
       */
    };

    glm::vec3 cube_positions[]={
        //Rotate about here
	glm::vec3( 0.0f, 0.0f, 0.0f),
        //Rotate form here
	glm::vec3(0.5f,0.5f,0.00f),
	
	glm::vec3( 2.0f, 5.0f,-15.0f),
        glm::vec3(-1.5f,-2.2f,-2.5f),
        glm::vec3(-3.8f,-2.0f,-12.3f),
        glm::vec3( 2.4f,-0.4f,-3.5f),
        glm::vec3(-1.7f, 3.0f,-7.5f),
        glm::vec3( 1.3f,-2.0f,-2.5f),
        glm::vec3( 1.5f, 2.0f,-2.5f),
        glm::vec3( 1.5f, 0.2f,-1.5f),
        glm::vec3(-1.3f, 1.0f,-1.5f)
    };

    GLuint VAO,VBO;
    glGenBuffers(1,&VBO);
    glGenVertexArrays(1,&VAO);

    // bind vertex array object
    glBindVertexArray(VAO);

    // copy the vertices in a buffer
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);

    // set position attribute pointers
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,5*sizeof(GL_FLOAT),(GLvoid*)0);
    
    glEnableVertexAttribArray(0);
    
    // set texture attribute pointers
    glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,5*sizeof(GL_FLOAT),(GLvoid*)(3*sizeof(GLfloat)));
    
    glEnableVertexAttribArray(1);

    // unbind the vertex array object
    glBindVertexArray(0);


    // view space transform
    glm::mat4 view(1.0f);
    
    // note that we're translating the scene in the reverse direction of where we want to move
    view=glm::translate(view,glm::vec3(0.0f,0.0f,-3.0f));

    glEnable(GL_DEPTH_TEST);

    //Shader VFShader("transform.vs","shader.frag");

    while(!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        glClearColor(.2f,.3f,.3f,1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // use shader program
        our_shader.Use();

        // bind textures
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,texture1);
        glUniform1i(glGetUniformLocation(our_shader.program,"our_texture1"),0);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D,texture2);
        glUniform1i(glGetUniformLocation(our_shader.program,"our_texture2"),1);

        glUniform1f(glGetUniformLocation(our_shader.program,"mix_value"),mix_value);

        //GLuint model_location=glGetUniformLocation(our_shader.program,"model");
        GLuint view_location=glGetUniformLocation(our_shader.program,"view");
        glUniformMatrix4fv(view_location,1,GL_FALSE,glm::value_ptr(view));
        
        GLuint projection_location=glGetUniformLocation(our_shader.program,"projection");
        glUniformMatrix4fv(projection_location,1,GL_FALSE,glm::value_ptr(projection));
       
        //VFShader.Use();

        // draw
        glBindVertexArray(VAO);
        for(GLuint i=0;i<10;++i)
        {

 	   GLuint model_location=glGetUniformLocation(our_shader.program,"model");

	    //glm::vec3 V3RA(0.0f,0.0f,1.0f);
    	    //float AR = glm::half_pi<float>();
  	    //glm::mat4x4 MR = glm::rotate(AR,V3RA);

            // world space transform
            glm::mat4 model(1.0f);
            
  	    model=glm::translate(model,glm::vec3(0,0.5,0));
            model=glm::rotate(model,glm::radians((GLfloat)glfwGetTime()*50.0f),glm::vec3(0.0f,0.0f,1.0f));
            //model=glm::rotate(
 	    model=glm::translate(model,cube_positions[1]);

   	    //GLfloat angle=glm::radians(20.0f*i);
            
    /*	    //model=glm::rotate(model,angle,glm::vec3(0.0f,10.0f,0.0f));
            
		glm::mat4 trans(1.0f);
    trans=translate(trans,glm::vec3(0.0f,-0.0f,0.0f));
    trans=rotate(trans,glm::radians((GLfloat)glfwGetTime()*50.0f),glm::vec3(0.0f,0.0f,1.0f));


            GLuint transform_location=glGetUniformLocation(VFShader.program,"transform");
	    glUniformMatrix4fv(transform_location,1,GL_FALSE,glm::value_ptr(trans));
*/
    	    glUniformMatrix4fv(model_location,1,GL_FALSE,glm::value_ptr(model));

            glDrawArrays(GL_TRIANGLES,0,/*36*//*18*/12);
        
	    //glDrawElements(GL_TRIANGLES,3,GL_UNSIGNED_INT,0);
 	}
        glBindVertexArray(0);

        glfwSwapBuffers(window);
    }

    // deallocate all resources
    glDeleteVertexArrays(1,&VAO);
    glDeleteBuffers(1,&VBO);
    // terminate GLFW
    glfwTerminate();

    return 0;
}
