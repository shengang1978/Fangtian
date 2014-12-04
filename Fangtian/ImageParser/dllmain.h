// dllmain.h : Declaration of module class.

class CImageParserModule : public ATL::CAtlDllModuleT< CImageParserModule >
{
public :
	DECLARE_LIBID(LIBID_ImageParserLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_IMAGEPARSER, "{5F3B8123-37F5-4FC7-94BC-393C536251D3}")
};

extern class CImageParserModule _AtlModule;
