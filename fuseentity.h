#ifndef _FUSEENTITY_H_
#define _FUSEENTITY_H_

#include "fuseoper.h"

#include <assert.h>
#include <memory>


#include <stdio.h>
class FuseEntity
{
private:
    struct fuse_operations           m_fuseOper; 
    static std::shared_ptr<FuseOper> m_operHook;
    
public:
    FuseEntity(); 
    FuseEntity(FuseOper *oper);
    ~FuseEntity();

    void attachFuseOper(FuseOper *oper)
    {
        std::shared_ptr<FuseOper> tmpOperPtr(oper);

        m_operHook = tmpOperPtr;
    }

    int fuseMain(int argc, char *argv[])
    {
        assert(m_operHook.get() != nullptr); 

        return fuse_main(argc, argv, &m_fuseOper, NULL);
    }

    static int Getattr(const char *path, struct stat *stbuf)
    {
        assert(m_operHook.get() != nullptr);

        return m_operHook->Getattr(path, stbuf);
    }

    static int Fgetattr(const char *path,
                        struct stat *stbuf,
                        struct fuse_file_info *fi)
    {
        assert(m_operHook.get() != nullptr);

        return m_operHook->Fgetattr(path, stbuf, fi);
    }

    static int Access(const char *path, int mask)
    {
        assert(m_operHook.get() != nullptr); 

        return m_operHook->Access(path, mask);
    }

    static int Create(const char *path, mode_t mode, struct fuse_file_info *fi)
    {
        assert(m_operHook.get() != nullptr);

        return m_operHook->Create(path, mode, fi);
    }

    static int Opendir(const char *path, struct fuse_file_info *fi)
    {
        assert(m_operHook.get() != nullptr);

        return m_operHook->Opendir(path, fi);
    }

    static int Releasedir(const char *path, struct fuse_file_info *fi)
    {
        assert(m_operHook.get() != nullptr);

        return m_operHook->Opendir(path, fi);
    }

    static int Readlink(const char *path, char *buf, size_t size)
    {
        assert(m_operHook.get() != nullptr); 

        return m_operHook->Readlink(path, buf, size);
    }

    static int Readdir(const char *path,
                       void *buf,
                       fuse_fill_dir_t filler,
                       off_t offset,
                       struct fuse_file_info *fi)
    {
        assert(m_operHook.get() != nullptr); 

        return m_operHook->Readdir(path, buf, filler, offset, fi);
    }

    static int Mknod(const char *path, mode_t mode, dev_t rdev)
    {
        assert(m_operHook.get() != nullptr);

        return m_operHook->Mknod(path, mode, rdev);
    }

    static int Mkdir(const char *path, mode_t mode)
    {
        assert(m_operHook.get() != nullptr);

        return m_operHook->Mkdir(path, mode);
    }

    static int Unlink(const char *path)
    {
        assert(m_operHook.get() != nullptr);

        return m_operHook->Unlink(path);
    }

    static int Rmdir(const char *path)
    {
        assert(m_operHook.get() != nullptr);

        return m_operHook->Rmdir(path);
    }

    static int Symlink(const char *from, const char *to)
    {
        assert(m_operHook.get() != nullptr);

        return m_operHook->Symlink(from, to);
    }

    static int Rename(const char *from, const char *to)
    {
        assert(m_operHook.get() != nullptr);

        return m_operHook->Rename(from, to);
    }

    static int Link(const char *from, const char *to)
    {
        assert(m_operHook.get() != nullptr);

        return m_operHook->Link(from, to);
    }

    static int Chmod(const char *path, mode_t mode)
    {
        assert(m_operHook.get() != nullptr);

        return m_operHook->Chmod(path, mode);
    }

    static int Chown(const char *path, uid_t uid, gid_t gid)
    {
        assert(m_operHook.get() != nullptr);

        return m_operHook->Chown(path, uid, gid);
    }

    static int Truncate(const char *path, off_t size)
    {
        assert(m_operHook.get() != nullptr);

        return m_operHook->Truncate(path, size);
    }

