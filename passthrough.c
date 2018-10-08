#define FUSE_USE_VERSION 30

#include <fuse.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
static int do_getattr( const char *path, struct stat *st )
{
	printf( "Getattr function Called for: %s\n",path);
	st->st_uid = getuid(); // The owner of the file/directory is the user who mounted the filesystem
	st->st_gid = getgid(); // The group of the file/directory is the same as the group of the user who mounted the filesystem
	st->st_atime = time( NULL ); // The last access of the file/directory is right now
	st->st_mtime = time( NULL ); // The last modification of the file/directory is right now
	
	if ( strcmp( path, "/" ) == 0 )
	{
		st->st_mode = S_IFDIR | 0777;
		st->st_nlink = 2; 
	}
	else
	{
		st->st_mode = S_IFREG | 0777;
		st->st_nlink = 1;
		st->st_size = 4096;
	}
	
	return 0;
}


static int do_readdir( const char *path, void *buffer, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi )
{
	printf( "\nReaddir function called for :%s\n", path );
	
	filler( buffer, ".", NULL, 0 ); // Current Directory
	filler( buffer, "..", NULL, 0 ); // Parent Directory
	
	if ( strcmp( path, "/" ) == 0 ) // If the user is trying to show the files/directories of the root directory show the following
	{
		filler( buffer, "Shashank", NULL, 0 );
		filler( buffer, "Shailesh", NULL, 0 );
		filler( buffer, "Satyam", NULL, 0 );
		filler( buffer, "Sankarshana", NULL, 0 );
		filler(buffer,"dummy",NULL,0);
	}
	
	return 0;
}

static int do_read( const char *path, char *buffer, size_t size, off_t offset, struct fuse_file_info *fi )
{
	printf( "--> Trying to read %s, %u, %u\n", path, offset, size );
	
	char ShashankText[] = "My name is Shashank Bongale";
	char SankarshanaText[] = "My name is Sankarshana Rao";
	char ShaileshText[] = "My name is Shailesh Sridhar";
	char SatyamText[] = "My name is Satyam Shivam Sundaram";

	char *selectedText = NULL;
	
	// ... //
	
	if ( strcmp( path, "/Shashank" ) == 0 )
		selectedText = ShashankText;
	else if ( strcmp( path, "/Sankarshana" ) == 0 )
		selectedText = SankarshanaText;
	if ( strcmp( path, "/Shailesh" ) == 0 )
		selectedText = ShaileshText;
	else if ( strcmp( path, "/Satyam" ) == 0 )
		selectedText = SatyamText;

	else
		return -1;
	
	// ... //
	
	memcpy( buffer, selectedText + offset, size );
		
	return strlen( selectedText ) - offset;
}
static int do_open(const char *path, struct fuse_file_info *fi) {
  printf("hello");
  return 0;
}

static struct fuse_operations operations = {
    .getattr	= do_getattr,
    .readdir	= do_readdir,
    .read		= do_read,
    .open = do_open,
};

int main( int argc, char *argv[] )
{
	return fuse_main( argc, argv, &operations, NULL );
}