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

  fieldExtendedData.load(file_, header.fieldDataOffset.asUInt, header.fieldDataCount.asUInt);
  fieldIndexData.load(file_, header.fieldIndicesOffset.asUInt, header.fieldIndicesCount.asUInt);
  listIndexData.load(file_, header.listIndicesOffset.asUInt, header.listIndicesCount.asUInt);

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
//    auto fieldIndex = *( reinterpret_cast<const DataChunk*>(
//        &this->fieldIndexList.at(byteOffset + iField*sizeof(DataChunk)))
//    );
//    LogDebug << GET_VAR_NAME_VALUE(fieldIndex) << std::endl;
    const Field* fieldPtr = &this->fieldList.at(
        fieldIndexData.readAt<uint32_t>(byteOffset + iField*sizeof(uint32_t))
    );
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
      json_["value"] = fieldExtendedData.readAt<unsigned long>(field_.data.asUInt);
      break;
    case GffDataType::Long:
      json_["value"] = fieldExtendedData.readAt<long>(field_.data.asUInt);
      break;
    case GffDataType::Double:
      json_["value"] = fieldExtendedData.readAt<double>(field_.data.asUInt);
      break;

    case GffDataType::ExoString:
    {
      fieldExtendedData.setPos( field_.data.asUInt );
      auto length = fieldExtendedData.read<uint32_t>();
      if( length != 0 ){ json_["value"] = fieldExtendedData.readString(length); }
      if( debug ){ json_["value"]["length"] = length; }
      break;
    }

    case GffDataType::ResourceReference:
    {
      fieldExtendedData.setPos( field_.data.asUInt );
      auto length = fieldExtendedData.read<uint8_t>(); // one byte size - 255 as maximum
      if( length != 0 ){ json_["value"] = fieldExtendedData.readString(length); }
      if( debug ){ json_["value"]["length"] = length; }
      break;
    }

    case GffDataType::LocalizedString:
    {
      fieldExtendedData.setPos( field_.data.asUInt );
      auto length = fieldExtendedData.read<uint32_t>();
      if( debug ){ json_["value"]["length"] = length; }
      if( length != 0 ){
        json_["value"]["strref"] = fieldExtendedData.read<uint32_t>();
        auto count = fieldExtendedData.read<uint32_t>();
        if( debug ){ json_["value"]["count"] = count; }

        for( unsigned int iStr = 0 ; iStr < count ; iStr++ ){
          auto& strJson = json_["value"]["str"].emplace_back();
          strJson["language"] = fieldExtendedData.read<LanguageId>().toString();
          auto strLength = fieldExtendedData.read<uint32_t>();
          strJson["string"] = fieldExtendedData.readString(strLength);
          if( debug ){ strJson["length"] = strLength; }
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
      auto length = listIndexData.readAt<uint32_t>(field_.data.asUInt);
      for( unsigned int iEntry = 0 ; iEntry < length ; iEntry++ ){
        auto structIndex = listIndexData.readAt<uint32_t>(field_.data.asUInt + iEntry*sizeof(uint32_t));
        this->structToJson(json_["value"].emplace_back(), this->structList[structIndex]);
      }
      break;
    }
    case GffDataType::Orientation:
    {
      std::vector<float> value(4);
      fieldExtendedData.setPos( field_.data.asUInt );
      value[0] = fieldExtendedData.read<float>();
      value[1] = fieldExtendedData.read<float>();
      value[2] = fieldExtendedData.read<float>();
      value[3] = fieldExtendedData.read<float>();
      json_["value"] = value;
      break;
    }
    case GffDataType::Position:
    {
      std::vector<float> value(3);
      fieldExtendedData.setPos( field_.data.asUInt );
      value[0] = fieldExtendedData.read<float>();
      value[1] = fieldExtendedData.read<float>();
      value[2] = fieldExtendedData.read<float>();
      json_["value"] = value;
      break;
    }
    case GffDataType::StringReference:
    {
      fieldExtendedData.setPos( field_.data.asUInt );
      if( fieldExtendedData.read<unsigned int>() != 4 ){ json_["error"] = "invalid strref size."; }
      json_["value"] = fieldExtendedData.read<unsigned int>();
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
