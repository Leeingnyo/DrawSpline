
class BSPNode {
    friend class BSPTree;
    private:
        BSPNode *front = NULL;
        BSPNode *back = NULL;
        std::vector<glm::vec3> polygon; // standard, triangle 
        void (*material_func)() = NULL;
    public:
        BSPNode(const std::vector<glm::vec3> &polygon, void (*material_func)()){
            this->polygon = polygon;
            this->material_func = material_func;
        }
        void Draw(const glm::vec3 &camera) const;
        void Delete(){
            if (front){
                front->Delete();
                delete front;
            }
            if (back){
                back->Delete();
                delete back;
            }
        }
        void Print(const glm::vec3 &camera) const {
            const BSPNode *first;
            const BSPNode *later;
            
            glm::vec3 N = glm::cross(polygon[1] - polygon[0], polygon[2] - polygon[1]);
            float d = glm::dot(N, polygon[0]);
            
            if (glm::dot(N, camera) - d <= 0){
                // ī�޶� �ڿ� ������ 
                first = this->front; 
                later = this->back;
            }
            else{
                first = this->back;
                later = this->front;
            }
            
            if (first){
                std::cout << "����" << std::endl;
                first->Print(camera);
            }
            std::cout << "Me" << std::endl;
            std::cout << "0: (" << polygon[0].x << " " << polygon[0].y << " " << polygon[0].z << ") ";
            std::cout << "1: (" << polygon[1].x << " " << polygon[1].y << " " << polygon[1].z << ") ";
            std::cout << "2: (" << polygon[2].x << " " << polygon[2].y << " " << polygon[2].z << ") " << std::endl;
            if (later){
                std::cout << "����" << std::endl;
                later->Print(camera);
            }
        }
};

class BSPTree {
    private:
        BSPNode *root = NULL;
        enum { back, front, same };
        void insert(BSPNode *&node, const std::vector<glm::vec3> &inserted, void (*material_func)()){
            if (inserted.size() < 3) return;
            {
                glm::vec3 N = glm::cross(inserted[1] - inserted[0], inserted[2] - inserted[1]);
                if (N == glm::vec3(0,0,0)) {
                    return;
                }
            }
            if (node == NULL){
                node = new BSPNode(inserted, material_func);
            } else {
                // �� ��尡 �̹� �������� ������ �ؾ��Ѵ� 
                // �ϴ� �� ���� �� �������� �߸��� �� �����Ѵ� 
                
                    // ����� ������ ���ϱ� 
                // �켱 �� ����� ����� �������� ���Ѵ� 
                // N * (x, y, z)  = d 
                // d = N * node->polygon[0];
                glm::vec3 N = glm::cross(node->polygon[1] - node->polygon[0], node->polygon[2] - node->polygon[1]);
                float d = glm::dot(N, node->polygon[0]);
                
                    // �� ������ 
                std::vector<glm::vec3> sequence[3];
                int prev_state;
                glm::vec3 prev_point;
                int size = inserted.size();
                {
                    const glm::vec3 &target = inserted[size - 1];
                    float dn = glm::dot(N, target);
                    int state = ((dn - d) == 0 ? same : ((dn - d) > 0));
                    prev_state = state;
                    prev_point = target;
                }
                for (int i = 0; i < size; i++){
                    const glm::vec3 &target = inserted[i];
                    float dn = glm::dot(N, target);
                    int state = ((dn - d) == 0 ? same : ((dn - d) > 0));
                    // �� �� ���� ��� �ִ��� Ȯ���Ѵ� 
                    if (prev_state != state){
                    // prev �� �����̶� ���� �ٸ��� 
                        // �߶�� �Ѵ� 
                        // prev�� ���� ���� ���� ���� ��鿡 �����غ��� 
                        float dn = glm::dot(N, target);
                        float dp = glm::dot(N, prev_point);
                        // dn = N * pn
                        // dp = N * pp
                        // ���� ���� ������ ������ ���Ѵ� 
                        float rn = abs(dn - d);
                        float rp = abs(dp - d);
                        // rn = |dn - d|
                        // rp = |dp - d|
                        // �� ������ �� ���� ������ ���� ���Ѵ� 
                        glm::vec3 pm = target * (rp / (rn + rp)) + prev_point * (rn / (rn + rp));
                        // pm = pn * (rp / (rn + rp)) + pp * (rn / (rn + rp))
                        sequence[prev_state].push_back(pm);
                        // �������� ���� ���¿� �ִ´� 
                        sequence[state].push_back(pm);
                        // �������� ���� ���¿� �ִ´�
                    }
                    sequence[state].push_back(target);
                    prev_state = state;
                    prev_point = target;
                    // �� ���� �ִ´� 
                        // 0 �̻��̸� // front �� 
                        // �̸��̸� // back �� �־�� �� // ���� �ִ´� 
                }
                
                    // �� �ֱ� 
                // ������ ���� �ո� �� ������, �޸� �� �������� ���ؼ� ���� 
                for (int s = 0; s < 3; s++){
                    const std::vector<glm::vec3> &seq = sequence[s];
                    BSPNode *&target_node = (s ? node->front : node->back);
                    int n = seq.size();
                    if (n < 3) continue;
                    // 2�� ���ϸ� ���� 
                    for (int i = 1; i < n - 1; i++){
                        std::vector<glm::vec3> polygon;
                        polygon.push_back(seq[0]);
                        polygon.push_back(seq[i]);
                        polygon.push_back(seq[i + 1]);
                        insert(target_node, polygon, material_func);
                        // vector<��> �� 
                        // 0 �ְ� 
                        // i �ְ� 
                        // i + 1 �ְ� 
                        // ����<��> �ش� ��忡 �μ�Ʈ 
                    }
                }
            }
        }
    public:
        BSPTree(){
        }
        ~BSPTree(){
            if (root){
                root->Delete();
                delete root;
            }
        }
        void insert(const std::vector<glm::vec3> &polygon, void (*material_func)()){
            
            insert(root, polygon, material_func);
        }
        void Draw(const glm::vec3 &camera){
            root->Draw(camera);
        }
        void Print(const glm::vec3 &camera){
            if (root)
            root->Print(camera);
            else std::cout << "Empty." << std::endl;
        }
};

