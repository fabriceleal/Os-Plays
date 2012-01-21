/**
 * @file dummy_fs.h
 * @author Fabrice Leal <fabrice [dot]leal[dot]ch[at]gmail[dot]com>
 *
 * @section LICENSE
 *
 * < ... goes here ... >
 *
 * @section DESCRIPTION
 * 
 * < ... goes here ... >
 *
 */


#ifndef ___VAMI_DUMMY_FS
#define ___VAMI_DUMMY_FS

#include "dummy_streaming.h"

typedef struct dummy_file {
	int offset;
	buffer name;
} dummy_file;


typedef struct dummy_dir {
	buffer name;

	struct dummy_dir* parent;

	int subdirs_count;
	struct dummy_dir* subdirs;

	int files_count;
	dummy_file* files;
} dummy_dir;

int generate_dummy_fs(char* model_root_dir, char * destiny_file );

#endif // ___VAMI_DUMMY_FS
