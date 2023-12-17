//
// Created by Nadrino on 11/12/2023.
//

#ifndef KOTOR_CPP_TOOLS_KOTORBINARYFILE_H
#define KOTOR_CPP_TOOLS_KOTORBINARYFILE_H

#include "nlohmann/json.hpp"

#include "iostream"
#include "string"

class KotorBinaryFile{

public:
  KotorBinaryFile() = default;

  virtual void read(const std::string& filePath_);
  virtual void write(const std::string& filePath_);

  // overrides: binary
  virtual void readBinary(std::ifstream& file_){  };
  virtual void writeBinary(std::ofstream& file_) const {  };

  //  overrides: json interface
  virtual void readJson(nlohmann::json& json_) {  };
  virtual void writeJson(nlohmann::json& json_) const{  };

  [[nodiscard]] virtual std::string getSummary() const { return {}; };


};


#endif //KOTOR_CPP_TOOLS_KOTORBINARYFILE_H
