#include "hdsfuseoper.h"

extern "C" {
#include <ulockmgr.h>
}

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <dirent.h>

#include "debugUtility.h"

using namespace std;

static void get_file_mode(string &strmode, int mode) 
{
    if(mode & S_IRWXU)
        strmode = strmode + "S_IRWXU ";
    if(mode & S_IRUSR)
        strmode = strmode + "S_IRUSR ";
    if(mode & S_IWUSR)
        strmode = strmode + "S_IWUSR ";
    if(mode & S_IXUSR)
        strmode = strmode + "S_IXUSR ";
    if(mode & S_IRWXG)
        strmode = strmode + "S_IRWXG ";
    if(mode & S_IRGRP)
        strmode = strmode + "S_IRGRP ";
    if(mode & S_IWGRP)
        strmode = strmode + "S_IWGRP ";
    if(mode & S_IXGRP)
        strmode = strmode + "S_IXGRP ";
    if(mode & S_IRWXO)
        strmode = strmode + "S_IRWXO ";
    if(mode & S_IROTH)
        strmode = strmode + "S_IROTH ";
    if(mode & S_IWOTH)
        strmode = strmode + "S_IWOTH ";
    if(mode & S_IXOTH)
        strmode = strmode + "S_IXOTH ";
    if(mode & S_IFIFO)
        strmode = strmode + "S_IFIFO ";
    if(mode & S_IXOTH)
        strmode = strmode + "S_IXOTH ";
    if(mode & S_IFCHR)
        strmode = strmode + "S_IFCHR ";
    if(mode & S_IFDIR)
        strmode = strmode + "S_IFDIR ";
    if(mode & S_IFBLK)
        strmode = strmode + "S_IFBLK ";
    if(mode & S_IFREG)
        strmode = strmode + "S_IFREG ";
    if(mode & S_ISUID)
        strmode = strmode + "S_ISUID ";
    if(mode & S_ISGID)
        strmode = strmode + "S_ISGID ";
    if(mode & S_ISVTX)
        strmode = strmode + "S_ISVTX ";
}

static void get_file_flags(string &strflags, int flags) 
{
    if(flags & O_APPEND)
        strflags = strflags + "O_APPEND ";
    if(flags & O_ASYNC)
        strflags = strflags + "O_ASYNC ";
    if(flags & O_CLOEXEC)
        strflags = strflags + "O_CLOEXEC ";
    if(flags & O_CREAT)
        strflags = strflags + "O_CREAT ";
    if(flags & O_DIRECT)
        strflags = strflags + "O_DIRECT ";
    if(flags & O_DIRECTORY)
        strflags = strflags + "O_DIRECTORY ";
    if(flags & O_EXCL)
        strflags = strflags + "O_EXCL ";
    if(flags & O_LARGEFILE)
        strflags = strflags + "O_LARGEFILE ";
    if(flags & O_NOATIME)
        strflags = strflags + "O_NOATIME ";
    if(flags & O_NOCTTY)
        strflags = strflags + "O_NOCTTY ";
    if(flags & O_PATH)
        strflags = strflags + "O_PATH ";
    if(flags & O_SYNC)
        strflags = strflags + "O_SYNC ";
    if(flags & O_TRUNC)
        strflags = strflags + "O_TRUNC ";
}

HdsFuseOper::HdsFuseOper()
{
    initMapperPath_();
}

HdsFuseOper::HdsFuseOper(const string &path) :  m_basePath(path)
{
    size_t pos = m_basePath.size();

    initMapperPath_();

    if(m_basePath[pos - 1] == '/') 
        m_basePath.erase(pos - 1, 1);
}

HdsFuseOper::~HdsFuseOper()
{
    DEBUG_PRINT_COLOR(LIGHT_GREEN, "\n");
}


