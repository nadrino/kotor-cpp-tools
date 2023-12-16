//
// Created by Adrien Blanchet on 14/12/2023.
//

#include "GenericFileFormat.h"

#include "GenericToolbox.h"
#include "Logger.h"


LoggerInit([]{ Logger::getUserHeader() << "[GenericFileFormat]"; });


void GenericFileFormat::readBinary( std::ifstream& file_){
  file_.seekg(0);

  GenericToolbox::fillData(file_, header.fileType);
  GenericToolbox::fillData(file_, header.fileVersion);
  GenericToolbox::fillData(file_, header.structOffset);
  GenericToolbox::fillData(file_, header.structCount);
  GenericToolbox::fillData(file_, header.fieldOffset);
  GenericToolbox::fillData(file_, header.fieldCount);
  GenericToolbox::fillData(file_, header.labelOffset);
  GenericToolbox::fillData(file_, header.labelCount);
  GenericToolbox::fillData(file_, header.fieldDataOffset);
  GenericToolbox::fillData(file_, header.fieldDataCount);
  GenericToolbox::fillData(file_, header.fieldIndicesOffset);
  GenericToolbox::fillData(file_, header.fieldIndicesCount);
  GenericToolbox::fillData(file_, header.listIndicesOffset);
  GenericToolbox::fillData(file_, header.listIndicesCount);

  structArrayList.resize( header.structCount );

  if( not structArrayList.empty() ){
    file_.seekg( header.structOffset );
    for( auto& structArray : structArrayList ){
      GenericToolbox::fillData(file_, structArray.type);
      GenericToolbox::fillData(file_, structArray.dataOffset);
      GenericToolbox::fillData(file_, structArray.fieldCount);
    }
  }

}


void GenericFileFormat::writeJson( nlohmann::json& json_) const{
  json_["header"]["fileType"] = GenericToolbox::toString(header.fileType);
  json_["header"]["fileVersion"] = GenericToolbox::toString(header.fileVersion);
//  json_["header"]["structOffset"] = header.structOffset;
//  json_["header"]["structCount"] = header.structCount;
//  json_["header"]["fieldOffset"] = header.fieldOffset;
//  json_["header"]["fieldCount"] = header.fieldCount;
//  json_["header"]["labelOffset"] = header.labelOffset;
//  json_["header"]["labelCount"] = header.labelCount;
//  json_["header"]["fieldDataOffset"] = header.fieldDataOffset;
//  json_["header"]["fieldDataCount"] = header.fieldDataCount;
//  json_["header"]["fieldIndicesOffset"] = header.fieldIndicesOffset;
//  json_["header"]["fieldIndicesCount"] = header.fieldIndicesCount;
//  json_["header"]["listIndicesOffset"] = header.listIndicesOffset;
//  json_["header"]["listIndicesCount"] = header.listIndicesCount;

  for( auto& structArray : structArrayList ){
    auto& element = json_["struct"].emplace_back();
    element["type"] = structArray.type.toString();
    element["dataOffset"] = structArray.dataOffset;
    element["fieldCount"] = structArray.fieldCount;
  }

}