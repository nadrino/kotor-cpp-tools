//
// Created by Adrien Blanchet on 14/12/2023.
//

#ifndef KOTOR_CPP_TOOLS_GENERICFILEFORMAT_H
#define KOTOR_CPP_TOOLS_GENERICFILEFORMAT_H

#include "KotorBinaryFile.h"

#include <array>
#include <vector>


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
  ENUM_ENTRY( Vector ) \
  ENUM_ENTRY( StringReference )
#include "GenericToolbox.MakeEnum.h"
#undef MAKE_ENUM

class GenericFileFormat : public KotorBinaryFile {

public:
  GenericFileFormat() = default;

  // overrides - binary
  void readBinary(std::ifstream& file_) override;

  // overrides - json
  void writeJson(nlohmann::json& json_) const override;

  // https://kotor-modding.fandom.com/wiki/GFF_Format
  struct Header{
    std::array<char, 4> fileType{};
    std::array<char, 4> fileVersion{};
    unsigned int structOffset{};
    unsigned int structCount{};
    unsigned int fieldOffset{};
    unsigned int fieldCount{};
    unsigned int labelOffset{};
    unsigned int labelCount{};
    unsigned int fieldDataOffset{};
    unsigned int fieldDataCount{};
    unsigned int fieldIndicesOffset{};
    unsigned int fieldIndicesCount{};
    unsigned int listIndicesOffset{};
    unsigned int listIndicesCount{};
  };
  Header header{};

  struct StructArray{
    GffDataType type{};
    unsigned int dataOffset{};
    unsigned int fieldCount{};
  };
  std::vector<StructArray> structArrayList{};

  struct LabelArray{
    GffDataType type{};
    unsigned int labelIndex{};
    unsigned int dataOffset{};
  };
  std::vector<LabelArray> labelArrayList{};

  struct FieldDataBlock{

  };
  FieldDataBlock fieldDataBlock{};

  struct FieldIndicesArray{

  };
  FieldIndicesArray fieldIndicesArray{};

  struct ListIndicesArray{

  };
  ListIndicesArray listIndicesArray{};



};


#endif //KOTOR_CPP_TOOLS_GENERICFILEFORMAT_H
