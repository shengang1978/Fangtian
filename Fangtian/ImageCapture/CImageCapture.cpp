// CImageCapture.cpp : Implementation of CCImageCapture

#include "stdafx.h"
#include "CImageCapture.h"
#include "HikNetApi.h"

// CCImageCapture



STDMETHODIMP CCImageCapture::Init(void)
{
	HikNetApi_Init();

	return S_OK;
}


STDMETHODIMP CCImageCapture::Destory(void)
{
	// TODO: Add your implementation code here

	return S_OK;
}
