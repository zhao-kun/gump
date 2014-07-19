#ifndef MP2EP_H_HEADER_INCLUDED_B84D1B37
#define MP2EP_H_HEADER_INCLUDED_B84D1B37

#include <mypkg.h>
#include <epack.h>

extern int epkg_to_mpkg(CEasyPkg &aInPkg, CMyPkg &aOutPkg);
extern int mpkg_to_epkg(CMyPkg &aInPkg, CEasyPkg &aOutPkg);

#endif 
