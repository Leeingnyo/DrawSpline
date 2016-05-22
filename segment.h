#pragma once

#include <iostream>
#include "mymath.h"

template <typename T>
class Segment {
    protected:
        static int t;
    public:
        virtual std::vector<T> GeneratePoints()=0;
};

template <typename T>
class CubicBezierSegment : public Segment<T> {
    private:
        T p[4];
    public:
        static float basis(int l, float t){
            return mymath::Combinations(3, l) * std::pow(1 - t, 3 - l) * std::pow(t, l);
        }
        CubicBezierSegment(T p0, T p1, T p2, T p3){
            p[0] = p0;
            p[1] = p1;
            p[2] = p2;
            p[3] = p3;
        }
        virtual std::vector<T> GeneratePoints(){
            std::vector<T> draw_points;
            for (int i = 0; i < Segment<T>::t; i++){
                T point{};
                for (int pi = 0; pi < 4; pi++){
                    point += p[pi] * basis(pi, ((float)i / (float)Segment<T>::t));
                }
                draw_points.push_back(point);
            }
            return draw_points;
        }
};

template <typename T>
class CatmullRomSegment : public Segment<T> {
    private:
        T p[4];
    public:
        CatmullRomSegment(T p0, T p1, T p2, T p3){
            p[0] = p0;
            p[1] = p1;
            p[2] = p2;
            p[3] = p3;
        }
        virtual std::vector<T> GeneratePoints(){
            T control1 = (p[2] - p[0]) * (1.0f / 6.0f) + p[1];
            T control2 = (p[1] - p[3]) * (1.0f / 6.0f) + p[2];
            CubicBezierSegment<T> s = CubicBezierSegment<T>(p[1], control1, control2, p[2]);
            std::vector<T> points = s.GeneratePoints();
            return points;
        };
};

template <typename T>
class BSplineSegment : public Segment<T> {
    private:
        T p[4];
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
        BSplineSegment(T p0, T p1, T p2, T p3){
            p[0] = p0;
            p[1] = p1;
            p[2] = p2;
            p[3] = p3;
        }
        virtual std::vector<T> GeneratePoints(){
            std::vector<T> draw_points;
            for (int i = 0; i < Segment<T>::t; i++){
                T point{};
                for (int pi = 0; pi < 4; pi++){
                    point += p[pi] * basis(pi, ((float)i / (float)Segment<T>::t));
                }
                draw_points.push_back(point);
            }
            return draw_points;
        }
};

template <typename T>
int Segment<T>::t = 8;

