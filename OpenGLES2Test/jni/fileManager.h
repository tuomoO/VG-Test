
#pragma once

#include <string>
#include <vector>
#include "../native_app_glue/android_native_app_glue.h"

namespace vg
{
    namespace sound
    {
        struct SoundEffectData;
    }
    namespace core
    {
		/**
		Manages the reading and writing of files
		*/
        class FileManager
        {
        public:

			#if defined (OS_ANDROID)

			/**
			Used for checking if the path is internal or external
			*/
			enum DataPath
			{
				Internal,
				External
			};

			#else
			enum DataPath
			{
				File
			};
			#endif

            /**
                Filemanagers constructor
                @param app Android application
                */
			FileManager(android_app* app);

            ~FileManager() = default;

            /**
                Reads an asset and gives out string file for the read data
                @param path Filepath for the asset we are reading
                @param outData Data in which the read file will be stored in(string)
                @return Returns true if reading is succesful
                */
            bool readAsset(const std::string& path, std::string& outData);

            /**
                Reads an asset and gives out unsigned char file for the read data
                @param path Filepath for the asset we are reading
                @param outData Data in which the read file will be stored in(vector<unsigned char>)
                @return Returns true if reading is succesful
                */
            bool readAsset(const std::string& path, std::vector<unsigned char>& outData);
		
        //    /**
        //        Reads an asset and gives out sound data
        //        @param path Filepath for the asset we are reading
        //        @param soundoutDat Sound effect data in which the read file will be stored in
        //        @return Returns true if reading is succesful
        //        */
        //    bool readAsset(const std::string& path, vg::sound::SoundEffectData* soundOutDat);

        //    /**
        //        Reads a file and gives out string file for the read file
        //        @param dataPath Used to choose if the path Internal or External
        //        @param path Filepath for the file we are reading
        //        @param outData Data in which the read file will be stored in
        //        @return Returns true if reading is succesful
        //        */
        //    bool readFile(DataPath dataPath, const std::string& path, std::string& outData);

        //    /**
        //        Writes in a file the data given
        //        @param dataPath Used to choose if the path Internal or External
        //        @param path Filepath for the file we are writing in
        //        @param inData The data which will be written in the file
        //        @return Returns true if reading is succesful
        //        */
        //    bool writeFile(DataPath dataPath, const std::string& path, const std::string& inData) const;
        //private:
             };
    }
}
