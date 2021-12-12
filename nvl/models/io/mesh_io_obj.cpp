#include "mesh_io_obj.h"

#include <nvl/utilities/string_utils.h>
#include <nvl/utilities/file_utils.h>
#include <nvl/utilities/locale_utils.h>

#include <fstream>
#include <map>
#include <unordered_map>

namespace nvl {

template<class Material>
void meshSaveObjMaterial(std::ofstream& fMtl, const Material& material, const std::string& path);

template<class P, class VN, class UV, class VC, class PC, class FN, class M>
bool meshLoadDataFromOBJ(
        const std::string& filename,
        IOMeshData<P,VN,UV,VC,PC,FN,M>& data,
        IOMeshError& error)
{
    data.clear();

    std::ifstream fObj, fMtl; //File streams

    fObj.imbue(streamDefaultLocale());
    fMtl.imbue(streamDefaultLocale());

    std::string line, mtlLine;

    error = IO_MESH_SUCCESS;

    //Open file
    fObj.open(filename);
    if(!fObj.is_open()) { //Error opening file
        error = IO_MESH_FILE_ERROR;
        return false;
    }

    //File info descriptor
    std::string path = filenamePath(filename);

    //Material data
    std::unordered_map<std::string, Index> materialMap;
    Index currentMaterialId;

    bool materialLoaded = false;

    while (std::getline(fObj, line)) {
        if (line.empty())
            continue;

        std::vector<std::string> lineSplitted = stringSplit(line, ' ');

        const std::string& token = lineSplitted[0];

        //Handle material library
        if (token == "mtllib") {
            if (lineSplitted.size() < 2) {
                error = IO_MESH_FORMAT_NON_RECOGNISED;
                continue;
            }

            //Get mtl file
            std::string mtlFile = "";
            mtlFile = stringTrim(stringUnion(lineSplitted, 1));
            mtlFile = filenameAbsolutePath(mtlFile, path);

            //Open mtl file
            fMtl.open(mtlFile);

            if(fMtl.is_open()) {
                materialLoaded = true;

                Index loadingMaterialId = NULL_ID;

                while (std::getline(fMtl, mtlLine)) {
                    std::vector<std::string> mtlSplitted = stringSplit(mtlLine, ' ');

                    const std::string& mtlToken = mtlSplitted[0];

                    if (mtlToken == "newmtl") {
                        if (mtlSplitted.size() < 2) {
                            error = IO_MESH_FORMAT_NON_RECOGNISED;
                            continue;
                        }

                        M material;

                        std::string materialName = stringTrim(stringUnion(mtlSplitted, 1));
                        material.setName(materialName);

                        loadingMaterialId = data.materials.size();
                        data.materials.push_back(material);

                        materialMap.insert(std::make_pair(material.name(), loadingMaterialId));
                    }
                    else if (mtlToken == "Kd") {
                        if (mtlSplitted.size() < 4) {
                            error = IO_MESH_FORMAT_NON_RECOGNISED;
                            continue;
                        }

                        typename M::Color color(std::stof(mtlSplitted[1]), std::stof(mtlSplitted[2]), std::stof(mtlSplitted[3]));
                        data.materials[loadingMaterialId].setDiffuseColor(color);
                    }
                    else if (mtlToken == "Ka") {
                        if (mtlSplitted.size() < 4) {
                            error = IO_MESH_FORMAT_NON_RECOGNISED;
                            continue;
                        }

                        typename M::Color color(std::stof(mtlSplitted[1]), std::stof(mtlSplitted[2]), std::stof(mtlSplitted[3]));
                        data.materials[loadingMaterialId].setAmbientColor(color);
                    }
                    else if (mtlToken == "Ks") {
                        if (mtlSplitted.size() < 4) {
                            error = IO_MESH_FORMAT_NON_RECOGNISED;
                            continue;
                        }

                        typename M::Color color(std::stof(mtlSplitted[1]), std::stof(mtlSplitted[2]), std::stof(mtlSplitted[3]));
                        data.materials[loadingMaterialId].setSpecularColor(color);
                    }
                    else if (mtlToken == "illum") {
                        if (mtlSplitted.size() < 2) {
                            error = IO_MESH_FORMAT_NON_RECOGNISED;
                            continue;
                        }

                        typename M::IlluminationModel illum = static_cast<typename M::IlluminationModel>(std::stoi(mtlSplitted[1]));
                        data.materials[loadingMaterialId].setIlluminationModel(illum);

                    }
                    else if (mtlToken == "d") {
                        if (mtlSplitted.size() < 2) {
                            error = IO_MESH_FORMAT_NON_RECOGNISED;
                            continue;
                        }

                        float transparency = std::stof(mtlSplitted[1]);
                        data.materials[loadingMaterialId].setTransparency(1 - transparency);
                    }
                    else if (mtlToken == "Tr") {
                        if (mtlSplitted.size() < 2) {
                            error = IO_MESH_FORMAT_NON_RECOGNISED;
                            continue;
                        }

                        float transparency = std::stof(mtlSplitted[1]);
                        data.materials[loadingMaterialId].setTransparency(transparency);
                    }
                    else if (mtlToken == "map_Kd") {
                        if (mtlSplitted.size() < 2) {
                            error = IO_MESH_FORMAT_NON_RECOGNISED;
                            continue;
                        }                        

                        std::string mapFile = stringTrim(stringUnion(mtlSplitted, 1));
                        mapFile = filenameAbsolutePath(mapFile, path);

                        data.materials[loadingMaterialId].setDiffuseMap(mapFile);
                    }
                    else if (mtlToken == "map_Ka") {
                        if (mtlSplitted.size() < 2) {
                            error = IO_MESH_FORMAT_NON_RECOGNISED;
                            continue;
                        }

                        std::string mapFile = stringTrim(stringUnion(mtlSplitted, 1));
                        mapFile = filenameAbsolutePath(mapFile, path);

                        data.materials[loadingMaterialId].setAmbientMap(mapFile);
                    }
                    else if (mtlToken == "map_Ks") {
                        if (mtlSplitted.size() < 2) {
                            error = IO_MESH_FORMAT_NON_RECOGNISED;
                            continue;
                        }

                        std::string mapFile = stringTrim(stringUnion(mtlSplitted, 1));
                        mapFile = filenameAbsolutePath(mapFile, path);

                        data.materials[loadingMaterialId].setSpecularMap(mapFile);
                    }
                    else if (mtlToken == "map_d") {
                        if (mtlSplitted.size() < 2) {
                            error = IO_MESH_FORMAT_NON_RECOGNISED;
                            continue;
                        }

                        std::string mapFile = stringTrim(stringUnion(mtlSplitted, 1));
                        mapFile = filenameAbsolutePath(mapFile, path);

                        data.materials[loadingMaterialId].setTransparencyMap(mapFile);
                    }
                    else if (mtlToken == "map_Bump" || mtlToken == "bump") {
                        if (mtlSplitted.size() < 2) {
                            error = IO_MESH_FORMAT_NON_RECOGNISED;
                            continue;
                        }

                        std::string mapFile = stringTrim(stringUnion(mtlSplitted, 1));
                        mapFile = filenameAbsolutePath(mapFile, path);

                        data.materials[loadingMaterialId].setNormalMap(mapFile);
                    }
                }
            }
        }

        //Handle materials
        else if (token == "usemtl") {
            if (lineSplitted.size() < 2) {
                error = IO_MESH_FORMAT_NON_RECOGNISED;
                continue;
            }

            std::string materialName = stringTrim(stringUnion(lineSplitted, 1));

            currentMaterialId = materialMap.at(materialName);
        }

        //Handle vertex UV coords
        else if (token == "vt") {
            if (lineSplitted.size() < 3) {
                error = IO_MESH_FORMAT_NON_RECOGNISED;
                continue;
            }

            data.vertexUVs.push_back(UV(std::stof(lineSplitted[1]), std::stof(lineSplitted[2])));
        }

        //Handle vertex normals
        else if (token == "vn") {
            if (lineSplitted.size() != 4) {
                error = IO_MESH_FORMAT_NON_RECOGNISED;
                continue;
            }

            data.vertexNormals.push_back(VN(std::stof(lineSplitted[1]), std::stof(lineSplitted[2]), std::stof(lineSplitted[3])));
        }

        //Handle vertices
        else if (token == "v") {
            if (lineSplitted.size() < 4) {
                error = IO_MESH_FORMAT_NON_RECOGNISED;
                continue;
            }

            data.vertices.push_back(P(std::stof(lineSplitted[1]), std::stof(lineSplitted[2]), std::stof(lineSplitted[3])));

            if (lineSplitted.size() == 7) {
                data.vertexColors.push_back(VC(std::stof(lineSplitted[4]), std::stof(lineSplitted[5]), std::stof(lineSplitted[6])));
            }
            else {
                data.vertexColors.push_back(Color(0.7, 0.7, 0.7));
            }
        }

        //Handle faces
        else if (token == "f") {
            if (lineSplitted.size() < 4) {
                error = IO_MESH_FORMAT_NON_RECOGNISED;
                continue;
            }

            std::vector<Index> vertexIds;

            //Handling vertices
            for (Index i = 1; i < lineSplitted.size(); ++i) {
                std::vector<std::string> faceSplitted = stringSplit(lineSplitted[i], '/');

                if (faceSplitted[0].empty()) {
                    error = IO_MESH_FORMAT_NON_RECOGNISED;
                    continue;
                }

                Index vertexId = std::stoul(faceSplitted[0]) - 1;

                vertexIds.push_back(vertexId);
            }

            data.faces.push_back(vertexIds);

            //Handle UVs
            std::vector<Index> faceVertexUVs;

            for (Index i = 1; i < lineSplitted.size(); ++i) {
                std::vector<std::string> faceSplitted = stringSplit(lineSplitted[i], '/');

                if (faceSplitted.size() >= 2) {
                    if (!faceSplitted[1].empty()) {
                        Index uvId = std::stoul(faceSplitted[1]) - 1;
                        faceVertexUVs.push_back(uvId);
                    }
                }
            }

            data.faceVertexUVs.push_back(faceVertexUVs);

            //Handle vertex normals
            std::vector<Index> faceVertexNormals;

            for (Index i = 1; i < lineSplitted.size(); ++i) {
                std::vector<std::string> faceSplitted = stringSplit(lineSplitted[i], '/');

                if (faceSplitted.size() == 3) {
                    //Handling vertex normals
                    if (!faceSplitted[2].empty()) {
                        Index normalId = std::stoul(faceSplitted[2]) - 1;
                        faceVertexNormals.push_back(normalId);
                    }
                }
            }

            data.faceVertexNormals.push_back(faceVertexNormals);

            if (materialLoaded) {
                //Handling face materials
                data.faceMaterials.push_back(currentMaterialId);
            }
        }

        //Handle polylines
        else if (token == "l") {
            if (lineSplitted.size() < 3) {
                error = IO_MESH_FORMAT_NON_RECOGNISED;
                continue;
            }

            std::vector<Index> vertexIds;

            for (Index i = 1; i < lineSplitted.size(); ++i) {
                vertexIds.push_back(std::stoul(lineSplitted[i]) - 1);
            }

            data.polylines.push_back(vertexIds);
        }
    }

    //Successfully loaded
    return true;
}

template<class P, class VN, class UV, class VC, class PC, class FN, class M>
bool meshSaveDataToOBJ(
        const std::string& filename,
        const IOMeshData<P,VN,UV,VC,PC,FN,M> &data,
        IOMeshError& error)
{
    std::ofstream fObj, fMtl; //File streams

    fObj.imbue(streamDefaultLocale());
    fMtl.imbue(streamDefaultLocale());

    error = IO_MESH_SUCCESS;

    //Open file
    fObj.open(filename);
    if(!fObj.is_open()) { //Error opening file
        error = IO_MESH_FILE_ERROR;
        return false;
    }

    //Set precision
    fObj.precision(6);
    fObj.setf(std::ios::fixed, std::ios::floatfield);

    //File info descriptor
    std::string path = filenamePath(filename);
    std::string name = filenameName(filename);

    if (!data.materials.empty()) {
        std::string filenameMtl = path + name + ".mtl";

        //Open mtl file
        fMtl.open(filenameMtl);
        if(!fMtl.is_open()) {
            error = IO_MESH_FILE_ERROR;
            return false;
        }

        //Set precision
        fMtl.precision(4);
        fMtl.setf(std::ios::fixed, std::ios::floatfield);
    }

    //Create default material
    M defaultMaterial;
    defaultMaterial.setName("nvl_default_material");

    //Printing header
    fObj <<
            "###############################" << std::endl <<
            "# Generated by nuvolib (OBJ)" << std::endl <<
            "# Wavefront OBJ file" << std::endl;
    if (!data.vertices.empty())
        fObj <<
            "# Vertices: " << data.vertices.size() << std::endl;
    if (!data.faces.empty())
        fObj <<
            "# Faces: " << data.faces.size() << std::endl;
    if (!data.polylines.empty())
        fObj <<
            "# Lines: " << data.polylines.size() << std::endl;
    if (!data.vertexUVs.empty())
        fObj <<
            "# UV coords: " << data.vertexUVs.size() << std::endl;
    if (!data.vertexNormals.empty())
        fObj <<
            "# Vertex normals: " << data.vertexNormals.size() << std::endl;
    fObj <<
            "###############################" << std::endl;

    if (!data.materials.empty()) {
        //Printing header on mtl file
        fMtl <<
                "###############################" << std::endl <<
                "# Generated by nuvolib (MTL)" << std::endl <<
                "# Wavefront material file" << std::endl;
        fMtl << "# Materials: " << data.materials.size() << std::endl;
        fMtl << "###############################" << std::endl;

        fObj << std::endl << "mtllib " << name + ".mtl" << std::endl;
    }

    //Vertex UV coordinates
    if (!data.vertexUVs.empty()) {
        fObj << std::endl << "#Vertex UV coordinates:" << std::endl << std::endl;

        for (Index i = 0; i < data.vertexUVs.size(); ++i) {
            const UV& uv = data.vertexUVs[i];
            fObj << "vt " << uv.x() << " " << uv.y() << std::endl;
        }
    }

    //Vertex normals
    if (!data.vertexNormals.empty()) {
        fObj << std::endl << "#Vertex normals:" << std::endl << std::endl;

        for (Index i = 0; i < data.vertexNormals.size(); ++i) {
            const VN& normal = data.vertexNormals[i];
            fObj << "vn " << normal.x() << " " << normal.y() << " " << normal.z() << std::endl;
        }
    }

    //Vertices
    if (!data.vertices.empty()) {
        fObj << std::endl << "#Vertices:" << std::endl << std::endl;

        for (Index i = 0; i < data.vertices.size(); ++i) {
            //Coordinates
            const P& vertex = data.vertices[i];
            fObj << "v " << vertex.x() << " " << vertex.y() << " " << vertex.z();

            //Vertex colors
            if (!data.vertexColors.empty()) {
                const VC& color = data.vertexColors[i];
                fObj << " " << color.redF() << " " << color.greenF() << " " << color.blueF();
            }

            fObj << std::endl;
        }
    }

    //Materials
    if (!data.materials.empty()) {
        Index matUnnamedMaterialId = 1;

        for (const M& material : data.materials) {
            M materialCopy = material;

            if (materialCopy.name().empty()) {
                materialCopy.setName("material" + std::to_string(matUnnamedMaterialId));

                ++matUnnamedMaterialId;
            }

            meshSaveObjMaterial(fMtl, materialCopy, path);
        }
    }

    //Faces
    if (!data.faces.empty()) {
        fObj << std::endl << "#Faces:" << std::endl;
        if (data.materials.empty()) {
            fObj << std::endl;
        }

        //Current active material
        Index currentMaterialId = NULL_ID;
        bool createdDefaultMaterial = false;

        //Save faces data
        for (Index fId = 0; fId < data.faces.size(); ++fId) {

            //Face Material
            if (!data.materials.empty() && !data.faceMaterials.empty()) {
                Index faceMaterialId = data.faceMaterials[fId];

                //If face has no material
                if (faceMaterialId == nvl::NULL_ID && !createdDefaultMaterial) {
                    meshSaveObjMaterial(fMtl, defaultMaterial, path);
                    createdDefaultMaterial = true;

                    fObj << std::endl;

                    fObj << "usemtl " << defaultMaterial.name() << std::endl;

                    currentMaterialId = faceMaterialId;
                }

                if (currentMaterialId != faceMaterialId) {
                    fObj << std::endl;

                    fObj << "usemtl " << data.materials[faceMaterialId].name() << std::endl;

                    currentMaterialId = faceMaterialId;
                }
            }

            //Faces
            fObj << "f";

            for (Index j = 0; j < data.faces[fId].size(); ++j) {
                const Index& vId = data.faces[fId][j];

                fObj << " " << vId + 1;

                if ((!data.faceVertexUVs.empty() && !data.faceVertexUVs[fId].empty()) ||
                    (!data.faceVertexNormals.empty() && !data.faceVertexNormals[fId].empty()))
                {
                    fObj << "/";

                    //Face UV coords
                    if (!data.faceVertexUVs.empty() && !data.faceVertexUVs[fId].empty()) {
                        fObj << data.faceVertexUVs[fId][j] + 1;
                    }

                    fObj << "/";

                    //Face vertex normal
                    if (!data.faceVertexNormals.empty() && !data.faceVertexNormals[fId].empty()) {
                        fObj << data.faceVertexNormals[fId][j] + 1;
                    }
                }
            }

            fObj << std::endl;
        }
    }

    //Polylines
    if (!data.polylines.empty()) {
        fObj << std::endl << "#Lines:" << std::endl << std::endl;

        //Save faces data
        for (const std::vector<Index>& polylineVertices : data.polylines) {
            fObj << "l";
            for (const Index& vId : polylineVertices) {
                fObj << " " << vId + 1;
            }

            fObj << std::endl;
        }
    }

    //Close material file
    if (!data.materials.empty()) {
        fMtl.close();
    }

    //Close obj file
    fObj.close();

    return true;
}

template<class Material>
void meshSaveObjMaterial(std::ofstream& fMtl, const Material& material, const std::string& path) {
    typedef typename Material::Color MaterialColor;

    //Save name
    fMtl << std::endl << "newmtl " << material.name() << std::endl;

    //Save colors
    const MaterialColor& ambientColor = material.ambientColor();
    const MaterialColor& diffuseColor = material.diffuseColor();
    const MaterialColor& specularColor = material.specularColor();

    //Colors
    fMtl << "Ka " << ambientColor.redF() << " " << ambientColor.greenF() << " " << ambientColor.blueF() << std::endl;
    fMtl << "Kd " << diffuseColor.redF() << " " << diffuseColor.greenF() << " " << diffuseColor.blueF() << std::endl;
    fMtl << "Ks " << specularColor.redF() << " " << specularColor.greenF() << " " << specularColor.blueF() << std::endl;

    //Illumination model
    fMtl << "illum " << material.illuminationModel() << std::endl;

    //Transparency
    fMtl << "d " << 1 - material.transparency() << std::endl;
    fMtl << "Tr " << material.transparency() << std::endl;

    //Texture path
    const std::string texturePathRel("textures/");
    const std::string texturePathAbs = filenameAbsolutePath(texturePathRel, path);

    //Create directory
    if (!material.diffuseMap().empty() ||
        !material.ambientMap().empty() ||
        !material.specularMap().empty() ||
        !material.normalMap().empty() ||
        !material.transparencyMap().empty())
    {
        nvl::createDirectory(texturePathAbs);
    }

    //Maps
    if (!material.diffuseMap().empty()) {
        std::string filename = material.diffuseMap();
        std::string name = filenameFile(filename);
        fMtl << "map_Kd " << texturePathRel + name << std::endl;
        fileCopy(filename, texturePathAbs + name);
    }
    if (!material.ambientMap().empty()) {
        const std::string& filename = material.ambientMap();
        std::string name = filenameFile(filename);
        fMtl << "map_Ka " << texturePathRel + name << std::endl;
        fileCopy(filename, texturePathAbs + name);
    }
    if (!material.specularMap().empty()) {
        std::string filename = material.specularMap();
        std::string name = filenameFile(filename);
        fMtl << "map_Ks " << texturePathRel + name << std::endl;
        fileCopy(filename, texturePathAbs + name);
    }
    if (!material.normalMap().empty()) {
        std::string filename = material.normalMap();
        std::string name = filenameFile(filename);
        fMtl << "map_Bump " << texturePathRel + name << std::endl;
        fileCopy(filename, texturePathAbs + name);
    }
    if (!material.transparencyMap().empty()) {
        std::string filename = material.transparencyMap();
        std::string name = filenameFile(filename);
        fMtl << "map_d " << texturePathRel + name << std::endl;
        fileCopy(filename, texturePathAbs + name);
    }
}

}
