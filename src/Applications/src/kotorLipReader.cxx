//
// Created by Adrien Blanchet on 10/12/2023.
//

#include "LipFile.h"

#include "GenericToolbox.h"
#include "CmdLineParser.h"
#include "Logger.h"

LoggerInit([]{
  Logger::getUserHeader() << "[" << FILENAME << "]";
});

int main(int argc, char** argv){

  // --------------------------
  // Read Command Line Args:
  // --------------------------
  CmdLineParser clParser;

  clParser.addDummyOption("Main options:");
  clParser.addOption("filePath", {"-f", "--file-path"}, "Path to .lip file");

  LogInfo << "Usage: " << std::endl;
  LogInfo << clParser.getConfigSummary() << std::endl << std::endl;

  clParser.parseCmdLine(argc, argv);

  LogThrowIf(clParser.isNoOptionTriggered(), "No option was provided.");

  LogInfo << "Provided arguments: " << std::endl;
  LogInfo << clParser.getValueSummary() << std::endl << std::endl;

  // Sanity checks
  LogThrowIf(not clParser.isOptionTriggered("filePath"), "No lip file provided.");

  LipFile lipFile;
  lipFile.setFilePath( clParser.getOptionVal<std::string>("filePath") );
  lipFile.load();

  LogDebug << lipFile.getContent().getSummary() << std::endl;

}
