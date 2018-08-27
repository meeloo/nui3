#include "nui.h"

nuiSurfaceMap nuiSurface::mpSurfaces;
nglCriticalSection nuiSurface::mSurfacesCS(nglString(__FILE__).Add(":").Add(__LINE__).GetChars());

void nuiSurface::DumpSurfaces()
{
  nglCriticalSectionGuard gcs(mSurfacesCS);
  int i = 0;
  NGL_OUT("Dump %d surfaces\n", mpSurfaces.size());
  for (auto it : mpSurfaces)
  {
    nuiSurface* pSurface = it.second;
    NGL_ASSERT(pSurface != nullptr);
    
    NGL_OUT("%d nuiSurface %p (%d) %s %dx%d\n", i, pSurface, pSurface->GetRefCount(), pSurface->GetObjectName().GetChars(), pSurface->GetWidth(), pSurface->GetHeight());
    i++;
    
  }
}


nuiSurface* nuiSurface::GetSurface (const nglString& rName, bool Acquired)
{
  nglCriticalSectionGuard gcs(mSurfacesCS);
  nuiSurface* pSurface = NULL;

  nuiSurfaceMap::const_iterator it = mpSurfaces.find(rName);
  if (it != mpSurfaces.end())
  {
    pSurface = it->second;
    if (!Acquired)
      pSurface->Acquire();
  }
  return pSurface;
}

nuiSurface* nuiSurface::CreateSurface (const nglString& rName, int32 Width, int32 Height, nglImagePixelFormat PixelFormat, float scale)
{
  nglCriticalSectionGuard gcs(mSurfacesCS);
  nuiSurface* pSurface = NULL;
  //NGL_OUT("nuiSurface::CreateSurface(%s, %.1f, %.1f)\n", rName.GetChars(), Width, Height);
  nuiSurfaceMap::const_iterator it = mpSurfaces.find(rName);
  if (it != mpSurfaces.end())
  {
    pSurface = it->second;
    if (pSurface->GetWidth() == Width && pSurface->GetHeight() == Height && pSurface->GetPixelFormat() == PixelFormat)
    {
      pSurface->Acquire();
      return pSurface;
    }
  }

  pSurface = new nuiSurface(rName, Width, Height, PixelFormat, scale);
  mpSurfaces[rName] = pSurface;

//  NGL_OUT("nuiSurface CreateSurface [0x%x] NAME: [%s] COUNT [%d]\n", pSurface, rName.GetChars(), mpSurfaces.size());

//  DumpSurfaces();
  return pSurface;
}

nuiSurface::nuiSurface(const nglString& rName, int32 Width, int32 Height, nglImagePixelFormat PixelFormat, float scale)
  : nuiObject()
{
  SetObjectClass("nuiSurface");
  SetObjectName(rName);
  static bool tr = false;
  if (tr)
  {
    mTrace = true;
    tr = false;
  }
  mPermanent = false;
  mWidth = Width;
  mHeight= Height;
  mScale = scale;
  mPixelFormat = PixelFormat;
  mDepth = 0;
  mStencil = 0;
  mRenderToTexture = true;
  mpTexture = NULL;
  mDirty = true;
  
  mpTexture = nuiTexture::GetTexture(this);
  
  //NGL_OUT("nuiSurface CTOR [0x%x] SIZE[%dx%d]\n", this, Width, Height);
  //SetTrace(true);
  
//  DumpSurfaces();
}

nuiSurface::~nuiSurface()
{
  {
  nglCriticalSectionGuard gcs(mSurfacesCS);
  mpSurfaces.erase(GetObjectName());
  }

  if (mpTexture)
  {
    mpTexture->DetachSurface();
    mpTexture->Release();
  }
  //NGL_OUT("nuiSurface DTOR [0x%x] NAME: [%s] COUNT [%d]\n", this, GetObjectName().GetChars(), mpSurfaces.size());

  nuiPainter::BroadcastDestroySurface(this);
  
//  DumpSurfaces();
}

int32 nuiSurface::GetWidth() const
{
  return mWidth;
}

int32 nuiSurface::GetHeight() const
{
  return mHeight;
}

nglImagePixelFormat nuiSurface::GetPixelFormat() const
{
  return mPixelFormat;
}


void nuiSurface::SetDepth(int32 bits)
{
  mDepth = bits;
}

int32 nuiSurface::GetDepth() const
{
  return mDepth;
}

int32 nuiSurface::GetStencil() const
{
  return mStencil;
}
void nuiSurface::SetStencil(int32 bits)
{
  mStencil = bits;
}

bool nuiSurface::GetRenderToTexture() const
{
  return mRenderToTexture;
}

nuiTexture* nuiSurface::GetTexture() const
{
  return mpTexture;
}

float nuiSurface::GetScale() const
{
  if (mScale != 0)
    return mScale;
  return nuiGetScaleFactor();
}

void nuiSurface::SetPermanent(bool Permanent)
{
  if (Permanent)
  {
    if (mPermanent)
      return;
    Acquire();
    mPermanent = true;
  }
  else
  {
    if (!mPermanent)
      return;
    mPermanent = false;
    Release();
  }
}

bool nuiSurface::IsPermanent()
{
  return mPermanent;
}

