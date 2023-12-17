//
// Created by Nadrino on 11/12/2023.
//

#include "MdlFile.h"

#include "GenericToolbox.h"

#include "string"


void MdlFile::readBinary(std::ifstream& file_){
  // File Header
  GenericToolbox::fillData(file_, fileHeader.unused);
  GenericToolbox::fillData(file_, fileHeader.mdlSize);
  GenericToolbox::fillData(file_, fileHeader.mdxSize);

  // Model Header - Size is 168 bytes, including the geometry header.
  GenericToolbox::fillData(file_, modelHeader.geometryHeader.functionPointer);
  GenericToolbox::fillData(file_, modelHeader.geometryHeader.modelName);
  GenericToolbox::fillData(file_, modelHeader.geometryHeader.rootNodeOffset);
  GenericToolbox::fillData(file_, modelHeader.geometryHeader.nodeCount);
  GenericToolbox::fillData(file_, modelHeader.geometryHeader.unknown);
  GenericToolbox::fillData(file_, modelHeader.geometryHeader.geometryType);
  GenericToolbox::fillData(file_, modelHeader.geometryHeader.padding);

  GenericToolbox::fillData(file_, modelHeader.modelType);
  GenericToolbox::fillData(file_, modelHeader.unknown);
  GenericToolbox::fillData(file_, modelHeader.padding);
  GenericToolbox::fillData(file_, modelHeader.disableFog);
  GenericToolbox::fillData(file_, modelHeader.childModelCount);
  GenericToolbox::fillData(file_, modelHeader.animationArrayOffset);
  GenericToolbox::fillData(file_, modelHeader.animationCount);
  GenericToolbox::fillData(file_, modelHeader.animationCount2);
  GenericToolbox::fillData(file_, modelHeader.unknown2);
  GenericToolbox::fillData(file_, modelHeader.boundingBoxMin);
  GenericToolbox::fillData(file_, modelHeader.boundingBoxMax);
  GenericToolbox::fillData(file_, modelHeader.radius);
  GenericToolbox::fillData(file_, modelHeader.animationScale);
  GenericToolbox::fillData(file_, modelHeader.supermodelName);
}

void MdlFile::writeJson(nlohmann::json& json_) const {
  // File Header
  json_["fileHeader"]["mdlSize"] = fileHeader.mdlSize;
  json_["fileHeader"]["mdxSize"] = fileHeader.mdxSize;

  // Model Header - Size is 168 bytes, including the geometry header.
  json_["modelHeader"]["geometryHeader"]["functionPointer"] = modelHeader.geometryHeader.functionPointer.toString();
  json_["modelHeader"]["geometryHeader"]["modelName"] = std::string(modelHeader.geometryHeader.modelName.data());
  json_["modelHeader"]["geometryHeader"]["rootNodeOffset"] = modelHeader.geometryHeader.rootNodeOffset;
  json_["modelHeader"]["geometryHeader"]["nodeCount"] = modelHeader.geometryHeader.nodeCount;
  json_["modelHeader"]["geometryHeader"]["geometryType"] = modelHeader.geometryHeader.geometryType.toString();

  json_["modelHeader"]["modelType"] = modelHeader.modelType;
  json_["modelHeader"]["padding"] = modelHeader.padding;
  json_["modelHeader"]["disableFog"] = bool(modelHeader.disableFog);
  json_["modelHeader"]["childModelCount"] = modelHeader.childModelCount;
  json_["modelHeader"]["animationArrayOffset"] = modelHeader.animationArrayOffset;
  json_["modelHeader"]["animationCount"] = modelHeader.animationCount;
  json_["modelHeader"]["animationCount2"] = modelHeader.animationCount2;
  json_["modelHeader"]["boundingBoxMin"] = modelHeader.boundingBoxMin;
  json_["modelHeader"]["boundingBoxMax"] = modelHeader.boundingBoxMax;
  json_["modelHeader"]["radius"] = modelHeader.radius;
  json_["modelHeader"]["animationScale"] = modelHeader.animationScale;
  json_["modelHeader"]["supermodelName"] = std::string(modelHeader.supermodelName.data());
}