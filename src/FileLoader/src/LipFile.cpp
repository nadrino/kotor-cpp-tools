//
// Created by Adrien Blanchet on 10/12/2023.
//

#include "LipFile.h"

#include "Logger.h"

LoggerInit([]{
  Logger::getUserHeader() << "[LipFile]";
});


void LipFile::load(){
  LogThrowIf(_filePath_.empty(), "Can't load file: file path not set.");

  std::ifstream fStream(_filePath_, std::ios::binary);

  LogThrowIf(not fStream.is_open(), "Could not open file: " << _filePath_);

  // https://kotor-modding.fandom.com/wiki/LIP_Format

  // Header

  fillData(fStream, _content_.fileType, 4);
  fillData(fStream, _content_.fileVersion, 4);
  fillData(fStream, _content_.duration);
  fillData(fStream, _content_.entryCount);

}