void HdsFuseOper::initMapperPath_(void)
{
    FILE     *fp;
    string   cmd = "mount | grep mapper | awk '{print $1,$3}'";
    mapper_s external{"/share/external", "/share/external"};

    m_mapperVec.push_back(external);
    fp = popen(cmd.c_str(), "r");
    if(fp) {
        char buf[1024] = { 0 };

        while(fgets(buf, sizeof(buf), fp) != NULL) {
            mapper_s mapper;
            char     *v_;

            v_ = strchr(buf, ' ');
            if(v_) {
                size_t pos;

                *v_ = 0;
                mapper.from = buf;
                ++v_;
                mapper.to = v_;
                pos = mapper.from.find("\n");
                if(pos != string::npos)
                    mapper.from.erase(pos, mapper.from.size() - pos);
                pos = mapper.to.find("\n");
                if(pos != string::npos)
                    mapper.to.erase(pos, mapper.to.size() - pos); 

                m_mapperVec.push_back(mapper);
            }
        }
        pclose(fp);
    }
    
    for(auto it =  m_mapperVec.begin(); it != m_mapperVec.end(); ++it) {
        DEBUG_PRINT_COLOR(LIGHT_GREEN, "from:%s   to%s\n", it->from.c_str(), it->to.c_str());  
    }
}

int HdsFuseOper::Getattr(const char *path, struct stat *stbuf)
{
    string realPath = m_basePath + path;
    int    ret; 

    //DEBUG_PRINT_COLOR(LIGHT_GREEN, "Path:%s\n", path);  
    //DEBUG_PRINT_COLOR(LIGHT_GREEN, "RealPath:%s\n", realPath.c_str());

    ret = lstat(realPath.c_str(), stbuf);
    if(ret < 0) {
        ret = -errno;
      //  DEBUG_PRINT_COLOR(LIGHT_RED, "[err]%s\n", strerror(errno));
    }

    return 0;
}

int HdsFuseOper::Fgetattr(const char *path,
                          struct stat *stbuf,
                          struct fuse_file_info *fi) 
{
    string realPath = m_basePath + path;
    int    ret;

    DEBUG_PRINT_COLOR(LIGHT_GREEN, "fd:%lu\n", fi->fh);

    ret = fstat(fi->fh, stbuf);
    if(ret < 0) {
        ret = -errno;
        DEBUG_PRINT_COLOR(LIGHT_RED, "[err]%s\n", strerror(errno));
    }

    return 0;
}

int HdsFuseOper::Access(const char *path, int mask)
{
    string realPath = m_basePath + path; 
    int    ret; 

    DEBUG_PRINT_COLOR(LIGHT_GREEN, "mask:%d  RealPath:%s\n", mask, realPath.c_str());

    ret = access(realPath.c_str(), mask);
    if(ret < 0) {
        ret = -errno;
        DEBUG_PRINT_COLOR(LIGHT_RED, "[err]%s\n", strerror(errno));
    }

    return 0;
}

int HdsFuseOper::Create(const char *path, mode_t mode, struct fuse_file_info *fi) 
{
    string realPath = m_basePath + path;
    int    fd;

    DEBUG_PRINT_COLOR(LIGHT_GREEN, "RealPath:%s\n", realPath.c_str());
    {
        string strmode;

        get_file_mode(strmode, mode);
        DEBUG_PRINT_COLOR(LIGHT_GREEN, "Mode(%X):%s\n", mode, strmode.c_str()); 
    }

    //fd = creat(realPath.c_str(), mode);
    fd = open(path, fi->flags, mode);
    if(fd < 0) {
        fd = -errno;
        DEBUG_PRINT_COLOR(LIGHT_RED, "[err]%s\n", strerror(errno));
        return fd;
    }

#ifdef USE_FILE_PATH
    close(fd);
#else
    fi->fh = fd;
    DEBUG_PRINT_COLOR(LIGHT_GREEN, "fd:%lu:%d\n", fi->fh, fd); 
#endif

    return 0;
}

int HdsFuseOper::Readlink(const char *path, char *buf, size_t size)
{
    string realPath = m_basePath + path; 
    int    ret; 

    DEBUG_PRINT_COLOR(LIGHT_GREEN, "size:%lu realPath:%s\n", size, realPath.c_str());

    memset(buf, 0, size);
    ret = readlink(realPath.c_str(), buf, size - 1);
    if(ret < 0) {
        ret = -errno;
        DEBUG_PRINT_COLOR(LIGHT_RED, "[err]%s\n", strerror(errno));
    } else if(static_cast<size_t>(ret) < size) {
        ret = 0;
    }

    return ret;
}

