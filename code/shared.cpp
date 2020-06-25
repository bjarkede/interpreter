#include "shared.hpp"

#include <malloc.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

File::File() : file_buffer(NULL) {};

File::~File() {
  if (file_buffer != NULL)
    fclose((FILE*)file_buffer);
}

bool File::Open(const char* filePath, const char* mode) {
  file_buffer = fopen(filePath, mode);
  return file_buffer != NULL;
}

bool File::Close() {
  return fclose((FILE*)file_buffer);
}

bool File::IsValid() {
  return file_buffer != NULL;
}

bool File::Read(void* data, size_t bytes) {
  return fread(data, bytes, 1, (FILE*)file_buffer) == 1;
}

bool File::Write(const void* data, size_t bytes) {
  bool result = fwrite(data, sizeof(char), bytes, (FILE*)file_buffer) == 1;
  fflush((FILE*)file_buffer);
  return result;
}

bool AllocBuffer(Buffer& buffer, uptr bytes)
{
	buffer.buffer = malloc(bytes);
	if(!buffer.buffer)
	{
		return false;
	}

	buffer.length = bytes;
	return true;
}

bool ReadEntireFile(Buffer& buffer, const char* filePath) {
	FILE* file = fopen(filePath, "rb");
	if(!file)
	{
		return false;
	}

	fseek(file, 0, SEEK_END);
	const uptr bufferLength = (uptr)ftell(file);
	if(!AllocBuffer(buffer, bufferLength + 1))
	{
		fclose(file);
		return false;
	}

	fseek(file, 0, SEEK_SET);
	if(fread(buffer.buffer, (size_t)bufferLength, 1, file) != 1)
	{
		fclose(file);
		return false;
	}

	fclose(file);
	((char*)buffer.buffer)[bufferLength] = '\0';
	return true;
}
