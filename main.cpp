#include <iostream>
#include <vector>
#include <cmath>
#define GLFW_INCLUDE_GLU
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/quaternion.hpp"

#include "segment.h"
#include "spline.h"

#define ZZZZ float(1)
#define Clamp(val, min, max) ((val) > (max) ? (max) : ((val) < (min) ? (min) : (val)))
#define SetColor() glColor3f(1, 1, 1)

int width, height; 

glm::vec3 eye(0, 0, 10);
glm::vec3 ori(0, 0, 0);
glm::vec3 up(0, 1, 0);

double prev_x = 0;
double prev_y = 0;
double prev_z = 0;

float fov = 45.f;
bool is_shift = false;
bool is_mouse_left_clicked = false;

void Draw(std::vector<glm::vec3> &&points);

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    // Translate camera
    if (key == GLFW_KEY_UP && (action == GLFW_PRESS || action == GLFW_REPEAT)){
        eye += ZZZZ * up;
        ori += up * ZZZZ;
    }
    if (key == GLFW_KEY_RIGHT && (action == GLFW_PRESS || action == GLFW_REPEAT)){
        glm::vec3 v = ori - eye;
        glm::vec3 right = glm::normalize(glm::cross(v, up));
        
        eye += right * ZZZZ;
        ori += right * ZZZZ;
    }
    if (key == GLFW_KEY_DOWN && (action == GLFW_PRESS || action == GLFW_REPEAT)){
        eye -= up * ZZZZ;
        ori -= up * ZZZZ;
    }
    if (key == GLFW_KEY_LEFT && (action == GLFW_PRESS || action == GLFW_REPEAT)){
        glm::vec3 v = ori - eye;
        glm::vec3 right = glm::normalize(glm::cross(v, up));
        
        eye -= right * ZZZZ;
        ori -= right * ZZZZ;
    }
    
    // Dolly setting
    is_shift = false;
    if (key == GLFW_KEY_LEFT_SHIFT && (action == GLFW_PRESS || action == GLFW_REPEAT)){
        is_shift = true;
    }
    
    // Show all
    if (key == GLFW_KEY_ENTER && (action == GLFW_PRESS)){
        eye -= ori;
        ori = glm::vec3(0, 0, 0);
        
        glm::vec3 n_eye = glm::normalize(eye);
        float l_eye = glm::length(eye);
        eye = n_eye * float(2 / (tan(fov / 2 * 3.14 / 180) * 2));
    }
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    
    if (is_mouse_left_clicked){
        static glm::quat q(1, 0, 0, 0);
        
        double x = (xpos - width / 2) / (width / 2);
        double y = -(ypos - height / 2) / (height / 2);
        double r = sqrt(x * x + y * y);
        double z = r >= 1 ? 0 : sqrt(1 - r * r);
        
        if (!(prev_x == 0 && prev_y == 0)){
            glm::vec3 now(x, y, z);
            glm::vec3 prev(prev_x, prev_y, prev_z);
            
            now = q * now;
            prev = q * prev;
            
            glm::vec3 axis = glm::normalize(glm::cross(now, prev));
            
            float costheta = glm::dot(now, prev) / (glm::length(now) * glm::length(prev));
            float cos = sqrt((costheta + 1) / 2.0f);
            float sin = sqrt((1 - costheta) / 2.0f);
            
            if (sin < 0.002f){
                sin = 0;
                cos = 1;
            }
            
            glm::quat q2 = glm::quat(cos, sin * axis.x, sin * axis.y, sin * axis.z);
            glm::vec3 v = eye - ori;
            v = q2 * v;
            eye = v + ori;
            up = q2 * up;
            q = q2 * q;
        }
        
        prev_x = x;
        prev_y = y;
        prev_z = z;
    }
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    if (is_shift){
        glm::vec3 v = glm::normalize(ori - eye);
        eye += float(yoffset) * v * 0.1f;
    } else {
        fov -= yoffset * 0.5;
        fov = Clamp(fov, 5, 60);
    }
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS){
        is_mouse_left_clicked = true;
        prev_x = prev_y = prev_z = 0;
    }
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
        is_mouse_left_clicked = false;
}

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(960, 720, "Spline", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    
    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    
    /* Set callbacks */
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSwapInterval(1);
    
    /* Enable options */
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CW);
    glDepthFunc(GL_LESS);
    
    /* Light */
    /*
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    */
    
    /* Init Data */
    Spline<glm::vec3> spline(Spline<glm::vec3>::Kind::BSpline, true);
    std::cout << (int)spline.GetKind() << std::endl;
    std::cout << spline.IsDrawable() << std::endl;
    spline.PopPoint();
    spline.AddPoint(glm::vec3(1, 1, 0));
    spline.AddPoint(glm::vec3(-1, 1, 0));
    spline.AddPoint(glm::vec3(-1, -1, 0));
    spline.AddPoint(glm::vec3(1, -1, 0));
    std::cout << spline.IsDrawable() << std::endl;
    
    std::cout << CubicBezierSegment<int>::basis(0, 0) << std::endl;
    
    BSplineSegment<glm::vec3> s(glm::vec3(1, 1, 0),glm::vec3(-1, 1, 0),glm::vec3(-1, -1, 0),glm::vec3(1, -1, 0));
    std::vector<glm::vec3> &&points = spline.GeneratePoints();
    
    for (glm::vec3 point : points){
        //std::cout << point.x << " " << point.y << " " << point.z << std::endl;
    }
    
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        float ratio;
        glfwGetFramebufferSize(window, &width, &height);
        ratio = width / (float) height;
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        /* Projection */
        {
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
    	    gluPerspective(fov, ratio, 0.1, 50.0f);
        }
        
        /* Render */
        { // display
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            /*
            GLfloat ambientLight[] = {.8, .8, .8, 1};
            glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
            GLfloat diffuseLight[] = {.2, .2, .2, 1};
            glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
            GLfloat position[] = {1.0, 1.5, 0.0, 0.0};
            glLightfv(GL_LIGHT0, GL_POSITION, position);
            */
    	    gluLookAt(eye.x, eye.y, eye.z, ori.x, ori.y, ori.z, up.x, up.y, up.z);
            
            { // Draw Axis
                glLineWidth(2.5);
                glColor3f(1.0, 0.0, 0.0);
                glBegin(GL_LINES);
                glVertex3f(0.0, 0.0, 0.0);
                glVertex3f(0.5, 0, 0);
                glEnd();
                // x
                glColor3f(0.0, 1.0, 0.0);
                glBegin(GL_LINES);
                glVertex3f(0.0, 0.0, 0.0);
                glVertex3f(0, 0.5, 0);
                glEnd();
                // y
                glColor3f(0.0, 0.0, 1.0);
                glBegin(GL_LINES);
                glVertex3f(0.0, 0.0, 0.0);
                glVertex3f(0, 0, 0.5);
                glEnd();
                // z
                glLineWidth(1);
            }
            
            Draw(spline.GeneratePoints());
            
            /* Swap front and back buffers */
            glfwSwapBuffers(window);
        }    

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void Draw(std::vector<glm::vec3> &&points){
    SetColor();
    glBegin(GL_LINE_STRIP);
    for (glm::vec3 point : points){
        glVertex3f(point.x, point.y, point.z);
    }
    glEnd();
}

