#include "fuseentity.h"

#include <string.h>

#include <iostream>

using namespace std;

//FuseOper *FuseEntity::m_operHook = nullptr;
//unique_ptr<FuseOper> FuseEntity::m_operHook(nullptr);
shared_ptr<FuseOper> FuseEntity::m_operHook(nullptr);

FuseEntity::FuseEntity()
{
    memset(&m_fuseOper, 0, sizeof(m_fuseOper));

    m_fuseOper.getattr     = FuseEntity::Getattr;
#ifndef USE_FILE_PATH
    m_fuseOper.fgetattr    = FuseEntity::Fgetattr;
#endif
    m_fuseOper.access      = FuseEntity::Access;
    m_fuseOper.create      = FuseEntity::Create;
    m_fuseOper.readlink    = FuseEntity::Readlink;
#ifndef USE_FILE_PATH
    m_fuseOper.opendir     = FuseEntity::Opendir;
    m_fuseOper.releasedir  = FuseEntity::Releasedir;
#endif
    m_fuseOper.readdir     = FuseEntity::Readdir;
    m_fuseOper.mknod       = FuseEntity::Mknod;
    m_fuseOper.mkdir       = FuseEntity::Mkdir;
    m_fuseOper.symlink     = FuseEntity::Symlink;
    m_fuseOper.unlink      = FuseEntity::Unlink;
    m_fuseOper.rmdir       = FuseEntity::Rmdir;
    m_fuseOper.rename      = FuseEntity::Rename;
    m_fuseOper.link        = FuseEntity::Link;
    m_fuseOper.chmod       = FuseEntity::Chmod;
    m_fuseOper.chown       = FuseEntity::Chown;
    m_fuseOper.truncate    = FuseEntity::Truncate;
#ifndef USE_FILE_PATH
    m_fuseOper.ftruncate   = FuseEntity::Ftruncate;
#endif 
    m_fuseOper.open        = FuseEntity::Open;
    m_fuseOper.read        = FuseEntity::Read;
    m_fuseOper.write       = FuseEntity::Write;
#ifndef USE_FILE_PATH
    m_fuseOper.read_buf    = FuseEntity::ReadBuf;
    m_fuseOper.write_buf   = FuseEntity::WriteBuf;
#endif
    m_fuseOper.statfs      = FuseEntity::Statfs;

    m_fuseOper.setxattr    = FuseEntity::Setxattr;
    m_fuseOper.getxattr    = FuseEntity::Getxattr;
    m_fuseOper.listxattr   = FuseEntity::Listxattr;
    m_fuseOper.removexattr = FuseEntity::Removexattr;

#ifndef USE_FILE_PATH
    m_fuseOper.flush       = FuseEntity::Flush;
    m_fuseOper.fsync       = FuseEntity::Fsync;
    m_fuseOper.release     = FuseEntity::Release;
    m_fuseOper.flock       = FuseEntity::Flock;
#endif

    //m_fuseOper.ioctl       = FuseEntity::Ioctl;
    //m_fuseOper.utimens     = FuseEntity::Utimens;

#ifndef USE_FILE_PATH
    m_fuseOper.flag_nullpath_ok = 1;
#endif
}

FuseEntity::FuseEntity(FuseOper *oper) : FuseEntity()
{
    shared_ptr<FuseOper> ptr(oper);

    m_operHook = ptr;
}

FuseEntity::~FuseEntity() 
{
}




