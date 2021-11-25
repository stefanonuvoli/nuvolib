#include "mesh_material.h"

namespace nvl {

template<class C>
MeshMaterial<C>::MeshMaterial() :
    vName(""),
    vDiffuseColor(Color(1.0, 1.0, 1.0)),
    vAmbientColor(Color(1.0, 1.0, 1.0)),
    vSpecularColor(Color(1.0, 1.0, 1.0)),
    vTransparency(0.0),
    vDiffuseMap(""),
    vAmbientMap(""),
    vSpecularMap(""),
    vTransparencyMap(""),
    vNormalMap(""),
    vShadingModel(SHADING_STANDARD),
    vIlluminationModel(ILLUMINATION_HIGHLIGHT_ON)
{

}

template<class C>
typename MeshMaterial<C>::Id& MeshMaterial<C>::id()
{
    return vId;
}

template<class C>
const typename MeshMaterial<C>::Id& MeshMaterial<C>::id() const
{
    return vId;
}

template<class C>
void MeshMaterial<C>::setId(const Id& id)
{
    vId = id;
}

template<class C>
typename std::string& MeshMaterial<C>::name()
{
    return vName;
}

template<class C>
const std::string& MeshMaterial<C>::name() const
{
    return vName;
}

template<class C>
void MeshMaterial<C>::setName(const std::string& name)
{
    vName = name;
}

template<class C>
void MeshMaterial<C>::setColor(const Color& color)
{
    setAmbientColor(color);
    setDiffuseColor(color);
    setSpecularColor(color);
}

template<class C>
void MeshMaterial<C>::setDiffuseColor(const Color& color)
{
    vDiffuseColor = color;
}

template<class C>
typename MeshMaterial<C>::Color& MeshMaterial<C>::diffuseColor()
{
    return vDiffuseColor;
}

template<class C>
const typename MeshMaterial<C>::Color& MeshMaterial<C>::diffuseColor() const
{
    return vDiffuseColor;
}

template<class C>
void MeshMaterial<C>::setAmbientColor(const Color& color)
{
    vAmbientColor = color;
}

template<class C>
typename MeshMaterial<C>::Color& MeshMaterial<C>::ambientColor()
{
    return vAmbientColor;
}

template<class C>
const typename MeshMaterial<C>::Color& MeshMaterial<C>::ambientColor() const
{
    return vAmbientColor;
}

template<class C>
void MeshMaterial<C>::setSpecularColor(const Color& color)
{
    vSpecularColor = color;
}

template<class C>
typename MeshMaterial<C>::Color& MeshMaterial<C>::specularColor()
{
    return vSpecularColor;
}

template<class C>
const typename MeshMaterial<C>::Color& MeshMaterial<C>::specularColor() const
{
    return vSpecularColor;
}

template<class C>
void MeshMaterial<C>::setDiffuseMap(const std::string& diffuseMap)
{
    vDiffuseMap = diffuseMap;
}

template<class C>
const std::string& MeshMaterial<C>::diffuseMap() const
{
    return vDiffuseMap;
}

template<class C>
std::string& MeshMaterial<C>::diffuseMap()
{
    return vDiffuseMap;
}

template<class C>
void MeshMaterial<C>::setAmbientMap(const std::string& ambientMap)
{
    vAmbientMap = ambientMap;
}

template<class C>
const std::string& MeshMaterial<C>::ambientMap() const
{
    return vAmbientMap;
}

template<class C>
std::string& MeshMaterial<C>::ambientMap()
{
    return vAmbientMap;
}

template<class C>
void MeshMaterial<C>::setSpecularMap(const std::string& specularMap)
{
    vSpecularMap = specularMap;
}

template<class C>
const std::string& MeshMaterial<C>::specularMap() const
{
    return vSpecularMap;
}

template<class C>
std::string& MeshMaterial<C>::specularMap()
{
    return vSpecularMap;
}

template<class C>
void MeshMaterial<C>::setTransparencyMap(const std::string& transparencyMap)
{
    vTransparencyMap = transparencyMap;
}

template<class C>
const std::string& MeshMaterial<C>::transparencyMap() const
{
    return vTransparencyMap;
}

template<class C>
std::string& MeshMaterial<C>::transparencyMap()
{
    return vTransparencyMap;
}

template<class C>
void MeshMaterial<C>::setNormalMap(const std::string& normalMap)
{
    vNormalMap = normalMap;
}

template<class C>
const std::string& MeshMaterial<C>::normalMap() const
{
    return vNormalMap;
}

template<class C>
std::string& MeshMaterial<C>::normalMap()
{
    return vNormalMap;
}

template<class C>
void MeshMaterial<C>::setShadingModel(ShadingModel shadingModel)
{
    vShadingModel = shadingModel;
}

template<class C>
typename MeshMaterial<C>::ShadingModel& MeshMaterial<C>::shadingModel()
{
    return vShadingModel;
}

template<class C>
const typename MeshMaterial<C>::ShadingModel& MeshMaterial<C>::shadingModel() const
{
    return vShadingModel;
}

template<class C>
void MeshMaterial<C>::setIlluminationModel(IlluminationModel illuminationModel)
{
    vIlluminationModel = illuminationModel;
}

template<class C>
typename MeshMaterial<C>::IlluminationModel& MeshMaterial<C>::illuminationModel()
{
    return vIlluminationModel;
}

template<class C>
const typename MeshMaterial<C>::IlluminationModel& MeshMaterial<C>::illuminationModel() const
{
    return vIlluminationModel;
}

template<class C>
void MeshMaterial<C>::setTransparency(float transparency)
{
    vTransparency = transparency;
}

template<class C>
float& MeshMaterial<C>::transparency()
{
    return vTransparency;
}

template<class C>
const float& MeshMaterial<C>::transparency() const
{
    return vTransparency;
}

}
