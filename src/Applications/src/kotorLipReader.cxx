//
// Created by Adrien Blanchet on 10/12/2023.
//

#include "LipFile.h"
#include "VersionConfig.h"

#include "GenericToolbox.h"
#include "CmdLineParser.h"
#include "Logger.h"

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

  LogInfo << "Usage: " << std::endl;
  LogInfo << clp.getConfigSummary() << std::endl << std::endl;

  clp.parseCmdLine(argc, argv);

  LogThrowIf(clp.isNoOptionTriggered(), "No option was provided.");

  LogInfo << "Provided arguments: " << std::endl;
  LogInfo << clp.getValueSummary() << std::endl << std::endl;

  // Sanity checks
  LogThrowIf(not clp.isOptionTriggered("filePath"), "No lip file provided.");

  LipFile lipFile;
  lipFile.setFilePath( clp.getOptionVal<std::string>("filePath") );
  lipFile.load();

  LogDebug << lipFile.getSummary() << std::endl;

  if( clp.isOptionTriggered("output") ){
    LogInfo << "Writing: " << clp.getOptionVal<std::string>("output") << std::endl;
    std::ofstream ofile(clp.getOptionVal<std::string>("output"), std::ios::binary);
    lipFile.getContent().write(ofile);
  }

}
