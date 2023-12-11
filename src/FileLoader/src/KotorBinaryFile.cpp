//
// Created by Adrien Blanchet on 11/12/2023.
//

#include "KotorBinaryFile.h"

#include "GenericToolbox.h"
#include "GenericToolbox.Json.h"
#include "Logger.h"

LoggerInit([]{
  Logger::getUserHeader() << "[KotorBinaryFile]";
});


void KotorBinaryFile::read(const std::string& filePath_){
  LogDebug << __METHOD_NAME__ << std::endl;
  if( GenericToolbox::doesFilePathHasExtension(filePath_, "json") ){
    auto json = GenericToolbox::Json::readConfigFile( filePath_ );
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
  if( GenericToolbox::doesFilePathHasExtension(filePath_, "json") ){
    nlohmann::json jsonData;
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
