#pragma once

class Spline{
    public:
        enum class Kind {CatmullRom, BSpline};
    private:
        std::vector<glm::vec3> control_points;
        Kind kind;
        bool is_closed;
    public:
        Spline(Kind kind, bool is_closed){
            this->kind = kind;
            this->is_closed = is_closed;
        }
        Kind GetKind(){
            return kind;
        }
        
        void AddPoint(glm::vec3 point){
            control_points.push_back(point);
        }
        void PopPoint(){
            if (control_points.size() > 0)
                control_points.pop_back();
        }
        bool IsDrawable(){
            return control_points.size() >= 4;
        }
        void Draw();
        std::vector<glm::vec3> GeneratePoints();
        
        
};
