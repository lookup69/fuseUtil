#ifndef _FUSEOPER_H_
#define _FUSEOPER_H_

#define FUSE_USE_VERSION 26

#include <fuse.h>
#include <sys/types.h>
#include <dirent.h>
#include <attr/xattr.h>


class FuseOper
{
public:
    virtual ~FuseOper()
    {
    }

    virtual int Getattr(const char *path, struct stat *stbuf)
    {
        return -EACCES;
    }

    virtual int Fgetattr(const char *path,
                         struct stat *stbuf,
                         struct fuse_file_info *fi)
    {
        return -EACCES;
    }

    virtual int Access(const char *path, int mask)
    {
        return -EACCES;
    }

    virtual int Create(const char *path, mode_t mode, struct fuse_file_info *fi)
    {
        return -EACCES;
    }

    virtual int Readlink(const char *path, char *buf, size_t size)
    {
        return -EACCES;
    }

    virtual int Opendir(const char *path, fuse_file_info *fi)
    {
        return -EACCES;
    }

    virtual int Releasedir(const char *path, struct fuse_file_info *fi)
    {
        return -ENOSYS;
    }

    virtual int Readdir(const char *path,
                        void *buf,
                        fuse_fill_dir_t filler,
                        off_t offset,
                        struct fuse_file_info *fi)
    {
        return -EACCES;
    }

    virtual int Mknod(const char *path, mode_t mode, dev_t rdev)
    {
        return -EACCES;
    }

    virtual int Mkdir(const char *path, mode_t mode)
    {
        return -EACCES;
    }

    virtual int Unlink(const char *path)
    {
        return -EACCES;
    }

    virtual int Rmdir(const char *path)
    {
        return -EACCES;
    }

    virtual int Symlink(const char *from, const char *to)
    {
        return -EACCES;
    }

    virtual int Rename(const char *from, const char *to)
    {
        return -EACCES;
    }

    virtual int Link(const char *from, const char *to)
    {
        return -EACCES;
    }

    virtual int Chmod(const char *path, mode_t mode)
    {
        return -EACCES;
    }

    virtual int Chown(const char *path, uid_t uid, gid_t gid)
    {
        return -EACCES;
    }

    virtual int Truncate(const char *path, off_t size)
    {
        return -EACCES;
    }

    virtual int Ftruncate(const char *path,
                          off_t size, 
                          struct fuse_file_info *fi) 
    {
        return -ENOSYS; 
    }

    virtual int Open(const char *path, struct fuse_file_info *fi)
    {
        return -EACCES;
    }

    virtual int Read(const char *path,
                     char *buf,
                     size_t size,
                     off_t offset,
                     struct fuse_file_info *fi)
    {
        return -EINVAL;
    }

    virtual int Write(const char *path,
                      const char *buf,
                      size_t size,
                      off_t offset,
                      struct fuse_file_info *fi)
    {
        return -EINVAL;
    }

    virtual int ReadBuf(const char *path,
                       struct fuse_bufvec **bufp,
                       size_t size,
                       off_t offset,
                       struct fuse_file_info *fi)
    {
        return -ENOSYS;
    }

    virtual int WriteBuf(const char *path,
                         struct fuse_bufvec *buf,
                         off_t offset,
                         struct fuse_file_info *fi)
    {
        return -ENOSYS;
    }

    virtual int Statfs(const char *path, struct statvfs *stbuf)
    {
        return -EACCES;
    }

    virtual int Fsync(const char *path, int isdatasync, struct fuse_file_info *fi)
    {
        return -ENOSYS;
    }

    virtual int Flush(const char *path, struct fuse_file_info *fi)
    {
        return -ENOSYS;
    }

    virtual int Release(const char *path, struct fuse_file_info *fi)
    {
        return -ENOSYS;
    }

    virtual int Setxattr(const char *path,
                         const char *name,
                         const char *value,
                         size_t size, int flags)
    {
        return -EACCES;
    }

    virtual int Getxattr(const char *path,
                         const char *name,
                         char *value,
                         size_t size)
    {
        return -EACCES;
    }

    virtual int Listxattr(const char *path, char *list, size_t size)
    {
        return -EACCES;
    }

    virtual int Removexattr(const char *path, const char *name)
    {
        return -EACCES;
    }

    virtual int Ioctl(const char *path,
                      int cmd,
                      void *arg,
                      struct fuse_file_info *fi,
                      unsigned int flags,
                      void *data)
    {
        return -EACCES;
    }

    virtual int Lock(const char *path,
                     struct fuse_file_info *fi,
                     int cmd,
                     struct flock *lock)
    {
        return -EACCES;
    }

    virtual int Flock(const char *path,
                      struct fuse_file_info *fi,
                      int op)
    {
        return -EACCES;
    }

    virtual int Utimens(const char *path,
                        const struct timespec ts[2])
    {
        return -ENOSYS;
    }
};

#endif
