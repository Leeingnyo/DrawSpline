
class Sphere {
    private:
        glm::vec3 position;
        float scale;
    public:
        Sphere(glm::vec3 position, float scale){
            this->position = position;
            this->scale = scale;
        }
        void Draw();
};
