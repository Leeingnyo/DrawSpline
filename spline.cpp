
#include <vector>
#include <cmath>
#define GLFW_INCLUDE_GLU
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"

#include "segment.h"
#include "spline.h"

#define SetColor() glColor3f(1, 1, 1)

void Spline::Draw(){
    std::vector<glm::vec3> &&points = GeneratePoints();
    
    SetColor();
    glBegin(GL_LINE_STRIP);
    for (glm::vec3 point : points){
        glVertex3f(point.x, point.y, point.z);
    }
    glEnd();
}

std::vector<glm::vec3> Spline::GeneratePoints(){
    std::vector<glm::vec3> points;
    if (!IsDrawable()){
        return points;
    }
    
    int size = control_points.size();
    for (int i = 0; i < size - (!is_closed ? 3 : 0); i++){
        glm::vec3 &p0 = control_points[(i) % size];
        glm::vec3 &p1 = control_points[(i + 1) % size];
        glm::vec3 &p2 = control_points[(i + 2) % size];
        glm::vec3 &p3 = control_points[(i + 3) % size];
        
        Segment *segment;
        // FIXME use shared_ptr
        switch (kind){
            case Kind::CatmullRom:
                segment = new CatmullRomSegment(p0, p1, p2, p3);
                break;
            case Kind::BSpline:
                segment = new BSplineSegment(p0, p1, p2, p3);
                break;
            default:
                segment = new CubicBezierSegment(p0, p1, p2, p3);
                break;
        }
        // FIXME use factory pattern
        
        std::vector<glm::vec3> points_generated = segment->GeneratePoints();
        points.insert(points.end(), points_generated.begin(), points_generated.end());
        
        delete segment;
    }
}
