// BinaryStreamFixture.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#ifdef __unix__
#define BINARY_STREAM_FIXTURE_MAIN 
#define BINARY_STREAM_FIXTURE_ARGV char
#elif defined(_WIN32) || defined(WIN32)
#include <SDKDDKVer.h>
#include <tchar.h>
#include <Windows.h>
#define BINARY_STREAM_FIXTURE_MAIN _tmain
#define BINARY_STREAM_FIXTURE_ARGV _TCHAR
#endif


int BINARY_STREAM_FIXTURE_MAIN(int argc, BINARY_STREAM_FIXTURE_ARGV* argv[])
{
	return 0;
}

