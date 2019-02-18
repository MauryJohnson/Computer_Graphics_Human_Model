#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "SOIL.h"
#include <vector>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/ext.hpp>

#include "Shader.h"

GLfloat mix_value=0.2f;

#define PI 3.14159

namespace MatrixBuilder{

//Radians from 0 -> 2*PI
// R rows and C columns
//Offset to the index of first num to read!
//NumTextures is number of textures labelled after data matrix


    //Return GLuFloat[] Arr Corresponding to size of ALL
    // objects by Row,Column
    // then number of matrices
    // then by which radians
    // then by offset, (WHERE IDX to START)
    // then by number of Textures to SKIP!!
std::vector<GLfloat> Rotate(GLfloat* M, GLuint R, GLuint C,GLuint Sections,double Radians,GLuint Offset,GLuint NumTextures){

if(M==nullptr || R<=0 || C<=0 || Sections<=0 || Offset<0 || NumTextures<0){
printf("Bad INPUT!!!");
exit(-1);
}

std::vector<GLfloat>Ret;

GLuint TotalC = C+NumTextures;

GLuint IDX = 0;
GLuint IDXOffSet=0;
for(;IDX<Offset;IDX+=1){
}

IDXOffSet = IDX;

double Rot3[]=
{
cos(Radians),-sin(Radians),0,
sin(Radians),cos(Radians),0,
0,              0,        1
};

GLfloat NewM[R*C];

//GLfloat MyTextures[R][NumTextures==0? 1:NumTextures];

//Iterate all sections
for(int k=0;k<Sections;k+=1){

GLfloat MyTextures[R][NumTextures<1? 1:NumTextures];

//Section 0..N
int COffSet = 0;
for(int i=0;i<R;i+=1){

//std::vector<GLfloat> M1;

for(int j=0;j<C;j+=1){

//printf("\n IDX [%d,%d] -> %f",i,j,M[( ( i*C)+ IDXOffSet + (j+COffSet) )]);

//M1.push_back(M[ (i*C) + (j+COffSet)]);

NewM[(i*C)+j] = M[ (i*C)+ IDXOffSet + (j+COffSet)];

IDX+=1;
}

int L = 0;

printf("\n");
for(int l=C; l<C+NumTextures;l+=1){

//printf("Texture of IDX[%d,%d]:%f\n",i,l,M[(i*C)+ IDXOffSet + l +COffSet]);

MyTextures[i][L] = M[(i*C)+ IDXOffSet +l+COffSet];

L+=1;
}


COffSet+=NumTextures;

IDX+=NumTextures;

}

IDXOffSet=IDX;

printf("\n FINAL IDX:%d\n",IDXOffSet);

glm::mat3 aaa = glm::make_mat3(NewM);

std::cout<<glm::to_string(aaa)<<std::endl;

//printf("%s",to_string(aaa));

glm::mat3 r3 = glm::make_mat3(Rot3);

glm::mat3 r3Xa = r3*aaa;

printf("\n ROTATE %f RADIANS -> RESULT:\n",Radians);

std::cout<<glm::to_string(r3Xa)<<std::endl;

GLfloat* p3 = glm::value_ptr(r3Xa);

int CR = 0;
int CT = 0;
//Push rotated values
//printf("\n ALL OF THE ARRAY\n");
for(int i=0; i<R*C;i+=1){

printf("B4:%f ",p3[i]);
Ret.push_back(p3[i]);

CT+=1;

if(CT>=C){
//Check if end of normal column
for(int u=0; u<NumTextures;u+=1){

printf("AF:%f ",MyTextures[CR][u]);
Ret.push_back(MyTextures[CR][u]);

}
printf("\n");

CR+=1;
CT=0;
}

}

}

//2 Iterate and replace values
return Ret;
}


std::vector<GLfloat> OneToTwo(GLfloat**M,GLuint R, GLuint C){
std::vector<GLfloat> M2;


return M2;
}

std::vector<GLfloat> TwoToOne(GLfloat ** M,GLuint All,GLuint R, GLuint C,GLuint Sections){
//GLFloat[sizeof(M)*sizeof(M[0])]M2;

std::vector<GLfloat> M2;

//int ROW = 0;
//int CPerROW  = C;
int IDX = 0;
int OFFSET=0;
//Iterate through each slice
for(int u=0; u<All;u+=1){
//Iterate each section of the slice
for(int p=0;p<Sections;p+=1){
//Iterate piece by piece
for(int i=0; i<R;i+=1){

for(int j=0; j<C;j+=1){

printf("%f",  M[u][(i*C) + OFFSET + j]);

M2.push_back( 
  M[u][(i*C) + OFFSET + j]
  );

IDX+=1;
}
printf("\n");
}

OFFSET=IDX;

printf("\n");
}
printf("\n\n");
//Reset offset and IDX in arr
OFFSET=0;
IDX=0;

}

return M2;

}

//return M2;

}

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

    //Z
    GLfloat TWidth = 0.5f;
    //X
    GLfloat TLen = 0.75;
    //Y
    GLfloat THeight = 0.5f;    

    GLfloat Triangle1[]={

	//T1 TRIANGLE FACE
	 // positions        // textures
         TLen,0.0f,0.0f,  0.0f,0.0f,
         TLen,THeight,0.0f,  1.0f,0.0f,
         0.0f, 0.0f,0.0f,  1.0f,1.0f,

	//T1 TRIANGLE FACE shifted
	TLen,0.0f,TWidth,  0.0f,0.0f,
        TLen,THeight,TWidth,  1.0f,0.0f,
        0.0f, 0.0f,TWidth,  1.0f,1.0f,
        //0.75f, 0.0f,0.5f, 0.0f,1.0f,

	//T1 TRIANGLE TOP HALF 1
	//0.75f,0.0f,0.5f,  0.0f,0.0f,
        TLen,THeight,TWidth,  0.0f,0.0f,
	TLen,THeight,0.0f,  1.0f,0.0f,
        //0.75f,0.0f,0.0f,  1.0f,0.0f,
	0.0f,0.0f,TWidth,   1.0f,1.0f,
		
	//T1 TRIANGLE TOP HALF 2
	//0.75f,0.0f,0.5f, 1.0f,0.0f,
        0.75f,THeight,0.0f,  1.0f,1.0f,
        0.0f,0.0f,TWidth,  1.0f,0.0f,
        0.0f,0.0f,0.0f,   0.0f,0.0f,		

	//T1 TRIANGLE BOTTOM HALF 1
	0.0f,0.0f,0.0f, 1.0f,1.0f,
	TLen,0.0f,0.0f, 1.0f,0.0f,
	0.0f,0.0f,TWidth, 0.0f,0.0f,	
	
	//T1 TRIANGLE BOTTOM HALF 2
        0.0f,0.0f,TWidth, 1.0f,1.0f,
        TLen,0.0f,0.0f, 1.0f,0.0f,
        TLen,0.0f,TWidth, 0.0f,0.0f,

	//T1 TRIANGLE BACK HALF 1
	TLen,0.0f,0.0f, 1.0f, 1.0f,
	TLen,THeight,0.0f, 1.0f,0.0f,
	TLen,THeight,TWidth,0.0f,0.0f,
	
	//T1 TRIANGLE BACK HALF 2
        TLen,0.0f,0.0f, 1.0f, 1.0f,
        TLen,0.0,TWidth, 1.0f,0.0f,
        TLen,THeight,TWidth,0.0f,0.0f

    };

    //Return GLuFloat[] Arr Corresponding to size of ALL
    // objects by Row,Column
    // then number of matrices
    // then by which radians
    // then by offset, (WHERE IDX to START)
    // then by number of Textures to SKIP!!
    GLfloat * Triangle2 = MatrixBuilder::Rotate(Triangle1,3,3,8,-PI/2,0,2).data();
    GLfloat * Triangle3 =  MatrixBuilder::Rotate(Triangle1,3,3,8,-PI,0,2).data();
    GLfloat * Triangle4 =  MatrixBuilder::Rotate(Triangle1,3,3,8,-3*PI/2,0,2).data();
    
     GLfloat* V[4]{
	Triangle1,Triangle2,Triangle3,Triangle4
     };
    
    GLfloat vertices[4*3*5*8];
 
    GLfloat * v = MatrixBuilder::TwoToOne(V,4,3,5,8).data();
    if(vertices==NULL){
 	exit(-1);
    } 
    for(int i=0; i<4*3*5*8;i+=1)
    vertices[i]=v[i];

    /*GLfloat vertices[]={
        
	//T1 TRIANGLE FACE
	 // positions        // textures
         TLen,0.0f,0.0f,  0.0f,0.0f,
         TLen,THeight,0.0f,  1.0f,0.0f,
         0.0f, 0.0f,0.0f,  1.0f,1.0f,
	 //0.75f, 0.0f, 0.5f, 0.0f,1.0f,
         //0.5f, 0.5f,-0.5f,  1.0f,1.0f,
        //-0.5f, 0.5f,-0.5f,  0.0f,1.0f,
        //-0.5f,-0.5f,-0.5f,  0.0f,0.0f,

	//T1 TRIANGLE FACE shifted
	TLen,0.0f,TWidth,  0.0f,0.0f,
        TLen,THeight,TWidth,  1.0f,0.0f,
        0.0f, 0.0f,TWidth,  1.0f,1.0f,
        //0.75f, 0.0f,0.5f, 0.0f,1.0f,

	//T1 TRIANGLE TOP HALF 1
	//0.75f,0.0f,0.5f,  0.0f,0.0f,
        TLen,THeight,TWidth,  0.0f,0.0f,
	TLen,THeight,0.0f,  1.0f,0.0f,
        //0.75f,0.0f,0.0f,  1.0f,0.0f,
	0.0f,0.0f,TWidth,   1.0f,1.0f,
		
	//T1 TRIANGLE TOP HALF 2
	//0.75f,0.0f,0.5f, 1.0f,0.0f,
        0.75f,THeight,0.0f,  1.0f,1.0f,
        0.0f,0.0f,TWidth,  1.0f,0.0f,
        0.0f,0.0f,0.0f,   0.0f,0.0f,		

	//T1 TRIANGLE BOTTOM HALF 1
	0.0f,0.0f,0.0f, 1.0f,1.0f,
	TLen,0.0f,0.0f, 1.0f,0.0f,
	0.0f,0.0f,TWidth, 0.0f,0.0f,	
	
	//T1 TRIANGLE BOTTOM HALF 2
        0.0f,0.0f,TWidth, 1.0f,1.0f,
        TLen,0.0f,0.0f, 1.0f,0.0f,
        TLen,0.0f,TWidth, 0.0f,0.0f,

	//T1 TRIANGLE BACK HALF 1
	TLen,0.0f,0.0f, 1.0f, 1.0f,
	TLen,THeight,0.0f, 1.0f,0.0f,
	TLen,THeight,TWidth,0.0f,0.0f,
	
	//T1 TRIANGLE BACK HALF 2
        TLen,0.0f,0.0f, 1.0f, 1.0f,
        TLen,0.0,TWidth, 1.0f,0.0f,
        TLen,THeight,TWidth,0.0f,0.0f,
	
	//BASE
	 0.0f,0.0f,0.0f,1.0f,0.0f,
         -0.10f,-1.0f,0.0f,1.0f,1.0f,
         0.10f,-1.0f,0.0f,0.0f,1.0f,
         //0.0f,-1.0f,0.10f,0.0f,1.0f
	 
    };
	*/
    glm::vec3 cube_positions[]={
        
	//Rotate T1 about here
	glm::vec3(0.0, 0.0f, 0.0f),
        //Rotate T1 from here
	glm::vec3(0.00f,0.0f,0.00f),
	
	//RAT2
	glm::vec3( 2.0f, 5.0f,-15.0f),
        //RFT2
	glm::vec3(-1.5f,-2.2f,-2.5f),
        
	//RAT3
	glm::vec3(-3.8f,-2.0f,-12.3f),
        //RAT3
	glm::vec3( 2.4f,-0.4f,-3.5f),
        
	//RAT4
	glm::vec3(-1.7f, 3.0f,-7.5f),
        //RAT4
	glm::vec3( 1.3f,-2.0f,-2.5f),
       
 	//RAT5 (BASE)
	glm::vec3( 1.5f, 2.0f,-2.5f),
        //RAT5 (BASE)
	glm::vec3( 1.5f, 0.2f,-1.5f),
        
	//CIRC1
	glm::vec3(-1.3f, 1.0f,-1.5f),	 
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
        
	for(GLuint i=0;i<5;i+=2)
        {

 	   GLuint model_location=glGetUniformLocation(our_shader.program,"model");

	    //glm::vec3 V3RA(0.0f,0.0f,1.0f);
    	    //float AR = glm::half_pi<float>();
  	    //glm::mat4x4 MR = glm::rotate(AR,V3RA);

            // world space transform
            glm::mat4 model(1.0f);
            
  	    model=glm::translate(model,cube_positions[i]);
            model=glm::rotate(model,glm::radians((GLfloat)glfwGetTime()*50.0f),glm::vec3(0.0f,1.0f,1.0f));
            //model=glm::rotate(
 	    model=glm::translate(model,cube_positions[i+1]);

   	    //GLfloat angle=glm::radians(20.0f*i);
            
    /*	    //model=glm::rotate(model,angle,glm::vec3(0.0f,10.0f,0.0f));
            
		glm::mat4 trans(1.0f);
    trans=translate(trans,glm::vec3(0.0f,-0.0f,0.0f));
    trans=rotate(trans,glm::radians((GLfloat)glfwGetTime()*50.0f),glm::vec3(0.0f,0.0f,1.0f));


            GLuint transform_location=glGetUniformLocation(VFShader.program,"transform");
	    glUniformMatrix4fv(transform_location,1,GL_FALSE,glm::value_ptr(trans));
*/
    	    glUniformMatrix4fv(model_location,1,GL_FALSE,glm::value_ptr(model));

            glDrawArrays(GL_TRIANGLES,0,/*36*//*18*/24);
            //glDrawArrays(GL_TRIANGLE_FAN,2,3);
	    //3d effect of first triangle
	    //glDrawArrays(GL_TRIANGLES,2,3);
	    //glDrawArrays(GL_TRIANGLES,3,3);
	    //glDrawArrays(GL_TRIANGLES,3,4);


	    //glDrawElements(GL_TR,3,GL_UNSIGNED_INT,0);
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
