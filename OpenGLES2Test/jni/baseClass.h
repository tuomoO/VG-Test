
#pragma once

#include <string>
#include <map>
#include <iostream>

namespace vg
{
	/**
	Class used for detecting memory leaks
	*/
	class BaseClass
	{
	public:
		static std::map<std::string, int> memLeaks; ///< Map with the name of the class the memory leak is in and by how much
		std::string className; ///< For indicating what class the memory leaks are in
		BaseClass(std::string name);
		~BaseClass();

		/**
		@return Returns the amount of memory leaks, used for printing out the amount of leaks
		*/
		static int getValue(std::string name);
	};
}