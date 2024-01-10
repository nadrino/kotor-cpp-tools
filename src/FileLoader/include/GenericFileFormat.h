//
// Created by Nadrino on 14/12/2023.
//

#ifndef KOTOR_CPP_TOOLS_GENERIC_FILE_FORMAT_H
#define KOTOR_CPP_TOOLS_GENERIC_FILE_FORMAT_H


#include "KotorBinaryFile.h"

#include "GenericToolbox.h"
#include "GenericToolbox.Stream.h"

#include <array>
#include <vector>
#include <sstream>
#include <string>
#include <cstdint>


#define ENUM_NAME GffDataType
#define ENUM_TYPE unsigned int
#define ENUM_OVERFLOW ENUM_FIELD( BadGffDataType, 0x0FFFFFFF )
#define ENUM_FIELDS \
  ENUM_FIELD( UChar, 0 ) ENUM_FIELD( Char ) \
  ENUM_FIELD( UShort ) ENUM_FIELD( Short ) \
  ENUM_FIELD( UInt ) ENUM_FIELD( Int ) \
  ENUM_FIELD( ULong ) ENUM_FIELD( Long ) \
  ENUM_FIELD( Float ) ENUM_FIELD( Double ) \
  ENUM_FIELD( ExoString ) \
  ENUM_FIELD( ResourceReference ) \
  ENUM_FIELD( LocalizedString ) \
  ENUM_FIELD( Void ) \
  ENUM_FIELD( Struct ) \
  ENUM_FIELD( List ) \
  ENUM_FIELD( Orientation ) \
  ENUM_FIELD( Position ) \
  ENUM_FIELD( StringReference ) \
  ENUM_FIELD( TopLevelStruct, 0xFFFFFFFF )
#include "GenericToolbox.MakeEnum.h"


#define ENUM_NAME LanguageId
#define ENUM_TYPE unsigned int
#define ENUM_FIELDS \
  ENUM_FIELD( English, 0 )\
  ENUM_FIELD( French ) \
  ENUM_FIELD( German ) \
  ENUM_FIELD( Italian ) \
  ENUM_FIELD( Spanish ) \
  ENUM_FIELD( Polish ) \
  ENUM_FIELD( Korean, 128 ) \
  ENUM_FIELD( ChineseTraditional, 129 ) \
  ENUM_FIELD( ChineseSimplified, 130 ) \
  ENUM_FIELD( Japanese, 131 )
#include "GenericToolbox.MakeEnum.h"


union DataChunk{
  // 32 bits data chunk
  std::array<unsigned char, 4> asUChar;
  std::array<char, 4> asChar;
  std::array<unsigned short, 2> asUShort;
  std::array<short, 2> asShort;
  unsigned int asUInt{0x0};
  int asInt;
  float asFloat;

  friend std::ostream& operator<< (std::ostream& stream, const DataChunk& this_){
    stream << "0x" << GenericToolbox::toHex(this_.asUInt);
    return stream;
  }
};

class GenericFileFormat : public KotorBinaryFile {

public:
  GenericFileFormat() = default;

  // overrides - binary
  void readBinary(std::ifstream& file_) override;

  // overrides - json
  void writeJson(nlohmann::ordered_json& json_) const override;


private:
  bool debug{false};

  // https://kotor-modding.fandom.com/wiki/GFF_Format
  struct Header{
    DataChunk fileType{};
    DataChunk fileVersion{};

    DataChunk structOffset{};
    DataChunk structCount{}; // 3 bytes each

    DataChunk fieldOffset{};
    DataChunk fieldCount{}; // 3 bytes each

    DataChunk labelOffset{};
    DataChunk labelCount{}; // 16 bytes each

    DataChunk fieldDataOffset{};
    DataChunk fieldDataCount{}; // number of bytes

    DataChunk fieldIndicesOffset{};
    DataChunk fieldIndicesCount{}; // number of bytes (/4 for number of obj)

    DataChunk listIndicesOffset{};
    DataChunk listIndicesCount{}; // number of bytes (/4 for number of obj)

    [[nodiscard]] std::string getSummary() const {
      std::stringstream ss;
      ss << "fileType(" << GenericToolbox::toString(fileType.asChar) << "), fileVersion(" << GenericToolbox::toString(fileVersion.asChar) << ")";
      ss << std::endl << "structOffset(" << structOffset << "), structCount(" << structCount.asUInt << ")";
      ss << std::endl << "fieldOffset(" << fieldOffset << "), fieldCount(" << fieldCount.asUInt << ")";
      ss << std::endl << "labelOffset(" << labelOffset << "), labelCount(" << labelCount.asUInt << ")";
      ss << std::endl << "fieldDataOffset(" << fieldDataOffset << "), fieldDataCount(" << fieldDataCount.asUInt << ")";
      ss << std::endl << "fieldIndicesOffset(" << fieldIndicesOffset << "), fieldIndicesCount(" << fieldIndicesCount.asUInt << ")";
      ss << std::endl << "listIndicesOffset(" << listIndicesOffset << "), listIndicesOffset(" << listIndicesOffset.asUInt << ")";
      return ss.str();
    }

    friend std::ostream& operator<< (std::ostream& stream, const Header& this_){ stream << this_.getSummary(); return stream; }

  };
  Header header{};

  struct Struct{
    GffDataType type{};
    DataChunk fieldOffset{}; // nb of bytes
    DataChunk fieldCount{}; // nb of fields (3*4 bytes)

    [[nodiscard]] std::string getSummary() const {
      std::stringstream ss;
      ss << "Struct{ type(" << type.toString() << "), fieldOffset(" << fieldOffset << "), fieldCount(" << fieldCount.asUInt << ") }";
      return ss.str();
    }
    friend std::ostream& operator<< (std::ostream& stream, const Struct& this_){ stream << this_.getSummary(); return stream; }
  };
  std::vector<Struct> structList{};

  struct Field{
    GffDataType type{};
    DataChunk labelIndex{};
    DataChunk data{}; // or data offset if it is an extended type

    [[nodiscard]] std::string getSummary() const {
      std::stringstream ss;
      ss << "Field{ type(" << type.toString() << "), labelIndex(" << labelIndex.asUInt << "), data(" << data << ") }";
      return ss.str();
    }
    friend std::ostream& operator<< (std::ostream& stream, const Field& this_){ stream << this_.getSummary(); return stream; }
  };
  std::vector<Field> fieldList{};

  typedef std::array<char, 16> Label;
  std::vector<Label> labelList{};

  GenericToolbox::DataStream fieldIndexData{};
  GenericToolbox::DataStream listIndexData{};
  GenericToolbox::DataStream fieldExtendedData{};

  void structToJson(nlohmann::ordered_json& json_, const Struct& struct_) const;
  void fieldToJson(nlohmann::ordered_json& json_, const Field& field_) const;

};


#endif // KOTOR_CPP_TOOLS_GENERIC_FILE_FORMAT_H
