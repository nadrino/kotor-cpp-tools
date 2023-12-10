//
// Created by Adrien Blanchet on 10/12/2023.
//

#ifndef KOTOR_CPP_TOOLS_LIPFILE_H
#define KOTOR_CPP_TOOLS_LIPFILE_H

#include "GenericToolbox.h"

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


struct LipFileContent{

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

  void read(std::ifstream& file_);

  [[nodiscard]] std::string getSummary() const;

};

class LipFile{

public:
  LipFile() = default;

  // setters
  void setFilePath(const std::string& filePath_){ _filePath_ = filePath_; }

  // const-getters
  [[nodiscard]] const LipFileContent& getContent() const { return _content_; }

  // core
  void load();

  [[nodiscard]] std::string getSummary() const;


private:
  std::string _filePath_{};
  LipFileContent _content_{};

};



#endif //KOTOR_CPP_TOOLS_LIPFILE_H
