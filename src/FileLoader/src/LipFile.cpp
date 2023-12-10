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


}
