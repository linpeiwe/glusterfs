
#include "glusterfs.h"
#include "xlator.h"

const char *specfile;
const char *mount_options;

static int
glusterfs_getattr (const char *path,
		   struct stat *stbuf)
{
  struct xlator *xlator = fuse_get_context ()->private_data;
  int ret = xlator->fops->getattr (xlator, path, stbuf);
  return ret;
}

static int
glusterfs_readlink (const char *path,
		    char *dest,
		    size_t size)
{
  struct xlator *xlator = fuse_get_context ()->private_data;
  return xlator->fops->readlink (xlator, path, dest, size);
}

/*
static int
glusterfs_getdir (const char *path,
		  fuse_dirh_t dirh,
		  fuse_dirfil_t dirfil)
{
  int ret = 0;
  FUNCTION_CALLED;
  return ret;
}
*/

static int
glusterfs_mknod (const char *path,
		 mode_t mode,
		 dev_t dev)
{
  struct xlator *xlator = fuse_get_context ()->private_data;
  return xlator->fops->mknod (xlator, path, mode, dev, 
			    fuse_get_context ()->uid, 
			    fuse_get_context ()->gid);
}

static int
glusterfs_mkdir (const char *path,
		 mode_t mode)
{
  struct xlator *xlator = fuse_get_context ()->private_data;
  return xlator->fops->mkdir (xlator, path, mode,
			     fuse_get_context ()->uid,
			     fuse_get_context ()->gid);
}

static int
glusterfs_unlink (const char *path)
{
  struct xlator *xlator = fuse_get_context ()->private_data;
  return xlator->fops->unlink (xlator, path);
}

static int
glusterfs_rmdir (const char *path)
{
  struct xlator *xlator = fuse_get_context ()->private_data;
  return xlator->fops->rmdir (xlator, path);
}

static int
glusterfs_symlink (const char *oldpath,
		   const char *newpath)
{
  struct xlator *xlator = fuse_get_context ()->private_data;
  return xlator->fops->symlink (xlator, oldpath, newpath,
			       fuse_get_context ()->uid,
			       fuse_get_context ()->gid);
}

static int
glusterfs_rename (const char *oldpath,
		  const char *newpath)
{
  struct xlator *xlator = fuse_get_context ()->private_data;
  return xlator->fops->rename (xlator, oldpath, newpath,
			      fuse_get_context ()->uid,
			      fuse_get_context ()->gid);
}

static int
glusterfs_link (const char *oldpath,
		const char *newpath)
{
  struct xlator *xlator = fuse_get_context ()->private_data;
  return xlator->fops->link (xlator, oldpath, newpath,
			    fuse_get_context ()->uid,
			    fuse_get_context ()->gid);
}

static int
glusterfs_chmod (const char *path,
		 mode_t mode)
{
  struct xlator *xlator = fuse_get_context ()->private_data;
  return xlator->fops->chmod (xlator, path, mode);
}

static int
glusterfs_chown (const char *path,
		 uid_t uid,
		 gid_t gid)
{
  struct xlator *xlator = fuse_get_context ()->private_data;
  return xlator->fops->chown (xlator, path, uid, gid);
}

static int
glusterfs_truncate (const char *path,
		    off_t offset)
{
  struct xlator *xlator = fuse_get_context ()->private_data;
  return xlator->fops->truncate (xlator, path, offset);
}

static int
glusterfs_utime (const char *path,
		 struct utimbuf *buf)
{
  struct xlator *xlator = fuse_get_context ()->private_data;
  return xlator->fops->utime (xlator, path, buf);
}

static int
glusterfs_open (const char *path,
		struct fuse_file_info *info)
{
  int ret;
  struct xlator *xlator = fuse_get_context ()->private_data;
  struct file_context *cxt = (void *) calloc (1, sizeof (*cxt));

  ret = xlator->fops->open (xlator, path, info->flags, cxt);

