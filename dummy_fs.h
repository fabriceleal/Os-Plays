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

typedef struct {
	buffer name;
	dummy_dir * subdirs;
	dummy_file * files;
} dummy_dir;

typedef struct {
	int offset;
	buffer name;
} dummy_file;



#endif // ___VAMI_DUMMY_FS
