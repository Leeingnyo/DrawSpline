
namespace Quaternion {
    extern int t;
    
    inline std::vector<glm::quat> DeCasteljau(glm::quat q0, glm::quat q1, glm::quat q2, glm::quat q3){
        
        std::vector<glm::quat> quaternion_spline;
        
        for (int i = 0; i < t; i++) {
            glm::quat f00 = glm::mix(q0, q1, (float)i / (float)t);
            glm::quat f01 = glm::mix(q1, q2, (float)i / (float)t);
            glm::quat f02 = glm::mix(q2, q3, (float)i / (float)t);
            glm::quat f10 = glm::mix(f00, f01, (float)i / (float)t);
            glm::quat f11 = glm::mix(f01, f02, (float)i / (float)t);
            glm::quat f20 = glm::mix(f10, f11, (float)i / (float)t);
            
            quaternion_spline.push_back(f20);
        }
        
        return quaternion_spline;
    }
};

class QuaternionSegment {
    private:
        glm::quat q[4];
    public:
        QuaternionSegment(glm::quat q0, glm::quat q1, glm::quat q2, glm::quat q3){
            q[0] = q0;
            q[1] = q1;
            q[2] = q2;
            q[3] = q3;
        }
        std::vector<glm::quat> GeneratePoints(){
            glm::quat control1 = q[1] * glm::exp(1.0f/6.0f * glm::log(glm::inverse(q[0]) * q[2]));
            glm::quat control2 = q[2] * glm::inverse(glm::exp(1.0f/6.0f * glm::log(glm::inverse(q[1]) * q[3])));
            return Quaternion::DeCasteljau(q[1], control1, control2, q[2]);
        }
};

class QuaternionSpline {
    public:
    private:
        std::vector<glm::quat> control_quaternions;
        bool is_closed;
    public:
        QuaternionSpline(bool is_closed){
            this->is_closed = is_closed;
        }
        
        void AddPoint(glm::quat quaternion){
            control_quaternions.push_back(quaternion);
        }
        void PopPoint(){
            if (control_quaternions.size() > 0)
                control_quaternions.pop_back();
        }
        
        bool IsDrawable(){
            return control_quaternions.size() >= 4;
        }
        std::vector<glm::quat> GeneratePoints(){
            std::vector<glm::quat> points;
            
            if (!IsDrawable()){
                return points;
            }
            
            int size = control_quaternions.size();
            for (int i = 0; i < size - (!is_closed ? 3 : 0); i++){
                glm::quat &p0 = control_quaternions[(i) % size];
                glm::quat &p1 = control_quaternions[(i + 1) % size];
                glm::quat &p2 = control_quaternions[(i + 2) % size];
                glm::quat &p3 = control_quaternions[(i + 3) % size];
                
                QuaternionSegment segment(p0, p1, p2, p3);
                std::vector<glm::quat> quaternions_generated = segment.GeneratePoints();
                points.insert(points.end(), quaternions_generated.begin(), quaternions_generated.end());
            }
            return points;
        }
};
