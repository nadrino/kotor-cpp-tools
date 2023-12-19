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

  LogDebugIf(debug) << "header" << header << std::endl;

  if( header.structCount.asUInt != 0 ){
    InstantiateStream guard(file_);
    file_.seekg( header.structOffset.asUInt );

    LogDebugIf(debug) << "structList{" << std::endl;
    structList.resize( header.structCount.asUInt );
    for( auto& structObject : structList ){
      LogScopeIndent;
      GenericToolbox::fillData(file_, structObject.type);
      GenericToolbox::fillData(file_, structObject.fieldOffset);
      GenericToolbox::fillData(file_, structObject.fieldCount);
      LogDebugIf(debug) << structObject << std::endl;
    }
    LogDebugIf(debug) << "}" << std::endl;
  }

  if( header.fieldCount.asUInt != 0 ){
    InstantiateStream guard(file_);
    file_.seekg( header.fieldOffset.asUInt );

    LogDebugIf(debug) << "fieldList{" << std::endl;
    fieldList.resize( header.fieldCount.asUInt );
    for( auto& field : fieldList ){
      LogScopeIndent;
      GenericToolbox::fillData(file_, field.type);
      GenericToolbox::fillData(file_, field.labelIndex);
      GenericToolbox::fillData(file_, field.data);
      LogDebugIf(debug) << field << std::endl;
    }
    LogDebugIf(debug) << "}" << std::endl;
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
    for( auto& fieldIndex : fieldIndexList ){
      GenericToolbox::fillData(file_, fieldIndex);
      LogTraceIf(fieldIndex.asUInt != 0) << GET_VAR_NAME_VALUE(fieldIndex.asUInt) << std::endl;
    }
  }

  if( header.listIndicesCount.asUInt != 0 ){
    InstantiateStream guard(file_);
    file_.seekg( header.listIndicesOffset.asUInt );

    listIndexList.resize( header.listIndicesCount.asUInt );
    for( auto& listIndex: listIndexList ){
      GenericToolbox::fillData(file_, listIndex);
      LogTrace << GET_VAR_NAME_VALUE(listIndex.asUInt) << std::endl;
    }
  }

}


void GenericFileFormat::writeJson( nlohmann::ordered_json& json_) const{

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
    this->structToJson(structJson, structObject);
  }

}

void GenericFileFormat::structToJson(nlohmann::ordered_json& json_, const Struct& struct_) const{
  json_["class"] = "struct";
  json_["type"] = struct_.type.toString();

  for( unsigned int iField = 0 ; iField < struct_.fieldCount.asUInt ; iField++ ){
    this->fieldToJson(json_["fields"].emplace_back(), this->fieldList[iField]);
  }

  if( debug ){
    json_["fieldOffset"] = struct_.fieldOffset.asUInt;
    json_["fieldCount"] = struct_.fieldCount.asUInt;
  }
}
void GenericFileFormat::fieldToJson(nlohmann::ordered_json& json_, const Field& field_) const{
  json_["class"] = "Field";
  json_["type"] = field_.type.toString();

  json_["label"] = std::string(this->labelList[field_.labelIndex.asUInt].data());

  switch( field_.type.value ){
    // simple types
    case GffDataType::UChar:
      json_["value"] = "0x" + GenericToolbox::toHex(field_.data.asUChar[0]);
      break;
    case GffDataType::Char:
      json_["value"] = "0x" + GenericToolbox::toHex(field_.data.asChar[0]);
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
      break;
    }
    case GffDataType::Struct:
    {
      this->structToJson(json_["value"], this->structList[field_.data.asUInt]);
      break;
    }
    case GffDataType::List:
    {
      LogDebug << GET_VAR_NAME_VALUE(field_.data.asUInt) << std::endl;
      LogDebug << GET_VAR_NAME_VALUE(this->listIndexList[field_.data.asUInt].asUInt) << std::endl;
      auto listSize = this->listIndexList[field_.data.asUInt].asUInt;

      for( unsigned int iEntry = 0 ; iEntry < listSize ; iEntry++ ){
        auto structIndex = this->listIndexList[field_.data.asUInt + 1 + iEntry].asUInt;
        LogDebug << GET_VAR_NAME_VALUE(structIndex) << std::endl;
      }


      break;
    }

    default:
      LogError << "Unknown type: " << field_.type.toString() << std::endl;
      break;
  }


  if( debug ){
    // debug
    json_["typeIndex"] = field_.type.value;
    json_["labelIndex"] = field_.labelIndex.asUInt;
  }
}
