// FileSystemRedirector.cpp : Defines the entry point for the console application.
//

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>

#include <Shlobj.h>

int _tmain(int argc, _TCHAR* argv[])
{
  const int buffer_size = MAX_PATH + 1;
  TCHAR buffer[buffer_size];
  BOOL get_special_folder_path_result = SHGetSpecialFolderPath(
    NULL,                 // Reserved.
    buffer,               // A pointer to a null-terminated string that receives the drive and path of the specified folder.
    CSIDL_PROGRAM_FILES,  // A CSIDL that identifies the folder of interest.
    FALSE                 // Indicates whether the folder should be created if it does not already exist
    );


  return 0;
}

