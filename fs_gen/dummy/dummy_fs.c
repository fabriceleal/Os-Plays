/**
 * @file dummy_fs.c
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

#include "dummy_fs.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "dirent.h"



int dir_count(const char * directory)
{
	// Count sub-directories in a given directory
	int ret = 0;
	DIR *dir = opendir(directory);
	while( readdir(dir) != NULL && ++ret);
	close(dir);
	LOG( "Counted %d dirs for directory %s\n", ret, directory );
	return ret;
}



#define WRKDIR_SUCCESS 0

int work_directory(char * dir_name, dummy_dir * dir_st, char * root_name)
{
	struct dirent *dp = NULL;
	
	// List subdirs and files	
	// Get count; alocate accordingly
	int dir_c = dir_count( dir_name );

	LOG("[enter]Ptr is %x\n", dir_st);

	dir_st->subdirs_count = dir_c;

	if( dir_c == 0 )
	{
		dir_st->subdirs = NULL;
	}
	else
	{
		dir_st->subdirs = malloc(sizeof(dummy_dir) * dir_c);

		DIR *dir = opendir(dir_name);
		int i = 0;
		while( (dp = readdir(dir)) != NULL )
		{
			// Allocate and call recursively (for each dir)
			dummy_dir * new_child = dir_st->subdirs + i++;

			LOG("Ptr is %x\n" , new_child);
			LOG("Working directory %s\n", dp->d_name);

			// Init name here, its easier than decomposing the full path in the recursive call.
			init( &new_child->name );
			write( &new_child->name, dp->d_name, strlen(dp->d_name)+1 );
	
			LOG( "Cat'ed dir is = %s\n", sprintf("%s%s", dir_name , dp->d_name ) );
			work_directory( sprintf("%s%s", dir_name , dp->d_name ), new_child, root_name );		
		}
		closedir(dir);
	}	
	return WRKDIR_SUCCESS;
}



#define WRKFL_SUCCESS 0

int work_file(char * file_name, dummy_file * file, buffer * final_fs)
{
	// TODO: Read file content
	
	// TODO: Update dummy_file structure

	// TODO: output into final_fs

	return WRKFL_SUCCESS;
}

int free_dir(dummy_dir * dir)
{
	// TODO: Destroy all buffers of children first (call recursively for dirs)

	// TODO: Frees itself

	return 0;
}



#define GEN_FS_SUCCESS 0
#define GEN_FS_MEM 1

/*
	Generates a dummy fs using a *real* directory as a *model*
*/
int generate_dummy_fs(char* model_root_dir, char * destiny_file )
{
	buffer * buf = malloc(sizeof(buffer));
	if(buf == NULL)
	{
		return GEN_FS_MEM;
	}
	
	dummy_dir * fs = malloc(sizeof(dummy_dir)); // fs is the root dir

	init( &fs->name );
	write( &fs->name, "", 1); // Empty string {'\0'}

	// Walk through dirs, build dir and file structure
	work_directory( model_root_dir , fs, model_root_dir );

	// TODO: Write into buffer


	// TODO: Walk through all files in fs, read content and output to buffer


	// TODO: Write buffer to destiny file ...	


	// Free memory
	free_dir(fs); free(fs);
	free(buf);

	return GEN_FS_SUCCESS ;
}