int HdsFuseOper::Opendir(const char *path, fuse_file_info *fi) 
{
    HdsFuseOper::dir_s *dirP;
    string              realPath = m_basePath + path;

    DEBUG_PRINT_COLOR(LIGHT_GREEN, "RealPath:%s\n", realPath.c_str()); 
    dirP = static_cast<HdsFuseOper::dir_s *>(new HdsFuseOper::dir_s);
    if(dirP == NULL)
        return -ENOMEM;

    dirP->dp = opendir(realPath.c_str());
    if(dirP->dp == NULL) {
        int ret = -errno;

        DEBUG_PRINT_COLOR(LIGHT_RED, "[err]%s\n", strerror(errno)); 
        delete dirP;
        return ret;
    }
    dirP->offset = 0;
    dirP->entry = NULL;

    fi->fh = (uint64_t)dirP;

    return 0;
}

int HdsFuseOper::Releasedir(const char *path,
                            struct fuse_file_info *fi) 
{
    //HdsFuseOper::dir_s *dirP  = (HdsFuseOper::dir_s *)fi->fh; 
    HdsFuseOper::dir_s *dirP  = static_cast<HdsFuseOper::dir_s *>((void *)fi->fh); 
    closedir(dirP->dp);
    delete dirP;

    return 0;
}

int HdsFuseOper::Readdir(const char *path,
                         void *buf,
                         fuse_fill_dir_t filler,
                         off_t offset,
                         struct fuse_file_info *fi)
{
#ifdef USE_FILE_PATH
    string        realPath = m_basePath + path; 
    struct dirent *de;
    DIR           *dp; 

    (void)offset;
    (void)fi;

    DEBUG_PRINT_COLOR(LIGHT_GREEN, "realPath:%s\n", realPath.c_str());

    dp = opendir(realPath.c_str());
    if(dp == NULL) {
        int ret = errno;

        DEBUG_PRINT_COLOR(LIGHT_RED, "[err]%s\n", strerror(errno));
        return -ret;
    }

    while((de = readdir(dp)) != NULL) {
        struct stat st;

        memset(&st, 0, sizeof(st));
        st.st_ino = de->d_ino;

        if(realPath.size() > (m_basePath.size() + 1)) {
                if(filler(buf, de->d_name, &st, 0))
                    break;
        } else {
            if((strcmp(de->d_name, ".") == 0) ||
               (strcmp(de->d_name, "..") == 0) ||
               (de->d_type & DT_LNK) ||
               isEntryOfMapperDir_(de->d_name)) {
                if(filler(buf, de->d_name, &st, 0))
                    break;
            }
        }
    }

    closedir(dp);
#else
    HdsFuseOper::dir_s *dirP = (HdsFuseOper::dir_s *)fi->fh;
    //struct dirent      *de;
    //DIR                *dp; 

    if(offset != dirP->offset) {
        seekdir(dirP->dp, offset);
        dirP->entry = NULL;
        dirP->offset = offset;
    }
    while (1) {
        struct stat st;
        off_t nextoff;

        if(!dirP->entry) {
            dirP->entry = readdir(dirP->dp);
            if(!dirP->entry)
                break;
        }

        memset(&st, 0, sizeof(st));
        st.st_ino = dirP->entry->d_ino;
        st.st_mode = dirP->entry->d_type << 12;
        nextoff = telldir(dirP->dp);
        //printf("xxxx %s\n", dirP->entry->d_name);
        if(filler(buf, dirP->entry->d_name, &st, nextoff))
            break;
        //if((strcmp(de->d_name, ".") == 0) ||
        //   (strcmp(de->d_name, "..") == 0) ||
        //   (de->d_type & DT_LNK) ||
        //   isEntryOfMapperDir_(de->d_name))
        //{
        //    if(filler(buf, de->d_name, &st, 0))
        //        break;
        //}

        dirP->entry = NULL;
        dirP->offset = nextoff;
    }
#endif
    return 0;
}

