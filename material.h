
namespace Material{
    float color[][3]={
        {1,0,0}, // Ruby
        {0,0,1}, // Sapphire
        {1,1,0}, // Gold
        {1,0.5,1}, // iPhone pink
        {0.5,0,0.25}, // 형용할 수 없는 
        {0,0.75,0.75}, // Turquoise
        {0.5,0,1}, // MnM chocolate purple
        {1,1,1}, // Cool-White
        {1,1,0}, // Gold
        {0.1,0.1,0.1}, // Obsidian
        {1,0.5,0}, // Copper
        {0,0.5,0}, // Emerald
        {0,1,0}, // Jade
        {1.0f, 1.0f, 0.5f} // boiled egg yolk
    };
    void Ruby(){
        float mat_ambient[] = { 0.1745f, 0.01175f, 0.01175f, 1.0f };
        float mat_diffuse[] = { 0.61424f, 0.04136f, 0.04136f, 1.0f };
        float mat_specular[] = { 0.727811f, 0.626959f, 0.626959f, 1.0f };
        float shine = 76.8f;
        glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
        glMaterialf(GL_FRONT, GL_SHININESS, shine);
    }
    void Sapphire(){
        float mat_ambient[] = { 0.01745f, 0.01175f, 0.1175f, 1.0f };
        float mat_diffuse[] = { 0.061424f, 0.04136f, 0.4136f, 1.0f };
        float mat_specular[] = { 0.727811f, 0.626959f, 0.626959f, 1.0f };
        float shine = 76.8f;
        glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
        glMaterialf(GL_FRONT, GL_SHININESS, shine);
    }
    void Gold(){
        GLfloat amb[]={0.24725f,0.1995f,0.0745f,1.0f};
        GLfloat diff[]={0.75164f,0.60648f,0.22648f,1.0f};
        GLfloat spec[]={0.628281f,0.555802f,0.366065f,1.0f};
        GLfloat shine=51.2f;
        glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,amb);
        glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,diff);
        glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,spec);
        glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shine);
    }
    void iPhonePink(){
        GLfloat amb[]={0.32f,0.2f,0.22f,1.0f};
        GLfloat diff[]={0.81f,0.48f,0.56f,1.0f};
        GLfloat spec[]={.3f,.3f,.3f,1.0f};
        GLfloat shine=10.0f;
        glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,amb);
        glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,diff);
        glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,spec);
        glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shine);
    }
    void IDontKnow(){
        GLfloat amb[]={0.025f,0.0f,0.0125f,1.0f};
        GLfloat diff[]={0.5f,0.0f,0.2f,1.0f};
        GLfloat spec[]={0.7f,0.04f,0.35f,1.0f};
        GLfloat shine=10.0f;
        glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,amb);
        glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,diff);
        glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,spec);
        glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shine);
    }
    void MnM(){
        GLfloat amb[]={0.12f,0.1f,0.22f,1.0f};
        GLfloat diff[]={0.61f,0.38f,0.86f,1.0f};
        GLfloat spec[]={1.0f,1.0f,1.0f,1.0f};
        GLfloat shine=128.0f;
        glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,amb);
        glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,diff);
        glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,spec);
        glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shine);
    }
    void Turquoise(){
        GLfloat amb[]={0.1f,0.18725f,0.1745f,1.0f};
        GLfloat diff[]={0.396f,0.74151f,0.69102f,1.0f};
        GLfloat spec[]={0.297254f,0.30829f,0.306678f,1.0f};
        GLfloat shine=12.8f;
        glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,amb);
        glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,diff);
        glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,spec);
        glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shine);
    }
    void CoolWhite(){
        GLfloat amb[]={0.2f,0.2f,0.3f,1.0f};
        GLfloat diff[]={0.8f,0.9f,1.0f,1.0f};
        GLfloat spec[]={0.2f,0.2f,0.4f,1.0f};
        GLfloat shine=44.8f;
        glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,amb);
        glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,diff);
        glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,spec);
        glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shine);
    }
    // Gold
    void Obsidian(){
        GLfloat amb[]={0.05375f,0.05f,0.06625f,1.0f};
        GLfloat diff[]={0.18275f,0.17f,0.22525f,1.0f};
        GLfloat spec[]={0.332741f,0.328634f,0.346435f,1.0f};
        GLfloat shine=38.4f;
        glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,amb);
        glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,diff);
        glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,spec);
        glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shine);
    }
    void Copper(){
        GLfloat amb[]={0.19125f,0.0735f,0.0225f,1.0f};
        GLfloat diff[]={0.7038f,0.27048f,0.0828f,1.0f};
        GLfloat spec[]={0.256777f,0.137622f,0.086014f,1.0f};
        GLfloat shine=12.8f;
        glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,amb);
        glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,diff);
        glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,spec);
        glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shine);
    }
    void Emerald(){
        GLfloat amb[]={0.0215f,0.1745f,0.0215f,1.0f};
        GLfloat diff[]={0.07568f,0.61424f,0.07568f,1.0f};
        GLfloat spec[]={0.633f,0.727811f,0.633f,1.0f};
        GLfloat shine=76.8f;
        glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,amb);
        glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,diff);
        glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,spec);
        glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shine);
    }
    void Jade(){
        GLfloat amb[]={0.135f,0.2225f,0.1575f,1.0f};
        GLfloat diff[]={0.54f,0.89f,0.63f,1.0f};
        GLfloat spec[]={0.316228f,0.316228f,0.316228f,1.0f};
        GLfloat shine=12.8f;
        glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,amb);
        glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,diff);
        glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,spec);
        glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shine);
    }
    void Yolk(){
        GLfloat amb[]={0.36f,0.36f,0.19f,1.0f};
        GLfloat diff[]={0.94f,0.94f,0.55f,1.0f};
        GLfloat spec[]={0.01f,0.01f,0.0f,1.0f};
        GLfloat shine = 22.0f;
        glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,amb);
        glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,diff);
        glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,spec);
        glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shine);
    }
    void (*func[13])() = {
        Ruby,
        Sapphire,
        Gold,
        iPhonePink,
        IDontKnow,
        MnM,
        Turquoise,
        CoolWhite,
        Gold,
        Obsidian,
        Copper,
        Emerald,
        Jade
    };
    void TranslucentRed(){
        GLfloat amb[]={0.5,0,0,0.1f};
        GLfloat diff[]={1,0,0,.33f};
        GLfloat spec[]={0,0,0,1.0f};
        GLfloat shine = 0;
        glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,amb);
        glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,diff);
        glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,spec);
        glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shine);
    }
    void TranslucentWhite(){
        GLfloat amb[]={0.1,.1,.1,0.1f};
        GLfloat diff[]={1,1,1,.33f};
        GLfloat spec[]={0,0,0,1.0f};
        GLfloat shine = 0;
        glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,amb);
        glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,diff);
        glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,spec);
        glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shine);
    }
    void TranslucentYellow(){
        GLfloat amb[]={0.2,0.2,0,0.1f};
        GLfloat diff[]={1,1,0,.8f};
        GLfloat spec[]={0,0,0,1.0f};
        GLfloat shine = 0;
        glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,amb);
        glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,diff);
        glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,spec);
        glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shine);
    }
}
