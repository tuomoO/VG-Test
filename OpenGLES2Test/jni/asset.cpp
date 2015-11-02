
#include "asset.h"
using namespace vg::core;

Asset::Asset(const std::string& path)
	: mPath(path)
{
}

bool Asset::isLoaded() const
{
	return mIsLoaded;
}
std::string Asset::getPath()
{
	return mPath;
}