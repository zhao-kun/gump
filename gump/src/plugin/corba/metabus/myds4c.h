/* MetaDataSet£¨C”Ô—‘∑Á∏ÒAPI */

#ifdef __WIN32__
#define DECL_EXPORT_DLL 	__declspec(dllexport)
#else
#define DECL_EXPORT_DLL
#endif

#ifdef __cplusplus
extern "C"
{
#endif

	/* CDsBufReader API */
	DECL_EXPORT_DLL     void* dsbr_new();
	DECL_EXPORT_DLL     void dsbr_free(void *pdsbr);
	DECL_EXPORT_DLL     int 	dsbr_open(void *pdsbr, char* dsbuf, int len);
	DECL_EXPORT_DLL     int 	dsbr_fetch(void *pdsbr);
	DECL_EXPORT_DLL     int 	dsbr_get_fldnum(void *pdsbr);
	DECL_EXPORT_DLL     int 	dsbr_get_rownum(void *pdsbr);
	DECL_EXPORT_DLL     const char* dsbr_get_fldname(void *pdsbr, int fldno);
	DECL_EXPORT_DLL     char dsbr_get_fldtype(void *pdsbr, int fldno);
	DECL_EXPORT_DLL     int dsbr_get_fldlen(void *pdsbr, int fldno);
	DECL_EXPORT_DLL     const char* dsbr_get_errmsg(void *pdsbr);

	DECL_EXPORT_DLL     short dsbr_get_short(void *pdsbr, int fldno);
	DECL_EXPORT_DLL     short dsbr_get_short_byname(void *pdsbr, const char* fldname);
	DECL_EXPORT_DLL     unsigned short dsbr_get_ushort(void *pdsbr, int fldno);
	DECL_EXPORT_DLL     unsigned short dsbr_get_ushort_byname(void *pdsbr, const char* fldname);
	DECL_EXPORT_DLL     int dsbr_get_int(void *pdsbr, int fldno);
	DECL_EXPORT_DLL     int dsbr_get_int_byname(void *pdsbr, const char* fldname);
	DECL_EXPORT_DLL     unsigned int dsbr_get_uint(void *pdsbr, int fldno);
	DECL_EXPORT_DLL     unsigned int dsbr_get_uint_byname(void *pdsbr, const char* fldname);
	DECL_EXPORT_DLL     long dsbr_get_long(void *pdsbr, int fldno);
	DECL_EXPORT_DLL     long dsbr_get_long_byname(void *pdsbr, const char* fldname);
	DECL_EXPORT_DLL     unsigned long dsbr_get_ulong(void *pdsbr, int fldno);
	DECL_EXPORT_DLL     unsigned long dsbr_get_ulong_byname(void *pdsbr, const char* fldname);
	DECL_EXPORT_DLL     float dsbr_get_float(void *pdsbr, int fldno);
	DECL_EXPORT_DLL     float dsbr_get_float_byname(void *pdsbr, const char* fldname);
	DECL_EXPORT_DLL     double dsbr_get_double(void *pdsbr, int fldno);
	DECL_EXPORT_DLL     double dsbr_get_double_byname(void *pdsbr, const char* fldname);
	DECL_EXPORT_DLL     const char* dsbr_get_string(void *pdsbr, int fldno);
	DECL_EXPORT_DLL     const char* dsbr_get_string_byname(void *pdsbr, const char* fldname);

	/* CDsBufWriter API */
	DECL_EXPORT_DLL     void* dsbw_new();
	DECL_EXPORT_DLL     void 	dsbw_free(void *pdsbw);
	DECL_EXPORT_DLL     int 	dsbw_open(void *pdsbw, char aFD, char aRD);
	DECL_EXPORT_DLL     int 	dsbw_post(void *pdsbw);
	DECL_EXPORT_DLL     int 	dsbw_add_fldname(void *pdsbw, char* fldname);
	DECL_EXPORT_DLL     const char* dsbw_get_dsbuf(void *pdsbw);
	DECL_EXPORT_DLL     int dsbw_get_dslen(void *pdsbw);
	DECL_EXPORT_DLL     const char* dsbw_get_err(void *pdsbw);

	DECL_EXPORT_DLL	    int dsbw_add_int(void *pdsbw, int val);
	DECL_EXPORT_DLL	    int dsbw_add_uint(void *pdsbw, unsigned int val);
	DECL_EXPORT_DLL	    int dsbw_add_short(void *pdsbw, short val);
	DECL_EXPORT_DLL	    int dsbw_add_ushort(void *pdsbw, unsigned short val);
	DECL_EXPORT_DLL	    int dsbw_add_long(void *pdsbw, long val);
	DECL_EXPORT_DLL	    int dsbw_add_ulong(void *pdsbw, unsigned long val);
	DECL_EXPORT_DLL	    int dsbw_add_float(void *pdsbw, float val);
	DECL_EXPORT_DLL	    int dsbw_add_double(void *pdsbw, double val);
	DECL_EXPORT_DLL	    int dsbw_add_string(void *pdsbw, const char *val);

#ifdef __cplusplus
}
#endif
