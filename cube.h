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
};