int HdsFuseOper::Mknod(const char *path, mode_t mode, dev_t rdev) 
{
    string realPath = m_basePath + path;
    int    ret;

    DEBUG_PRINT_COLOR(LIGHT_GREEN, "dev:%lX realPath:%s\n", rdev, realPath.c_str());
    {
        string strmode;

        get_file_mode(strmode, mode);
        DEBUG_PRINT_COLOR(LIGHT_GREEN, "Mode(%X):%s\n", mode, strmode.c_str()); 
    }

    if(S_ISFIFO(mode))
        ret = mkfifo(realPath.c_str(), mode);
    else
        ret = mknod(realPath.c_str(), mode, rdev);
    if(ret < 0) {
        ret = -errno;
        DEBUG_PRINT_COLOR(LIGHT_RED, "[err]%s\n", strerror(errno));
    }

    return ret;
}

int HdsFuseOper::Mkdir(const char *path, mode_t mode)
{
    string realPath = m_basePath + path; 
    int    ret;

    DEBUG_PRINT_COLOR(LIGHT_GREEN, "Path    :%s\n", path);
    DEBUG_PRINT_COLOR(LIGHT_GREEN, "realPath:%s\n", realPath.c_str());
    {
        string strmode;

        get_file_mode(strmode, mode);
        DEBUG_PRINT_COLOR(LIGHT_GREEN, "Mode(%X):%s\n", mode, strmode.c_str()); 
    }

    ret = mkdir(realPath.c_str(), mode);
    if(ret < 0) {
        ret = -errno;
        DEBUG_PRINT_COLOR(LIGHT_RED, "[err]%s\n", strerror(errno));
    }

    return ret;
}

int HdsFuseOper::Unlink(const char *path) 
{
    string realPath = m_basePath + path;
    int    ret;

    DEBUG_PRINT_COLOR(LIGHT_GREEN, "Path    :%s\n", path);
    DEBUG_PRINT_COLOR(LIGHT_GREEN, "realPath:%s\n", realPath.c_str()); 

    ret = unlink(realPath.c_str());
    if(ret < 0) {
        ret = -errno;
        DEBUG_PRINT_COLOR(LIGHT_RED, "[err]%s\n", strerror(errno));
    }

    return ret;
}

int HdsFuseOper::Rmdir(const char *path) 
{
    string realPath = m_basePath + path;
    int    ret; 

    DEBUG_PRINT_COLOR(LIGHT_GREEN, "Path    :%s\n", path);
    DEBUG_PRINT_COLOR(LIGHT_GREEN, "realPath:%s\n", realPath.c_str()); 

    ret = rmdir(realPath.c_str());
    if(ret < 0) {
        ret = -errno;
        DEBUG_PRINT_COLOR(LIGHT_RED, "[err]%s\n", strerror(errno));
    }

    return ret;
}

int HdsFuseOper::Symlink(const char *from, const char *to) 
{
    string realFromPath = from;
    string realToPath = m_basePath + to;
    int    ret; 

    DEBUG_PRINT_COLOR(LIGHT_GREEN, "from :%s   to:%s\n", from, to);
    DEBUG_PRINT_COLOR(LIGHT_GREEN, "realFromPath:%s  realToPath:%s\n", realFromPath.c_str(), realToPath.c_str());

    ret = symlink(realFromPath.c_str(), realToPath.c_str());
    if(ret < 0) {
        ret = -errno;
        DEBUG_PRINT_COLOR(LIGHT_RED, "[err]%s\n", strerror(errno));
    }

    return ret;
}

int HdsFuseOper::Rename(const char *from, const char *to) 
{
    string realFromPath = m_basePath + from;
    string realToPath = m_basePath + to; 
    int    ret; 

    DEBUG_PRINT_COLOR(LIGHT_GREEN, "realFromPath:%s  realToPath:%s\n", realFromPath.c_str(), realToPath.c_str()); 

    ret = rename(realFromPath.c_str(), realToPath.c_str());
    if(ret < 0) {
        ret = -errno;
        DEBUG_PRINT_COLOR(LIGHT_RED, "[err]%s\n", strerror(errno));
    }

    return ret;
}

