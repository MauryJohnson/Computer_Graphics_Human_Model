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

bool keys[1024];
GLfloat delta_time=0.0f;                // time between current frame and last frame
GLfloat last_frame=0.0f;                // time of last frame
GLfloat last_X=0.0f,last_Y=0.0f;
GLfloat fov=45.0f;
GLfloat yaw=-90.0f;                     // yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing
                                        // to the right (due to how Euler angles work) so we initially rotate a bit to the left.
GLfloat pitch=0.0f;
bool first_mouse=true;

glm::vec3 camera_position=glm::vec3(0.0f,0.0f,7.0f);
glm::vec3 camera_front=glm::vec3(0.0f,0.0f,-1.0f);
glm::vec3 camera_up=glm::vec3(0.0f,1.0f,0.0f);

GLfloat mix_value=0.2f;

GLfloat ROT = 0;

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
    if(action==GLFW_PRESS || action==GLFW_REPEAT) keys[key]=true;
    else if(action==GLFW_RELEASE) keys[key]=false;

    printf("\nKEY PRESSED:%d",key);

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

    //Rotate ALL models clockwise about y axis
    if(key==GLFW_KEY_LEFT && action==GLFW_PRESS || key==GLFW_KEY_LEFT && action==GLFW_REPEAT){
	ROT-=PI/2;
    }

    //Rotate ALL models counterclockwise about y axis
    if(key==GLFW_KEY_RIGHT && (action==GLFW_PRESS) || key==GLFW_KEY_RIGHT && (action==GLFW_REPEAT) ){
	ROT+=PI/2;
    }

    if((keys[GLFW_KEY_A]/* && (action==GLFW_PRESS) || key==GLFW_KEY_A && (action==GLFW_REPEAT)*/)){
	printf("LEFT ARM");
	if(keys[GLFW_KEY_J]){
		printf("\n Want to move left arm Counter Clockwise");
		
	}	
	else if(keys[GLFW_KEY_K]){
		printf("\n Want to move left arm Clockwise");
	  		
	}	

    }

    if((keys[GLFW_KEY_S]/* && (action==GLFW_PRESS) || key==GLFW_KEY_A && (action==GLFW_REPEAT)*/)){
        printf("RIGHT ARM");
        if(keys[GLFW_KEY_J]){
                printf("\n Want to move right arm Counter Clockwise");

        }       
        else if(keys[GLFW_KEY_K]){
                printf("\n Want to move right arm Clockwise");

        }       

    } 
	
    if((keys[GLFW_KEY_Z]/* && (action==GLFW_PRESS) || key==GLFW_KEY_A && (action==GLFW_REPEAT)*/)){
        printf("LEFT LEG");
        if(keys[GLFW_KEY_J]){
                printf("\n Want to move left leg Counter Clockwise");

        }       
        else if(keys[GLFW_KEY_K]){
                printf("\n Want to move left leg  Clockwise");

        }       

    } 	
 

    if((keys[GLFW_KEY_X]/* && (action==GLFW_PRESS) || key==GLFW_KEY_A && (action==GLFW_REPEAT)*/)){
        printf("RIGHT LEG");
        if(keys[GLFW_KEY_J]){
                printf("\n Want to move right leg Counter Clockwise");

        }       
        else if(keys[GLFW_KEY_K]){
                printf("\n Want to move right leg Clockwise");

        }       

    } 

}


void do_movement()
{
    // camera controls
    GLfloat camera_speed=5.0f*delta_time;
    if(keys[GLFW_KEY_W]) camera_position+=camera_speed*camera_front;
    if(keys[GLFW_KEY_S]) camera_position-=camera_speed*camera_front;
    if(keys[GLFW_KEY_A]) camera_position-=glm::normalize(glm::cross(camera_front,camera_up))*camera_speed;
    if(keys[GLFW_KEY_D]) camera_position+=glm::normalize(glm::cross(camera_front,camera_up))*camera_speed;
}

void scroll_callback(GLFWwindow *window,double xoffset,double yoffset)
{
    if(fov>=1.0f && fov<=45.0f)  fov-=yoffset;
    if(fov<=1.0f) fov=1.0f;
    if(fov>=45.0f) fov=45.0f;
}

