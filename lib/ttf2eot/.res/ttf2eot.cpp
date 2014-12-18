/* Trivial utility to create EOT files on Linux */
 
#include <stdio.h> 

#include <vector>

#ifndef _MSC_VER
# include <stdint.h>
#else
typedef unsigned char uint8_t;
#endif

 

#include "OpenTypeUtilities.h"

#ifndef SIZE_MAX
# define SIZE_MAX UINT_MAX
#endif

using std::vector;
 
extern "C" __declspec(dllexport) int __cdecl ttf2eot( unsigned char *fontData, size_t fontSize,const char * outputPath)  
{
    const size_t kFontInitSize = 8192;
    vector<uint8_t> eotHeader(512);
    size_t overlayDst = 0;
    size_t overlaySrc = 0;
    size_t overlayLength = 0;  

	FILE *output = fopen(outputPath, "w+b");
	if (output == NULL) { 
		return 0;
	}
  
    if (getEOTHeader(fontData, fontSize, eotHeader, overlayDst, overlaySrc, overlayLength)) {
        fwrite(&eotHeader[0], eotHeader.size(), 1, output);
        fwrite(fontData, fontSize, 1, output);
		fclose(output);
        return 1;
    }  
	return 0;
}
 