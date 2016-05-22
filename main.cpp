#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#define GLFW_INCLUDE_GLU
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/quaternion.hpp"

#include "segment.h"
#include "spline.h"
#include "quaternion_spline.h"

#include "section.h"
#include "parser.h"

#define ZZZZ float(1)
#define Clamp(val, min, max) ((val) > (max) ? (max) : ((val) < (min) ? (min) : (val)))

int G;

bool bone;
int width, height; 

glm::vec3 eye(0, 0, 100);
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
    
    /*
    // Show all
    if (key == GLFW_KEY_ENTER && (action == GLFW_PRESS)){
        eye -= ori;
        ori = glm::vec3(0, 0, 0);
        
        glm::vec3 n_eye = glm::normalize(eye);
        float l_eye = glm::length(eye);
        eye = n_eye * float(2 / (tan(fov / 2 * 3.14 / 180) * 2));
    }
    */
    // Toggle
    if (key == GLFW_KEY_ENTER && (action == GLFW_PRESS)){
        bone = !bone;
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
        eye += float(yoffset) * v * 1.0f;
    } else {
        fov -= yoffset * 0.5;
        fov = Clamp(fov, 5, 90);
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

int main(int argc, char *argv[])
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
//    glEnable(GL_CULL_FACE);
//    glFrontFace(GL_CCW);
    glDepthFunc(GL_LESS);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable( GL_BLEND );
    glClearColor(0.0,0.0,0.0,0.0);
    
    /* Light */
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    
    /* Init Data */
    Parser parser;
    SurfaceBuilder &&builder = parser.Parse(argv[1]);
    // builder.Print();
    Surface surface = builder.Build();
    //surface.ExtractSTL();
    
    GLfloat ambientLight[] = {.2, .2, .2, 1};
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
    GLfloat diffuseLight[] = {.8, .8, .8, 1};
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
    GLfloat position[] = {0.0, 10.0, 0.0, 0.0};
    glLightfv(GL_LIGHT0, GL_POSITION, position);
    
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
    	    gluPerspective(fov, ratio, 0.1, 500.0f);
        }
        
        /* Render */
        { // display
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            
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
            
            glShadeModel(GL_SMOOTH);
            
            surface.Draw();
            
            /* Swap front and back buffers */
            glfwSwapBuffers(window);
        }    

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

bool debug = true;

void Surface::Draw(){
    glColor3f(1, 1, 0);
    int length = areas.size();
    for (int l = 0; l < length - 1; l++){
        std::vector<glm::vec3> &former = areas[l];
        std::vector<glm::vec3> &latter = areas[l + 1];
        std::vector<glm::vec3> &normal_former = normals[l];
        std::vector<glm::vec3> &normal_latter = normals[l + 1];
        int size = former.size();
        glBegin(GL_TRIANGLES);
        for (int i = 0; i < size; i++){
            glNormal3f(normal_former[(i) % size].x, normal_former[(i) % size].y, normal_former[(i) % size].z);
            glVertex3f(former[(i) % size].x, former[(i) % size].y, former[(i) % size].z);
            glNormal3f(normal_latter[(i) % size].x, normal_latter[(i) % size].y, normal_latter[(i) % size].z);
            glVertex3f(latter[(i) % size].x, latter[(i) % size].y, latter[(i) % size].z);
            glNormal3f(normal_former[(i + 1) % size].x, normal_former[(i + 1) % size].y, normal_former[(i + 1) % size].z);
            glVertex3f(former[(i + 1) % size].x, former[(i + 1) % size].y, former[(i + 1) % size].z);
            
            glNormal3f(normal_latter[(i) % size].x, normal_latter[(i) % size].y, normal_latter[(i) % size].z);
            glVertex3f(latter[(i) % size].x, latter[(i) % size].y, latter[(i) % size].z);
            glNormal3f(normal_latter[(i + 1) % size].x, normal_latter[(i + 1) % size].y, normal_latter[(i + 1) % size].z);
            glVertex3f(latter[(i + 1) % size].x, latter[(i + 1) % size].y, latter[(i + 1) % size].z);
            glNormal3f(normal_former[(i + 1) % size].x, normal_former[(i + 1) % size].y, normal_former[(i + 1) % size].z);
            glVertex3f(former[(i + 1) % size].x, former[(i + 1) % size].y, former[(i + 1) % size].z);
            /*
            */
        }
        glEnd();
    }
}

