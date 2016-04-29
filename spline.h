#pragma once

template <typename T>
class Spline{
    public:
        enum class Kind {CatmullRom, BSpline};
    private:
        std::vector<T> control_points;
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
        
        void AddPoint(T point){
            control_points.push_back(point);
        }
        void PopPoint(){
            if (control_points.size() > 0)
                control_points.pop_back();
        }
        bool IsDrawable(){
            return control_points.size() >= 4;
        }
        std::vector<T> GeneratePoints(){
            std::vector<T> points;
            if (!IsDrawable()){
                return points;
            }
            
            int size = control_points.size();
            for (int i = 0; i < size - (!is_closed ? 3 : 0); i++){
                T &p0 = control_points[(i) % size];
                T &p1 = control_points[(i + 1) % size];
                T &p2 = control_points[(i + 2) % size];
                T &p3 = control_points[(i + 3) % size];
                
                Segment<T> *segment;
                // FIXME use shared_ptr
                switch (kind){
                    case Kind::CatmullRom:
                        segment = new CatmullRomSegment<T>(p0, p1, p2, p3);
                        break;
                    case Kind::BSpline:
                        segment = new BSplineSegment<T>(p0, p1, p2, p3);
                        break;
                    default:
                        segment = new CubicBezierSegment<T>(p0, p1, p2, p3);
                        break;
                }
                // FIXME use factory pattern
                
                std::vector<T> points_generated = segment->GeneratePoints();
                points.insert(points.end(), points_generated.begin(), points_generated.end());
                
                delete segment;
            }
        }
};
