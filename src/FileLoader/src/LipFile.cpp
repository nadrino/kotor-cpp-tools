//
// Created by Adrien Blanchet on 10/12/2023.
//

#include "LipSyncFile.h"

#include "Logger.h"
#include "GenericToolbox.Json.h"

#include "sstream"


LoggerInit([]{
  Logger::getUserHeader() << "[LipFile]";
});



void LipSyncFile::readBinary( std::ifstream& file_){
  GenericToolbox::fillData(file_, fileType, 4);
  fileType.pop_back(); // remove last char

  GenericToolbox::fillData(file_, fileVersion, 4);
  GenericToolbox::fillData(file_, duration);
  GenericToolbox::fillData(file_, entryCount);

  // Loop over entries
  keyFrameList.reserve(entryCount);
  for( unsigned int iEntry = 0 ; iEntry < entryCount ; iEntry++ ){
    keyFrameList.emplace_back();
    GenericToolbox::fillData(file_, keyFrameList.back().time);
    GenericToolbox::fillData(file_, keyFrameList.back().shape);
  }
}
void LipSyncFile::writeBinary( std::ofstream &file_ ) const {
  GenericToolbox::writeData(file_, fileType); // 3 bytes
  GenericToolbox::writeData(file_, ' ');
  GenericToolbox::writeData(file_, fileVersion);
  GenericToolbox::writeData(file_, duration);
  GenericToolbox::writeData(file_, entryCount);
  for( auto& keyFrame : keyFrameList ){
    GenericToolbox::writeData(file_, keyFrame.time);
    GenericToolbox::writeData(file_, keyFrame.shape);
  }
}

void LipSyncFile::readJson( nlohmann::json& json_){
  fileType = GenericToolbox::Json::fetchValue<std::string>(json_, "fileType");
  fileVersion = GenericToolbox::Json::fetchValue<std::string>(json_, "fileVersion");
  duration = GenericToolbox::Json::fetchValue<float>(json_, "duration");
  keyFrameList.clear();

  auto keyFrameListJson = GenericToolbox::Json::fetchValue<nlohmann::json>(json_, "keyFrameList");
  entryCount = keyFrameListJson.size();
  keyFrameList.reserve(entryCount);
  for( auto& keyFrameJson : keyFrameListJson ){
    keyFrameList.emplace_back();
    keyFrameList.back().time = GenericToolbox::Json::fetchValue<float>(keyFrameJson, "time");
    keyFrameList.back().shape = LipShape::toEnum(
        GenericToolbox::Json::fetchValue<std::string>(keyFrameJson, "shape")
        );
  }
}
void LipSyncFile::writeJson( nlohmann::json& json_) const{
  json_["fileType"] = fileType;
  json_["fileVersion"] = fileVersion;
  json_["duration"] = duration;
  for( auto& keyFrame : keyFrameList ){
    json_["keyFrameList"].emplace_back();
    json_["keyFrameList"].back()["time"] = keyFrame.time;
    json_["keyFrameList"].back()["shape"] = LipShape::toString(keyFrame.shape);
  }
}

std::string LipSyncFile::getSummary() const{
  std::stringstream ss;
  ss << fileType << "/" << fileVersion << ": duration(" << duration << "), entryCount(" << entryCount << ")";
  if( entryCount != 0 ){
    ss << " {";
    for( auto& keyFrame : keyFrameList ){
      ss << std::endl << "  { ";
      ss << "time(" << keyFrame.time << "), ";
      ss << "shape(" << LipShape::toString(keyFrame.shape) << ")";
      ss << " },";
    }
    ss << std::endl << "}";
  }
  return ss.str();
}
