#ifndef INCL_MYSEC
#define INCL_MYSEC

#include <mypub.h>

class CMySec
{
  public:

	DECL_EXPORT_DLL CMySec();
	DECL_EXPORT_DLL ~CMySec();

	DECL_EXPORT_DLL int genMac(const char *indata, int inlen);
	DECL_EXPORT_DLL int genDac(const char *indata, int inlen);
	DECL_EXPORT_DLL int chkMac(const char *indata, int inlen, const char *mac);
	DECL_EXPORT_DLL int chkDac(const char *indata, int inlen, const char *dac);
	DECL_EXPORT_DLL int genEncBlock(const char *indata, int inlen);
	DECL_EXPORT_DLL int genDecBlock(const char *indata, int inlen);
	DECL_EXPORT_DLL int genEncTxt(const char *indata, int inlen);
	DECL_EXPORT_DLL int genDecTxt(const char *indata, int inlen);

	DECL_EXPORT_DLL const char* getOutData();
	DECL_EXPORT_DLL int getOutLen();

  private:

    u_char* OutBuf;
	int OutLen;
	int BufLen;
};

#endif