  if (ret < 0)
    free (cxt);
  else
    info->fh = cxt;

  return ret;
}

static int
glusterfs_read (const char *path,
		char *buf,
		size_t size,
		off_t offset,
		struct fuse_file_info *info)
{
  struct xlator *xlator = fuse_get_context ()->private_data;
  return xlator->fops->read (xlator, path, buf, size, offset, (void *)info->fh);
}

static int
glusterfs_write (const char *path,
		 const char *buf,
		 size_t size,
		 off_t offset,
		 struct fuse_file_info *info)
{
  struct xlator *xlator = fuse_get_context ()->private_data;
  return xlator->fops->write (xlator, path, buf, size, offset, (void *)info->fh);
}

static int
glusterfs_statfs (const char *path,
		  struct statvfs *buf)
{
  struct xlator *xlator = fuse_get_context ()->private_data;
  return xlator->fops->statfs (xlator, path, buf);
}

static int
glusterfs_flush (const char *path,
		 struct fuse_file_info *info)
{
  struct xlator *xlator = fuse_get_context ()->private_data;
  return xlator->fops->flush (xlator, path, (void *)info->fh);
}

static int
glusterfs_release (const char *path,
		   struct fuse_file_info *info)
{
  struct xlator *xlator = fuse_get_context ()->private_data;
  int ret = xlator->fops->release (xlator, path, (void *) info->fh);

  free ((void *)info->fh);
  info->fh = 0;

  return ret;
}

static int
glusterfs_fsync (const char *path,
		 int datasync,
		 struct fuse_file_info *info)
{
  struct xlator *xlator = fuse_get_context ()->private_data;
  return xlator->fops->fsync (xlator, path, datasync, (void *)info->fh);
}

static int
glusterfs_setxattr (const char *path,
		    const char *name,
		    const char *value,
		    size_t size,
		    int flags)
{
  struct xlator *xlator = fuse_get_context ()->private_data;
  return xlator->fops->setxattr (xlator, path, name, value, size, flags);
}

static int
glusterfs_getxattr (const char *path,
		    const char *name,
		    char *value,
		    size_t size)
{
  struct xlator *xlator = fuse_get_context ()->private_data;
  return xlator->fops->getxattr (xlator, path, name, value, size);
}

static int
glusterfs_listxattr (const char *path,
		     char *list,
		     size_t size)
{
  struct xlator *xlator = fuse_get_context ()->private_data;
  return xlator->fops->listxattr (xlator, path, list, size);
}
		     
static int
glusterfs_removexattr (const char *path,
		       const char *name)
{
  struct xlator *xlator = fuse_get_context ()->private_data;
  return xlator->fops->removexattr (xlator, path, name);
}

static int
glusterfs_opendir (const char *path,
		   struct fuse_file_info *info)
{
  struct xlator *xlator = fuse_get_context ()->private_data;
  return xlator->fops->opendir (xlator, path, (void *)info->fh);
}

static int
glusterfs_releasedir (const char *path,
		      struct fuse_file_info *info)
{
  struct xlator *xlator = fuse_get_context ()->private_data;
  return xlator->fops->releasedir (xlator, path, (void *)info->fh);
}

static int
glusterfs_fsyncdir (const char *path,
		    int datasync,
		    struct fuse_file_info *info)
{
  struct xlator *xlator = fuse_get_context ()->private_data;
  return xlator->fops->fsyncdir (xlator, path, datasync, (void *)info->fh);
}

static int
glusterfs_access (const char *path,
		  int mode)
{
  struct xlator *xlator = fuse_get_context ()->private_data;
  return xlator->fops->access (xlator, path, mode);
}

static int
glusterfs_create (const char *path,
		  mode_t mode,
		  struct fuse_file_info *info)
{
  struct xlator *xlator = fuse_get_context ()->private_data;
  return xlator->fops->create (xlator, path, mode, (void *)info->fh);
}

