#pragma once

class Cube {
    private:
        std::vector<glm::vec3> points;
        glm::vec3 position;
        float scale;
        glm::quat rotation;
    public:
        Cube(glm::vec3 position, float scale, glm::quat rotation){
            this->position = position;
            this->scale = scale;
            this->rotation = rotation;
            
            points.push_back(glm::vec3(1, 1, 1));
            points.push_back(glm::vec3(1, -1, 1));
            points.push_back(glm::vec3(-1, -1, 1));
            points.push_back(glm::vec3(-1, 1, 1));
            
            points.push_back(glm::vec3(1, 1, -1));
            points.push_back(glm::vec3(1, -1, -1));
            points.push_back(glm::vec3(-1, -1, -1));
            points.push_back(glm::vec3(-1, 1, -1));
        }
        void Draw();
        std::vector<std::vector<glm::vec3>> GetArea(){
            std::vector<std::vector<glm::vec3>> areas;
            std::vector<glm::vec3> points_;
            
            for (int i = 0; i < points.size(); i++)
                points_.push_back(rotation * (scale * points[i]) + position);
            
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
            return areas;
        }
};

