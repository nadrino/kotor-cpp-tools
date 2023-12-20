//
// Created by Nadrino on 14/12/2023.
//

#ifndef KOTOR_CPP_TOOLS_GENERICFILEFORMAT_H
#define KOTOR_CPP_TOOLS_GENERICFILEFORMAT_H

#include "KotorBinaryFile.h"

#include "GenericToolbox.h"

#include <array>
#include <vector>
#include <sstream>
#include <string>


#define MAKE_ENUM \
  ENUM_NAME( GffDataType ) \
  ENUM_TYPE( unsigned int ) \
  ENUM_ENTRY( UChar, 0 ) ENUM_ENTRY( Char ) \
  ENUM_ENTRY( UShort ) ENUM_ENTRY( Short ) \
  ENUM_ENTRY( UInt ) ENUM_ENTRY( Int ) \
  ENUM_ENTRY( ULong ) ENUM_ENTRY( Long ) \
  ENUM_ENTRY( Float ) ENUM_ENTRY( Double ) \
  ENUM_ENTRY( ExoString ) \
  ENUM_ENTRY( ResourceReference ) \
  ENUM_ENTRY( LocalizedString ) \
  ENUM_ENTRY( Void ) \
  ENUM_ENTRY( Struct ) \
  ENUM_ENTRY( List ) \
  ENUM_ENTRY( Orientation ) \
  ENUM_ENTRY( Position ) \
  ENUM_ENTRY( StringReference ) \
  ENUM_ENTRY( TopLevelStruct, 0xFFFFFFFF ) \
  ENUM_OVERFLOW( BadGffDataType, 0x0FFFFFFF )
#include "GenericToolbox.MakeEnum.h"
#undef MAKE_ENUM

#define MAKE_ENUM \
  ENUM_NAME( LanguageId ) \
  ENUM_TYPE( unsigned int ) \
  ENUM_ENTRY( English, 0 )\
  ENUM_ENTRY( French ) \
  ENUM_ENTRY( German ) \
  ENUM_ENTRY( Italian ) \
  ENUM_ENTRY( Spanish ) \
  ENUM_ENTRY( Polish ) \
  ENUM_ENTRY( Korean, 128 ) \
  ENUM_ENTRY( ChineseTraditional, 129 ) \
  ENUM_ENTRY( ChineseSimplified, 130 ) \
  ENUM_ENTRY( Japanese, 131 )
#include "GenericToolbox.MakeEnum.h"
#undef MAKE_ENUM



class InstantiateStream{

public:
  InstantiateStream(std::istream& stream_): _stream_(&stream_), _state_(stream_.tellg()){}
  ~InstantiateStream(){ _stream_->seekg( _state_ ); }

private:
  std::istream* _stream_{nullptr};
  std::istream::pos_type _state_{};

};

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

  typedef unsigned char FieldIndex;
  std::vector<FieldIndex> fieldIndexList{};

  typedef unsigned char ListIndex;
  std::vector<ListIndex> listIndexList{};

  typedef unsigned char FieldRawData;
  std::vector<FieldRawData> fieldRawDataList{};

  struct LocalizedString{
    unsigned int id{};
    unsigned int count{};
    std::string str{};
  };


  void structToJson(nlohmann::ordered_json& json_, const Struct& struct_) const;
  void fieldToJson(nlohmann::ordered_json& json_, const Field& field_) const;

};


#endif //KOTOR_CPP_TOOLS_GENERICFILEFORMAT_H