void ExtractFacet(std::ofstream &ofst, std::vector<glm::vec3> &&former, std::vector<glm::vec3> &&latter){
    int size = former.size();
    for (int i = 0; i < size; i++){
        glm::vec3 normal;
        normal = glm::normalize(glm::cross(former[(i + 1) % size] * 100.f - latter[(i) % size] * 100.f, former[(i) % size] * 100.f - latter[(i) % size] * 100.f));
        if (std::isnan(normal.x))
            normal = glm::vec3(0, 0, 1);
        ofst << "  facet normal " << normal.x << " " << normal.y << " " << normal.z << std::endl;
        ofst << "    outer loop" << std::endl;
        ofst << "      vertex " << former[(i) % size].z << " " << former[(i) % size].x << " " << former[(i) % size].y << std::endl;
        ofst << "      vertex " << latter[(i) % size].z << " " << latter[(i) % size].x << " " << latter[(i) % size].y << std::endl;
        ofst << "      vertex " << former[(i + 1) % size].z << " " << former[(i + 1) % size].x << " " << former[(i + 1) % size].y << std::endl;
        ofst << "    endloop" << std::endl;
        ofst << "  endfacet" << std::endl;
        
        ofst << "  facet normal " << normal.x << " " << normal.y << " " << normal.z << std::endl;
        ofst << "    outer loop" << std::endl;
        ofst << "      vertex " << latter[(i) % size].x << " " << latter[(i) % size].y << " " << latter[(i) % size].z << std::endl;
        ofst << "      vertex " << latter[(i + 1) % size].x << " " << latter[(i + 1) % size].y << " " << latter[(i + 1) % size].z << std::endl;
        ofst << "      vertex " << former[(i + 1) % size].x << " " << former[(i + 1) % size].y << " " << former[(i + 1) % size].z << std::endl;
        ofst << "    endloop" << std::endl;
        ofst << "  endfacet" << std::endl;
    }
}

void Surface::ExtractSTL(){
    std::ofstream stl_file("model.stl");
    stl_file << "solid model" << std::scientific << std::endl;
    /*
    int size = sections.size();
    for (int i = 0; i < size - 1; i++){
        Section &section_former = sections[i];
        Section &section_latter = sections[i + 1];
        Spline<glm::vec3> spline_for_draw_former(spline_type == SurfaceType::CatmullRom ? Spline<glm::vec3>::Kind::CatmullRom : Spline<glm::vec3>::Kind::BSpline, true);
        for (glm::vec3 point : section_former.control_points){
            glm::vec3 p = point;
            p = p * section_former.scaling_factor;
            p = section_former.rotation * p;
            p = p + section_former.position;
            spline_for_draw_former.AddPoint(p);
        }
        Spline<glm::vec3> spline_for_draw_latter(spline_type == SurfaceType::CatmullRom ? Spline<glm::vec3>::Kind::CatmullRom : Spline<glm::vec3>::Kind::BSpline, true);
        for (glm::vec3 point : section_latter.control_points){
            glm::vec3 p = point;
            p = p * section_latter.scaling_factor;
            p = section_latter.rotation * p;
            p = p + section_latter.position;
            spline_for_draw_latter.AddPoint(p);
        }
        ExtractFacet(stl_file, spline_for_draw_former.GeneratePoints(), spline_for_draw_latter.GeneratePoints());
    }
    */
    stl_file << "endsolid model" << std::endl;
    stl_file.close();
}

