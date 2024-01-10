//
// Created by Nadrino on 11/12/2023.
//

#ifndef KOTOR_CPP_TOOLS_MDLFILE_H
#define KOTOR_CPP_TOOLS_MDLFILE_H

#include "KotorBinaryFile.h"

#include <array>
#include <iostream>


#define ENUM_NAME GeometryReference
#define ENUM_TYPE size_t
#define ENUM_FIELDS \
  ENUM_FIELD( K1,      0x0040552000413670 ) \
  ENUM_FIELD( K1_Anim, 0x0043ece0004134f0 ) \
  ENUM_FIELD( K2,      0x0040560000416310 ) \
  ENUM_FIELD( K2_Anim, 0x004503b000416190 )
#include "GenericToolbox.MakeEnum.h"

#define ENUM_NAME GeometryType
#define ENUM_TYPE unsigned char
#define ENUM_FIELDS \
  ENUM_FIELD( RootNode, 0x02 ) \
  ENUM_FIELD( AnimationNode, 0x05 )
#include "GenericToolbox.MakeEnum.h"


class MdlFile : public KotorBinaryFile{

public:
  MdlFile() = default;

  // overrides - binary
  void readBinary(std::ifstream& file_) override;

  // overrides - json
  void writeJson(nlohmann::ordered_json& json_) const override;


  // https://kotor-modding.fandom.com/wiki/MDL_Format
  struct FileHeader{
    unsigned int unused{}; // Always set to 0.
    unsigned int mdlSize{};
    unsigned int mdxSize{};
  };
  FileHeader fileHeader{};

  struct ModelHeader{

    struct GeometryHeader{
      GeometryReference functionPointer{};
      std::array<char, 32> modelName{};
      unsigned int rootNodeOffset{};
      unsigned int nodeCount{};
      std::array<unsigned int, 7> unknown{};
      GeometryType geometryType{};
      std::array<unsigned char, 3> padding{};
    };

    GeometryHeader geometryHeader{};
    unsigned char modelType{};
    unsigned char unknown{};
    unsigned char padding{};
    unsigned char disableFog{};
    unsigned int childModelCount{};
    unsigned int animationArrayOffset{};
    unsigned int animationCount{};
    unsigned int animationCount2{}; // duplicate
    unsigned int unknown2{};
    std::array<float, 3> boundingBoxMin{};
    std::array<float, 3> boundingBoxMax{};
    float radius{};
    float animationScale{};
    std::array<char, 32> supermodelName{};
  };
  ModelHeader modelHeader{};

  struct NamesHeader{
    unsigned int offsetToRootNode{};
    unsigned int unused{};
    unsigned int mdxFileSize{};
    unsigned int mdxOffset{};
    unsigned int namesArrayOffset{};
    unsigned int namesUsedCount{};
    unsigned int namesAllocatedCount{};
  };
  NamesHeader namesHeader{};



};


#endif //KOTOR_CPP_TOOLS_MDLFILE_H
