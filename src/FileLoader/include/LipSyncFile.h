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


#define MAKE_ENUM \
  ENUM_NAME( LipShape ) \
  ENUM_TYPE( unsigned char ) \
  ENUM_ENTRY( EE, 0 ) ENUM_ENTRY( EH ) ENUM_ENTRY( SCHWA )   \
  ENUM_ENTRY( AH ) ENUM_ENTRY( OH ) ENUM_ENTRY( OOH )   \
  ENUM_ENTRY( Y ) ENUM_ENTRY( S_TS ) ENUM_ENTRY( F_V ) ENUM_ENTRY( N_NG )   \
  ENUM_ENTRY( TH ) ENUM_ENTRY( M_P_B ) ENUM_ENTRY( T_D ) ENUM_ENTRY( J_SH )   \
  ENUM_ENTRY( L_R ) ENUM_ENTRY( K_G )
#include "GenericToolbox.MakeEnum.h"
#undef MAKE_ENUM


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