int HdsFuseOper::Link(const char *from, const char *to) 
{
    string realFromPath = m_basePath + from;
    string realToPath = m_basePath + to; 
    int    ret; 

    DEBUG_PRINT_COLOR(LIGHT_GREEN, "realFromPath:%s  realToPath:%s\n", realFromPath.c_str(), realToPath.c_str()); 

    ret = link(realFromPath.c_str(), realToPath.c_str());
    if(ret < 0) {
        ret = -errno;
        DEBUG_PRINT_COLOR(LIGHT_RED, "[err]%s\n", strerror(errno));
    }

    return ret;
}

int HdsFuseOper::Chmod(const char *path, mode_t mode) 
{
    string realPath = m_basePath + path; 
    int    ret; 

    DEBUG_PRINT_COLOR(LIGHT_GREEN, "Path    :%s\n", path);
    DEBUG_PRINT_COLOR(LIGHT_GREEN, "realPath:%s\n", realPath.c_str()); 
    {
        string strmode;

        get_file_mode(strmode, mode);
        DEBUG_PRINT_COLOR(LIGHT_GREEN, "Mode(%X):%s\n", mode, strmode.c_str()); 
    }

    ret = chmod(realPath.c_str(), mode);
    if(ret < 0) {
        ret = -errno;
        DEBUG_PRINT_COLOR(LIGHT_RED, "[err]%s\n", strerror(errno));
    }

    return ret;
}

int HdsFuseOper::Chown(const char *path, uid_t uid, gid_t gid) 
{
    string realPath = m_basePath + path; 
    int    ret; 

    DEBUG_PRINT_COLOR(LIGHT_GREEN, "uid:%u git:%u realPath:%s\n", uid, gid, realPath.c_str()); 

    ret = chown(realPath.c_str(), uid, gid);
    if(ret < 0) {
        ret = -errno;
        DEBUG_PRINT_COLOR(LIGHT_RED, "[err]%s\n", strerror(errno));
    }

    return 0;
}

int HdsFuseOper::Truncate(const char *path, off_t size) 
{
    string realPath = m_basePath + path; 
    int    ret = 0; 

    DEBUG_PRINT_COLOR(LIGHT_GREEN, "size:%lu realPath:%s\n", size, realPath.c_str()); 

    ret = truncate(realPath.c_str(), size);
    if(ret < 0) {
        ret = -errno;
        DEBUG_PRINT_COLOR(LIGHT_RED, "[err]%s\n", strerror(errno));
    }

    return ret;
}

int HdsFuseOper::Ftruncate(const char *path,
                           off_t size,
                           struct fuse_file_info *fi) 
{
    int ret = 0;

    {
        string realPath = m_basePath + path; 
        DEBUG_PRINT_COLOR(LIGHT_GREEN, "size:%lu realPath:%s\n", size, realPath.c_str()); 
    }

    ret = ftruncate(fi->fh, size);
    if(ret < 0) {
        ret = -errno;
        DEBUG_PRINT_COLOR(LIGHT_RED, "[err]%s\n", strerror(errno));
    }

    return ret; 
}

int HdsFuseOper::Open(const char *path, struct fuse_file_info *fi)
{
    string realPath = m_basePath + path;
    int    fd;

    //DEBUG_PRINT_COLOR(LIGHT_GREEN, "Path    :%s\n", path);
    DEBUG_PRINT_COLOR(LIGHT_GREEN, "realPath:%s\n", realPath.c_str()); 
    {
        string flags;
        get_file_flags(flags, fi->flags);
        DEBUG_PRINT_COLOR(LIGHT_GREEN, "FLAGS(%X)  :%s\n", fi->flags, flags.c_str());
    }


    fd = open(realPath.c_str(), fi->flags);
    if(fd < 0) {
        fd = errno;
        DEBUG_PRINT_COLOR(LIGHT_RED, "[err]%s\n", strerror(errno));
        return -fd;
    }
#ifdef USE_FILE_PATH
    close(fd);
#else
    fi->fh = fd;
    DEBUG_PRINT_COLOR(LIGHT_GREEN, "fd:%lu  realPath:%s\n", fi->fh, realPath.c_str());
#endif

    return 0;
}

