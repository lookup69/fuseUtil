#ifndef _HDS_FUSEOPER_H_
#define _HDS_FUSEOPER_H_

#include "fuseoper.h"

#include <string.h>
#include <string>
#include <vector>

class HdsFuseOper : public FuseOper
{
private:
    struct mapper_s
    {
        std::string from;
        std::string to;
    }; 

    struct dir_s {
        DIR           *dp;
        struct dirent *entry;
        off_t         offset;
    }; 

private:
    std::string           m_basePath;
    std::vector<mapper_s> m_mapperVec;

private:
    void initMapperPath_(void);
    bool isEntryOfMapperDir_(const std::string &path) 
    {
        for(auto it = m_mapperVec.begin(); it != m_mapperVec.end(); ++it) {
            if(it->to.find(path) != std::string::npos) {
                return true;
            }
        }

        return false;
    }

public:
    HdsFuseOper();  
    explicit HdsFuseOper(const std::string &path);
    virtual ~HdsFuseOper();

    virtual int Getattr(const char *path, struct stat *stbuf) override;
    virtual int Fgetattr(const char *path,
                         struct stat *stbuf,
                         struct fuse_file_info *fi) override;
    virtual int Access(const char *path, int mask) override;
    virtual int Create(const char *path, mode_t mode, struct fuse_file_info *fi) override;
    virtual int Readlink(const char *path, char *buf, size_t size) override;
    virtual int Opendir(const char *path, fuse_file_info *fi) override;
    virtual int Releasedir(const char *path, struct fuse_file_info *fi) override;
    virtual int Readdir(const char *path,
                        void *buf,
                        fuse_fill_dir_t filler,
                        off_t offset,
                        struct fuse_file_info *fi) override;
    virtual int Mknod(const char *path, mode_t mode, dev_t rdev) override;
    virtual int Mkdir(const char *path, mode_t mode) override;
    virtual int Unlink(const char *path) override;
    virtual int Rmdir(const char *path) override;
    virtual int Symlink(const char *from, const char *to) override;
    virtual int Rename(const char *from, const char *to) override;
    virtual int Link(const char *from, const char *to) override;
    virtual int Chmod(const char *path, mode_t mode) override;
    virtual int Chown(const char *path, uid_t uid, gid_t gid) override;
    virtual int Truncate(const char *path, off_t size) override;
    virtual int Ftruncate(const char *path,
                          off_t size,
                          struct fuse_file_info *fi) override;
    virtual int Open(const char *path, struct fuse_file_info *fi) override;
    virtual int Read(const char *path,
                     char *buf,
                     size_t size,
                     off_t offset,
                     struct fuse_file_info *fi) override;

    virtual int Write(const char *path,
                      const char *buf,
                      size_t size,
                      off_t offset,
                      struct fuse_file_info *fi) override;

    virtual int ReadBuf(const char *path,
                        struct fuse_bufvec **bufp,
                        size_t size,
                        off_t offset,
                        struct fuse_file_info *fi) override;

    virtual int WriteBuf(const char *path,
                         struct fuse_bufvec *buf,
                         off_t offset,
                         struct fuse_file_info *fi) override;

    virtual int Statfs(const char *path, struct statvfs *stbuf) override;
    virtual int Flush(const char *path, struct fuse_file_info *fi) override;
    virtual int Fsync(const char *path, int isdatasync, struct fuse_file_info *fi)override;
    virtual int Release(const char *path, struct fuse_file_info *fi)override;

    virtual int Setxattr(const char *path,
                         const char *name,
                         const char *value,
                         size_t size, 
                         int flags) override;

    virtual int Getxattr(const char *path,
                         const char *name,
                         char *value,
                         size_t size)override;

    virtual int Listxattr(const char *path,
                          char *list,
                          size_t size) override;

    virtual int Removexattr(const char *path, const char *name) override;

    virtual int Ioctl(const char *path,
                      int cmd,
                      void *arg,
                      struct fuse_file_info *fi,
                      unsigned int flags,
                      void *data) override;

    virtual int Lock(const char *path,
                     struct fuse_file_info *fi,
                     int cmd,
                     struct flock *lock) override;

    virtual int Flock(const char *path,
                      struct fuse_file_info *fi,
                      int op) override;

    virtual int Utimens(const char *path,
                        const struct timespec ts[2]) override;

};
#endif
