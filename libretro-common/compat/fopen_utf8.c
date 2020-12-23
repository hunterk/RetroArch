/* Copyright  (C) 2010-2020 The RetroArch team
 *
 * ---------------------------------------------------------------------------------------
 * The following license statement only applies to this file (fopen_utf8.c).
 * ---------------------------------------------------------------------------------------
 *
 * Permission is hereby granted, free of charge,
 * to any person obtaining a copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include <compat/fopen_utf8.h>
#include <retro_miscellaneous.h>
#include <encodings/utf.h>
#include <stdio.h>
#include <stdlib.h>

#if defined(_WIN32_WINNT) && _WIN32_WINNT < 0x0500 || defined(_XBOX)
#ifndef LEGACY_WIN32
#define LEGACY_WIN32
#endif
#endif

#ifdef _WIN32
#undef fopen

void *fopen_utf8(const char * filename, const char * mode)
{
   const char * filename_local = NULL;
   const char* windows_long_prefix = "\\\\?\\";
   char long_filename[PATH_MAX_LENGTH];

#if defined(LEGACY_WIN32)
   FILE             *ret = NULL;
   filename_local = utf8_to_local_string_alloc(filename);

   if (!filename_local)
      return NULL;
   ret = fopen(filename_local, mode);
   if (filename_local)
      free(filename_local);
   return ret;
#else
#ifdef _WIN32
   /*
      prefix to tell Windows to bypass the ~260 characters limit in many I/O APIs
      https://docs.microsoft.com/en-us/windows/win32/fileio/naming-a-file
   */
   snprintf(long_filename, PATH_MAX_LENGTH, "%s%s", windows_long_prefix, filename);
   filename_local = long_filename;
#else
   filename_local = filename;
#endif
   wchar_t * filename_w = utf8_to_utf16_string_alloc(filename_local);
   wchar_t * mode_w = utf8_to_utf16_string_alloc(mode);
   FILE* ret = _wfopen(filename_w, mode_w);
   free(filename_w);
   free(mode_w);
   return ret;
#endif
}
#endif
