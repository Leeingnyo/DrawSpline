#pragma once

#include "mymath.h"

class Segment {
    protected:
        static int t;
    public:
        virtual std::vector<glm::vec3> GeneratePoints()=0;
};

class CubicBezierSegment : public Segment {
    private:
        glm::vec3 p[4];
    public:
        static float basis(int l, float t){
            return mymath::Combinations(3, l) * std::pow(1 - t, 3 - l) * std::pow(t, l);
        }
        
        CubicBezierSegment(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3){
            p[0] = p0;
            p[1] = p1;
            p[2] = p2;
            p[3] = p3;
        }
        virtual std::vector<glm::vec3> GeneratePoints();
};

class CatmullRomSegment : public Segment {
    private:
        glm::vec3 p[4];
    public:
        CatmullRomSegment(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3){
            p[0] = p0;
            p[1] = p1;
            p[2] = p2;
            p[3] = p3;
        }
        virtual std::vector<glm::vec3> GeneratePoints();
};

class BSplineSegment : public Segment {
    private:
        glm::vec3 p[4];
    public:
        static float basis(int l, float t){
            switch (l){
                case 0:
                    return (std::pow(1-t, 3)) / 6.0f;
                case 1:
                    return (3*t*t*t - 6*t*t + 4) / 6.0f;
                case 2:
                    return (-3*t*t*t + 3*t*t + 3*t + 1) / 6.0f;
                case 3:
                    return (std::pow(t, 3)) / 6.0f;
                default:
                    return 0;
            }
        }
        BSplineSegment(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3){
            p[0] = p0;
            p[1] = p1;
            p[2] = p2;
            p[3] = p3;
        }
        virtual std::vector<glm::vec3> GeneratePoints();
};
