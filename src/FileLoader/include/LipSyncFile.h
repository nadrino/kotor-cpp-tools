//
// Created by Nadrino on 10/12/2023.
//

#ifndef KOTOR_CPP_TOOLS_LIPSYNCFILE_H
#define KOTOR_CPP_TOOLS_LIPSYNCFILE_H

#include "KotorBinaryFile.h"

#include "GenericToolbox.h"

#include "nlohmann/json.hpp"

#include <string>
#include <vector>


#define ENUM_NAME LipShape
#define ENUM_TYPE unsigned char
#define ENUM_FIELDS \
  ENUM_FIELD( EE, 0 ) ENUM_FIELD( EH ) ENUM_FIELD( SCHWA )   \
  ENUM_FIELD( AH ) ENUM_FIELD( OH ) ENUM_FIELD( OOH )   \
  ENUM_FIELD( Y ) ENUM_FIELD( S_TS ) ENUM_FIELD( F_V ) ENUM_FIELD( N_NG )   \
  ENUM_FIELD( TH ) ENUM_FIELD( M_P_B ) ENUM_FIELD( T_D ) ENUM_FIELD( J_SH )   \
  ENUM_FIELD( L_R ) ENUM_FIELD( K_G )
#include "GenericToolbox.MakeEnum.h"


class LipSyncFile : public KotorBinaryFile {

public:
  LipSyncFile() = default;

  // overrides - binary
  void readBinary(std::ifstream& file_) override;
  void writeBinary(std::ofstream& file_) const override;

  // overrides - json
  void readJson(nlohmann::ordered_json& json_) override;
  void writeJson(nlohmann::ordered_json& json_) const override;

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
    LipShape shape{};
  };
  std::vector<KeyFrame> keyFrameList{};

};



#endif //KOTOR_CPP_TOOLS_LIPSYNCFILE_H
