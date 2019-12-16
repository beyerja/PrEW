#include <Input/XMLReader.h>
#include "spdlog/spdlog.h"

#include <string>

namespace PREW {
namespace Input {

//------------------------------------------------------------------------------
// Constructors

XMLReader::XMLReader(std::string steering_path, std::string process_dir) :
  m_steering_path(steering_path)
{
  m_doc.LoadFile(m_steering_path.c_str());
 
  // If xml file was found & readable read it
  if (m_doc.ErrorID() == 0) {
    this->read_steering();
  } else {
    spdlog::warn("In XMLReader::XMLReader: File could not be opened: {}", m_steering_path);
  }
}

//------------------------------------------------------------------------------
// xml file reading and interpretation

void XMLReader::read_steering() {
  /** Read the main steering file.
      => Find the setups and interpret them.
  **/
  // TODO Develop structure into which this is read
  // TODO Test that test xml file is actually read correctly
  
  // TODO Fix hardcoded names somewhere else and use here as variables => Maybe in namespace???
  tinyxml2::XMLElement *e_setup = m_doc.FirstChildElement("Setup"); 
  
  // Check that any setup exists at all
  if (e_setup == NULL) {  
    spdlog::warn("In XMLReader::read_steering: No Setup found in file: {}", m_steering_path);
    return;
  }
  
  for ( /** **/ ; e_setup != nullptr; e_setup = e_setup->NextSiblingElement("Setup") ) {
    std::string name;
    name = e_setup->Attribute("Name");
    
    tinyxml2::XMLElement *e_energyrun = e_setup->FirstChildElement("EnergyRun");
    
    if (e_energyrun == NULL) {  
      spdlog::warn("In XMLReader::read_steering: No EnergyRun for setup, skipping it. In file: {}", m_steering_path);
      continue;
    }
    
    for ( /** **/ ; e_energyrun != nullptr; e_energyrun = e_energyrun->NextSiblingElement("EnergyRun") ) {
      int energy, luminosity;
      e_energyrun->QueryIntAttribute("Energy", &energy);
      e_energyrun->QueryIntAttribute("Luminosity", &luminosity);
      
      tinyxml2::XMLElement *e_beampolarizations = e_energyrun->FirstChildElement("BeamPolarizations");
      tinyxml2::XMLElement *e_polarisationsetups = e_energyrun->FirstChildElement("PolarisationSetups");
      tinyxml2::XMLElement *e_processes = e_energyrun->FirstChildElement("Processes");
      
      if (e_beampolarizations == NULL) {  
        spdlog::warn("In XMLReader::read_steering: No BeamPolarizations for EnergyRun, skipping it. In file: {}", m_steering_path);
        continue;
      } else if (e_polarisationsetups == NULL) {
        spdlog::warn("In XMLReader::read_steering: No PolarisationSetups for EnergyRun, skipping it. In file: {}", m_steering_path);
        continue;
      } else if (e_processes == NULL) {
        spdlog::warn("In XMLReader::read_steering: No Processes for EnergyRun, skipping it. In file: {}", m_steering_path);
        continue;
      }
      
      double e_pol, p_pol;
      e_beampolarizations->QueryDoubleAttribute("Electrons", &e_pol);
      e_beampolarizations->QueryDoubleAttribute("Positrons", &p_pol);
      
      tinyxml2::XMLElement *e_polarisationrun = e_polarisationsetups->FirstChildElement("PolarisationRun");
      tinyxml2::XMLElement *e_process = e_processes->FirstChildElement("Process");
      
      if (e_polarisationrun == NULL) {
        spdlog::warn("In XMLReader::read_steering: No PolarisationRun for PolarisationSetups, skipping it. In file: {}", m_steering_path);
        continue;
      } else if (e_process == NULL) {
        spdlog::warn("In XMLReader::read_steering: No Process for Processes, skipping it. In file: {}", m_steering_path);
        continue;
      }
      
      for ( /** **/ ; e_polarisationrun != nullptr; e_polarisationrun = e_polarisationrun->NextSiblingElement("PolarisationRun") ) {

        std::string name, e_pol_sign, p_pol_sign;
        double lumi_fraction;
        name = e_polarisationrun->Attribute("Name");
        e_pol_sign = e_polarisationrun->Attribute("ePol");
        p_pol_sign = e_polarisationrun->Attribute("pPol");
        e_polarisationrun->QueryDoubleAttribute("LumiFraction", &lumi_fraction);
      }
      
      for ( /** **/ ; e_process != nullptr; e_process = e_process->NextSiblingElement("Process") ) {
        std::string name;
        name = e_process->Attribute("Name");
        // TODO Trigger process file reading
      }
    }
  }
}

//------------------------------------------------------------------------------

}
}