// dllmain.h : Declaration of module class.

class CImageCaptureModule : public ATL::CAtlDllModuleT< CImageCaptureModule >
{
public :
	DECLARE_LIBID(LIBID_ImageCaptureLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_IMAGECAPTURE, "{4B9B59CC-81E0-4E98-A245-7757388F21AA}")
};

extern class CImageCaptureModule _AtlModule;
