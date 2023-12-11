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
    std::array<unsigned char, 80> geometryHeader{}; // 80 byte
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
    std::array<unsigned char, 32> supermodelName{};
  };
  ModelHeader modelHeader{};



};


#endif //KOTOR_CPP_TOOLS_MDLFILE_H
