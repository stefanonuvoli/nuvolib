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

    enum IlluminationModel {
        ILLUMINATION_STANDARD = 0,
        ILLUMINATION_COLOR_ON_AND_AMBIENT_ON = 1,
        ILLUMINATION_HIGHLIGHT_ON = 2,
        ILLUMINATION_REFLECTION_ON_AND_RAY_TRACE_ON = 3,
        ILLUMINATION_GLASS_ON_AND_RAY_TRACE_ON = 4,
        ILLUMINATION_FRESNEL_ON_AND_RAY_TRACE_ON = 5,
        ILLUMINATION_REFRACTION_ON_AND_FRESNEL_OFF_AND_RAY_TRACE_ON = 6,
        ILLUMINATION_REFRACTION_ON_AND_FRESNEL_ON_AND_RAY_TRACE_ON = 7,
        ILLUMINATION_REFLECTION_ON_AND_RAY_TRACE_OFF = 8,
        ILLUMINATION_GLASS_ON_AND_RAY_TRACE_OFF = 9,
        ILLUMINATION_CAST_SHADOW_ONTO_INVISIBLE = 10};

    enum ShadingModel {
        SHADING_STANDARD = 0,
        SHADING_LAMBERT = 1,
        SHADING_PHONG = 2, };

    explicit MeshMaterial();

    Id& id();
    const Id& id() const;
    void setId(const Id& id);

    std::string& name();
    const std::string& name() const;
    void setName(const std::string& name);

    void setColor(const Color& color);

    void setDiffuseColor(const Color& color);
    Color& diffuseColor();
    const Color& diffuseColor() const;

    void setAmbientColor(const Color& color);
    Color& ambientColor();
    const Color& ambientColor() const;

    void setSpecularColor(const Color& color);
    Color& specularColor();
    const Color& specularColor() const;

    void setDiffuseMap(const std::string& diffuseMap);
    const std::string& diffuseMap() const;
    std::string& diffuseMap();

    void setAmbientMap(const std::string& ambientMap);
    const std::string& ambientMap() const;
    std::string& ambientMap();

    void setSpecularMap(const std::string& specularMap);
    const std::string& specularMap() const;
    std::string& specularMap();

    void setTransparencyMap(const std::string& transparencyMap);
    const std::string& transparencyMap() const;
    std::string& transparencyMap();

    void setNormalMap(const std::string& normalMap);
    const std::string& normalMap() const;
    std::string& normalMap();

    void setShadingModel(ShadingModel shadingModel);
    ShadingModel& shadingModel();
    const ShadingModel& shadingModel() const;

    void setIlluminationModel(IlluminationModel illuminationModel);
    IlluminationModel& illuminationModel();
    const IlluminationModel& illuminationModel() const;

    void setTransparency(float transparency);
    float& transparency();
    const float& transparency() const;

private:

    std::string vName;

    Color vDiffuseColor;
    Color vAmbientColor;
    Color vSpecularColor;

    float vTransparency;

    std::string vDiffuseMap;
    std::string vAmbientMap;
    std::string vSpecularMap;
    std::string vTransparencyMap;
    std::string vNormalMap;

    ShadingModel vShadingModel;
    IlluminationModel vIlluminationModel;

    Id vId;

};

}

#include "mesh_material.cpp"

#endif // NVL_MODELS_MESH_MATERIAL_H
