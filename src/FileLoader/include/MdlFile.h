//
// Created by Adrien Blanchet on 11/12/2023.
//

#ifndef KOTOR_CPP_TOOLS_MDLFILE_H
#define KOTOR_CPP_TOOLS_MDLFILE_H

#include "KotorBinaryFile.h"

#include "array"






class MdlFile : public KotorBinaryFile{

public:
  MdlFile() = default;

  // overrides - binary
  void readBinary(std::ifstream& file_) override;

  // overrides - json
  void writeJson(nlohmann::json& json_) const override;


  // https://kotor-modding.fandom.com/wiki/MDL_Format
  struct FileHeader{
    unsigned int unused{}; // Always set to 0.
    unsigned int mdlSize{};
    unsigned int mdxSize{};
  };
  FileHeader fileHeader{};

  struct ModelHeader{

    struct GeometryHeader{
      unsigned int functionPointer{}; // K1 = 4273776, K1 Anim = 4273392, K2 = 4285200, K2 Anim = 4284816
      unsigned int functionPointer2{}; // K1 = 4216096, K1 Anim = 4451552, K2 = 4216320, K2 Anim = 4522928
      std::array<char, 32> modelName{};
      unsigned int rootNodeOffset{};
      unsigned int nodeCount{};
      std::array<unsigned int, 7> unknown{};
      unsigned char geometryType{};
      std::array<unsigned char, 3> padding{}; // 0x02 = Root Node, 0x05 = Animation Node
    };

    GeometryHeader geometryHeader{};
    unsigned char modelType{};
    unsigned char unknown{};
    unsigned char padding{};
    unsigned char disableFog{}; // 0 = Fog, 1 = No Fog
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
