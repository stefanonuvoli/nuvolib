#include "mesh_io_obj.h"

#include <nvl/utilities/string_utils.h>
#include <nvl/utilities/file_utils.h>

#include <fstream>
#include <map>

namespace nvl {

template<class Material>
void meshSaveObjMaterial(std::ofstream& fMtl, const Material& material, const std::string& path);

template<class P, class VN, class VU, class VC, class FN, class M>
bool meshLoadDataFromObj(
        const std::string& filename,
        IOMeshData<P,VN,VU,VC,FN,M>& data,
        IOMeshError& error,
        IOMeshMode& mode)
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
    std::string currentMaterialString;

    while (std::getline(fObj, objLine)) {
        if (objLine.empty())
            continue;

        std::vector<std::string> objSplitted = splitString(objLine, ' ');
        const std::string& objKey = objSplitted[0];

        //Handle material library
        if (mode.materials && objKey == "mtllib") {
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
                Index loadingMaterialId = MAX_INDEX;

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

                        data.materials.push_back(material);

                        loadingMaterialId = data.materials.size() - 1;
                    }
                    else if (mtlKey == "Ka") {
                        if (mtlSplitted.size() < 4) {
                            error = IO_MESH_FORMAT_NON_RECOGNISED;
                            continue;
                        }

                        typename M::Color color(std::stof(mtlSplitted[1]), std::stof(mtlSplitted[2]), std::stof(mtlSplitted[3]));
                        data.materials[loadingMaterialId].setAmbientColor(color);
                    }
                    else if (mtlKey == "Kd") {
                        if (mtlSplitted.size() < 4) {
                            error = IO_MESH_FORMAT_NON_RECOGNISED;
                            continue;
                        }

                        typename M::Color color(std::stof(mtlSplitted[1]), std::stof(mtlSplitted[2]), std::stof(mtlSplitted[3]));
                        data.materials[loadingMaterialId].setDiffuseColor(color);
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

                        typename M::IlluminationModel illum = std::stoi(mtlSplitted[1]);
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
                }
            }
        }

        //Handle materials
        else if (mode.materials && objKey == "usemtl") {
            if (objSplitted.size() != 2) {
                error = IO_MESH_FORMAT_NON_RECOGNISED;
                continue;
            }

            currentMaterialString = objSplitted[1];
        }

        //Handle vertex UV coords
        else if (mode.vertexUV && objKey == "vt") {
            if (objSplitted.size() < 3) {
                error = IO_MESH_FORMAT_NON_RECOGNISED;
                continue;
            }

            data.vertexUV.push_back(VU(std::stof(objSplitted[1]), std::stof(objSplitted[2])));
        }

        //Handle vertex normals
        else if (mode.vertexNormals && objKey == "vn") {
            if (objSplitted.size() != 4) {
                error = IO_MESH_FORMAT_NON_RECOGNISED;
                continue;
            }

            data.vertexNormals.push_back(VN(std::stof(objSplitted[1]), std::stof(objSplitted[2]), std::stof(objSplitted[3])));
        }

        //Handle vertices
        else if (mode.vertices && objKey == "v") {
            if (objSplitted.size() < 4) {
                error = IO_MESH_FORMAT_NON_RECOGNISED;
                continue;
            }

            data.vertices.push_back(P(std::stof(objSplitted[1]), std::stof(objSplitted[2]), std::stof(objSplitted[3])));

            if (mode.vertexColors && objSplitted.size() == 7) {
                data.vertexColors.push_back(VC(std::stof(objSplitted[4]), std::stof(objSplitted[5]), std::stof(objSplitted[6])));
            }
            else {
                data.vertexColors.push_back(Color(0.7, 0.7, 0.7));
            }
        }

        //Handle faces
        else if (mode.faces && objKey == "f") {
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
            if (mode.vertexUV) {
                std::vector<Index> faceVertexUV;

                for (Index i = 1; i < objSplitted.size(); ++i) {
                    std::vector<std::string> faceSplitted = splitString(objSplitted[i], '/');

                    if (faceSplitted.size() >= 2) {
                        if (!faceSplitted[1].empty()) {
                            Index uvId = std::stoul(faceSplitted[1]) - 1;
                            faceVertexUV.push_back(uvId);
                        }
                    }
                }

                data.faceVertexUV.push_back(faceVertexUV);
            }

            //Handle vertex normals
            if (mode.vertexNormals) {
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
            }

            if (mode.materials) {
                //Handling face materials
                data.faceMaterials.push_back(currentMaterialString);
            }
        }

        //Handle polylines
        else if (mode.polylines && objKey == "l") {
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

template<class P, class VN, class VU, class VC, class FN, class M>
bool meshSaveDataToObj(
        const std::string& filename,
        const IOMeshData<P,VN,VU,VC,FN,M> &data,
        IOMeshError& error,
        const IOMeshMode& mode)
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

    if (mode.materials && data.materials.size() > 0) {
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


    //Printing header
    fObj <<
            "###############################" << std::endl <<
            "# Generated by nuvolib (OBJ)" << std::endl <<
            "# Wavefront OBJ file" << std::endl;
    if (mode.vertices)
        fObj <<
            "# Vertices: " << data.vertices.size() << std::endl;
    if (mode.faces)
        fObj <<
            "# Faces: " << data.faces.size() << std::endl;
    if (mode.polylines)
        fObj <<
            "# Lines: " << data.polylines.size() << std::endl;
    if (mode.vertexUV)
        fObj <<
            "# UV coords: " << data.vertexUV.size() << std::endl;
    if (mode.vertexNormals)
        fObj <<
            "# Vertex normals: " << data.vertexNormals.size() << std::endl;
    fObj <<
            "###############################" << std::endl;

    if (mode.materials && data.materials.size() > 0) {
        //Printing header on mtl file
        fMtl <<
                "###############################" << std::endl <<
                "# Generated by nuvolib (MTL)" << std::endl <<
                "# Wavefront material file" << std::endl <<
                "###############################" << std::endl;

        fObj << std::endl << "mtllib " << name + ".mtl" << std::endl;
    }

    //Vertex UV coordinates
    if (mode.vertexUV && !data.vertexUV.empty()) {
        fObj << std::endl << "#Vertex UV coordinates:" << std::endl << std::endl;

        for (Index i = 0; i < data.vertexUV.size(); ++i) {
            const VU& uvCoords = data.vertexUV[i];
            fObj << "vt " << uvCoords.x() << " " << uvCoords.y() << std::endl;
        }
    }

    //Vertex normals
    if (mode.vertexNormals && !data.vertexNormals.empty()) {
        fObj << std::endl << "#Vertex normals:" << std::endl << std::endl;

        for (Index i = 0; i < data.vertexNormals.size(); ++i) {
            const VN& normal = data.vertexNormals[i];
            fObj << "vn " << normal.x() << " " << normal.y() << " " << normal.z() << std::endl;
        }
    }

    //Vertices
    if (mode.vertices && data.vertices.size() > 0) {
        fObj << std::endl << "#Vertices:" << std::endl << std::endl;

        for (Index i = 0; i < data.vertices.size(); ++i) {
            //Coordinates
            const P& vertex = data.vertices[i];
            fObj << "v " << vertex.x() << " " << vertex.y() << " " << vertex.z();

            //Vertex colors
            if (mode.vertexColors && !data.vertexColors.empty()) {
                const VC& color = data.vertexColors[i];
                fObj << " " << color.redF() << " " << color.greenF() << " " << color.blueF();
            }

            fObj << std::endl;
        }
    }

    //Materials
    if (mode.materials && data.materials.size() > 0) {
        Index matUnnamedMaterialId = 1;

        for (const M& material : data.materials) {
            M copyMaterial = material;

            if (copyMaterial.name().empty()) {
                copyMaterial.setName("material" + std::to_string(matUnnamedMaterialId));

                ++matUnnamedMaterialId;
            }

            meshSaveObjMaterial(fMtl, copyMaterial, path);
        }
    }

    //Faces
    if (mode.faces && data.faces.size() > 0) {
        fObj << std::endl << "#Faces:" << std::endl;
        if (!mode.materials || data.materials.size() == 0) {
            fObj << std::endl;
        }

        std::string currentMaterialName; //Current active material
        bool createdDefaultMaterial = false;

        //Save faces data
        for (Index i = 0; i < data.faces.size(); ++i) {
            //Face Material
            if (mode.materials && data.materials.size() > 0) {
                std::string faceMaterialName = data.faceMaterials[i];

                //If faces has no material
                if (faceMaterialName.empty()) {
                    faceMaterialName = "nvl_default_material";

                    //Create default material
                    if (!createdDefaultMaterial) {
                        M defaultMaterial;

                        defaultMaterial.setName("nvl_default_material");

                        meshSaveObjMaterial(fMtl, defaultMaterial, path);

                        createdDefaultMaterial = true;
                    }
                }

                if (currentMaterialName != faceMaterialName) {
                    assert(!faceMaterialName.empty());

                    fObj << std::endl;

                    fObj << "usemtl " << faceMaterialName << std::endl;

                    currentMaterialName = faceMaterialName;
                }
            }

            //Faces
            fObj << "f";

            for (Index j = 0; j < data.faces[i].size(); ++j) {
                const Index& vId = data.faces[i][j];

                fObj << " " << vId + 1;

                if (mode.vertexNormals || mode.vertexUV) {
                    fObj << "/";

                    //Face UV coords
                    if (mode.vertexUV && !data.faceVertexUV.empty() && !data.faceVertexUV[i].empty()) {
                        fObj << data.faceVertexUV[i][j] + 1;
                    }

                    fObj << "/";

                    //Face vertex normal
                    if (mode.vertexNormals && !data.faceVertexNormals.empty() && !data.faceVertexNormals[i].empty()) {
                        fObj << data.faceVertexNormals[i][j] + 1;
                    }
                }
            }

            fObj << std::endl;
        }
    }

    //Polylines
    if (mode.polylines && data.polylines.size() > 0) {
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
    if (mode.materials && data.materials.size() > 0) {
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
    if (!material.ambientMap().empty()) {
        std::string filename = material.ambientMap();
        std::string file = filenameFile(filename);
        fMtl << "map_Ka " << file << std::endl;
        fileCopy(filename, path + file);
    }
    if (!material.diffuseMap().empty()) {
        std::string filename = material.diffuseMap();
        std::string file = filenameFile(filename);
        fMtl << "map_Kd " << file << std::endl;
        fileCopy(filename, path + file);
    }
    if (!material.specularMap().empty()) {
        std::string filename = material.specularMap();
        std::string file = filenameFile(filename);
        fMtl << "map_Ks " << file << std::endl;
        fileCopy(filename, path + file);
    }
}

}