/*
void key_callback(GLFWwindow *window,int key,int scancode,int action,int mode)
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

    if(action==GLFW_PRESS) keys[key]=true;
    else if(action==GLFW_RELEASE) keys[key]=false;
}
*/
void mouse_callback(GLFWwindow *window,double xpos,double ypos)
{
    if(first_mouse)
    {
        last_X=xpos;
        last_Y=ypos;
        first_mouse=false;
    }

    GLfloat x_offset=xpos-last_X;
    GLfloat y_offset=last_Y-ypos;
    last_X=xpos;last_Y=ypos;

    GLfloat sensitivity=0.05f;
    x_offset*=sensitivity;
    y_offset*=sensitivity;

    yaw+=x_offset;
    pitch+=y_offset;

    if(pitch>89.0f) pitch=89.0f;
    else if(pitch<-89.0f) pitch=-89.0f;

    glm::vec3 front;
    front.x=cos(glm::radians(pitch))*cos(glm::radians(yaw));
    front.y=sin(glm::radians(pitch));
    front.z=cos(glm::radians(pitch))*sin(glm::radians(yaw));
    camera_front=glm::normalize(front);
}

GLuint Validate(Shader VFShader,glm::mat4 view, glm::mat4 projection){

GLuint view_location1=glGetUniformLocation(VFShader.program,"view");
        glUniformMatrix4fv(view_location1,1,GL_FALSE,glm::value_ptr(view));
        GLuint projection_location1=glGetUniformLocation(VFShader.program,"projection");

        glUniformMatrix4fv(projection_location1,1,GL_FALSE,glm::value_ptr(projection));

        GLuint model_location1=glGetUniformLocation(VFShader.program,"model");

return model_location1;

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
    glfwSetCursorPosCallback(window,mouse_callback);
    glfwSetScrollCallback(window,scroll_callback);

    Shader our_shader("shader.vs","tshader.frag");
    Shader VFShader("shader.vs","shader.frag");
    Shader VFShader2("shader.vs","shader2.frag");
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
 
    GLfloat Body[] = {
        //Body Front
        -0.5f,0.75f,0.0f,
        -0.5f,-0.75f,0.0f,
        0.5f,0.75f,0.0f,
        0.5f,-0.75f,0.0f,
	//Body Bottom
	-0.5f,-0.75f,0.0f,
	-0.5f,-0.75f,-0.25f,
	0.5f,-0.75f,0.0f,
	0.5f,-0.75f,-0.25f,
    	//Body Back
	-0.5f,0.75f,-0.25f,
        -0.5f,-0.75f,-0.25f,
        0.5f,0.75f,-0.25f,
        0.5f,-0.75f,-0.25f,
	//Body Top
	-0.5f,0.75f,0.0f,
        -0.5f,0.75f,-0.25f,
         0.5f,0.75f,0.0f,
         0.5f,0.75f,-0.25f,
    	//Body Right
	0.5f,0.75f,0.0f,
	0.5f,0.75f,-0.25f,
	0.5f,-0.75f,0.0f,
	0.5f,-0.75f,-0.25f,
	//Body Left
	-0.5f,0.75f,0.0f,
        -0.5f,0.75f,-0.25f,
        -0.5f,-0.75f,0.0f,
        -0.5f,-0.75f,-0.25f,
		
    };

    glm::vec3 Body_Position(0.0f,0.0f,0.0f);

    GLfloat AX2 = -0.75f;
    GLfloat AX1 = -0.50f;

    GLfloat AY2 = 0.75f;
    GLfloat AY1 = 0.0f;

    GLfloat AZ2 = -0.25f;
    GLfloat AZ1 = 0.0f;

    GLfloat RArm[] = {
        //Right Arm

	//Front
        AX2,AY2,AZ1,
        AX1,AY2,AZ1,
        AX2,AY1,AZ1,
        AX1,AY1,AZ1,

	//Bottom
	AX2,AY1,AZ1,
        AX2,AY1,AZ2,
	AX1,AY1,AZ1,
        AX1,AY1,AZ2,
	
	//Back
	AX2,AY2,AZ2,
        AX1,AY2,AZ2,
        AX2,AY1,AZ2,
        AX1,AY1,AZ2,
	
	//Top
	AX2,AY2,AZ1,
        AX2,AY2,AZ2,
        AX1,AY2,AZ1,
        AX1,AY2,AZ2,

	//Left
	AX2,AY1,AZ1,
	AX2,AY1,AZ2,
	AX2,AY2,AZ1,
	AX2,AY2,AZ2,

	//Right
	AX1,AY1,AZ1,
        AX1,AY1,AZ2,
        AX1,AY2,AZ1,
        AX1,AY2,AZ2,

/*
	//Right
	-0.25f,0.0f,0.0f,
        //-0.25f,0.0f,-0.3f,
        -0.25f,0.75f,0.0f,
        -0.25f,0.0f,-0.3f,
	-0.25f,0.75f,-0.3f
*/
    	//Right Arm Second Half!!

	//TOP OF Elbow, BOTTOM
	AX2,AY1-0.1f,AZ1,
        AX2,AY1-0.1f,AZ2,
        AX1,AY1-0.1f,AZ1,
        AX1,AY1-0.1f,AZ2,

	//Bottom Of Arm, BOTTOM
	AX2,AY1-AY2-0.1f,AZ1,
        AX2,AY1-AY2-0.1f,AZ2,
        AX1,AY1-AY2-0.1f,AZ1,
        AX1,AY1-AY2-0.1f,AZ2,

	//Front of Bottom Arm
	AX2,AY2-AY2-0.1f,AZ1,
        AX1,AY2-AY2-0.1f,AZ1,
        AX2,AY1-AY2-0.1f,AZ1,
        AX1,AY1-AY2-0.1f,AZ1,

	//Left of Bottom Arm
	AX2,AY1-AY2-0.1f,AZ1,
        AX2,AY1-AY2-0.1f,AZ2,
        AX2,AY2-AY2-0.1f,AZ1,
        AX2,AY2-AY2-0.1f,AZ2,

	//Right of Bottom Arm
	AX1,AY1-AY2-0.1f,AZ1,
        AX1,AY1-AY2-0.1f,AZ2,
        AX1,AY2-AY2-0.1f,AZ1,
        AX1,AY2-AY2-0.1f,AZ2,

	//Back of Bottom Arm		
	AX2,AY2-AY2-0.1f,AZ2,
        AX1,AY2-AY2-0.1f,AZ2,
        AX2,AY1-AY2-0.1f,AZ2,
        AX1,AY1-AY2-0.1f,AZ2

    };

    glm::vec3 RArm_Position(0.0f,0.0f,0.0f);

    GLfloat LArm[] = {
        //Left Arm
        //Front
        AX2+1.25f,AY2,AZ1,
        AX1+1.25f,AY2,AZ1,
        AX2+1.25f,AY1,AZ1,
        AX1+1.25f,AY1,AZ1,

        //Bottom
        AX2+1.25f,AY1,AZ1,
        AX2+1.25f,AY1,AZ2,
        AX1+1.25f,AY1,AZ1,
        AX1+1.25f,AY1,AZ2,

        //Back
        AX2+1.25f,AY2,AZ2,
        AX1+1.25f,AY2,AZ2,
        AX2+1.25f,AY1,AZ2,
        AX1+1.25f,AY1,AZ2,

        //Top
        AX2+1.25f,AY2,AZ1,
        AX2+1.25f,AY2,AZ2,
        AX1+1.25f,AY2,AZ1,
        AX1+1.25f,AY2,AZ2,

	//Left
	AX2+1.25f,AY1,AZ1,
        AX2+1.25f,AY1,AZ2,
        AX2+1.25f,AY2,AZ1,
        AX2+1.25f,AY2,AZ2,

        //Right
        AX1+1.25f,AY1,AZ1,
        AX1+1.25f,AY1,AZ2,
        AX1+1.25f,AY2,AZ1,
        AX1+1.25f,AY2,AZ2,

    	//Left Arm Second Half

        //TOP OF Elbow, BOTTOM
        AX2+1.25f,AY1-0.1f,AZ1,
        AX2+1.25f,AY1-0.1f,AZ2,
        AX1+1.25f,AY1-0.1f,AZ1,
        AX1+1.25f,AY1-0.1f,AZ2,

        //Bottom Of Arm, BOTTOM
        AX2+1.25f,AY1-AY2-0.1f,AZ1,
        AX2+1.25f,AY1-AY2-0.1f,AZ2,
        AX1+1.25f,AY1-AY2-0.1f,AZ1,
        AX1+1.25f,AY1-AY2-0.1f,AZ2,

        //Front of Bottom Arm
        AX2+1.25f,AY2-AY2-0.1f,AZ1,
        AX1+1.25f,AY2-AY2-0.1f,AZ1,
        AX2+1.25f,AY1-AY2-0.1f,AZ1,
        AX1+1.25f,AY1-AY2-0.1f,AZ1,

        //Left of Bottom Arm
        AX2+1.25f,AY1-AY2-0.1f,AZ1,
        AX2+1.25f,AY1-AY2-0.1f,AZ2,
        AX2+1.25f,AY2-AY2-0.1f,AZ1,
        AX2+1.25f,AY2-AY2-0.1f,AZ2,

	//Right of Bottom Arm
	AX1+1.25f,AY1-AY2-0.1f,AZ1,
        AX1+1.25f,AY1-AY2-0.1f,AZ2,
        AX1+1.25f,AY2-AY2-0.1f,AZ1,
        AX1+1.25f,AY2-AY2-0.1f,AZ2,

        //Back of Bottom Arm            
        AX2+1.25f,AY2-AY2-0.1f,AZ2,
        AX1+1.25f,AY2-AY2-0.1f,AZ2,
        AX2+1.25f,AY1-AY2-0.1f,AZ2,
        AX1+1.25f,AY1-AY2-0.1f,AZ2

    };
    glm::vec3 LArm_Position(0.0f,0.0f,0.0f);

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

    GLuint VAO[4],VBO[4];
    glGenBuffers(4,VBO);
    glGenVertexArrays(4,VAO);

    // bind vertex array object
    glBindVertexArray(/* TO CHANGE  */VAO[0]);

    // copy the vertices in a buffer
    glBindBuffer(GL_ARRAY_BUFFER,/* TO CHANGE  */VBO[0]);
    glBufferData(GL_ARRAY_BUFFER,/* TO CHANGE  */sizeof(vertices),/* TO CHANGE  */vertices,GL_STATIC_DRAW);

    // set position attribute pointers
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,/* TO CHANGE  */5*sizeof(GL_FLOAT),(GLvoid*)0);
    
    glEnableVertexAttribArray(0);
    
    // set texture attribute pointers
    glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,/* TO CHANGE  */5*sizeof(GL_FLOAT),(GLvoid*)(3*sizeof(GLfloat)));
    
    glEnableVertexAttribArray(1);

    // unbind the vertex array object
    glBindVertexArray(0);

    //SETUP BODY

    // bind vertex array object
    glBindVertexArray(/* TO CHANGE  */VAO[1]);

    // copy the vertices in a buffer
    glBindBuffer(GL_ARRAY_BUFFER,/* TO CHANGE  */VBO[1]);
    glBufferData(GL_ARRAY_BUFFER,/* TO CHANGE  */sizeof(Body),/* TO CHANGE  */Body,GL_STATIC_DRAW);

    // set position attribute pointers
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,/* TO CHANGE  */3*sizeof(GL_FLOAT),(GLvoid*)0);
        
    glEnableVertexAttribArray(0);
   
    //glEnableVertexAttribArray(1);

    // unbind the vertex array object
    glBindVertexArray(0);

    //END SETUP BODY

    //SETUP RARM

    // bind vertex array object
    glBindVertexArray(/* TO CHANGE  */VAO[2]);

    // copy the vertices in a buffer
    glBindBuffer(GL_ARRAY_BUFFER,/* TO CHANGE  */VBO[2]);
    glBufferData(GL_ARRAY_BUFFER,/* TO CHANGE  */sizeof(RArm),/* TO CHANGE  */RArm,GL_STATIC_DRAW);

    // set position attribute pointers
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,/* TO CHANGE  */3*sizeof(GL_FLOAT),(GLvoid*)0);

    glEnableVertexAttribArray(0);

    //glEnableVertexAttribArray(1);

    // unbind the vertex array object
    glBindVertexArray(0);

    //END SETUP RARM

    //SETUP LARM

    // bind vertex array object
    glBindVertexArray(/* TO CHANGE  */VAO[3]);

    // copy the vertices in a buffer
    glBindBuffer(GL_ARRAY_BUFFER,/* TO CHANGE  */VBO[3]);
    glBufferData(GL_ARRAY_BUFFER,/* TO CHANGE  */sizeof(LArm),/* TO CHANGE  */LArm,GL_STATIC_DRAW);

    // set position attribute pointers
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,/* TO CHANGE  */3*sizeof(GL_FLOAT),(GLvoid*)0);

    glEnableVertexAttribArray(0);

    //glEnableVertexAttribArray(1);

    // unbind the vertex array object
    glBindVertexArray(0);

    //END SETUP LARM

    //Shader VFShader("shader.vs","shader.frag");

    // view space transform
    glm::mat4 view(1.0f);
    
    //Model Space
    //glm::mat4 model(1.0f);

    // note that we're translating the scene in the reverse direction of where we want to move
    view=glm::translate(view,glm::vec3(0.0f,0.0f,-100.0f));
    GLfloat R = PI/4;
    ///glm::mat4 model = glm::rotate(glm::mat4(1.0f),R,glm::vec3(0.0,1.0,0.0));	

    glEnable(GL_DEPTH_TEST);

    //Shader VFShader("transform.vs","shader.frag");

    while(!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        glClearColor(.2f,.3f,.3f,1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	GLfloat current_frame=glfwGetTime();
        delta_time=current_frame-last_frame;
        last_frame=current_frame;

	//Update View
	view=glm::lookAt(camera_position,camera_position+camera_front,camera_up);

	//Update Projection
        projection=glm::perspective(glm::radians(fov),(float)width/height,0.1f,100.0f);

        // use shader program
        our_shader.Use();

        // bind textures
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,texture1);
        glUniform1i(glGetUniformLocation(our_shader.program,"our_texture1"),0);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D,texture2);
        glUniform1i(glGetUniformLocation(our_shader.program,"our_texture2"),1);

	//Validate matrices for Shader
        glUniform1f(glGetUniformLocation(our_shader.program,"mix_value"),mix_value);
        GLuint view_location=glGetUniformLocation(our_shader.program,"view");
        glUniformMatrix4fv(view_location,1,GL_FALSE,glm::value_ptr(view));
        GLuint projection_location=glGetUniformLocation(our_shader.program,"projection");
        glUniformMatrix4fv(projection_location,1,GL_FALSE,glm::value_ptr(projection));
        ///////////////////////

        // draw
        glBindVertexArray(VAO[0]);
        
	for(GLuint i=0;i<5;i+=2)
        {

 	   GLuint model_location=glGetUniformLocation(our_shader.program,"model");

            // world space transform
            glm::mat4 model(1.0f);
            
  	    model=glm::translate(model,cube_positions[i]);
            model=glm::rotate(model,glm::radians(/*(GLfloat)glfwGetTime()*50.0f*/ROT),glm::vec3(0.0f,1.0f,0.0f));
            
	    model=glm::translate(model,cube_positions[i+1]);

    	    glUniformMatrix4fv(model_location,1,GL_FALSE,glm::value_ptr(model));

            glDrawArrays(GL_TRIANGLES,0,/*36*//*18*/24);
            
 	}
	glBindVertexArray(0);

	//Draw Body
	VFShader.Use();

	glBindVertexArray(VAO[1]);


	//Validate Shader Matrices
	
	GLuint model_location1 = Validate(VFShader,view,projection);

       	    /////////////////////////

	    // world space transformations
            glm::mat4 model1(1.0f);

            model1=glm::translate(model1,Body_Position);
            model1=glm::rotate(model1,glm::radians(/*(GLfloat)glfwGetTime()*50.0f*/ROT),glm::vec3(0.0f,1.0f,0.0f)); 
	  
   	    /////////////////////////////

            glUniformMatrix4fv(model_location1,1,GL_FALSE,glm::value_ptr(model1));
	    
            glDrawArrays(GL_TRIANGLE_STRIP,0,4);
	    glDrawArrays(GL_TRIANGLE_STRIP,4,4);
	    glDrawArrays(GL_TRIANGLE_STRIP,8,4);
	    glDrawArrays(GL_TRIANGLE_STRIP,12,4);
	    glDrawArrays(GL_TRIANGLE_STRIP,16,4);
	    glDrawArrays(GL_TRIANGLE_STRIP,20,4);

	glBindVertexArray(0);
	//End Draw Body

	//Draw RArm
	VFShader2.Use();

        glBindVertexArray(/* CHANGE */VAO[2]);

        //Validate Shader Matrices

        GLuint /* CHANGE */model_location2 = Validate(/* CHANGE */VFShader2,view,projection);

            /////////////////////////

            // world space transformations
            glm::mat4 /* CHANGE */model2(1.0f);

            model2=glm::translate(/* CHANGE */model2,/* CHANGE */RArm_Position);
            model2=glm::rotate(/* CHANGE */model2,glm::radians(/*(GLfloat)glfwGetTime()*50.0f*/ROT),glm::vec3(0.0f,1.0f,0.0f));

            /////////////////////////////

            glUniformMatrix4fv(/* CHANGE */ model_location2,1,GL_FALSE,glm::value_ptr(/* CHANGE */model2));

            //TOP ARM
	    glDrawArrays(GL_TRIANGLE_STRIP,0,4);
            glDrawArrays(GL_TRIANGLE_STRIP,4,4);
            glDrawArrays(GL_TRIANGLE_STRIP,8,4);
            glDrawArrays(GL_TRIANGLE_STRIP,12,4);
            glDrawArrays(GL_TRIANGLE_STRIP,16,4);
            glDrawArrays(GL_TRIANGLE_STRIP,20,4);
	    
	    //BOTTOM ARM
	    glDrawArrays(GL_TRIANGLE_STRIP,24,4);
            glDrawArrays(GL_TRIANGLE_STRIP,28,4);
	    glDrawArrays(GL_TRIANGLE_STRIP,32,4);
	    glDrawArrays(GL_TRIANGLE_STRIP,36,4);
	    glDrawArrays(GL_TRIANGLE_STRIP,40,4);
	    glDrawArrays(GL_TRIANGLE_STRIP,44,4);

	glBindVertexArray(0);

	//End Draw RArm

	//Draw LArm
	VFShader2.Use();

        glBindVertexArray(/* CHANGE */VAO[3]);

        //Validate Shader Matrices

        GLuint /* CHANGE */model_location3 = Validate(/* CHANGE */VFShader2,view,projection);

            /////////////////////////

            // world space transformations
            glm::mat4 /* CHANGE */model3(1.0f);

            model2=glm::translate(/* CHANGE */model3,/* CHANGE */LArm_Position);
            model2=glm::rotate(/* CHANGE */model3,glm::radians(/*(GLfloat)glfwGetTime()*50.0f*/ROT),glm::vec3(0.0f,1.0f,0.0f));

            /////////////////////////////

            glUniformMatrix4fv(/* CHANGE */ model_location3,1,GL_FALSE,glm::value_ptr(/* CHANGE */model2));

	    //TOP ARM
            glDrawArrays(GL_TRIANGLE_STRIP,0,4);
            glDrawArrays(GL_TRIANGLE_STRIP,4,4);
            glDrawArrays(GL_TRIANGLE_STRIP,8,4);
            glDrawArrays(GL_TRIANGLE_STRIP,12,4);
            glDrawArrays(GL_TRIANGLE_STRIP,16,4);
            glDrawArrays(GL_TRIANGLE_STRIP,20,4);

            //BOTTOM ARM
            glDrawArrays(GL_TRIANGLE_STRIP,24,4);
            glDrawArrays(GL_TRIANGLE_STRIP,28,4);
            glDrawArrays(GL_TRIANGLE_STRIP,32,4);
            glDrawArrays(GL_TRIANGLE_STRIP,36,4);
            glDrawArrays(GL_TRIANGLE_STRIP,40,4);
            glDrawArrays(GL_TRIANGLE_STRIP,44,4);


	glBindVertexArray(0);
	//End Draw LArm

        //glBindVertexArray(0);
	//glBindVertexArrat(VAO[1]);

        glfwSwapBuffers(window);
    }

    // deallocate all resources
    glDeleteVertexArrays(4,VAO);
    glDeleteBuffers(4,VBO);
    // terminate GLFW
    glfwTerminate();

    return 0;
}
