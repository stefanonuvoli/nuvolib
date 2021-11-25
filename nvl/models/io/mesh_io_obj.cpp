#include "mesh_io_obj.h"

#include <nvl/utilities/string_utils.h>
#include <nvl/utilities/file_utils.h>

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
    //Use "." as decimal separator
    std::setlocale(LC_NUMERIC, "en_US.UTF-8");

    data.clear();

    std::ifstream fObj, fMtl; //File streams
    std::string objLine, mtlLine;

    error = IO_MESH_SUCCESS;

    //Open file
    fObj.open(filename);
    if(!fObj.is_open()) { //Error opening file
        error = IO_MESH_FILE_ERROR;
        return false;
    }

    //File info descriptor
    std::string ext = filenameExtension(filename);
    std::string path = filenamePath(filename);

    //Material data
    std::unordered_map<std::string, Index> materialMap;
    Index currentMaterialId;

    bool materialLoaded = false;

    while (std::getline(fObj, objLine)) {
        if (objLine.empty())
            continue;

        std::vector<std::string> objSplitted = splitString(objLine, ' ');
        const std::string& objKey = objSplitted[0];

        //Handle material library
        if (objKey == "mtllib") {
            if (objSplitted.size() < 2) {
                error = IO_MESH_FORMAT_NON_RECOGNISED;
                continue;
            }

            //Get mtl file
            std::string filenameMtl = "";
            if (objSplitted[1].at(0) != '/') {
                filenameMtl += path;
            }
            for (Index i = 1; i < objSplitted.size(); i++) {
                filenameMtl += objSplitted[i];
            }

            //Open mtl file
            fMtl.open(filenameMtl);

            if(fMtl.is_open()) {
                materialLoaded = true;

                Index loadingMaterialId = NULL_ID;

                while (std::getline(fMtl, mtlLine)) {
                    std::vector<std::string> mtlSplitted = splitString(mtlLine, ' ');

                    const std::string& mtlKey = mtlSplitted[0];

                    if (mtlKey == "newmtl") {
                        if (mtlSplitted.size() < 2) {
                            error = IO_MESH_FORMAT_NON_RECOGNISED;
                            continue;
                        }

                        M material;

                        material.setName(mtlSplitted[1]);

                        loadingMaterialId = data.materials.size();
                        data.materials.push_back(material);

                        materialMap.insert(std::make_pair(material.name(), loadingMaterialId));
                    }
                    else if (mtlKey == "Kd") {
                        if (mtlSplitted.size() < 4) {
                            error = IO_MESH_FORMAT_NON_RECOGNISED;
                            continue;
                        }

                        typename M::Color color(std::stof(mtlSplitted[1]), std::stof(mtlSplitted[2]), std::stof(mtlSplitted[3]));
                        data.materials[loadingMaterialId].setDiffuseColor(color);
                    }
                    else if (mtlKey == "Ka") {
                        if (mtlSplitted.size() < 4) {
                            error = IO_MESH_FORMAT_NON_RECOGNISED;
                            continue;
                        }

                        typename M::Color color(std::stof(mtlSplitted[1]), std::stof(mtlSplitted[2]), std::stof(mtlSplitted[3]));
                        data.materials[loadingMaterialId].setAmbientColor(color);
                    }
                    else if (mtlKey == "Ks") {
                        if (mtlSplitted.size() < 4) {
                            error = IO_MESH_FORMAT_NON_RECOGNISED;
                            continue;
                        }

                        typename M::Color color(std::stof(mtlSplitted[1]), std::stof(mtlSplitted[2]), std::stof(mtlSplitted[3]));
                        data.materials[loadingMaterialId].setSpecularColor(color);
                    }
                    else if (mtlKey == "illum") {
                        if (mtlSplitted.size() < 2) {
                            error = IO_MESH_FORMAT_NON_RECOGNISED;
                            continue;
                        }

                        typename M::IlluminationModel illum = static_cast<typename M::IlluminationModel>(std::stoi(mtlSplitted[1]));
                        data.materials[loadingMaterialId].setIlluminationModel(illum);

                    }
                    else if (mtlKey == "d") {
                        if (mtlSplitted.size() < 2) {
                            error = IO_MESH_FORMAT_NON_RECOGNISED;
                            continue;
                        }

                        float transparency = std::stof(mtlSplitted[1]);
                        data.materials[loadingMaterialId].setTransparency(1 - transparency);
                    }
                    else if (mtlKey == "Tr") {
                        if (mtlSplitted.size() < 2) {
                            error = IO_MESH_FORMAT_NON_RECOGNISED;
                            continue;
                        }

                        float transparency = std::stof(mtlSplitted[1]);
                        data.materials[loadingMaterialId].setTransparency(transparency);
                    }
                    else if (mtlKey == "map_Kd") {
                        if (mtlSplitted.size() < 2) {
                            error = IO_MESH_FORMAT_NON_RECOGNISED;
                            continue;
                        }

                        std::string filenameMap = "";
                        if (mtlSplitted[1].at(0) != '/') {
                            filenameMap += path;
                        }
                        for (Index i = 1; i < mtlSplitted.size(); i++) {
                            filenameMap += mtlSplitted[i];
                        }
                        data.materials[loadingMaterialId].setDiffuseMap(filenameMap);
                    }
                    else if (mtlKey == "map_Ka") {
                        if (mtlSplitted.size() < 2) {
                            error = IO_MESH_FORMAT_NON_RECOGNISED;
                            continue;
                        }

                        std::string filenameMap = "";
                        if (mtlSplitted[1].at(0) != '/') {
                            filenameMap += path;
                        }
                        for (Index i = 1; i < mtlSplitted.size(); i++) {
                            filenameMap += mtlSplitted[i];
                        }
                        data.materials[loadingMaterialId].setAmbientMap(filenameMap);
                    }
                    else if (mtlKey == "map_Ks") {
                        if (mtlSplitted.size() < 2) {
                            error = IO_MESH_FORMAT_NON_RECOGNISED;
                            continue;
                        }

                        std::string filenameMap = "";
                        if (mtlSplitted[1].at(0) != '/') {
                            filenameMap += path;
                        }
                        for (Index i = 1; i < mtlSplitted.size(); i++) {
                            filenameMap += mtlSplitted[i];
                        }
                        data.materials[loadingMaterialId].setSpecularMap(filenameMap);
                    }
                    else if (mtlKey == "map_d") {
                        if (mtlSplitted.size() < 2) {
                            error = IO_MESH_FORMAT_NON_RECOGNISED;
                            continue;
                        }

                        std::string filenameMap = "";
                        if (mtlSplitted[1].at(0) != '/') {
                            filenameMap += path;
                        }
                        for (Index i = 1; i < mtlSplitted.size(); i++) {
                            filenameMap += mtlSplitted[i];
                        }
                        data.materials[loadingMaterialId].setTransparencyMap(filenameMap);
                    }
                    else if (mtlKey == "map_Bump" || mtlKey == "bump") {
                        if (mtlSplitted.size() < 2) {
                            error = IO_MESH_FORMAT_NON_RECOGNISED;
                            continue;
                        }

                        std::string filenameMap = "";
                        if (mtlSplitted[1].at(0) != '/') {
                            filenameMap += path;
                        }
                        for (Index i = 1; i < mtlSplitted.size(); i++) {
                            filenameMap += mtlSplitted[i];
                        }
                        data.materials[loadingMaterialId].setNormalMap(filenameMap);
                    }
                }
            }
        }

        //Handle materials
        else if (objKey == "usemtl") {
            if (objSplitted.size() != 2) {
                error = IO_MESH_FORMAT_NON_RECOGNISED;
                continue;
            }

            currentMaterialId = materialMap.at(objSplitted[1]);
        }

        //Handle vertex UV coords
        else if (objKey == "vt") {
            if (objSplitted.size() < 3) {
                error = IO_MESH_FORMAT_NON_RECOGNISED;
                continue;
            }

            data.vertexUVs.push_back(UV(std::stof(objSplitted[1]), std::stof(objSplitted[2])));
        }

        //Handle vertex normals
        else if (objKey == "vn") {
            if (objSplitted.size() != 4) {
                error = IO_MESH_FORMAT_NON_RECOGNISED;
                continue;
            }

            data.vertexNormals.push_back(VN(std::stof(objSplitted[1]), std::stof(objSplitted[2]), std::stof(objSplitted[3])));
        }

        //Handle vertices
        else if (objKey == "v") {
            if (objSplitted.size() < 4) {
                error = IO_MESH_FORMAT_NON_RECOGNISED;
                continue;
            }

            data.vertices.push_back(P(std::stof(objSplitted[1]), std::stof(objSplitted[2]), std::stof(objSplitted[3])));

            if (objSplitted.size() == 7) {
                data.vertexColors.push_back(VC(std::stof(objSplitted[4]), std::stof(objSplitted[5]), std::stof(objSplitted[6])));
            }
            else {
                data.vertexColors.push_back(Color(0.7, 0.7, 0.7));
            }
        }

        //Handle faces
        else if (objKey == "f") {
            if (objSplitted.size() < 4) {
                error = IO_MESH_FORMAT_NON_RECOGNISED;
                continue;
            }

            std::vector<Index> vertexIds;

            //Handling vertices
            for (Index i = 1; i < objSplitted.size(); ++i) {
                std::vector<std::string> faceSplitted = splitString(objSplitted[i], '/');

                if (faceSplitted[0].empty()) {
                    error = IO_MESH_FORMAT_NON_RECOGNISED;
                    continue;
                }

                Index vertexId = std::stoul(faceSplitted[0]) - 1;

                vertexIds.push_back(vertexId);
            }

            data.faces.push_back(vertexIds);

            //Handle textures
            std::vector<Index> faceVertexUVs;

            for (Index i = 1; i < objSplitted.size(); ++i) {
                std::vector<std::string> faceSplitted = splitString(objSplitted[i], '/');

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

            for (Index i = 1; i < objSplitted.size(); ++i) {
                std::vector<std::string> faceSplitted = splitString(objSplitted[i], '/');

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
        else if (objKey == "l") {
            if (objSplitted.size() < 3) {
                error = IO_MESH_FORMAT_NON_RECOGNISED;
                continue;
            }

            std::vector<Index> vertexIds;

            for (Index i = 1; i < objSplitted.size(); ++i) {
                vertexIds.push_back(std::stoul(objSplitted[i]) - 1);
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
    //Use "." as decimal separator
    std::setlocale(LC_NUMERIC, "en_US.UTF-8");

    std::ofstream fObj, fMtl; //File streams

    error = IO_MESH_SUCCESS;

    //Open file
    fObj.open(filename);
    if(!fObj.is_open()) { //Error opening file
        error = IO_MESH_FILE_ERROR;
        return false;
    }

    //Set precision
    fObj.precision(6);
    fObj.setf(std::ios::fixed, std:: ios::floatfield);

    //File info descriptor
    std::string ext = filenameExtension(filename);
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
        fMtl.setf(std::ios::fixed, std:: ios::floatfield);
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

    //Maps
    if (!material.diffuseMap().empty()) {
        std::string filename = material.diffuseMap();
        std::string file = filenameFile(filename);
        fMtl << "map_Kd " << file << std::endl;
        fileCopy(filename, path + file);
    }
    if (!material.ambientMap().empty()) {
        std::string filename = material.ambientMap();
        std::string file = filenameFile(filename);
        fMtl << "map_Ka " << file << std::endl;
        fileCopy(filename, path + file);
    }
    if (!material.specularMap().empty()) {
        std::string filename = material.specularMap();
        std::string file = filenameFile(filename);
        fMtl << "map_Ks " << file << std::endl;
        fileCopy(filename, path + file);
    }
    if (!material.normalMap().empty()) {
        std::string filename = material.normalMap();
        std::string file = filenameFile(filename);
        fMtl << "map_Bump " << file << std::endl;
        fileCopy(filename, path + file);
    }
    if (!material.transparencyMap().empty()) {
        std::string filename = material.transparencyMap();
        std::string file = filenameFile(filename);
        fMtl << "map_d " << file << std::endl;
        fileCopy(filename, path + file);
    }
}

}
