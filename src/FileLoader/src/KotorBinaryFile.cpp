//
// Created by Nadrino on 11/12/2023.
//

#include "KotorBinaryFile.h"

#include "GenericToolbox.h"
#include "GenericToolbox.Json.h"
#include "Logger.h"

LoggerInit([]{
  Logger::getUserHeader() << "[KotorBinaryFile]";
});


void KotorBinaryFile::read(const std::string& filePath_){
  LogInfo << __METHOD_NAME__ << std::endl;
  if( GenericToolbox::hasExtension(filePath_, "json") ){
    auto json = GenericToolbox::Json::readConfigFile<nlohmann::ordered_json>( filePath_ );
    this->readJson( json );
  }
  else{
    LogThrowIf(filePath_.empty(), "Can't load file: file path not set.");
    std::ifstream file(filePath_, std::ios::binary);

    LogThrowIf(not file.is_open(), "Could not open file: " << filePath_);
    this->readBinary( file );
  }
}
void KotorBinaryFile::write(const std::string& filePath_){
  if( GenericToolbox::hasExtension(filePath_, "json") ){
    nlohmann::ordered_json jsonData;
    this->writeJson( jsonData );
    GenericToolbox::dumpStringInFile(
        filePath_, GenericToolbox::Json::toReadableString(jsonData)
    );
  }
  else{
    LogInfo << "Writing .lip file..." << std::endl;
    std::ofstream ofile(filePath_, std::ios::binary);
    this->writeBinary(ofile);
  }
}