    static int Ftruncate(const char *path, 
                         off_t size, 
                         struct fuse_file_info *fi)
    {
        assert(m_operHook.get() != nullptr);

        return m_operHook->Ftruncate(path, size, fi);
    }

    static int Open(const char *path, struct fuse_file_info *fi)
    {
        assert(m_operHook.get() != nullptr);

        return m_operHook->Open(path, fi);
    }

    static int Read(const char *path,
                    char *buf,
                    size_t size,
                    off_t offset,
                    struct fuse_file_info *fi)
    {
        assert(m_operHook.get() != nullptr); 

        return m_operHook->Read(path, buf, size, offset, fi);
    }

    static int Write(const char *path,
                     const char *buf,
                     size_t size,
                     off_t offset,
                     struct fuse_file_info *fi)
    {
        assert(m_operHook.get() != nullptr); 

        return m_operHook->Write(path, buf, size, offset, fi);
    }

    static int ReadBuf(const char *path,
                       struct fuse_bufvec **bufp,
                       size_t size,
                       off_t offset,
                       struct fuse_file_info *fi)
    {
        assert(m_operHook.get() != nullptr);

        return m_operHook->ReadBuf(path, bufp, size, offset, fi);
    }

    static int WriteBuf(const char *path, 
                        struct fuse_bufvec *buf,
                        off_t offset, 
                        struct fuse_file_info *fi)
    {
        assert(m_operHook.get() != nullptr);

        return m_operHook->WriteBuf(path, buf, offset, fi);
    }

    static int Statfs(const char *path, struct statvfs *stbuf)
    {
        assert(m_operHook.get() != nullptr);

        return m_operHook->Statfs(path, stbuf);
    }

    static int Fsync(const char *path, int isdatasync, struct fuse_file_info *fi)
    {
        assert(m_operHook.get() != nullptr);

        return m_operHook->Fsync(path, isdatasync, fi);
    }

    static int Flush(const char *path, struct fuse_file_info *fi)
    {
        assert(m_operHook.get() != nullptr); 

        return m_operHook->Flush(path, fi); 
    }

    static int Release(const char *path, struct fuse_file_info *fi)
    {
        assert(m_operHook.get() != nullptr); 

        return m_operHook->Release(path, fi);
    }

    static int Setxattr(const char *path,
                        const char *name,
                        const char *value,
                        size_t size, 
                        int flags)
    {
        assert(m_operHook.get() != nullptr);

        return m_operHook->Setxattr(path, name, value, size, flags);
    }

    static int Getxattr(const char *path,
                        const char *name,
                        char *value,
                        size_t size)
    {
        assert(m_operHook.get() != nullptr);

        return m_operHook->Getxattr(path, name, value, size);
    }

    static int Listxattr(const char *path, char *list, size_t size)
    {
        assert(m_operHook.get() != nullptr); 

        return m_operHook->Listxattr(path, list, size);
    }

    static int Removexattr(const char *path, const char *name)
    {
        assert(m_operHook.get() != nullptr); 

        return m_operHook->Removexattr(path, name);
    }

    static int Ioctl(const char *path,
                     int cmd,
                     void *arg,
                     struct fuse_file_info *fi,
                     unsigned int flags,
                     void *data)
    {
        assert(m_operHook.get() != nullptr);

        return m_operHook->Ioctl(path, cmd, arg, fi, flags, data);
    }

    static int Lock(const char *path,
                    struct fuse_file_info *fi,
                    int cmd,
                    struct flock *lock)
    {
        assert(m_operHook.get() != nullptr);

        return m_operHook->Lock(path, fi, cmd, lock);
    }

    static int Flock(const char *path,
                     struct fuse_file_info *fi,
                     int op)
    {
        assert(m_operHook.get() != nullptr);

        return m_operHook->Flock(path, fi, op);
    }

    static int Utimens(const char *path, const struct timespec ts[2])
    {
        assert(m_operHook.get() != nullptr);

        return m_operHook->Utimens(path, ts);
    }
};


#endif