static int
glusterfs_ftruncate (const char *path,
		     off_t offset,
		     struct fuse_file_info *info)
{
  struct xlator *xlator = fuse_get_context ()->private_data;
  return xlator->fops->ftruncate (xlator, path, offset, (void *)info->fh);
}

static int
glusterfs_fgetattr (const char *path,
		    struct stat *buf,
		    struct fuse_file_info *info)
{
  struct xlator *xlator = fuse_get_context ()->private_data;
  return xlator->fops->fgetattr (xlator, path, buf, (void *)info->fh);
}

static int
glusterfs_readdir (const char *path,
		   void *buf,
		   fuse_fill_dir_t fill,
		   off_t offset,
		   struct fuse_file_info *info)
{
  struct xlator *xlator = fuse_get_context ()->private_data;
  char *ret = xlator->fops->readdir (xlator, path, offset);
  {
    int i = 0; 
    char tmp[256] = {0,};
    while (1) {
      int j = 0;
      while (ret[i] != '/') {
	tmp[j] = ret[i];
	i++; j++;
      }
      tmp[j] = '\0';
      i++;
      if (ret[i] == '\0')
	break;
      fill (buf, tmp, NULL, 0);
    }
  }
  //return ret;
  return 0;
}

static void *
glusterfs_init (void)
{
  FILE *conf = fopen (specfile, "r");

  if (!conf) {
    perror ("open()");
    exit (1);
  }
  printf ("Loading spec from %s\n", specfile);
  struct xlator *tree = file_to_xlator_tree (conf);
  struct xlator *trav = tree;

  while (trav) {
    if (trav->init)
      trav->init (trav);
    trav = trav->next;
  }

  while (tree->parent)
    tree = tree->parent;

  fclose (conf);
  return tree;
}

static void
glusterfs_destroy (void *data)
{
  struct xlator *xlator = data;

  free (xlator);
  return;
}

static struct fuse_operations glusterfs_fops = {
  .getattr     = glusterfs_getattr,
  .readlink    = glusterfs_readlink,
  .getdir      = NULL /*glusterfs_getdir */,
  .mknod       = glusterfs_mknod,
  .mkdir       = glusterfs_mkdir,
  .unlink      = glusterfs_unlink,
  .rmdir       = glusterfs_rmdir,
  .symlink     = glusterfs_symlink,
  .rename      = glusterfs_rename,
  .link        = glusterfs_link,
  .chmod       = glusterfs_chmod,
  .chown       = glusterfs_chown,
  .truncate    = glusterfs_truncate,
  .utime       = glusterfs_utime,
  .open        = glusterfs_open,
  .read        = glusterfs_read,
  .write       = glusterfs_write,
  .statfs      = glusterfs_statfs,
  .flush       = glusterfs_flush,
  .release     = glusterfs_release,
  .fsync       = glusterfs_fsync,
  .setxattr    = glusterfs_setxattr,
  .getxattr    = glusterfs_getxattr,
  .listxattr   = glusterfs_listxattr,
  .removexattr = glusterfs_removexattr,
  .opendir     = glusterfs_opendir,
  .readdir     = glusterfs_readdir,
  .releasedir  = glusterfs_releasedir,
  .fsyncdir    = glusterfs_fsyncdir,
  .init        = glusterfs_init,
  .destroy     = glusterfs_destroy,
  .access      = glusterfs_access,
  .create      = NULL, /*glusterfs_create,*/
  .ftruncate   = glusterfs_ftruncate,
  .fgetattr    = glusterfs_fgetattr
};

int
glusterfs_mount (char *spec, char *mount_point, char *options)
{
  char *argv[] = {
    "glusterfs",
    "-o", "default_permissions",
    "-o", "allow_other",
    "-o", "nonempty",
    "-o", "hard_remove",
    "-d",
    mount_point,
    NULL };
  specfile = spec;
  return fuse_main ((sizeof (argv) / sizeof (char *)) - 1, argv, &glusterfs_fops);
}
