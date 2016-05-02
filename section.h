#pragma once

class Section {
    private:
    public:
        std::vector<glm::vec3> control_points;
        float scaling_factor;
        glm::quat rotation;
        glm::vec3 position;
        
        void AddPoint(glm::vec3 point){
            control_points.push_back(point);
        }
        void SetScailingFactor(float sf){
            scaling_factor = sf;
        }
        void SetRotation(glm::quat q){
            rotation = q;
        }
        void SetPosition(glm::vec3 pos){
            position = pos;
        }
};

enum class SurfaceType { CatmullRom, BSpline };

class Surface {
    private:
        SurfaceType spline_type;
        std::vector<Section> sections;
    public:
        Surface(SurfaceType type){
            this->spline_type = type;
        }
        void AddSection(Section section){
            sections.push_back(section);
        }
        void AddSection(std::vector<Section> insert_sections){
            this->sections.insert(this->sections.end(), insert_sections.begin(), insert_sections.end());
        }
        void Draw();
};

class SurfaceBuilder {
    private:
        SurfaceType spline_type;
        int cross_section_num;
        int control_point_num;
        std::vector<Section> cross_sections;
    public:
        SurfaceBuilder(SurfaceType type, int crossnum, int controlnum){
            this->spline_type = type;
            this->cross_section_num = crossnum;
            this->control_point_num = controlnum;
        }
        void AddCrossSection(Section section){
            cross_sections.push_back(section);
        }
        Surface Build();
        
        void Print();
};
