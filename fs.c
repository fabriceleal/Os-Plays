/**
 * @file fs_node.c
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

#include "fs_node.h"

fs_node_t *fs_root = 0; // The root of the filesystem.

u32int read_fs(fs_node_t *node, u32int offset, u32int size, u8int *buffer)
{
	// Has the node got a read callback?
	if (node->read != 0)
	{
		return node->read(node, offset, size, buffer);
	}
	else
	{
		return 0;
	}
}

u32int write_fs(fs_node_t *node, u32int offset, u32int size, u8int *buffer)
{
	// Has the node got a write callback?
	if (node->write != 0)
	{
		return node->write(node, offset, size, buffer);
	}
	else
	{
		return 0;
	}
}

void open_fs(fs_node_t *node, u8int read, u8int write)
{
	// Has the node got a open callback?
	if (node->open != 0)
	{
		return node->open(node, read, write);
	}
	else
	{
		return 0;
	}
}

void close_fs(fs_node_t *node)
{
	// Has the node got a close callback?
	if (node->close != 0)
	{
		return node->close(node);
	}
	else
	{
		return 0;
	}
}

struct dirent *readdir_fs(fs_node_t *node, u32int index)
{
	// Has the node got a readdir callback?
	if ((node->flags&0x7) == FS_DIRECTORY && node->readdir != 0 ) 
	{
		return node->readdir(node, index);
	}
	else
	{
		return 0;
	}
}

fs_node_t *finddir_fs(fs_node_t *node, char *name)
{
	// Has the node got a finddir callback?
	if ((node->flags&0x7) == FS_DIRECTORY && node->finddir != 0 ) 
	{
		return node->finddir(node, name);
	}
	else
	{
		return 0;
	}
}