int HdsFuseOper::Read(const char *path,
                      char *buf,
                      size_t size,
                      off_t offset,
                      struct fuse_file_info *fi) 
{
    int ret = 0; 

#ifdef USE_FILE_PATH
    string realPath = m_basePath + path;
    int    fd;

    DEBUG_PRINT_COLOR(LIGHT_GREEN, "realPath:%s\n", realPath.c_str());

    fd = open(realPath.c_str(), O_RDONLY);
    if(fd == -1) {
        ret = -errno;
        DEBUG_PRINT_COLOR(LIGHT_RED, "[err]%s\n", strerror(errno));
        return ret;
    }

    ret = pread(fd, buf, size, offset);
    DEBUG_PRINT_COLOR(LIGHT_CYAN, "ret:%d\n", ret);
    if(ret < 0) {
        ret = -errno;
        DEBUG_PRINT_COLOR(LIGHT_RED, "[err]%s\n", strerror(errno));
    }

    close(fd);
#else
    DEBUG_PRINT_COLOR(LIGHT_GREEN, "fd:%ld size:%lu offset:%lu path:%s\n", fi->fh, size, offset, path);

    ret = pread(fi->fh, buf, size, offset);
    DEBUG_PRINT_COLOR(LIGHT_CYAN, "ret:%d\n", ret);
    if(ret < 0) {
        ret = -errno;
        DEBUG_PRINT_COLOR(LIGHT_RED, "[err]%s\n", strerror(errno));
    }
#endif

    return ret; 
}

int HdsFuseOper::Write(const char *path,
                       const char *buf,
                       size_t size,
                       off_t offset,
                       struct fuse_file_info *fi) 
{
    int ret = 0; 

#ifdef USE_FILE_PATH
    string realPath = m_basePath + path;
    int    fd;

    DEBUG_PRINT_COLOR(LIGHT_GREEN, "realPath:%s\n", realPath.c_str());

    fd = open(realPath.c_str(), O_WRONLY);
    if(fd == -1) {
        ret = -errno;
        DEBUG_PRINT_COLOR(LIGHT_RED, "[err]%s\n", strerror(errno));
        return ret;
    }

    ret = pwrite(fd, buf, size, offset);    
    DEBUG_PRINT_COLOR(LIGHT_CYAN, "ret:%d\n", ret);
    if(ret < 0) {
        ret = -errno;
        DEBUG_PRINT_COLOR(LIGHT_RED, "[err]%s\n", strerror(errno));
    }

    close(fd);
#else
    DEBUG_PRINT_COLOR(LIGHT_GREEN, "fd:%lu size:%lu path:%s\n", fi->fh, size, path);
    ret = pwrite(fi->fh, buf, size, offset);
    if(ret < 0) {
        ret = -errno;
        DEBUG_PRINT_COLOR(LIGHT_RED, "[err]%s\n", strerror(errno));
    }
#endif

    return ret;
}

int HdsFuseOper::ReadBuf(const char *path,
                         struct fuse_bufvec **bufp,
                         size_t size,
                         off_t offset,
                         struct fuse_file_info *fi) 
{
    struct fuse_bufvec *src;

    DEBUG_PRINT_COLOR(LIGHT_GREEN, "fd:%lu size:%lu offset:%lu Path:%s\n", fi->fh, size, offset, path); 

    src = static_cast<struct fuse_bufvec *>(malloc(sizeof(struct fuse_bufvec)));
    if (src == NULL)
        return -ENOMEM;
    DEBUG_PRINT_COLOR(LIGHT_GREEN, "bufp:%p\n", src); 
    *src = FUSE_BUFVEC_INIT(size);

