#include <iostream>
#include <fstream>
#include <algorithm>
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

#include "material.h"
#include "cube.h"
#include "sphere.h" 

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
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glDepthFunc(GL_LESS);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glClearColor(0.0,0.0,0.0,0.0);
    
    /* Light */
    //glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    {
        GLfloat ambientLight[] = {.1, .1, .1, 1};
        glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
        GLfloat diffuseLight[] = {0.4, 0.4, 0.4, 1};
        glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
        GLfloat position[] = {0.0, 0.0, 100.0, 0.0};
        glLightfv(GL_LIGHT0, GL_POSITION, position);
    }
    
    glEnable(GL_LIGHT1);
    {
        GLfloat ambientLight[] = {.05, .05, .05, 1};
        glLightfv(GL_LIGHT1, GL_AMBIENT, ambientLight);
        GLfloat diffuseLight[] = {0.2, 0.2, 0.2, 1};
        glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuseLight);
        GLfloat position[] = {0.0, 100.0, 0.0, 0.0};
        glLightfv(GL_LIGHT1, GL_POSITION, position);
    }
    
    glEnable(GL_LIGHT2);
    {
        GLfloat ambientLight[] = {.05, .05, .05, 1};
        glLightfv(GL_LIGHT2, GL_AMBIENT, ambientLight);
        GLfloat diffuseLight[] = {0.2, 0.2, 0.2, 1};
        glLightfv(GL_LIGHT2, GL_DIFFUSE, diffuseLight);
        GLfloat position[] = {100.0, 0.0, 0.0, 0.0};
        glLightfv(GL_LIGHT2, GL_POSITION, position);
    }
    
    glEnable(GL_LIGHT3);
    {
        GLfloat ambientLight[] = {.05, .05, .05, 1};
        glLightfv(GL_LIGHT3, GL_AMBIENT, ambientLight);
        GLfloat diffuseLight[] = {0.2, 0.2, 0.2, 1};
        glLightfv(GL_LIGHT3, GL_DIFFUSE, diffuseLight);
        GLfloat position[] = {100.0, 100.0, 1000.0, 0.0};
        glLightfv(GL_LIGHT3, GL_POSITION, position);
    }

    /* Init Data */
    Parser parser;
    SurfaceBuilder &&builder = parser.Parse(argv[1]);
    // builder.Print();
    Surface surface = builder.Build();
    //surface.ExtractSTL();
    
    Cube cube(glm::vec3(2.5f, 0, 0), 8.0f, glm::quat());
    
    std::vector<Sphere> spheres;
    for (int i = 0; i < 13; i++){
        spheres.push_back(Sphere(
                glm::vec3(
                    cos((360.0 / 13.0) * i * 3.141592 / 180) * 30, 0, 
                    sin((360.0 / 13.0) * i * 3.141592 / 180) * 30
                ), 
                2.0f
            )
        );
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
            Material::Yolk();
            glColor3fv(Material::color[13]);
            surface.Draw();
            // draw opaque object
            
            glPushMatrix();
            glRotatef(glfwGetTime() * 30.f, 0, 1, 0);
            for (int i = 0; i < 13; i++){
                glColor3fv(Material::color[i]);
                Material::func[i]();
                glPushMatrix();
                spheres[i].Draw();
                glPopMatrix();
            }
            glPopMatrix();
            // draw material objects
            
            glEnable(GL_COLOR_MATERIAL);
            glColor4f(1, 0, 0, 0.5f);
            glCullFace(GL_FRONT);
            cube.Draw();
            glCullFace(GL_BACK);
            cube.Draw();
            glDisable(GL_COLOR_MATERIAL);
            // draw translucent cube
            
            /* Swap front and back buffers */
            glfwSwapBuffers(window);
        }    

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void Cube::Draw(){
    glm::vec3 view = eye - ori;
    std::vector<std::vector<glm::vec3>> areas;
    std::vector<glm::vec3> points_;
    
    for (int i = 0; i < points.size(); i++)
        points_.push_back(rotation * (scale * points[i]));
    
    areas.push_back({points_[2], points_[1], points_[0]});
    areas.push_back({points_[0], points_[3], points_[2]});
    
    areas.push_back({points_[4], points_[5], points_[6]});
    areas.push_back({points_[6], points_[7], points_[4]});
    
    areas.push_back({points_[0], points_[1], points_[5]});
    areas.push_back({points_[5], points_[4], points_[0]});
    
    areas.push_back({points_[1], points_[2], points_[6]});
    areas.push_back({points_[6], points_[5], points_[1]});
    
    areas.push_back({points_[2], points_[3], points_[7]});
    areas.push_back({points_[7], points_[6], points_[2]});
    
    areas.push_back({points_[3], points_[0], points_[4]});
    areas.push_back({points_[4], points_[7], points_[3]});
    // prepare areas
    
    std::sort(areas.begin(), areas.end(), [view](std::vector<glm::vec3> a, std::vector<glm::vec3> b){
        glm::vec3 normal_a = glm::normalize(glm::cross(a[1] - a[0], a[2] - a[1]));
        glm::vec3 normal_b = glm::normalize(glm::cross(b[1] - b[0], b[2] - b[1]));
        return glm::dot(normal_a, view) < glm::dot(normal_b, view);
    });
    // sort
    
    glPushMatrix();
    glTranslatef(position.x, position.y, position.z);
    glBegin(GL_TRIANGLES);
    for (int i = 0; i < areas.size(); i++){
        std::vector<glm::vec3> &area = areas[i];
        glm::vec3 normal = glm::normalize(glm::cross(area[1] - area[0], area[2] - area[1]));
        glNormal3f(normal.x, normal.y, normal.z);
        glVertex3f(area[0].x, area[0].y, area[0].z);
        glVertex3f(area[1].x, area[1].y, area[1].z);
        glVertex3f(area[2].x, area[2].y, area[2].z);
    }
    glEnd();
    glPopMatrix();
    // draw
}

