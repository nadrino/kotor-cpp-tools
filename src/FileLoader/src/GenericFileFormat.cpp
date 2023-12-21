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

  LogDebugIf(debug) << "header{" << std::endl << Logger::indent() << header << Logger::unIndent() << std::endl << "}" << std::endl;

  if( header.structCount.asUInt != 0 ){
    InstantiateStream guard(file_);
    file_.seekg( header.structOffset.asUInt );

    LogDebugIf(debug) << "structList{" << std::endl;
    structList.resize( header.structCount.asUInt );
    int idx{0};
    for( auto& structObject : structList ){
      GenericToolbox::fillData(file_, structObject.type);
      GenericToolbox::fillData(file_, structObject.fieldOffset);
      GenericToolbox::fillData(file_, structObject.fieldCount);
      LogScopeIndent;
      LogDebugIf(debug) << "#" << idx++ << " " << structObject << std::endl;
    }
    LogDebugIf(debug) << "}" << std::endl;
  }

  if( header.fieldCount.asUInt != 0 ){
    InstantiateStream guard(file_);
    file_.seekg( header.fieldOffset.asUInt );

    LogDebugIf(debug) << "fieldList{" << std::endl;
    fieldList.resize( header.fieldCount.asUInt );
    int idx{0};
    for( auto& field : fieldList ){
      GenericToolbox::fillData(file_, field.type);
      GenericToolbox::fillData(file_, field.labelIndex);
      GenericToolbox::fillData(file_, field.data);

      LogScopeIndent;
      LogDebugIf(debug) << "#" << idx++ << " " << field << std::endl;
    }
    LogDebugIf(debug) << "}" << std::endl;
  }

  if( header.labelCount.asUInt != 0 ){
    InstantiateStream guard(file_);
    file_.seekg( header.labelOffset.asUInt );

    LogDebugIf(debug) << "labelList{" << std::endl;
    labelList.resize( header.labelCount.asUInt );
    int idx{0};
    for( auto& label : labelList ){
      GenericToolbox::fillData(file_, label);

      LogScopeIndent;
      LogDebugIf(debug) << "#" << idx++ << " " << label.data() << std::endl;
    }
    LogDebugIf(debug) << "}" << std::endl;
  }

  if( header.fieldDataCount.asUInt != 0 ){
    InstantiateStream guard(file_);
    file_.seekg( header.fieldDataOffset.asUInt );
    LogThrowIf(file_.tellg() == -1, "Can't reach section of file: " << GET_VAR_NAME_VALUE(header.fieldDataOffset));

    fieldRawDataList.resize( header.fieldDataCount.asUInt );
    for( auto& fieldData : fieldRawDataList ){ GenericToolbox::fillData(file_, fieldData); }
  }

  if( header.fieldIndicesCount.asUInt != 0 ){
    InstantiateStream guard(file_);
    file_.seekg( header.fieldIndicesOffset.asUInt );
    LogThrowIf(file_.tellg() == -1, "Can't reach section of file: " << GET_VAR_NAME_VALUE(header.fieldIndicesOffset));

    fieldIndexList.resize( header.fieldIndicesCount.asUInt );
    for( auto& fieldIndex : fieldIndexList ){
      GenericToolbox::fillData(file_, fieldIndex);
    }
  }

  if( header.listIndicesCount.asUInt != 0 ){
    InstantiateStream guard(file_);
    file_.seekg( header.listIndicesOffset.asUInt );
    LogThrowIf(file_.tellg() == -1, "Can't reach section of file: " << GET_VAR_NAME_VALUE(header.listIndicesOffset));

    listIndexList.resize( header.listIndicesCount.asUInt );
    for( auto& listIndex: listIndexList ){
      GenericToolbox::fillData(file_, listIndex);
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
  json_["class"] = "Struct";
  if( debug or struct_.type.value == GffDataType::TopLevelStruct ){
    json_["type"] = struct_.type.toString();
  }

  unsigned int byteOffset{struct_.fieldOffset.asUInt};

  for( unsigned int iField = 0 ; iField < struct_.fieldCount.asUInt ; iField++ ){
//    LogDebug << GET_VAR_NAME_VALUE(fieldIndexList.size()) << std::endl;
//    LogDebug << GET_VAR_NAME_VALUE(byteOffset + iField*sizeof(DataChunk)) << std::endl;
    auto fieldIndex = *( reinterpret_cast<const DataChunk*>(
        &this->fieldIndexList.at(byteOffset + iField*sizeof(DataChunk)))
    );
//    LogDebug << GET_VAR_NAME_VALUE(fieldIndex) << std::endl;
    const Field* fieldPtr = &this->fieldList.at(fieldIndex.asUInt);
    this->fieldToJson(json_["fields"].emplace_back(), *fieldPtr);
  }

  if( debug ){
    json_["fieldOffset"] = struct_.fieldOffset.asUInt;
    json_["fieldCount"] = struct_.fieldCount.asUInt;
  }
}
void GenericFileFormat::fieldToJson(nlohmann::ordered_json& json_, const Field& field_) const{
  json_["class"] = "Field";
  json_["label"] = std::string(this->labelList[field_.labelIndex.asUInt].data());

  json_["type"] = field_.type.toString();
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
      unsigned int byteOffset{field_.data.asUInt};
      // 4 bytes size
      auto length = *(reinterpret_cast<const DataChunk*>(&this->fieldRawDataList[byteOffset]));
      byteOffset += sizeof(length);
      if( length.asUInt != 0 ){
        std::string value; value.resize(length.asUInt);
        std::memcpy(&value[0], &this->fieldRawDataList[byteOffset], length.asUInt);
        json_["value"] = value;
      }
      break;
    }
    case GffDataType::ResourceReference:
    {
      unsigned int byteOffset{field_.data.asUInt};
      // one byte size - 255 as maximum
      auto length = this->fieldRawDataList[byteOffset];
      byteOffset += sizeof(length);
      if( length != 0 ){
        std::string value; value.resize(length);
        std::memcpy(&value[0], &this->fieldRawDataList[byteOffset], length);
        json_["value"] = value;
      }
      break;
    }
    case GffDataType::LocalizedString:
    {
      LocalizedString ls{};

      auto byteOffset{field_.data.asUInt};
      auto length = *(reinterpret_cast<const unsigned int*>(&this->fieldRawDataList[byteOffset]));
      json_["value"]["length"] = length;
      byteOffset += sizeof(length);

      if( length != 0 ){
        ls.id = *(reinterpret_cast<const unsigned int*>(&this->fieldRawDataList[byteOffset]));
        json_["value"]["id"] = ls.id;
        byteOffset += sizeof(ls.id);

        ls.count = *(reinterpret_cast<const unsigned int*>(&this->fieldRawDataList[byteOffset]));
        byteOffset += sizeof(ls.count);
        json_["value"]["count"] = ls.count;

        for( unsigned int iStr = 0 ; iStr < ls.count ; iStr++ ){

          auto& strJson = json_["value"]["str"].emplace_back();
          LanguageId l{*(reinterpret_cast<const unsigned int*>(&this->fieldRawDataList[byteOffset]))};
          strJson["language"] = l.toString();
          byteOffset += sizeof(l);
          unsigned int strLength{*(reinterpret_cast<const unsigned int*>(&this->fieldRawDataList[byteOffset]))};
          strJson["length"] = strLength;
          byteOffset += sizeof(strLength);
          byteOffset += strLength;
          std::string value; value.resize(strLength);
          std::memcpy(&value[0], &this->fieldRawDataList[byteOffset], strLength);
          json_["value"] = value;
        }
      }
      break;
    }
    case GffDataType::Struct:
    {
      this->structToJson(json_["value"], this->structList[field_.data.asUInt]);
      break;
    }
    case GffDataType::List:
    {
      unsigned int byteOffset{field_.data.asUInt};
      auto length = *(reinterpret_cast<const DataChunk*>(&this->listIndexList[byteOffset]));
      byteOffset += sizeof(length);
      for( unsigned int iEntry = 0 ; iEntry < length.asUInt ; iEntry++ ){
        auto structIndex = *(reinterpret_cast<const DataChunk*>(&this->listIndexList[byteOffset + iEntry*sizeof(DataChunk)]));
        this->structToJson(json_["value"].emplace_back(), this->structList[structIndex.asUInt]);
      }
      break;
    }
    case GffDataType::Orientation:
    {
      std::vector<float> value(4);
      auto* dataPtr = reinterpret_cast<const float*>(&this->fieldRawDataList[field_.data.asUInt]);
      value[0] = *(dataPtr++);
      value[1] = *(dataPtr++);
      value[2] = *(dataPtr++);
      value[3] = *(dataPtr++);
      json_["value"] = value;
      break;
    }
    case GffDataType::Position:
    {
      std::vector<float> value(3);
      auto* dataPtr = reinterpret_cast<const float*>(&this->fieldRawDataList[field_.data.asUInt]);
      value[0] = *(dataPtr++);
      value[1] = *(dataPtr++);
      value[2] = *(dataPtr++);
      json_["value"] = value;
      break;
    }
    case GffDataType::StringReference:
    {
      auto* dataPtr = reinterpret_cast<const unsigned int*>(&this->fieldRawDataList[field_.data.asUInt]);
      unsigned int size{*(dataPtr++)};
      if( size != 4 ){ json_["error"] = "invalid strref size."; }
      json_["value"] = *(dataPtr++);
      break;
    }

    default:
      LogThrow("Unknown field type: " << field_.type.toString());
      break;
  }


  if( debug ){
    // debug
    json_["typeIndex"] = field_.type.value;
    json_["labelIndex"] = field_.labelIndex.asUInt;
  }
}
