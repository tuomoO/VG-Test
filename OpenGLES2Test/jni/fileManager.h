
#pragma once

#include <string>
#include <vector>
#include "../native_app_glue/android_native_app_glue.h"

/**
Manages the reading and writing of files
*/
class FileManager
{
public:
	/**
	Used for checking if the path is internal or external
	*/
	enum DataPath
	{
		Internal,
		External
	};

	FileManager(android_app *app);
	bool readAsset(const std::string& path, std::string& outData);
	bool readAsset(const std::string& path, std::vector<unsigned char>& outData);

};