void Sphere::Draw(){
    std::vector<std::vector<glm::vec3>> areas;
    
    int n = 10;
    glm::vec3 p[n + 1][n + 1];
    for (int i = 0; i <= n; i++){
        for (int j = 0; j < n; j++){
            p[i][j].y = scale * cos(180.0 / n * i * 3.141592 / 180.0);
            p[i][j].x = scale * sin(180.0 / n * i * 3.141592 / 180.0) * cos(360.0 / n * j * 3.141592 / 180.0);
            p[i][j].z = scale * sin(180.0 / n * i * 3.141592 / 180.0) * sin(360.0 / n * j * 3.141592 / 180.0);
        }
    }
    
    glPushMatrix();
    glTranslatef(position.x, position.y, position.z);
    glBegin(GL_QUADS);
    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){
            std::vector<glm::vec3> area;
            area.push_back(p[i][j]);
            area.push_back(p[i + 1][j]);
            area.push_back(p[i + 1][(j + 1) % n]);
            area.push_back(p[i][(j + 1) % n]);
            {
                glm::vec3 normal = glm::normalize(glm::cross(area[1] - area[0], area[3] - area[2]));
                if (glm::dot(normal, area[0]) > 0)
                    normal = -normal;
                glNormal3f(normal.x, normal.y, normal.z);
            }
            //glNormal3f(area[0].x, area[0].y, area[0].z);
            glVertex3f(area[0].x, area[0].y, area[0].z);
            //glNormal3f(area[1].x, area[1].y, area[1].z);
            glVertex3f(area[1].x, area[1].y, area[1].z);
            //glNormal3f(area[2].x, area[2].y, area[2].z);
            glVertex3f(area[2].x, area[2].y, area[2].z);
            //glNormal3f(area[3].x, area[3].y, area[3].z);
            glVertex3f(area[3].x, area[3].y, area[3].z);
        }
    }
    glEnd();
    glPopMatrix();
    // draw
}

void Surface::Draw(){
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