    src->buf[0].flags = (fuse_buf_flags)(FUSE_BUF_IS_FD | FUSE_BUF_FD_SEEK);
    src->buf[0].fd = fi->fh;
    src->buf[0].pos = offset;

    *bufp = src;

    return 0;
}

int HdsFuseOper::WriteBuf(const char *path,
                          struct fuse_bufvec *buf,
                          off_t offset,
                          struct fuse_file_info *fi) 
{
    struct fuse_bufvec dst = FUSE_BUFVEC_INIT(fuse_buf_size(buf));

    DEBUG_PRINT_COLOR(LIGHT_GREEN, "fd:%lu offset:%lu buf:%p Path:%s\n", fi->fh, offset, buf, path); 

    dst.buf[0].flags = (fuse_buf_flags)(FUSE_BUF_IS_FD | FUSE_BUF_FD_SEEK);
    dst.buf[0].fd = fi->fh;
    dst.buf[0].pos = offset;

    return fuse_buf_copy(&dst, buf, FUSE_BUF_SPLICE_NONBLOCK);
}

int HdsFuseOper::Statfs(const char *path, struct statvfs *stbuf)
{
    string realPath = m_basePath + path;
    int    ret = 0;

    DEBUG_PRINT_COLOR(LIGHT_GREEN, "realPath:%s\n", realPath.c_str()); 

    ret = statvfs(path, stbuf);
    if(ret < 0) {
        ret = -errno;
        DEBUG_PRINT_COLOR(LIGHT_RED, "[err]%s\n", strerror(errno));
    }

    return ret;
}

int HdsFuseOper::Flush(const char *path, struct fuse_file_info *fi)
{
    int ret = 0;

    DEBUG_PRINT_COLOR(LIGHT_GREEN, "fd:%lu Path:%s\n", fi->fh, path); 

#ifndef USE_FILE_PATH
    ret = close(dup(fi->fh));
    if(ret == -1) {
        ret = -errno;
        DEBUG_PRINT_COLOR(LIGHT_RED, "[err]%s\n", strerror(errno)); 
    }

#endif

    return ret; 
}

int HdsFuseOper::Fsync(const char *path, int isdatasync, struct fuse_file_info *fi) 
{
    int ret = 0; 

    DEBUG_PRINT_COLOR(LIGHT_GREEN, "Path:%s\n", path); 

#ifndef USE_FILE_PATH
    if(isdatasync) 
        ret = fdatasync(fi->fh);
    else 
        ret = fsync(fi->fh);

    if(ret < 0) {
        ret = -errno;
        DEBUG_PRINT_COLOR(LIGHT_RED, "[err]%s\n", strerror(errno));
    }
#endif
    return ret;
}

int HdsFuseOper::Release(const char *path, struct fuse_file_info *fi) 
{
    DEBUG_PRINT_COLOR(LIGHT_GREEN, "fd:%lu Path:%s\n", fi->fh, path); 

#ifndef USE_FILE_PATH
    close(fi->fh);
#endif

    return 0;
}

int HdsFuseOper::Setxattr(const char *path,
                          const char *name,
                          const char *value,
                          size_t size, 
                          int flags) 
{
    string realPath = m_basePath + path;
    int    ret;

    DEBUG_PRINT_COLOR(LIGHT_GREEN, "name:%s  size:%lu flags:%X value:%s  Path:%s\n", name, size, flags, value, path);
    DEBUG_PRINT_COLOR(LIGHT_GREEN, "name:%s  size:%lu flags:%X valuse:%s realPath:%s\n", name, size, flags, value, realPath.c_str());

    ret = lsetxattr(realPath.c_str(), name, value, size, flags);
    if(ret < 0) {
        ret = errno;
        DEBUG_PRINT_COLOR(LIGHT_RED, "[err]%s\n", strerror(errno));
        return -ret;
    }

    return ret; 
}

