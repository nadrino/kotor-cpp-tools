//
// Created by Nadrino on 10/12/2023.
//

#include "LipSyncFile.h"
#include "VersionConfig.h"

#include "GenericToolbox.h"
#include "GenericToolbox.Json.h"
#include "CmdLineParser.h"
#include "Logger.h"

#include "string"

LoggerInit([]{
  Logger::getUserHeader() << "[" << FILENAME << "]";
});

int main(int argc, char** argv){

  LogInfo << FILENAME << " version: " << VersionConfig::getVersionStr() << std::endl;

  // --------------------------
  // Read Command Line Args:
  // --------------------------
  CmdLineParser clp;

  clp.addDummyOption("Main options:");
  clp.addOption("filePath", {"-f", "--file-path"}, "Path to .lip file");
  clp.addOption("output", {"-o"}, "Output file");
  clp.addOption("verbose", {"--verbose"}, "Set verbose printout level (default: 1)", 1, true);

  LogInfo << "Usage: " << std::endl;
  LogInfo << clp.getConfigSummary() << std::endl << std::endl;

  clp.parseCmdLine(argc, argv);

  LogThrowIf(clp.isNoOptionTriggered(), "No option was provided.");

  LogInfo << "Provided arguments: " << std::endl;
  LogInfo << clp.getValueSummary() << std::endl << std::endl;

  // Sanity checks
  LogThrowIf(not clp.isOptionTriggered("filePath"), "No lip file provided.");

  LipSyncFile lipFile{};

  LogInfo << "Reading file: " << clp.getOptionVal<std::string>("filePath") << std::endl;
  lipFile.read( clp.getOptionVal<std::string>("filePath") );

  if( clp.isOptionTriggered("verbose") ){
    LogDebug << lipFile.getSummary() << std::endl;
  }

  if( clp.isOptionTriggered("output") ){
    auto outPath{clp.getOptionVal<std::string>("output")};
    LogWarning << "Output file path: " << outPath << std::endl;
    lipFile.write( clp.getOptionVal<std::string>("output") );
  }

}
