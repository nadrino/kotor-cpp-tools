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

  [[nodiscard]] std::string getSummary() const;
  void read(std::ifstream& file_);

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


private:
  std::string _filePath_{};

  LipFileContent _content_{};


};

template<typename T> void fillData( std::ifstream& file_, T& buffer_ ){
  file_.read( reinterpret_cast<char*>(&buffer_), sizeof(T) );
}
void fillData( std::ifstream& file_, std::string& buffer_, size_t size_ ){
  buffer_.clear();
  buffer_.resize(size_);
  file_.read( buffer_.data(), long(size_) );
}


#endif //KOTOR_CPP_TOOLS_LIPFILE_H
