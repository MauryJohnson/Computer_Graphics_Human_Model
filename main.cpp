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

GLuint VAO[9],VBO[9];

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


GLfloat * PLArm;
GLfloat LArmR = 0;
GLfloat LArmR2 = 0;
GLfloat* PRArm;
GLfloat RArmR = 0;
GLfloat RArmR2 = 0;
GLfloat* PLLeg;
GLfloat LLegR = 0;
GLfloat LLegR2 = 0;
GLfloat* PRLeg;
GLfloat RLegR = 0;
GLfloat RLegR2 = 0;

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
		 int Range[] = {
		0,47		
		 };
		 LArmR+=PI/2;
		 //if(LArmR>=2*PI)
		//RotateArray(PLArm, Range, LArmR,0);				
	}	
	else if(keys[GLFW_KEY_K]){
		printf("\n Want to move left arm Clockwise");
	  	 //RotateArray(GLfloat* G, int* Range, GLfloat Radians,int Axis)
		LArmR-=PI/2;
	}	
	else if(keys[GLFW_KEY_U]){
		LArmR2+=PI/2;
	}
    	else if(keys[GLFW_KEY_I]){
		LArmR2-=PI/2;
	}
    }

    if((keys[GLFW_KEY_S]/* && (action==GLFW_PRESS) || key==GLFW_KEY_A && (action==GLFW_REPEAT)*/)){
        printf("RIGHT ARM");
        if(keys[GLFW_KEY_J]){
                printf("\n Want to move right arm Counter Clockwise");
		RArmR+=PI/2;
        }       
        else if(keys[GLFW_KEY_K]){
                printf("\n Want to move right arm Clockwise");
		RArmR-=PI/2;
        }       
	else if(keys[GLFW_KEY_U]){
                RArmR2+=PI/2;
        }
        else if(keys[GLFW_KEY_I]){
                RArmR2-=PI/2;
        }
	
    } 
	
    if((keys[GLFW_KEY_D]/* && (action==GLFW_PRESS) || key==GLFW_KEY_A && (action==GLFW_REPEAT)*/)){
        printf("LEFT LEG");
        if(keys[GLFW_KEY_J]){
                printf("\n Want to move left leg Counter Clockwise");
		LLegR+=PI/2;
        }       
        else if(keys[GLFW_KEY_K]){
                printf("\n Want to move left leg  Clockwise");
		LLegR-=PI/2;
        }       
	else if(keys[GLFW_KEY_U]){
                LLegR2+=PI/2;
        }
        else if(keys[GLFW_KEY_I]){
                LLegR2-=PI/2;
        }

    } 	
 

    if((keys[GLFW_KEY_F]/* && (action==GLFW_PRESS) || key==GLFW_KEY_A && (action==GLFW_REPEAT)*/)){
        printf("RIGHT LEG");
        if(keys[GLFW_KEY_J]){
                printf("\n Want to move right leg Counter Clockwise");
		RLegR+=PI/2;
        }       
        else if(keys[GLFW_KEY_K]){
                printf("\n Want to move right leg Clockwise");
		RLegR-=PI/2;
        }       
	else if(keys[GLFW_KEY_U]){
                RLegR2+=PI/2;
        }
        else if(keys[GLFW_KEY_I]){
                RLegR2-=PI/2;
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

void SetUp3Array(GLfloat* P,int IDX,int DIM){


    // bind vertex array object
    glBindVertexArray(/* TO CHANGE  */VAO[IDX]);

    // copy the vertices in a buffer
    glBindBuffer(GL_ARRAY_BUFFER,/* TO CHANGE  */VBO[IDX]);
    glBufferData(GL_ARRAY_BUFFER,/* TO CHANGE  */sizeof(GLfloat),/* TO CHANGE  */P,GL_STATIC_DRAW);

    // set position attribute pointers
    glVertexAttribPointer(0,DIM,GL_FLOAT,GL_FALSE,/* TO CHANGE  */DIM*sizeof(GL_FLOAT),(GLvoid*)0);

    glEnableVertexAttribArray(0);

    // unbind the vertex array object
    glBindVertexArray(0);


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
    //GLfloat * Triangle2 = MatrixBuilder::Rotate(Triangle1,3,3,8,-PI/2,0,2).data();
    //GLfloat * Triangle3 =  MatrixBuilder::Rotate(Triangle1,3,3,8,-PI,0,2).data();
    //GLfloat * Triangle4 =  MatrixBuilder::Rotate(Triangle1,3,3,8,-3*PI/2,0,2).data();
     /*
     GLfloat* V[4]{
	Triangle1,Triangle2,Triangle3,Triangle4
     };
     */
    GLfloat vertices[4*3*5*8];
 
    //GLfloat * v = MatrixBuilder::TwoToOne(V,4,3,5,8).data();
    //if(vertices==NULL){
 //	exit(-1);
   // } 
    //for(int i=0; i<4*3*5*8;i+=1)
    //vertices[i]=v[i];
 
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
    PRArm = RArm;
    glm::vec3 RArm_Position(0.0f,0.0f,0.0f);
    glm::vec3 RArm_Shoulder((AX1)/2.0,AY2,AZ2/2.0);
    glm::vec3 RArm_Elbow((AX2)/2.0,(AY1-0.1f)/2.0,AZ2/2.0);

    //AX2,AY1-0.1f,AZ1,
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
    PLArm = LArm;
    glm::vec3 LArm_Position(0.0f,0.0f,0.0f);
    glm::vec3 LArm_Shoulder((AX2+1.25f)/2.0,AY2,AZ2/2.0);
    glm::vec3 LArm_Elbow((AX2+1.25f)/2.0,(AY1-0.1f)/2.0,AZ2/2.0);

    //Alter values slightly

    AX2 = -0.5f;
    AX1 = -0.1f;

    AY2 = -1.5f;
    AY1 = -0.75f;

    AZ2 = -0.25f;
    AZ1 = 0.0f;


    GLfloat RLeg[] = {
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

	//TOP OF Knee, BOTTOM
	AX2,AY1-0.1f,AZ1,
        AX2,AY1-0.1f,AZ2,
        AX1,AY1-0.1f,AZ1,
        AX1,AY1-0.1f,AZ2,

	//Bottom Of Knee, BOTTOM
	AX2,AY1+AY1-0.1f,AZ1,
        AX2,AY1+AY1-0.1f,AZ2,
        AX1,AY1+AY1-0.1f,AZ1,
        AX1,AY1+AY1-0.1f,AZ2,

	//Front of Bottom Leg
	AX2,AY2+AY1-0.1f,AZ1,
        AX1,AY2+AY1-0.1f,AZ1,
        AX2,AY1+AY1-0.1f,AZ1,
        AX1,AY1+AY1-0.1f,AZ1,

	//Left of Bottom Leg
	AX2,AY1+AY1-0.1f,AZ1,
        AX2,AY1+AY1-0.1f,AZ2,
        AX2,AY2+AY1-0.1f,AZ1,
        AX2,AY2+AY1-0.1f,AZ2,

	//Right of Bottom Leg
	AX1,AY1+AY1-0.1f,AZ1,
        AX1,AY1+AY1-0.1f,AZ2,
        AX1,AY2+AY1-0.1f,AZ1,
        AX1,AY2+AY1-0.1f,AZ2,

	//Back of Bottom Leg		
	AX2,AY2+AY1-0.1f,AZ2,
        AX1,AY2+AY1-0.1f,AZ2,
        AX2,AY1+AY1-0.1f,AZ2,
        AX1,AY1+AY1-0.1f,AZ2

    };
    PRLeg = RLeg;
    glm::vec3 RLeg_Position(0.0f,0.0f,0.0f);
    glm::vec3 RLeg_Hip((AX2+AX1)/2.0,AY1,AZ2/2.0);
    glm::vec3 RLeg_Knee((AX2+AX1)/2.0,(2*AY1-0.1f),AZ2/2.0);

    AX2+=0.6f;
    AX1+=0.6f;
 
     GLfloat LLeg[] = {
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

	//TOP OF Knee, BOTTOM
	AX2,AY1-0.1f,AZ1,
        AX2,AY1-0.1f,AZ2,
        AX1,AY1-0.1f,AZ1,
        AX1,AY1-0.1f,AZ2,

	//Bottom Of Knee, BOTTOM
	AX2,AY1+AY1-0.1f,AZ1,
        AX2,AY1+AY1-0.1f,AZ2,
        AX1,AY1+AY1-0.1f,AZ1,
        AX1,AY1+AY1-0.1f,AZ2,

	//Front of Bottom Leg
	AX2,AY2+AY1-0.1f,AZ1,
        AX1,AY2+AY1-0.1f,AZ1,
        AX2,AY1+AY1-0.1f,AZ1,
        AX1,AY1+AY1-0.1f,AZ1,

	//Left of Bottom Leg
	AX2,AY1+AY1-0.1f,AZ1,
        AX2,AY1+AY1-0.1f,AZ2,
        AX2,AY2+AY1-0.1f,AZ1,
        AX2,AY2+AY1-0.1f,AZ2,

	//Right of Bottom Leg
	AX1,AY1+AY1-0.1f,AZ1,
        AX1,AY1+AY1-0.1f,AZ2,
        AX1,AY2+AY1-0.1f,AZ1,
        AX1,AY2+AY1-0.1f,AZ2,

	//Back of Bottom Leg		
	AX2,AY2+AY1-0.1f,AZ2,
        AX1,AY2+AY1-0.1f,AZ2,
        AX2,AY1+AY1-0.1f,AZ2,
        AX1,AY1+AY1-0.1f,AZ2

    };    

    PLLeg = LLeg;
    glm::vec3 LLeg_Position(0.0f,0.0f,0.0f);
    glm::vec3 LLeg_Hip((AX1+AX2)/2.0,AY1,AZ2/2.0);
    glm::vec3 LLeg_Knee((AX2+AX1)/2.0,(2*AY1-0.1f),AZ2/2.0);


    //AX2+1.25f,AY2,AZ1,
        //AX2+1.25f,AY2,AZ2,
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

    //GLuint VAO[4],VBO[4];
    glGenBuffers(9,VBO);
    glGenVertexArrays(9,VAO);

    // bind vertex array object
    //glBindVertexArray(/* TO CHANGE  */VAO[0]);

    // copy the vertices in a buffer
    //glBindBuffer(GL_ARRAY_BUFFER,/* TO CHANGE  */VBO[0]);
    //glBufferData(GL_ARRAY_BUFFER,/* TO CHANGE  */sizeof(vertices),/* TO CHANGE  */vertices,GL_STATIC_DRAW);

    // set position attribute pointers
    //glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,/* TO CHANGE  */5*sizeof(GL_FLOAT),(GLvoid*)0);
    
    //glEnableVertexAttribArray(0);
    
    // set texture attribute pointers
  //  g//lVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,/* TO CHANGE  */5*sizeof(GL_FLOAT),(GLvoid*)(3*sizeof(GLfloat)));
    
 //   /glEnableVertexAttribArray(1);
//
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

    //SETUP RARM SHOULDER

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

    //END SETUP RARM SHOULDER

    //SETUP RARM ELBOW

    // bind vertex array object
    glBindVertexArray(/* TO CHANGE  */VAO[5]);

    // copy the vertices in a buffer
    glBindBuffer(GL_ARRAY_BUFFER,/* TO CHANGE  */VBO[5]);
    glBufferData(GL_ARRAY_BUFFER,/* TO CHANGE  */sizeof(RArm),/* TO CHANGE  */RArm,GL_STATIC_DRAW);

    // set position attribute pointers
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,/* TO CHANGE  */3*sizeof(GL_FLOAT),(GLvoid*)0);

    glEnableVertexAttribArray(0);

    //glEnableVertexAttribArray(1);

    // unbind the vertex array object
    glBindVertexArray(0);

    //END SETUP RARM ELBOW

    //SETUP LARM SHOULDER

    //SetUp3Array(LArm,3,3);

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

    //END SETUP LARM SHOULDER

    //SETUP LARM ELBOW

    // bind vertex array object
    glBindVertexArray(/* TO CHANGE  */VAO[4]);

    // copy the vertices in a buffer
    glBindBuffer(GL_ARRAY_BUFFER,/* TO CHANGE  */VBO[4]);
    glBufferData(GL_ARRAY_BUFFER,/* TO CHANGE  */sizeof(LArm),/* TO CHANGE  */LArm,GL_STATIC_DRAW);

    // set position attribute pointers
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,/* TO CHANGE  */3*sizeof(GL_FLOAT),(GLvoid*)0);

    glEnableVertexAttribArray(0);

    // unbind the vertex array object
    glBindVertexArray(0);

    //END SETUP LARM ELBOW

    //SETUP RLEG HIP

     // bind vertex array object
     glBindVertexArray(/* TO CHANGE  */VAO[0]);

    // copy the vertices in a buffer
    glBindBuffer(GL_ARRAY_BUFFER,/* TO CHANGE  */VBO[0]);
    glBufferData(GL_ARRAY_BUFFER,/* TO CHANGE  */sizeof(RLeg),/* TO CHANGE  */RLeg,GL_STATIC_DRAW);

    // set position attribute pointers
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,/* TO CHANGE  */3*sizeof(GL_FLOAT),(GLvoid*)0);

    glEnableVertexAttribArray(0);

    // unbind the vertex array object
    glBindVertexArray(0);

    //END SETUP RLEG HIP

    //SETUP RLEG KNEE
 
    // bind vertex array object
    glBindVertexArray(/* TO CHANGE  */VAO[6]);

    // copy the vertices in a buffer
    glBindBuffer(GL_ARRAY_BUFFER,/* TO CHANGE  */VBO[6]);
    glBufferData(GL_ARRAY_BUFFER,/* TO CHANGE  */sizeof(RLeg),/* TO CHANGE  */RLeg,GL_STATIC_DRAW);

    // set position attribute pointers
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,/* TO CHANGE  */3*sizeof(GL_FLOAT),(GLvoid*)0);

    glEnableVertexAttribArray(0);

    // unbind the vertex array object
    glBindVertexArray(0);

    //END SETUP RLEG KNEE

    //SETUP LLEG HIP

     // bind vertex array object
    glBindVertexArray(/* TO CHANGE  */VAO[7]);

    // copy the vertices in a buffer
    glBindBuffer(GL_ARRAY_BUFFER,/* TO CHANGE  */VBO[7]);
    glBufferData(GL_ARRAY_BUFFER,/* TO CHANGE  */sizeof(LLeg),/* TO CHANGE  */LLeg,GL_STATIC_DRAW);

    // set position attribute pointers
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,/* TO CHANGE  */3*sizeof(GL_FLOAT),(GLvoid*)0);

    glEnableVertexAttribArray(0);

    // unbind the vertex array object
    glBindVertexArray(0);

    //END SETUP LLEG HIP

    //SETUP LLEG KNEE

     // bind vertex array object
    //glBindVertexArray(/* TO CHANGE  */VAO[8]);

    // copy the vertices in a buffer
    //glBindBuffer(GL_ARRAY_BUFFER,/* TO CHANGE  */VBO[8]);
    //glBufferData(GL_ARRAY_BUFFER,/* TO CHANGE  */sizeof(LArm),/* TO CHANGE  */LArm,GL_STATIC_DRAW);

    // set position attribute pointers
    //glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,/* TO CHANGE  */3*sizeof(GL_FLOAT),(GLvoid*)0);

    //glEnableVertexAttribArray(0);

    // unbind the vertex array object
    //glBindVertexArray(0);

    //END SETUP LLEG KNEE

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
//        glBindVertexArray(VAO[0]);
        
