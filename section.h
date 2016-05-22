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
        std::vector<std::vector<glm::vec3>> areas;
        std::vector<std::vector<glm::vec3>> normals;
    public:
        Surface(SurfaceType type){
            this->spline_type = type;
        }
        void AddArea(std::vector<glm::vec3> &&area){
            areas.push_back(area);
        }
        void AddArea(Section &section){
            Spline<glm::vec3> spline_for_draw(spline_type == SurfaceType::CatmullRom ? Spline<glm::vec3>::Kind::CatmullRom : Spline<glm::vec3>::Kind::BSpline, true);
            for (glm::vec3 point : section.control_points){
                glm::vec3 p = point;
                p = p * section.scaling_factor;
                p = section.rotation * p;
                p = p + section.position;
                spline_for_draw.AddPoint(p);
            }
            AddArea(spline_for_draw.GeneratePoints());
        }
        void AddArea(std::vector<Section> &sections){
            for (Section &section : sections)
                AddArea(section);
        }
        void CalculateNormals(){
            int length = areas.size();
            if (length < 2) return;
            for (int l = 0; l < length; l++){
                int size = areas[l].size();
                std::vector<glm::vec3> normal;
                for (int i = 0; i < size; i++){
                    glm::vec3 N;
                    std::vector<glm::vec3> P;
                    glm::vec3 p;
                    
                    p = areas[l][i];
                    if (l == 0){
                        P.push_back(areas[l][(i + 1) % size]);
                        P.push_back(areas[l + 1][i]);
                        P.push_back(areas[l + 1][(i - 1 + size) % size]);
                        P.push_back(areas[l][(i - 1 + size) % size]);
                        
                        N += glm::normalize(glm::cross((P[1] - p), (P[0] - p)));
                        N += glm::normalize(glm::cross((P[2] - p), (P[1] - p)));
                        N += glm::normalize(glm::cross((P[3] - p), (P[2] - p)));
                    }
                    else if (l == length - 1){
                        P.push_back(areas[l][(i - 1 + size) % size]);
                        P.push_back(areas[l - 1][i]);
                        P.push_back(areas[l - 1][(i + 1) % size]);
                        P.push_back(areas[l][(i + 1) % size]);
                        
                        N += glm::normalize(glm::cross((P[1] - p), (P[0] - p)));
                        N += glm::normalize(glm::cross((P[2] - p), (P[1] - p)));
                        N += glm::normalize(glm::cross((P[3] - p), (P[2] - p)));
                    }
                    else{
                        P.push_back(areas[l + 1][i]);
                        P.push_back(areas[l + 1][(i - 1 + size) % size]);
                        P.push_back(areas[l][(i - 1 + size) % size]);
                        P.push_back(areas[l - 1][i]);
                        P.push_back(areas[l - 1][(i + 1) % size]);
                        P.push_back(areas[l][(i + 1) % size]);
                        
                        N += glm::normalize(glm::cross((P[1] - p), (P[0] - p)));
                        N += glm::normalize(glm::cross((P[2] - p), (P[1] - p)));
                        N += glm::normalize(glm::cross((P[3] - p), (P[2] - p)));
                        N += glm::normalize(glm::cross((P[4] - p), (P[3] - p)));
                        N += glm::normalize(glm::cross((P[5] - p), (P[4] - p)));
                        N += glm::normalize(glm::cross((P[0] - p), (P[5] - p)));
                    }
                    N = glm::normalize(N);
                    normal.push_back(N);
                }
                normals.push_back(normal);
            }
        }
        void Draw();
        void ExtractSTL();
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
