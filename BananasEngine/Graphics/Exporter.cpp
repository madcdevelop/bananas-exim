#include "Exporter.h"

namespace GraphicsEngine
{

Exporter::Exporter()
{

}

Exporter::~Exporter()
{
    
}

bool Exporter::ExportModel(std::string& filePath, std::vector<Model>& models)
{
    // transform models data structure into format of .obj file

    // First line is comment about the program and version the obj file came from
    // # BananasImportExport v1.00.0 OBJ File

    // mtllib name_of_obj_file.mtl

    // o name_of_object

    // v reverse the index and only output unique vertex positions

    // vt reverse the index and only output unique texture coordinates

    // vn reverse the index and only output unique vertex normals

    // usemtl name_of_material_for_mesh

    // s off

    // f output with / in between to get face indices

    return true;
}

}