int HdsFuseOper::Getxattr(const char *path,
                          const char *name,
                          char *value,
                          size_t size) 
{
    string realPath = m_basePath + path;
    int    ret;

    //DEBUG_PRINT_COLOR(LIGHT_GREEN, "name:%s  size:%lu  Path:%s\n", name, size, path);
    //DEBUG_PRINT_COLOR(LIGHT_GREEN, "name:%s  size:%lu  realPath:%s\n", name, size, realPath.c_str());

    ret = lgetxattr(realPath.c_str(), name, value, size);
    if(ret < 0) {
        ret = errno;
        //DEBUG_PRINT_COLOR(LIGHT_RED, "[err]%s\n", strerror(errno)); 
        return -ret;
    }

    return ret;
}

int HdsFuseOper::Listxattr(const char *path,
                           char *list,
                           size_t size) 
{
    string realPath = m_basePath + path;
    int    ret;

    DEBUG_PRINT_COLOR(LIGHT_GREEN, "size:%lu  Path:%s\n", size, path);
    DEBUG_PRINT_COLOR(LIGHT_GREEN, "size:%lu  realPath:%s\n", size, realPath.c_str());

    ret = llistxattr(realPath.c_str(), list, size);
    if(ret < 0) {
        ret = errno;
        DEBUG_PRINT_COLOR(LIGHT_RED, "[err]%s\n", strerror(errno));
        return -ret;
    }

    return ret; 
}

int HdsFuseOper::Removexattr(const char *path, const char *name) 
{
    string realPath = m_basePath + path;
    int    ret;

    DEBUG_PRINT_COLOR(LIGHT_GREEN, "name:%s  Path:%s\n", name, path);
    DEBUG_PRINT_COLOR(LIGHT_GREEN, "name:%s  realPath:%s\n", name, realPath.c_str());

    ret = removexattr(realPath.c_str(), name);
    if(ret < 0) {
        ret = errno;
        DEBUG_PRINT_COLOR(LIGHT_RED, "[err]%s\n", strerror(errno));
        return -ret;
    }

    return ret; 
}

int HdsFuseOper::Ioctl(const char *path,
                       int cmd,
                       void *arg,
                       struct fuse_file_info *fi,
                       unsigned int flags,
                       void *data) 
{
    DEBUG_PRINT_COLOR(LIGHT_PURPLE, "[NO_IMP]path:%s\n", path);
    return -EACCES;
}

int HdsFuseOper::Lock(const char *path,
                      struct fuse_file_info *fi,
                      int cmd,
                      struct flock *lock) 

{
    int ret = 0;

    DEBUG_PRINT_COLOR(LIGHT_GREEN, "fd:%lu Path:%s\n", fi->fh, path); 
#ifndef USE_FILE_PATH
    //ret = ulockmgr_op(fi->fh, cmd, lock, &fi->lock_owner, sizeof(fi->lock_owner));
    ret = ulockmgr_op(fi->fh, cmd, lock, &fi->lock_owner,
               sizeof(fi->lock_owner));

    if(ret < 0) {
        ret = -errno;
        DEBUG_PRINT_COLOR(LIGHT_RED, "[err]%s\n", strerror(errno));
    }
#endif
    return ret;
}

int HdsFuseOper::Flock(const char *path,
                       struct fuse_file_info *fi,
                       int op)
{
    int ret = 0;

    DEBUG_PRINT_COLOR(LIGHT_GREEN, "fd:%lu Path:%s\n", fi->fh, path); 
#ifndef USE_FILE_PATH
    ret = flock(fi->fh, op);
    if(ret < 0) {
        ret = -errno;
        DEBUG_PRINT_COLOR(LIGHT_RED, "[err]%s\n", strerror(errno)); 
    }
#endif
    return ret;
}

int HdsFuseOper::Utimens(const char *path,
                         const struct timespec ts[2]) 
{
    string realPath = m_basePath + path;
    int    ret;

    DEBUG_PRINT_COLOR(LIGHT_GREEN, "realPath:%s\n", realPath.c_str()); 

    /* don't use utime/utimes since they follow symlinks */
    ret = utimensat(0, realPath.c_str(), ts, AT_SYMLINK_NOFOLLOW);
    if(ret < 0) {
        ret = -errno;
        DEBUG_PRINT_COLOR(LIGHT_RED, "[err]%s\n", strerror(errno));
    }

    return ret;
}
