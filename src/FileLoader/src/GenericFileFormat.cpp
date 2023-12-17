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

  if( header.structCount != 0 ){
    structList.resize( header.structCount );
    file_.seekg( header.structOffset );
    for( auto& structObject : structList ){
      GenericToolbox::fillData(file_, structObject.type);
      GenericToolbox::fillData(file_, structObject.dataOffset);
      GenericToolbox::fillData(file_, structObject.fieldCount);
    }
  }

  if( header.fieldCount != 0 ){
    fieldList.resize( header.fieldCount );
    file_.seekg( header.fieldOffset );
    for( auto& field : fieldList ){
      GenericToolbox::fillData(file_, field.type);
      GenericToolbox::fillData(file_, field.labelIndex);
      GenericToolbox::fillData(file_, field.data);
    }
  }

  if( header.labelCount != 0 ){
    labelList.resize( header.labelCount );
    file_.seekg( header.labelOffset );
    for( auto& label : labelList ){
      GenericToolbox::fillData(file_, label);
    }
  }

  if( header.fieldIndicesCount != 0 ){
    fieldIndexList.resize( header.fieldIndicesCount );
    file_.seekg( header.fieldIndicesOffset );
    for( auto& fieldIndex : fieldIndexList ){
      GenericToolbox::fillData(file_, fieldIndex);
    }
  }

}


void GenericFileFormat::writeJson( nlohmann::json& json_) const{
  json_["header"]["fileType"] = GenericToolbox::toString(header.fileType);
  json_["header"]["fileVersion"] = GenericToolbox::toString(header.fileVersion);

//  json_["header"]["structOffset"] = header.structOffset;
//  json_["header"]["structCount"] = header.structCount;
  json_["header"]["fieldOffset"] = header.fieldOffset;
//  json_["header"]["fieldCount"] = header.fieldCount;
//  json_["header"]["labelOffset"] = header.labelOffset;
//  json_["header"]["labelCount"] = header.labelCount;
//  json_["header"]["fieldDataOffset"] = header.fieldDataOffset;
//  json_["header"]["fieldDataCount"] = header.fieldDataCount;
//  json_["header"]["fieldIndicesOffset"] = header.fieldIndicesOffset;
//  json_["header"]["fieldIndicesCount"] = header.fieldIndicesCount;
//  json_["header"]["listIndicesOffset"] = header.listIndicesOffset;
//  json_["header"]["listIndicesCount"] = header.listIndicesCount;

  for( auto& structObject : structList ){
    auto& structJson = json_["structList"].emplace_back();

    structJson["structType"] = structObject.type.toString();
//    structJson["dataOffset"] = structObject.dataOffset;
//    structJson["fieldCount"] = structObject.fieldCount;

    unsigned int offSetIndex = structObject.dataOffset/4;
    for( unsigned int iField = 0 ; iField < structObject.fieldCount ; iField++ ){
      auto& fieldJson = structJson["fieldList"].emplace_back();
      fieldJson["type"] = fieldList[offSetIndex + iField].type.toString();
//      fieldJson["labelIndex"] = fieldList[offSetIndex + iField].labelIndex;
      fieldJson["label"] = std::string(labelList[fieldList[offSetIndex + iField].labelIndex].data());
      dataToJson(fieldJson["data"], fieldList[offSetIndex + iField].type, fieldList[offSetIndex + iField].data);
    }

  }

//  for( auto& field : fieldList ){
//    auto& element = json_["fieldList"].emplace_back();
//    element["type"] = field.type.toString();
//    element["labelIndex"] = field.labelIndex;
//    element["data"] = field.data;
//  }

//  for( auto& label : labelList ){
//    json_["labelList"].emplace_back(std::string(label.data()));
//  }

//  for( auto& fieldIndex : fieldIndexList ){
//    json_["fieldIndexList"].emplace_back( fieldIndex );
//  }

}

void GenericFileFormat::dataToJson(nlohmann::json& json_, GffDataType type_, unsigned int data_){

  if     ( type_ == GffDataType::UChar  ){ json_ = *((unsigned char*) (&data_)); }
  else if( type_ == GffDataType::Char   ){ json_ = *((char*) (&data_)); }
  else if( type_ == GffDataType::UShort ){ json_ = *((unsigned short*) (&data_)); }
  else if( type_ == GffDataType::Short  ){ json_ = *((short*) (&data_)); }
  else if( type_ == GffDataType::UInt   ){ json_ = *((unsigned int*) (&data_)); }
  else if( type_ == GffDataType::Int    ){ json_ = *((int*) (&data_)); }
  else if( type_ == GffDataType::Float  ){ json_ = *((float*) (&data_)); }
  else                                   { json_ = data_; }

}