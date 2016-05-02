
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>

class Parser {
    private:
        std::vector<std::string> Tokenize(std::ifstream &ifs) const {
            std::vector<std::string> tokens;
            std::string line, without_comment, word;
            while(getline(ifs, line)) {
                std::stringstream iss(line);
                getline(iss, without_comment, '#');
                std::stringstream iss_(without_comment);
                while(getline(iss_, word, ' ')) {
                    if (word == "") continue;
                    if (word[0] == '#') break;
                    tokens.push_back(word);
                }
            }
            return tokens;
        }
    public:
        SurfaceBuilder Parse(std::string filename){
            std::ifstream ifs(filename.c_str());
            
            if(ifs.is_open()) {
                std::vector<std::string> &&tokens = Tokenize(ifs);
                
                int index = 0;
                bool is_b_spline = (tokens[index++] == "BSPLINE");
                int section_num = atoi(tokens[index++].c_str());
                int point_num = atoi(tokens[index++].c_str());
                SurfaceBuilder builder(is_b_spline ? SurfaceType::BSpline : SurfaceType::CatmullRom, section_num, point_num);
                for (int section_index = 0; section_index < section_num; section_index++){
                    Section section;
                    
                    for (int point_index = 0; point_index < point_num; point_index++){
                        float x = atof(tokens[index++].c_str());
                        float z = atof(tokens[index++].c_str());
                        section.AddPoint(glm::vec3(x, 0, z));
                    }
                    float scailing_factor = atof(tokens[index++].c_str());
                    section.SetScailingFactor(scailing_factor);
                    float rotation_angle = atof(tokens[index++].c_str());
                    {
                        float x = atof(tokens[index++].c_str());
                        float y = atof(tokens[index++].c_str());
                        float z = atof(tokens[index++].c_str());
                        section.SetRotation(glm::angleAxis(rotation_angle, glm::vec3(x, y, z)));
                    }
                    {
                        float x = atof(tokens[index++].c_str());
                        float y = atof(tokens[index++].c_str());
                        float z = atof(tokens[index++].c_str());
                        section.SetPosition(glm::vec3(x, y, z));
                    }
                    
                    builder.AddCrossSection(section);
                }
                
                ifs.close();
                
                return builder;
            } else{
                std::cout << "파일이 없습니다" << std::endl;
                exit(1);
            }
        }
};
