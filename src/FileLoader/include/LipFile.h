//
// Created by Adrien Blanchet on 10/12/2023.
//

#ifndef KOTOR_CPP_TOOLS_LIPFILE_H
#define KOTOR_CPP_TOOLS_LIPFILE_H

#include "KotorBinaryFile.h"

#include "GenericToolbox.h"

#include "nlohmann/json.hpp"

#include <string>
#include <vector>


ENUM_EXPANDER(
    LipShape, 0,
    EE, EH,
    SCHWA,
    AH, OH, OOH, Y,
    S_TS, F_V, N_NG,
    TH, M_P_B,
    T_D, J_SH, L_R, K_G
);


class LipFile : public KotorBinaryFile {

public:
  LipFile() = default;

  // overrides - binary
  void readBinary(std::ifstream& file_) override;
  void writeBinary(std::ofstream& file_) const override;

  // overrides - json
  void readJson(nlohmann::json& json_) override;
  void writeJson(nlohmann::json& json_) const override;

  // overrides - misc
  [[nodiscard]] std::string getSummary() const override;

  // reference: https://kotor-modding.fandom.com/wiki/LIP_Format
  std::string fileType{}; // LIP
  std::string fileVersion{}; // V1.0
  float duration{std::nanf("unset")}; // Total duration (in seconds) of the lip sync animation
  unsigned int entryCount{}; // Number of key frames in the Key Frame Table

  struct KeyFrame{
    // 5 bytes
    float time{std::nanf("")};
    unsigned char shape{};
  };
  std::vector<KeyFrame> keyFrameList{};

};



#endif //KOTOR_CPP_TOOLS_LIPFILE_H
