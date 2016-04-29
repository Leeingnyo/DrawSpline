
#include <vector>
#include <cmath>
#include "glm/glm.hpp"

#include "segment.h"

int Segment::t = 20;

std::vector<glm::vec3> CubicBezierSegment::GeneratePoints(){
    std::vector<glm::vec3> draw_points;
    for (int i = 0; i <= t; i++){
        glm::vec3 point;
        for (int pi = 0; pi < 4; pi++){
            point += p[pi] * basis(pi, ((float)i / (float)t));
        }
        draw_points.push_back(point);
    }
    return draw_points;
}

std::vector<glm::vec3> CatmullRomSegment::GeneratePoints(){
    glm::vec3 control1 = (p[2] - p[0]) * (1.0f / 3.0f) + p[1];
    glm::vec3 control2 = (p[1] - p[3]) * (1.0f / 3.0f) + p[2];
    CubicBezierSegment s = CubicBezierSegment(p[1], control1, control2, p[2]);
    std::vector<glm::vec3> points = s.GeneratePoints();
    return points;
};

std::vector<glm::vec3> BSplineSegment::GeneratePoints(){
    std::vector<glm::vec3> draw_points;
    for (int i = 0; i <= t; i++){
        glm::vec3 point;
        for (int pi = 0; pi < 4; pi++){
            point += p[pi] * basis(pi, ((float)i / (float)t));
        }
        draw_points.push_back(point);
    }
    return draw_points;
}
