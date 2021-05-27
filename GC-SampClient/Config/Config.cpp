#include "Config.h"
#include "simpleini.h"
#include <filesystem>

namespace fs = std::filesystem;

bool Config::Load(std::string fileName)
{
	if (fs::exists(fileName) == false)
	{
		CSimpleIniA ini;
		SI_Error rc;
		rc = ini.SetBoolValue("", "useHostName", false);
		rc = ini.SetValue("", "address", "127.0.0.1");
		rc = ini.SetLongValue("", "port", 9920);
		rc = ini.SetValue("", "username", "user1");
		rc = ini.SetValue("", "password", "12345678");

		if (rc < 0)
			return false;

		rc = ini.SaveFile(fileName.c_str());
		return false;
	}
	CSimpleIniA ini;
	SI_Error rc = ini.LoadFile(fileName.c_str());
	if (rc < 0)
		return false;

	Config::useHostName = ini.GetBoolValue("", "useHostName");
	Config::address = ini.GetValue("", "address");
	Config::port = static_cast<uint16_t>(ini.GetLongValue("", "port"));
	Config::username = ini.GetValue("", "username");
	Config::password = ini.GetValue("", "password");
	return true;
}