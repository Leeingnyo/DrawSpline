#include <iostream>
#include <vector>
#include <cmath>

#include "glm/glm.hpp"

#include "BSP.h"

int main(int argc, char** argv) {
    BSPTree bsp_tree;
    std::vector<glm::vec3> d;
    {
        d.push_back(glm::vec3(6, 0, 0));
        d.push_back(glm::vec3(0, 0, 0));
        d.push_back(glm::vec3(0, 0, 4));
    }
    std::vector<glm::vec3> b;
    {
        b.push_back(glm::vec3(3, 2, 0));
        b.push_back(glm::vec3(3, -2, 0));
        b.push_back(glm::vec3(3, 0, 4));
    }
    std::vector<glm::vec3> a;
    {
        a.push_back(glm::vec3(4, 1, 0));
        a.push_back(glm::vec3(4, -1, 0));
        a.push_back(glm::vec3(4, 0, 2));
    }
    std::vector<glm::vec3> c;
    {
        c.push_back(glm::vec3(1, 1, 0));
        c.push_back(glm::vec3(1, -1, 0));
        c.push_back(glm::vec3(1, 1, 2));
    }
    bsp_tree.insert(b, 0);
    bsp_tree.insert(d, 0);
    bsp_tree.insert(a, 0);
    bsp_tree.insert(c, 0);
    
    bsp_tree.Print(glm::vec3(0, 4, 0));
	return 0;
}
