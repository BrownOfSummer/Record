/*************************************************************************
    > File Name: usefulutils.cpp
    > Author: li_pengju
    > Mail: 
    > Copyright: All Rights Reserved
    > Created Time: 2018-07-12 10:23:57
 ************************************************************************/

#include<iostream>
#include<sstream> //ostringstream
#include<vector>
#include<string>
#include<stdio.h>
#include<stdarg.h>
#include<dirent.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>

// All modern terminal emulators use ANSI escape codes to show colours and other things.
#define COLOR_RED     "\x1b[31m"
#define COLOR_GREEN   "\x1b[32m"
#define COLOR_YELLOW  "\x1b[33m"
#define COLOR_BLUE    "\x1b[34m"
#define COLOR_MAGENTA "\x1b[35m"
#define COLOR_CYAN    "\x1b[36m"
#define COLOR_RESET   "\x1b[0m"
using namespace std;

void vlog(const char *format,...);
void vlog(const char *format,...)
{
#if DEBUG
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
#endif
}
/*simaliar with vlog*/
void c_printf(const char* col, const char* fmt, ...) {
	va_list ap;
	va_start(ap, fmt);
	printf("%s", col);
	vprintf(fmt, ap);
	//printf(COLOR_RESET);
	va_end(ap);
}
/*fprintf, simaliar with vlog*/
void c_fprintf(const char* col, FILE* fp, const char* fmt, ...) {
	va_list ap;
	va_start(ap, fmt);
	fprintf(fp, "%s", col);
	vfprintf(fp, fmt, ap);
	//fprintf(fp, COLOR_RESET);
	va_end(ap);
}
void error_exit(const char *msg) {
	c_fprintf(COLOR_RED, stderr, "error: %s\n", msg);
	exit(1);
}
// generate color str
std::string TERM_COLOR(int k) {
	// k = 0 ~ 4
	std::ostringstream ss;
	ss << "\x1b[3" << k + 2 << "m";
	return ss.str();
}
void print_color()
{
      printf(COLOR_RED     "This text is RED!"     COLOR_RESET "\n");
      printf(COLOR_GREEN   "This text is GREEN!"   COLOR_RESET "\n");
      printf(COLOR_YELLOW  "This text is YELLOW!"  COLOR_RESET "\n");
      printf(COLOR_BLUE    "This text is BLUE!"    COLOR_RESET "\n");
      printf(COLOR_MAGENTA "This text is MAGENTA!" COLOR_RESET "\n");
      printf(COLOR_CYAN    "This text is CYAN!"    COLOR_RESET "\n");
}
/*similar to sprintf*/
std::string ssprintf(const char *fmt, ...) {
	int size = 100;
	char *p = (char *)malloc(size);

	va_list ap;

	std::string ret;

	while (true) {
		va_start(ap, fmt);
		int n = vsnprintf(p, size, fmt, ap);
		va_end(ap);

		if (n < 0) {
			free(p);
			return "";
		}

		if (n < size) {
			ret = p;
			free(p);
			return ret;
		}

		size = n + 1;

		char *np = (char *)realloc(p, size);
		if (np == nullptr) {
			free(p);
			return "";
		} else
			p = np;
	}
}

inline bool exists_file(const char* name) {
	struct stat buffer;
	return stat(name, &buffer) == 0;
}

inline bool endswith(const char* str, const char* suffix) {
	if (!str || !suffix) return false;
	//auto l1 = strlen(str), l2 = strlen(suffix);
	long l1 = strlen(str), l2 = strlen(suffix);
	if (l2 > l1) return false;
	return strncmp(str + l1 - l2, suffix, l2) == 0;
}
/*come from CImg.h*/
/*****************************************/
//! Convert ascii character to lower case.
inline char uncase(const char x) {
  return (char)((x<'A'||x>'Z')?x:x - 'A' + 'a');
}

//! Convert C-string to lower case.
inline void uncase(char *const str) {
  if (str) for (char *ptr = str; *ptr; ++ptr) *ptr = uncase(*ptr);
}
//! Check if a path is a directory.
/**
   \param path Specified path to test.
**/
inline bool is_directory(const char *const path) {
  if (!path || !*path) return false;
//#if cimg_OS==1
  struct stat st_buf;
  return (!stat(path,&st_buf) && S_ISDIR(st_buf.st_mode));
//#elif cimg_OS==2 windows
  //const unsigned int res = (unsigned int)GetFileAttributesA(path);
  //return res==INVALID_FILE_ATTRIBUTES?false:(res&16);
//#endif
}
//! Check if a path is a file.
/**
   \param path Specified path to test.
**/
inline bool is_file(const char *const path) {
  if (!path || !*path) return false;
  std::FILE *const file = std::fopen(path,"rb");
  if (!file) return false;
  std::fclose(file);
  return !is_directory(path);
}
//! Split filename into two C-strings \c body and \c extension.
/**
   filename and body must not overlap!
**/
inline const char *split_filename(const char *const filename, char *const body=0) {
  if (!filename) { if (body) *body = 0; return 0; }
  const char *p = 0; for (const char *np = filename; np>=filename && (p=np); np = std::strchr(np,'.') + 1) {}
  if (p==filename) {
    if (body) std::strcpy(body,filename);
    return filename + std::strlen(filename);
  }
  const unsigned int l = (unsigned int)(p - filename - 1);
  if (body) { if (l) std::memcpy(body,filename,l); body[l] = 0; }
  return p;
}
//! Return the basename of a filename.
inline const char* basename(const char *const s, const char separator='/')  {
  const char *p = 0, *np = s;
  while (np>=s && (p=np)) np = std::strchr(np,separator) + 1;
  return p;
}
/*****************************************/
vector<string> listDir( const char* path, const char *ext );
vector<string> listDir( const char* path, const char *ext )
{
    DIR* dirFile = opendir( path );
    vector<string> ret;
    if ( dirFile ) 
    {
        struct dirent* hFile;
        //errno = 0;
        while (( hFile = readdir( dirFile )) != NULL ) 
        {
            // skip . and ..
            if ( !strcmp( hFile->d_name, "."  )) continue;
            if ( !strcmp( hFile->d_name, ".." )) continue;

            // in linux hidden files all start with '.', skip it
            if (hFile->d_name[0] == '.' ) continue;

            // dirFile.name is the name of the file. Do whatever string comparison 
            // you want here. Something like:
            if(ext != NULL)
            {
                if ( strstr( hFile->d_name, ext ))
                    ret.push_back( hFile->d_name );
                //if( endswith(hFile->d_name, ext) )
                //    ret.push_back( hFile->d_name );
            }
            else
                ret.push_back( hFile->d_name );
      } 
      closedir( dirFile );
   }
    return ret;
}

/*check and mkdir*/
void check_dir(const char *dirpath)
{
    struct stat st = {0};

    if (stat(dirpath, &st) == -1)
    {
        printf("mkdir %s\n", dirpath);
        mkdir(dirpath, 0777);
    }
}
/* decode hh:mm:ss to seconds*/
bool str2secs(string time_str, double *secs)
{
    int h = 0, m = 0;
    double s = 0;
    *secs = 0;
    int ret = sscanf(time_str.c_str(), "%d:%d:%lf", &h, &m, &s);
    if(ret == 3)
    {
        if(h < 0 || m >= 60 || m < 0 || s >= 60 || s < 0)
        {
            printf("Time string invalid !\n");
            return false;
        }
        *secs = h * 3600 + m * 60 + s;
    }
    else
    {
        printf("Time string invalide.\n");
        return false;
    }
    return true;
}
