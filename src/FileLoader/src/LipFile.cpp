//
// Created by Adrien Blanchet on 10/12/2023.
//

#include "LipFile.h"

#include "Logger.h"

#include "sstream"


LoggerInit([]{
  Logger::getUserHeader() << "[LipFile]";
});


void LipFile::load(){
  LogInfo << "Loading LIP file: " << _filePath_ << std::endl;
  LogThrowIf(_filePath_.empty(), "Can't load file: file path not set.");

  std::ifstream file(_filePath_, std::ios::binary);

  LogThrowIf(not file.is_open(), "Could not open file: " << _filePath_);
  _content_.read(file);
}

std::string LipFileContent::getSummary() const{
  std::stringstream ss;
  ss << fileType << "/" << fileVersion << ": duration(" << duration << "), entryCount(" << entryCount << ")";
  for( auto& keyFrame : keyFrameList ){
    ss << std::endl << "  { ";
    ss << "time(" << keyFrame.time << "), ";
    ss << "shape(" << LipShapeEnumNamespace::toString(keyFrame.shape, true) << ")";
    ss << " }";
  }
  return ss.str();
}
void LipFileContent::read(std::ifstream& file_){

  // https://kotor-modding.fandom.com/wiki/LIP_Format

  fillData(file_, fileType, 4);
  fileType.pop_back(); // remove last char

  fillData(file_, fileVersion, 4);
  fillData(file_, duration);
  fillData(file_, entryCount);

  // Loop over entries
  keyFrameList.reserve(entryCount);
  for( unsigned int iEntry = 0 ; iEntry < entryCount ; iEntry++ ){
    keyFrameList.emplace_back();
    fillData(file_, keyFrameList.back().time);
    fillData(file_, keyFrameList.back().shape);
  }
}
