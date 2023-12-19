//
// Created by Nadrino on 14/12/2023.
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

  if( header.structCount.asUInt != 0 ){
    InstantiateStream guard(file_);
    file_.seekg( header.structOffset.asUInt );

    structList.resize( header.structCount.asUInt );
    for( auto& structObject : structList ){
      GenericToolbox::fillData(file_, structObject.type);
      GenericToolbox::fillData(file_, structObject.fielOffset);
      GenericToolbox::fillData(file_, structObject.fieldCount);
    }
  }

  if( header.fieldCount.asUInt != 0 ){
    InstantiateStream guard(file_);
    file_.seekg( header.fieldOffset.asUInt );

    fieldList.resize( header.fieldCount.asUInt );
    for( auto& field : fieldList ){
      GenericToolbox::fillData(file_, field.type);
      GenericToolbox::fillData(file_, field.labelIndex);
      GenericToolbox::fillData(file_, field.data);
    }
  }

  if( header.labelCount.asUInt != 0 ){
    InstantiateStream guard(file_);
    file_.seekg( header.labelOffset.asUInt );

    labelList.resize( header.labelCount.asUInt );
    for( auto& label : labelList ){ GenericToolbox::fillData(file_, label); }
  }

  if( header.fieldDataCount.asUInt != 0 ){
    InstantiateStream guard(file_);
    file_.seekg( header.fieldDataOffset.asUInt );

    fieldRawDataList.resize( header.fieldDataCount.asUInt );
    for( auto& fieldData : fieldRawDataList ){ GenericToolbox::fillData(file_, fieldData); }
  }

  if( header.fieldIndicesCount.asUInt != 0 ){
    InstantiateStream guard(file_);
    file_.seekg( header.fieldIndicesOffset.asUInt );

    fieldIndexList.resize( header.fieldIndicesCount.asUInt );
    for( auto& fieldIndex : fieldIndexList ){ GenericToolbox::fillData(file_, fieldIndex); }
  }


//  for( auto& structObject : structList ){
//    structObject.fieldList.resize( structObject.fieldCount );
//    for( auto& field : structObject.fieldList ){
//      int labelIndex{};
//
//      InstantiateStream guard(file_);
//      file_.seekg( header.fieldOffset + structObject.fielOffset ); // go to field section
//      GenericToolbox::fillData(file_, field.type);
//      GenericToolbox::fillData(file_, labelIndex);
//      this->readData(file_, field);
//
//      field.label = labelList[labelIndex].data();
//    }
//  }

}


void GenericFileFormat::writeJson( nlohmann::json& json_) const{

  auto& headerJson = json_["header"];
  headerJson["fileType"] = GenericToolbox::toString(header.fileType.asChar);
  headerJson["fileVersion"] = GenericToolbox::toString(header.fileVersion.asChar);

  if( debug ){
    headerJson["structOffset"] = header.structOffset.asUInt;
    headerJson["structCount"] = header.structCount.asUInt;
    headerJson["fieldOffset"] = header.fieldOffset.asUInt;
    headerJson["fieldCount"] = header.fieldCount.asUInt;
    headerJson["labelOffset"] = header.labelOffset.asUInt;
    headerJson["labelCount"] = header.labelCount.asUInt;
    headerJson["fieldDataOffset"] = header.fieldDataOffset.asUInt;
    headerJson["fieldDataCount"] = header.fieldDataCount.asUInt;
    headerJson["fieldIndicesOffset"] = header.fieldIndicesOffset.asUInt;
    headerJson["fieldIndicesCount"] = header.fieldIndicesCount.asUInt;
    headerJson["listIndicesOffset"] = header.listIndicesOffset.asUInt;
    headerJson["listIndicesCount"] = header.listIndicesCount.asUInt;
  }

  for( auto& structObject : structList ){

    if( structObject.type != GffDataType::TopLevelStruct ){ continue; }

    auto& structJson = json_["entries"].emplace_back();
    structJson["class"] = "struct";
    structJson["type"] = structObject.type.toString();

    for( unsigned int iField = 0 ; iField < structObject.fieldCount.asUInt ; iField++ ){
      fieldToJson(structJson["field"].emplace_back(), this->fieldList[iField]);
    }

    if( debug ){
      structJson["fielOffset"] = structObject.fielOffset.asUInt;
      structJson["fieldCount"] = structObject.fieldCount.asUInt;
    }

  }

}

void GenericFileFormat::fieldToJson(nlohmann::json& json_, const Field& field_) const{
  json_["class"] = "Field";
  json_["type"] = field_.type.toString();

  json_["label"] = std::string(this->labelList[field_.labelIndex.asUInt].data());

  switch( field_.type.value ){
    // simple types
    case GffDataType::UChar:
      json_["value"] = "0x" + GenericToolbox::toHex(*field_.data.asUChar.data());
      break;
    case GffDataType::Char:
      json_["value"] = "0x" + GenericToolbox::toHex(*field_.data.asChar.data());
      break;
    case GffDataType::UShort:
      json_["value"] = field_.data.asUShort[0];
      break;
    case GffDataType::Short:
      json_["value"] = field_.data.asShort[0];
      break;
    case GffDataType::UInt:
      json_["value"] = field_.data.asUInt;
      break;
    case GffDataType::Int:
      json_["value"] = field_.data.asInt;
      break;
    case GffDataType::Float:
      json_["value"] = field_.data.asFloat;
      break;

    // extended
    case GffDataType::ULong:
    {
      auto value{*( reinterpret_cast<const unsigned long*>(&this->fieldRawDataList[field_.data.asUInt]) )};
      json_["value"] = value;
      break;
    }
    case GffDataType::Long:
    {
      auto value{*( reinterpret_cast<const long*>(&this->fieldRawDataList[field_.data.asUInt]) )};
      json_["value"] = value;
      break;
    }
    case GffDataType::Double:
    {
      auto value{*( reinterpret_cast<const double*>(&this->fieldRawDataList[field_.data.asUInt]) )};
      json_["value"] = value;
      break;
    }
    case GffDataType::ExoString:
    {
      auto length = this->fieldRawDataList[field_.data.asUInt].asUInt;
      std::string value;
      value.resize(length);
      std::memcpy(&value[0], &this->fieldRawDataList[field_.data.asUInt+1], length);
      json_["value"] = value;
    }
      break;

    default:
      break;
  }


  if( debug ){
    // debug
    json_["typeIndex"] = field_.type.value;
    json_["labelIndex"] = field_.labelIndex.asUInt;
  }
}