//	for(GLuint i=0;i<5;i+=2)
  //      {
//
 //	   GLuint model_location=glGetUniformLocation(our_shader.program,"model");

            // world space transform
   //         glm::mat4 model(1.0f);
            
  ///	    model=glm::translate(model,cube_positions[i]);
     //       model=glm::rotate(model,glm::radians(/*(GLfloat)glfwGetTime()*50.0f*/ROT),glm::vec3(0.0f,1.0f,0.0f));
            
//	    model=glm::translate(model,cube_positions[i+1]);

  //  	    glUniformMatrix4fv(model_location,1,GL_FALSE,glm::value_ptr(model));

    //        glDrawArrays(GL_TRIANGLES,0,/*36*//*18*/24);
            
 //	}
//	glBindVertexArray(0);

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

	//Draw LArm
	VFShader2.Use();

        glBindVertexArray(/* CHANGE */VAO[3]);

        //Validate Shader Matrices

        GLuint /* CHANGE */model_location3 = Validate(/* CHANGE */VFShader2,view,projection);

            /////////////////////////

            // world space transformations
            glm::mat4 /* CHANGE */model3(1.0f);

            glm::mat4 model2=glm::translate(/* CHANGE */model3,/* CHANGE */LArm_Position);
            model2=glm::rotate(/* CHANGE */model3,glm::radians(/*(GLfloat)glfwGetTime()*50.0f*/ROT),glm::vec3(0.0f,1.0f,0.0f));

	    //MY SHOULDER    

	    model2=glm::translate(model2,LArm_Shoulder);	
	 
   	    model2=glm::rotate(model2,glm::radians(/*(GLfloat)glfwGetTime()*50.0f*/LArmR),glm::vec3(1.0f,0.0f,0.0f));

	    model2=glm::translate(model2,-LArm_Shoulder);

            glUniformMatrix4fv(/* CHANGE */ model_location3,1,GL_FALSE,glm::value_ptr(/* CHANGE */model2));

	    //TOP ARM
            glDrawArrays(GL_TRIANGLE_STRIP,0,4);
            glDrawArrays(GL_TRIANGLE_STRIP,4,4);
            glDrawArrays(GL_TRIANGLE_STRIP,8,4);
            glDrawArrays(GL_TRIANGLE_STRIP,12,4);
            glDrawArrays(GL_TRIANGLE_STRIP,16,4);
            glDrawArrays(GL_TRIANGLE_STRIP,20,4);


            //BOTTOM ARM
	glBindVertexArray(0);
	
	VFShader2.Use();

	glBindVertexArray(VAO[4]);	

	GLuint /* CHANGE */model_location4 = Validate(/* CHANGE */VFShader2,view,projection);
            // world space transformations
            glm::mat4 /* CHANGE */model4(1.0f);

            model2=glm::translate(/* CHANGE */model4,/* CHANGE */LArm_Position);
            model2=glm::rotate(/* CHANGE */model4,glm::radians(/*(GLfloat)glfwGetTime()*50.0f*/ROT),glm::vec3(0.0f,1.0f,0.0f));

	    //ROTATES WITH THE SHOULDER PARENT
		
            model2=glm::translate(model2,LArm_Shoulder);

            model2=glm::rotate(model2,glm::radians(/*(GLfloat)glfwGetTime()*50.0f*/LArmR),glm::vec3(1.0f,0.0f,0.0f));

            model2=glm::translate(model2,-LArm_Shoulder);
	    //////////////////////////////////
 
   	    //ROTATES WITH ITS OWN PERMISSION
	    model2=glm::translate(model2,/*CHANGE*/LArm_Elbow);

	    model2=glm::rotate(model2,glm::radians(/*(GLfloat)glfwGetTime()*50.0f*/LArmR2),glm::vec3(1.0f,0.0f,0.0f));

  	    model2=glm::translate(model2,/*CHANGE*/-LArm_Elbow);	   
            glUniformMatrix4fv(/* CHANGE */ model_location4,1,GL_FALSE,glm::value_ptr(/* CHANGE */model2));

              glDrawArrays(GL_TRIANGLE_STRIP,24,4);
              glDrawArrays(GL_TRIANGLE_STRIP,28,4);
              glDrawArrays(GL_TRIANGLE_STRIP,32,4);
              glDrawArrays(GL_TRIANGLE_STRIP,36,4);
              glDrawArrays(GL_TRIANGLE_STRIP,40,4);
              glDrawArrays(GL_TRIANGLE_STRIP,44,4);

	glBindVertexArray(0);
	//End Draw LArm

	//Draw RArm
	VFShader2.Use();

        glBindVertexArray(/* CHANGE */VAO[2]);

        //Validate Shader Matrices

        GLuint /* CHANGE */model_location5 = Validate(/* CHANGE */VFShader2,view,projection);

            /////////////////////////

            // world space transformations
            glm::mat4 /* CHANGE */model5(1.0f);

            model2=glm::translate(/* CHANGE */model5,/* CHANGE */RArm_Position);
            model2=glm::rotate(model2,glm::radians(ROT),glm::vec3(0.0f,1.0f,0.0f));

            //MY SHOULDER    

            model2=glm::translate(model2,/*CHANGE*/RArm_Shoulder);

            model2=glm::rotate(model2,glm::radians(/*CHANGE*/RArmR),glm::vec3(1.0f,0.0f,0.0f));

            model2=glm::translate(model2,/*CHANGE*/-RArm_Shoulder);

            glUniformMatrix4fv(/* CHANGE */ model_location5,1,GL_FALSE,glm::value_ptr(model2));

	    //TOP ARM
            glDrawArrays(GL_TRIANGLE_STRIP,0,4);
            glDrawArrays(GL_TRIANGLE_STRIP,4,4);
            glDrawArrays(GL_TRIANGLE_STRIP,8,4);
            glDrawArrays(GL_TRIANGLE_STRIP,12,4);
            glDrawArrays(GL_TRIANGLE_STRIP,16,4);
            glDrawArrays(GL_TRIANGLE_STRIP,20,4);

	glBindVertexArray(0);

        VFShader2.Use();

        glBindVertexArray(VAO[5]);

        GLuint /* CHANGE */model_location6 = Validate(/* CHANGE */VFShader2,view,projection);
            // world space transformations
            glm::mat4 /* CHANGE */model6(1.0f);

            model2=glm::translate(/* CHANGE */model6,/* CHANGE */RArm_Position);
            model2=glm::rotate(model2,glm::radians(ROT),glm::vec3(0.0f,1.0f,0.0f));

            //ROTATES WITH THE SHOULDER PARENT

            model2=glm::translate(model2,/*CHANGE*/RArm_Shoulder);

            model2=glm::rotate(model2,glm::radians(/*CHANGE*/RArmR),glm::vec3(1.0f,0.0f,0.0f));

            model2=glm::translate(model2,/*CHANGE*/-RArm_Shoulder);
            //////////////////////////////////

            //ROTATES WITH ITS OWN PERMISSION
            model2=glm::translate(model2,/*CHANGE*/RArm_Elbow);

            model2=glm::rotate(model2,glm::radians(RArmR2),glm::vec3(1.0f,0.0f,0.0f));

            model2=glm::translate(model2,/*CHANGE*/-RArm_Elbow);

	    glUniformMatrix4fv(/* CHANGE */ model_location6,1,GL_FALSE,glm::value_ptr(model2));

              glDrawArrays(GL_TRIANGLE_STRIP,24,4);
              glDrawArrays(GL_TRIANGLE_STRIP,28,4);
              glDrawArrays(GL_TRIANGLE_STRIP,32,4);
              glDrawArrays(GL_TRIANGLE_STRIP,36,4);
              glDrawArrays(GL_TRIANGLE_STRIP,40,4);
              glDrawArrays(GL_TRIANGLE_STRIP,44,4);

        glBindVertexArray(0);
        //End Draw RArm

	//Draw RLeg

	VFShader2.Use();

        glBindVertexArray(/* CHANGE */VAO[0]);

        //Validate Shader Matrices

        GLuint /* CHANGE */model_location7 = Validate(/* CHANGE */VFShader2,view,projection);

            /////////////////////////

            // world space transformations
            glm::mat4 /* CHANGE */model7(1.0f);

            model2=glm::translate(/* CHANGE */model7,/* CHANGE */RLeg_Position);
            model2=glm::rotate(model2,glm::radians(ROT),glm::vec3(0.0f,1.0f,0.0f));

            //MY HIP 

            model2=glm::translate(model2,/*CHANGE*/RLeg_Hip);

            model2=glm::rotate(model2,glm::radians(/*CHANGE*/RLegR),glm::vec3(1.0f,0.0f,0.0f));

            model2=glm::translate(model2,/*CHANGE*/-RLeg_Hip);

            glUniformMatrix4fv(/* CHANGE */ model_location7,1,GL_FALSE,glm::value_ptr(model2));
		
  	    glDrawArrays(GL_TRIANGLE_STRIP,0,4);
            glDrawArrays(GL_TRIANGLE_STRIP,4,4);
            glDrawArrays(GL_TRIANGLE_STRIP,8,4);
            glDrawArrays(GL_TRIANGLE_STRIP,12,4);
            glDrawArrays(GL_TRIANGLE_STRIP,16,4);
            glDrawArrays(GL_TRIANGLE_STRIP,20,4);

        glBindVertexArray(0);

	//DRAW RIGHT KNEE
	VFShader2.Use();

        glBindVertexArray(/* CHANGE */VAO[6]);

        //Validate Shader Matrices

        GLuint /* CHANGE */model_location8 = Validate(/* CHANGE */VFShader2,view,projection);

            /////////////////////////

            // world space transformations
            glm::mat4 /* CHANGE */model8(1.0f);

            model2=glm::translate(/* CHANGE */model8,/* CHANGE */RLeg_Position);
            model2=glm::rotate(model2,glm::radians(ROT),glm::vec3(0.0f,1.0f,0.0f));

            //MY HIP
            model2=glm::translate(model2,/*CHANGE*/RLeg_Hip);

            model2=glm::rotate(model2,glm::radians(/*CHANGE*/RLegR),glm::vec3(1.0f,0.0f,0.0f));

            model2=glm::translate(model2,/*CHANGE*/-RLeg_Hip);

	    //MY KNEE
	    model2=glm::translate(model2,/*CHANGE*/RLeg_Knee);

            model2=glm::rotate(model2,glm::radians(/*CHANGE*/RLegR2),glm::vec3(1.0f,0.0f,0.0f));

            model2=glm::translate(model2,/*CHANGE*/-RLeg_Knee);

            glUniformMatrix4fv(/* CHANGE */ model_location7,1,GL_FALSE,glm::value_ptr(model2));

	    //glDrawArrays(GL_TRIANGLE_STRIP,24,4);
            glDrawArrays(GL_TRIANGLE_STRIP,28,4);
            glDrawArrays(GL_TRIANGLE_STRIP,32,4);
            glDrawArrays(GL_TRIANGLE_STRIP,36,4);
            glDrawArrays(GL_TRIANGLE_STRIP,40,4);
            glDrawArrays(GL_TRIANGLE_STRIP,44,4);

        glBindVertexArray(0);	
	
	//End Draw RLeg

	//Draw LLeg

        VFShader2.Use();

        glBindVertexArray(/* CHANGE */VAO[7]);

        //Validate Shader Matrices

        GLuint /* CHANGE */model_location9 = Validate(/* CHANGE */VFShader2,view,projection);

            /////////////////////////

            // world space transformations
            glm::mat4 /* CHANGE */model9(1.0f);

            model2=glm::translate(/* CHANGE */model9,/* CHANGE */LLeg_Position);
            model2=glm::rotate(model2,glm::radians(ROT),glm::vec3(0.0f,1.0f,0.0f));

            //MY HIP 

            model2=glm::translate(model2,/*CHANGE*/LLeg_Hip);

            model2=glm::rotate(model2,glm::radians(/*CHANGE*/LLegR),glm::vec3(1.0f,0.0f,0.0f));

            model2=glm::translate(model2,/*CHANGE*/-LLeg_Hip);

            glUniformMatrix4fv(/* CHANGE */ model_location9,1,GL_FALSE,glm::value_ptr(model2));

	glDrawArrays(GL_TRIANGLE_STRIP,0,4);
            glDrawArrays(GL_TRIANGLE_STRIP,4,4);
            glDrawArrays(GL_TRIANGLE_STRIP,8,4);
            glDrawArrays(GL_TRIANGLE_STRIP,12,4);
            glDrawArrays(GL_TRIANGLE_STRIP,16,4);
            glDrawArrays(GL_TRIANGLE_STRIP,20,4);

        glBindVertexArray(0);

  	VFShader2.Use();

        glBindVertexArray(/* CHANGE */VAO[7]);

        //Validate Shader Matrices

        GLuint /* CHANGE */model_location10 = Validate(/* CHANGE */VFShader2,view,projection);

            /////////////////////////

            // world space transformations
            glm::mat4 /* CHANGE */model10(1.0f);

            model2=glm::translate(/* CHANGE */model10,/* CHANGE */LLeg_Position);
            model2=glm::rotate(model2,glm::radians(ROT),glm::vec3(0.0f,1.0f,0.0f));

            //MY HIP 

            model2=glm::translate(model2,/*CHANGE*/LLeg_Hip);

            model2=glm::rotate(model2,glm::radians(/*CHANGE*/LLegR),glm::vec3(1.0f,0.0f,0.0f));

            model2=glm::translate(model2,/*CHANGE*/-LLeg_Hip);

 	    //MY KNEE

	    model2=glm::translate(model2,/*CHANGE*/LLeg_Knee);

            model2=glm::rotate(model2,glm::radians(/*CHANGE*/LLegR2),glm::vec3(1.0f,0.0f,0.0f));

            model2=glm::translate(model2,/*CHANGE*/-LLeg_Knee);


            glUniformMatrix4fv(/* CHANGE */ model_location10,1,GL_FALSE,glm::value_ptr(model2));

	    //glDrawArrays(GL_TRIANGLE_STRIP,24,4);
            glDrawArrays(GL_TRIANGLE_STRIP,28,4);
            glDrawArrays(GL_TRIANGLE_STRIP,32,4);
            glDrawArrays(GL_TRIANGLE_STRIP,36,4);
            glDrawArrays(GL_TRIANGLE_STRIP,40,4);
            glDrawArrays(GL_TRIANGLE_STRIP,44,4);

	glBindVertexArray(0);
	   
	//End Draw LLeg

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
