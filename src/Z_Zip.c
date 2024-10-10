/* Z_Zone.c */

//-----------------------------------------------------------------------------
//
// DESCRIPTION:
//      Zone Memory Allocation. Neat.
//
// Based on the doom64 Ex code by Samuel Villarreal
// https://github.com/svkaiser/Doom64EX/blob/master/src/engine/zone/z_zone.cc
//-----------------------------------------------------------------------------

#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <zlib.h>

#include "DoomRPG.h"
#include "Z_Zip.h"

zip_file_t zipFile;

static void* zip_alloc(void* ctx, unsigned int items, unsigned int size)
{
	return SDL_malloc(items * size);
}

static void zip_free(void* ctx, void* ptr)
{
	SDL_free(ptr);
}

void findAndReadZipDir(zip_file_t* zipFile, int startoffset)
{
    printf("ZIP file handling disabled, reading from extracted files.\n");
    // Initialize zipFile with dummy values to prevent null pointer issues
    zipFile->entry_count = 0;
    zipFile->entry = NULL;
}

void openZipFile(const char* name, zip_file_t* zipFile)
{
	byte buf[512];
	int filesize, back, maxback;
	int i, n;

	zipFile->file = SDL_RWFromFile(name, "r");
	if (zipFile->file == NULL) {
		DoomRPG_Error("openZipFile: cannot open file %s\n", name);
	}

	filesize = (int)SDL_RWsize(zipFile->file);

	maxback = MIN(filesize, 0xFFFF + sizeof(buf));
	back = MIN(maxback, sizeof(buf));

	while (back < maxback)
	{
		SDL_RWseek(zipFile->file, filesize - back, SEEK_SET);
		n = sizeof(buf);
		SDL_RWread(zipFile->file, buf, sizeof(byte), sizeof(buf));
		for (i = n - 4; i > 0; i--)
		{
			if (!SDL_memcmp(buf + i, "PK\5\6", 4)) {
				findAndReadZipDir(zipFile, filesize - back + i);
				return;
			}
		}
		back += sizeof(buf) - 4;
	}

	DoomRPG_Error("cannot find end of central directory\n");
}

void closeZipFile(zip_file_t* zipFile)
{
	if (zipFile->entry) {
		SDL_free(zipFile->entry);
	}
	if (zipFile->file) {
		SDL_RWclose(zipFile->file);
	}
}

unsigned char* readZipFileEntry(const char* name, zip_file_t* zipFile, int* sizep)
{
    char filepath[256];
    snprintf(filepath, sizeof(filepath), "extracted/%s", name);
    
    SDL_RWops* file = SDL_RWFromFile(filepath, "rb");
    if (!file) {
        DoomRPG_Error("Could not open file: %s", filepath);
        return NULL;
    }

    int fileSize = SDL_RWsize(file);
    unsigned char* data = SDL_malloc(fileSize);
    
    if (SDL_RWread(file, data, 1, fileSize) != fileSize) {
        SDL_free(data);
        SDL_RWclose(file);
        DoomRPG_Error("Failed to read file: %s", filepath);
        return NULL;
    }

    SDL_RWclose(file);
    *sizep = fileSize;
    return data;
}
