//
// Created by Adrien Blanchet on 10/12/2023.
//

#include "LipFile.h"

#include "Logger.h"

#include "sstream"


LoggerInit([]{
  Logger::getUserHeader() << "[LipFile]";
});


std::string LipFileContent::getSummary() const{
  std::stringstream ss;
  ss << fileType << "/" << fileVersion << ": duration(" << duration << "), entryCount(" << entryCount << ")";
  if( entryCount != 0 ){
    ss << " {";
    for( auto& keyFrame : keyFrameList ){
      ss << std::endl << "  { ";
      ss << "time(" << keyFrame.time << "), ";
      ss << "shape(" << LipShapeEnumNamespace::toString(keyFrame.shape, true) << ")";
      ss << " },";
    }
    ss << std::endl << "}";
  }
  return ss.str();
}
void LipFileContent::read(std::ifstream& file_){
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
void LipFileContent::write( std::ofstream &file_ ) const {
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

void LipFile::load(){
  LogInfo << "Loading LIP file: " << _filePath_ << std::endl;
  LogThrowIf(_filePath_.empty(), "Can't load file: file path not set.");

  std::ifstream file(_filePath_, std::ios::binary);

  LogThrowIf(not file.is_open(), "Could not open file: " << _filePath_);
  _content_.read(file);
}
[[nodiscard]] std::string LipFile::getSummary() const{
  std::stringstream ss;
  ss << "LipFile: file(" << _filePath_ << ")";
  if( not _filePath_.empty() ){
    ss << " content:" << std::endl;
    ss << _content_.getSummary();
  }
  return ss.str();
}

