
#include <vector>
#include <iostream>
#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/quaternion.hpp"

#include "segment.h"
#include "spline.h"
#include "quaternion_spline.h"

#include "section.h"

void SurfaceBuilder::Print(){
    std::cout << (spline_type == SurfaceType::BSpline ? "BSPLINE" : "CATMULL_ROM") << std::endl;
    std::cout << cross_section_num << std::endl;
    std::cout << control_point_num << std::endl;

    for (int i = 0; i < cross_section_num; i++){
        std::cout << "# Cross Section " << i << std::endl;
        for (int j = 0; j < control_point_num; j++){
            std::cout << cross_sections[i].control_points[j].x << " "
                    << cross_sections[i].control_points[j].y << " "
                    << cross_sections[i].control_points[j].z << std::endl;
        }
        std::cout << cross_sections[i].scaling_factor << std::endl;
        std::cout << cross_sections[i].rotation.w << " ";
        std::cout << cross_sections[i].rotation.x << " "
                << cross_sections[i].rotation.y << " "
                << cross_sections[i].rotation.z << std::endl;
        std::cout << cross_sections[i].position.x << " "
                << cross_sections[i].position.y << " "
                << cross_sections[i].position.z << std::endl;
    }
}

Surface SurfaceBuilder::Build(){
    Surface surface(spline_type);
    // 실제 그려질 스웹트 서페이스의 정보를 담은 객체를 만든다 
    
    int size = cross_sections.size();
    for (int i = 0; i < size - 3; i++){
        std::vector<Section> sections;
        
        Section &section0 = cross_sections[(i) % size];
        Section &section1 = cross_sections[(i + 1) % size];
        Section &section2 = cross_sections[(i + 2) % size];
        Section &section3 = cross_sections[(i + 3) % size];
        // 섹션 4개를 취한다 
        
        // 4개에 대해서 각각 캣멀롬 스플라인을 
        Spline<glm::vec3> position_spline(Spline<glm::vec3>::Kind::CatmullRom, false);
        position_spline.AddPoint(section0.position);
        position_spline.AddPoint(section1.position);
        position_spline.AddPoint(section2.position);
        position_spline.AddPoint(section3.position);
        std::vector<glm::vec3> &&position_points = position_spline.GeneratePoints();
        // Position
        Spline<float> scaling_factor_spline(Spline<float>::Kind::CatmullRom, false);
        scaling_factor_spline.AddPoint(section0.scaling_factor);
        scaling_factor_spline.AddPoint(section1.scaling_factor);
        scaling_factor_spline.AddPoint(section2.scaling_factor);
        scaling_factor_spline.AddPoint(section3.scaling_factor);
        std::vector<float> &&scaling_factor_points = scaling_factor_spline.GeneratePoints();
        // Scaling Factor
        QuaternionSpline quaternion_spline(false);
        quaternion_spline.AddPoint(section0.rotation);
        quaternion_spline.AddPoint(section1.rotation);
        quaternion_spline.AddPoint(section2.rotation);
        quaternion_spline.AddPoint(section3.rotation);
        std::vector<glm::quat> &&quaternions = quaternion_spline.GeneratePoints();
        // Quaternion
        std::vector<glm::vec3> control_point_splines[control_point_num];
        for (int i = 0; i < control_point_num; i++){
            Spline<glm::vec3> control_point_spline(Spline<glm::vec3>::Kind::CatmullRom, false);
            control_point_spline.AddPoint(section0.control_points[i]);
            control_point_spline.AddPoint(section1.control_points[i]);
            control_point_spline.AddPoint(section2.control_points[i]);
            control_point_spline.AddPoint(section3.control_points[i]);
            control_point_splines[i] = control_point_spline.GeneratePoints();
        }
        // Control Points
    
        int size = position_points.size();
        for (int i = 0; i < size; i++){
            Section section;
            section.position = position_points[i];
            section.scaling_factor = scaling_factor_points[i];
            section.rotation = quaternions[i];
            for (int j = 0; j < control_point_num; j++){
                section.AddPoint(control_point_splines[j][i]);
            }
            sections.push_back(section);
        }
        // 벡터 섹션을 만들고 
        // 스플라인 한 개수 만큼 
        // 섹션에 Positoin, Scaling Factor, Quaternion, 컨트롤 포인터 넣기 
        surface.AddSection(sections);
    }
    return surface;
}
