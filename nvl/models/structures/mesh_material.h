#ifndef NVL_MODELS_MESH_MATERIAL_H
#define NVL_MODELS_MESH_MATERIAL_H

#include <nvl/nuvolib.h>

#include <string>

namespace nvl {

template<class C>
class MeshMaterial
{

public:

    /* Typedefs */

    typedef Index Id;

    typedef C Color;

    //1. Color on and Ambient on
    //2. Highlight on
    //3. Reflection on and Ray trace on
    //4. Transparency: Glass on, Reflection: Ray trace on
    //5. Reflection: Fresnel on and Ray trace on
    //6. Transparency: Refraction on, Reflection: Fresnel off and Ray trace on
    //7. Transparency: Refraction on, Reflection: Fresnel on and Ray trace on
    //8. Reflection on and Ray trace off
    //9. Transparency: Glass on, Reflection: Ray trace off
    //10. Casts shadows onto invisible surfaces
    enum IlluminationModel {
        COLOR_ON_AND_AMBIENT_ON = 1,
        HIGHLIGHT_ON = 2 };

    explicit MeshMaterial();

    Id& id();
    const Id& id() const;
    void setId(const Id& id);

    std::string& name();
    const std::string& name() const;
    void setName(const std::string& name);

    void setColor(const Color& color);

    void setAmbientColor(const Color& color);
    Color& ambientColor();
    const Color& ambientColor() const;

    void setDiffuseColor(const Color& color);
    Color& diffuseColor();
    const Color& diffuseColor() const;

    void setSpecularColor(const Color& color);
    Color& specularColor();
    const Color& specularColor() const;

    void setSpecularMap(const std::string& specularMap);
    const std::string& specularMap() const;
    std::string& specularMap();

    void setAmbientMap(const std::string& ambientMap);
    const std::string& ambientMap() const;
    std::string& ambientMap();

    void setDiffuseMap(const std::string& diffuseMap);
    const std::string& diffuseMap() const;
    std::string& diffuseMap();

    void setIlluminationModel(IlluminationModel illuminationModel);
    IlluminationModel& illuminationModel();
    const IlluminationModel& illuminationModel() const;

    void setTransparency(float transparency);
    float& transparency();
    const float& transparency() const;

private:

    std::string vName;

    Color vAmbientColor;
    Color vDiffuseColor;
    Color vSpecularColor;

    float vTransparency;

    std::string vAmbientMap;
    std::string vSpecularMap;
    std::string vDiffuseMap;
    IlluminationModel vIlluminationModel;

    Id vId;

};

}

#include "mesh_material.cpp"

#endif // NVL_MODELS_MESH_MATERIAL_H
