/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"

#define LOG_OPERATIONS 0

int64 nuiMetaPainter::Count = 0;
static nglCriticalSection gCountCS(nglString(__FILE__).Add(":").Add(__LINE__).GetChars());

// nuiMetaPainter:
nuiMetaPainter::nuiMetaPainter(nglContext* pContext)
: nuiPainter(pContext)
{
  mLastStateValid = false;
  mpCache = &mOperations;
  mNbDrawChild = 0;
  mNbDrawLayer = 0;
  mNbDrawArray = 0;
  mNbClearColor = 0;
  mNbOperations = 0;
  mDrawChildrenImmediat = false;
  mLastSize = -1;

  {
    nglCriticalSectionGuard g(gCountCS);
    Count++;
  }
//  printf("[NEW] MetaPainter Count: %lld %p\n", Count, this);
//  SetTrace(true);
#ifdef _DEBUG_
  mpDebugObjectRef = NULL;
#endif
  
}

nuiMetaPainter::~nuiMetaPainter()
{
  {
    nglCriticalSectionGuard g(gCountCS);
    Count--;
  }
//  printf("[DEL] MetaPainter Count: %lld %p\n", Count, this);

  Reset(NULL);
}

void nuiMetaPainter::DestroySurface(nuiSurface* pSurface)
{
}

void nuiMetaPainter::DestroyRenderArray(nuiRenderArray* pArray)
{
  NGL_ASSERT(0); //#TODO
}


void nuiMetaPainter::DestroyTexture(nuiTexture* pTexture)
{
}

void nuiMetaPainter::StoreOpCode(OpCode code)
{
  StoreInt((int32)code);
  mNbOperations++;
}

void nuiMetaPainter::StoreInt(int32 Val)
{
  if (mDummyMode)
    return;
  
  size_t pos = mOperations.size();
  mOperations.resize(pos + sizeof(Val));
  *(int32*)&(mOperations[pos]) = Val;
}

void nuiMetaPainter::StoreFloat(float Val)
{
  if (mDummyMode)
    return;
  
  size_t pos = mOperations.size();
  mOperations.resize(pos + sizeof(Val));
  *(float*)&(mOperations[pos]) = Val;
}

void nuiMetaPainter::StoreFloat(double Val)
{
  if (mDummyMode)
    return;
  
  size_t pos = mOperations.size();
  mOperations.resize(pos + sizeof(Val));
  *(double*)&(mOperations[pos]) = Val;
}

void nuiMetaPainter::StorePointer(void* pVal)
{
  if (mDummyMode)
    return;
  
  size_t pos = mOperations.size();
  mOperations.resize(pos + sizeof(void*));
  *(void**)&(mOperations[pos]) = pVal;
}

void nuiMetaPainter::StoreBuffer(const void* pBuffer, uint ElementSize, uint ElementCount)
{
  if (mDummyMode)
    return;

  size_t size = ElementSize * ElementCount;
  size_t pos = mOperations.size();
  mOperations.resize(pos + size);
  memcpy(&mOperations[pos], pBuffer, size);
}

nuiMetaPainter::OpCode nuiMetaPainter::FetchOpCode() const
{
  return (OpCode)FetchInt();
}

int32 nuiMetaPainter::FetchInt() const
{
  int32 tmp;
  NGL_ASSERT(mOperationPos + sizeof(tmp) <= mOperations.size());
  tmp = *(int32*)&mOperations[mOperationPos];
  mOperationPos += sizeof(tmp);
  return tmp;
}

void nuiMetaPainter::FetchFloat(double& rDouble) const
{
  NGL_ASSERT(mOperationPos + sizeof(rDouble) <= mOperations.size());
  rDouble = *(double*)&mOperations[mOperationPos];
  mOperationPos += sizeof(rDouble);
}

void nuiMetaPainter::FetchFloat(float& rFloat) const
{
  NGL_ASSERT(mOperationPos + sizeof(rFloat) <= mOperations.size());
  rFloat = *(float*)&mOperations[mOperationPos];
  mOperationPos += sizeof(rFloat);
}

void* nuiMetaPainter::FetchPointer() const
{
  void* tmp;
  NGL_ASSERT(mOperationPos + sizeof(tmp) <= mOperations.size());
  tmp = *(void**)&mOperations[mOperationPos];
  mOperationPos += sizeof(tmp);
  return tmp;
}

void nuiMetaPainter::FetchBuffer(void* pBuffer, uint ElementSize, uint ElementCount) const
{
  uint size = ElementSize * ElementCount;
  NGL_ASSERT(mOperationPos + size <= mOperations.size());
  memcpy(pBuffer, &mOperations[mOperationPos], size);
  mOperationPos += size;
}

// Rendering operations:
void nuiMetaPainter::StartRendering()
{
  StoreOpCode(eStartRendering);

  nuiPainter::StartRendering();
}

void nuiMetaPainter::BeginSession()
{
  StoreOpCode(eBeginSession);
}

void nuiMetaPainter::EndSession()
{
  StoreOpCode(eEndSession);
}

#if NUI_METAPAINTER_STATS
static int StateChanges = 0;
static int IgnoredStateChanges = 0;
#endif

void nuiMetaPainter::SetState(const nuiRenderState& rState, bool ForceApply)
{
  if (mDummyMode)
    return;
  
#if NUI_METAPAINTER_STATS
  StateChanges++;
#endif
  
  if (mLastStateValid && rState == mLastState)
  {
#if NUI_METAPAINTER_STATS
    IgnoredStateChanges++;
    printf("State changes %d (ignored: %d)\n", StateChanges, IgnoredStateChanges);
#endif
    return;
  }
  
  mLastStateValid = true;
  mLastState = rState;
  StoreOpCode(eSetState);
  StoreInt(mRenderStates.size());
  mRenderStates.push_back(rState);
  StoreInt(ForceApply?1:0);
}

void nuiMetaPainter::Clear(bool color, bool depth, bool stencil)
{
  StoreOpCode(eClear);
  int32 v = 0;
  if (color)
    v |= 1;
  if (depth)
    v |= 3;
  if (stencil)
    v |= 4;
  StoreInt(v);
  mNbClearColor++;
}

void nuiMetaPainter::DrawArray(nuiRenderArray* pRenderArray)
{
  if (mDummyMode)
    return;
  StoreOpCode(eDrawArray);
  StoreInt(mRenderArrays.size());
  mRenderArrays.push_back(pRenderArray);

  mNbDrawArray++;
  mRenderOperations++;
  mBatches++;
  mVertices += pRenderArray->GetSize();
}


void nuiMetaPainter::DrawWidget(nuiDrawContext* pContext, nuiWidget* pWidget)
{
  if (mDrawChildrenImmediat)
  {
//    pWidget->DrawWidget(pContext);
//    const nuiMetaPainter* pPainter = pWidget->GetRenderCache();
//    pPainter->ReDraw(pContext, nullptr, nullptr);
    NGL_ASSERT(0);
  }
  else
  {
    StoreOpCode(eDrawWidget);
    StorePointer(pWidget);
    mWidgets.push_back(pWidget);
  }
  mLastStateValid = false;
  mNbDrawChild++;
}

void nuiMetaPainter::DrawLayer(nuiDrawContext* pContext, nuiLayer* pLayer)
{
  if (mDrawChildrenImmediat)
  {
    pLayer->Draw(pContext);
  }
  else
  {
    StoreOpCode(eDrawLayer);
    StorePointer(pLayer);
  }
  mLastStateValid = false;
  mNbDrawLayer++;
}


void nuiMetaPainter::SetSurface(nuiSurface* pSurface)
{
  if (mDummyMode)
    return;

  if (pSurface)
  {
    pSurface->Acquire();
    mSurfaces.push_back(pSurface);
  }

  StoreOpCode(eSetSurface);
  StorePointer(pSurface);

}

void nuiMetaPainter::CreateSurface(nuiSurface* pSurface)
{
  if (mDummyMode)
    return;
  
  NGL_ASSERT(pSurface);

  pSurface->Acquire();
  mSurfaces.push_back(pSurface);
  
  StoreOpCode(eCreateSurface);
  StorePointer(pSurface);
  
}


void nuiMetaPainter::LoadMatrix(const nuiMatrix& rMatrix)
{
  if (mMatrixStack.top() == rMatrix)
    return;
  StoreOpCode(eLoadMatrix);
  StoreBuffer(rMatrix.Array, sizeof(nuiSize), 16);

  nuiPainter::LoadMatrix(rMatrix);
}

void nuiMetaPainter::MultMatrix(const nuiMatrix& rMatrix)
{
  StoreOpCode(eMultMatrix);
  StoreBuffer(rMatrix.Array, sizeof(nuiSize), 16);

  nuiPainter::MultMatrix(rMatrix);
}

void nuiMetaPainter::PushMatrix()
{
  StoreOpCode(ePushMatrix);

  nuiPainter::PushMatrix();
}

void nuiMetaPainter::PopMatrix()
{
  StoreOpCode(ePopMatrix);

  nuiPainter::PopMatrix();
}














void nuiMetaPainter::LoadProjectionMatrix(const nuiRect& rViewport, const nuiMatrix& rMatrix)
{
  StoreOpCode(eLoadProjectionMatrix);
  StoreFloat(rViewport.Left());
  StoreFloat(rViewport.Top());
  StoreFloat(rViewport.GetWidth());
  StoreFloat(rViewport.GetHeight());
  StoreBuffer(rMatrix.Array, sizeof(nuiSize), 16);  
  
  nuiPainter::LoadProjectionMatrix(rViewport, rMatrix);
}

void nuiMetaPainter::MultProjectionMatrix(const nuiMatrix& rMatrix)
{
  StoreOpCode(eMultProjectionMatrix);
  StoreBuffer(rMatrix.Array, sizeof(nuiSize), 16);
  
  nuiPainter::MultProjectionMatrix(rMatrix);
}

void nuiMetaPainter::PushProjectionMatrix()
{
  StoreOpCode(ePushProjectionMatrix);
  
  nuiPainter::PushProjectionMatrix();
}

void nuiMetaPainter::PopProjectionMatrix()
{
  StoreOpCode(ePopProjectionMatrix);
  
  nuiPainter::PopProjectionMatrix();
}









void nuiMetaPainter::PushClipping()
{
  StoreOpCode(ePushClipping);

  nuiPainter::PushClipping();
  mClippingDepth++;
}

void nuiMetaPainter::PopClipping()
{
  StoreOpCode(ePopClipping);

  nuiPainter::PopClipping();
  mClippingDepth--;
}

void nuiMetaPainter::Clip(const nuiRect& rRect)
{
  mLastStateValid = false;
  StoreOpCode(eClip);
  StoreFloat(rRect.mLeft);
  StoreFloat(rRect.mTop);
  StoreFloat(rRect.mRight);
  StoreFloat(rRect.mBottom);

  nuiPainter::Clip(rRect);
}

void nuiMetaPainter::ResetClipRect()
{
  mLastStateValid = false;
  StoreOpCode(eResetClipRect);

  nuiPainter::ResetClipRect();
}

void nuiMetaPainter::EnableClipping(bool set)
{
  mLastStateValid = false;
  StoreOpCode(eEnableClipping);
  StoreInt(set?1:0);

  nuiPainter::EnableClipping(set);
}


void nuiMetaPainter::AddBreakPoint()
{
  StoreOpCode(eBreak);
}

void nuiMetaPainter::AddPrint(const char* str)
{
  StoreOpCode(ePrint);
  int len = strlen(str) + 1;
  StoreInt(len);
  StoreBuffer(str, (uint32)len, 1);
}

void nuiMetaPainter::ReDraw(nuiDrawContext* pContext, const DrawWidgetDelegate& rDrawWidgetDelegate, const DrawLayerDelegate& rDrawLayerDelegate)
{
  NGL_ASSERT(!mClippingDepth);
  PartialReDraw(pContext, 0, mNbOperations, rDrawWidgetDelegate, rDrawLayerDelegate);
}

void nuiMetaPainter::Reset(nuiPainter const * pFrom)
{
  mOperationIndices.clear();
  mNbOperations = 0;
  mLastSize = -1;
  mOperationPos = 0;
  mLastStateValid = false;
  mNbDrawChild = 0;
  mNbDrawLayer = 0;
  mNbDrawArray = 0;
  mNbClearColor = 0;
  mRenderOperations = 0;
  mBatches = 0;
  mVertices = 0;
  
  mOperations.clear();
  mRenderStates.clear();
  for (uint32 i = 0; i < mRenderArrays.size(); i++)
    mRenderArrays[i]->Release();
  mRenderArrays.clear();

  for (auto surface : mSurfaces)
  {
//    NGL_OUT("nuiMetaPainter release surface %s %p\n", surface->GetObjectName().GetChars(), surface);
    surface->Release();
  }
  mSurfaces.clear();

  mpClippingStack = std::stack<nuiClipper>();
  mpClippingStack.push(nuiClipper(nuiRect(-1000000, -1000000, 2000000, 2000000), false));
  mMatrixStack = std::stack<nglMatrixf>();
  mMatrixStack.push(nuiMatrix());

  if (pFrom)
  {
    mpState = &pFrom->GetState();
//    bool clip = pFrom->GetClipRect(mpClippingStack.top(), false);

//    nuiMatrix m(pFrom->GetMatrix());
//    mpClippingStack.top().Move(-m.Elt.M14, -m.Elt.M24);
//    mpClippingStack.top().mEnabled = clip;
  }
    
  mWidgets.clear();
}


int32 nuiMetaPainter::GetNbOperations() const
{
  return mNbOperations;
}


void nuiMetaPainter::PartialReDraw(nuiDrawContext* pContext, int32 first, int32 last, const DrawWidgetDelegate& rDrawWidgetDelegate, const DrawLayerDelegate& rDrawLayerDelegate) const
{
#if LOG_OPERATIONS
  static int log_depth = 0;
  nglString logstart;
  for (int i = 0; i < log_depth; i++)
    logstart += "  ";
#endif

  nuiPainter* pPainter = pContext->GetPainter();
  mOperationPos = 0;
  size_t size = mOperations.size();
  
  const bool DoDrawChild = mNbDrawChild;
  const bool DoDrawLayer = mNbDrawLayer;
  const bool DoDrawArray = mNbDrawArray;
  const bool DoDrawSelf = DoDrawArray || mNbClearColor;
  if (!(DoDrawChild || DoDrawSelf || DoDrawLayer))
  {
//    for (int i = 0; i < GetNbOperations(); i++)
//    {
//      NGL_OUT("Skipped %d - %s\n", i, GetOperationDescription(i).GetChars());
//    }
    return;
  }
  
  int32 currentop = 0;
  while (mOperationPos < size && currentop < last)
  {
    bool draw = currentop >= first;

    int32 index = mOperationPos;
#if LOG_OPERATIONS
    {
      if (!index)
      {

        printf("nuiMetaPainter %p %s--------------------------------------- %s\n", this, logstart.GetChars(), GetName().GetChars());
      }
      nglString str(GetCurrentOperationDescription());
      printf("nuiMetaPainter %p %s%s\n", this, logstart.GetChars(), str.GetChars());
    }
#endif
    mOperationPos = index;


    OpCode code = FetchOpCode();
    switch (code)
    {
      case eStartRendering:
        if (draw)
        {
          pPainter->StartRendering();
        }
        break;
      case eSetState:
        {
          int32 index = FetchInt();
          const nuiRenderState& rState(mRenderStates[index]);
          bool ForceApply = FetchInt() ? true : false;
          if (draw)
          {
            if (DoDrawSelf)
              pPainter->SetState(rState, ForceApply);
          }
        }
        break;
      case eDrawArray:
        {
          int32 index = FetchInt();
          nuiRenderArray* pRenderArray = mRenderArrays[index];
          if (draw)
          {
            pRenderArray->Acquire(); // Pre acquire the render array as the painter will release it
            pPainter->DrawArray(pRenderArray);
          }
        }
        break;
      case eClear:
        {
          int32 v = FetchInt();
          bool color = v & 1;
          bool depth = v & 2;
          bool stencil = v & 4;
          if (draw)
            pPainter->Clear(color, depth, stencil);
        }
        break;
      case eBeginSession:
        if (draw)
          if (DoDrawSelf)
            pPainter->BeginSession();
        break;
      case eEndSession:
        if (draw)
          if (DoDrawSelf)
            pPainter->EndSession();
        break;
      case eDrawWidget:
        {
#if LOG_OPERATIONS
          log_depth++;
#endif
          nuiWidget* pWidget = (nuiWidget*)FetchPointer();
          if (draw)
          {
            if (rDrawWidgetDelegate)
              rDrawWidgetDelegate(pContext, pWidget);
            else
            {
              nuiRef<nuiMetaPainter> pPainter = pWidget->GetRenderCache();
              if (pPainter)
              {
                pPainter->ReDraw(pContext, rDrawWidgetDelegate, rDrawLayerDelegate);
              }
            }
          }
#if LOG_OPERATIONS
          log_depth--;
#endif
        }
        break;
      case eDrawLayer:
        {
#if LOG_OPERATIONS
          log_depth++;
#endif
          nuiLayer* pLayer = (nuiLayer*)FetchPointer();
          if (draw)
          {
            if (rDrawLayerDelegate)
              rDrawLayerDelegate(pContext, pLayer);
            else
              pLayer->Draw(pContext);
          }
        }
#if LOG_OPERATIONS
        log_depth--;
#endif
        break;
      case eSetSurface:
        {
          nuiSurface* pSurface = (nuiSurface*)FetchPointer();
          if (draw)
            pPainter->SetSurface(pSurface);
        }
        break;
      case eCreateSurface:
        {
          nuiSurface* pSurface = (nuiSurface*)FetchPointer();
          if (draw)
            pPainter->CreateSurface(pSurface);
        }
        break;
      case eLoadMatrix:
        {
          nuiMatrix m;
          FetchBuffer(m.Array, sizeof(nuiSize), 16);
          if (draw)
            pPainter->LoadMatrix(m);
        }
        break;
      case eMultMatrix:
        {
          nuiMatrix m;
          FetchBuffer(m.Array, sizeof(nuiSize), 16);
          if (draw)
            pPainter->MultMatrix(m);
        }
        break;
      case ePopMatrix:
        if (draw)
          pPainter->PopMatrix();
        break;
      case ePushMatrix:
        if (draw)
          pPainter->PushMatrix();
        break;
        
        
        
        
        
      case eLoadProjectionMatrix:
        {
          nuiMatrix m;
          float Left, Top, Width, Height;
          FetchFloat(Left);
          FetchFloat(Top);
          FetchFloat(Width);
          FetchFloat(Height);
          FetchBuffer(m.Array, sizeof(nuiSize), 16);
          if (draw)
          {
            pPainter->LoadProjectionMatrix(nuiRect(Left, Top, Width, Height), m);
          }
        }
        break;
      case eMultProjectionMatrix:
        if (draw)
        {
          nuiMatrix m;
          FetchBuffer(m.Array, sizeof(nuiSize), 16);
          pPainter->MultProjectionMatrix(m);
        }
        break;
      case ePopProjectionMatrix:
        if (draw)
          pPainter->PopProjectionMatrix();
        break;
      case ePushProjectionMatrix:
        if (draw)
          pPainter->PushProjectionMatrix();
        break;
        
        
        
        
        
        
      case ePushClipping:
        if (draw)
          pPainter->PushClipping();
        break;
      case ePopClipping:
        if (draw)
          pPainter->PopClipping();
        break;
      case eClip:
        {
          nuiSize x;
          nuiSize y;
          nuiSize xx;
          nuiSize yy;
          FetchFloat(x);
          FetchFloat(y);
          FetchFloat(xx);
          FetchFloat(yy);
          if (draw)
          {
            const nuiRect r(x, y, xx, yy, false);
            pPainter->Clip(r);
          }
        }
        break;
      case eEnableClipping:
        if (draw)
          pPainter->EnableClipping(FetchInt()?true:false);
        break;
      case eResetClipRect:
        if (draw)
          pPainter->ResetClipRect();
        break;
      case eBreak:
        if (draw)
        {
//          NGL_ASSERT(false);
          NGL_OUT("nuiMetaPainter Break %p %s\n", this, GetName().GetChars());
        }
        break;
      case ePrint:
        if (draw)
        {
          int len = FetchInt();
          char str[len];
          FetchBuffer(str, (uint32)len, 1);
          NGL_OUT("painter print: %s\n", str);
        }
        break;
      default:
        if (draw)
          NGL_ASSERT(false);
        break;
    }
    
    currentop++;
  }
}

static const nglChar* GetGLMode(GLenum mode)
{
  switch (mode)
  {
    case GL_POINTS: return "GL_POINTS";
    case GL_LINES: return "GL_LINES";
    case GL_LINE_LOOP: return "GL_LINE_LOOP";
    case GL_LINE_STRIP: return "GL_LINE_STRIP";
    case GL_TRIANGLES: return "GL_TRIANGLES";
    case GL_TRIANGLE_FAN: return "GL_TRIANGLE_FAN";
    case GL_TRIANGLE_STRIP: return "GL_TRIANGLE_STRIP";
#ifndef _OPENGL_ES_
    case GL_QUADS: return "GL_QUADS";
    case GL_QUAD_STRIP: return "GL_QUAD_STRIP";
    case GL_POLYGON: return "GL_POLYGON";
#endif
  }
  return "unknown";
}

nglString nuiMetaPainter::GetOperationDescription(int32 OperationIndex) const
{
  mOperationPos = GetOffsetFromOperationIndex(OperationIndex);
  return nglString().Add(OperationIndex).Add(GetCurrentOperationDescription());
}

nglString nuiMetaPainter::GetCurrentOperationDescription() const
{
  nglString str = "???";

  OpCode code = FetchOpCode();
  switch (code)
  {
    case eStartRendering:
      {
        nuiSize x, y;
        FetchFloat(x);
        FetchFloat(y);
        str.CFormat("StartRendering(%f, %f)", x, y);
      }
      break;
    case eSetState:
      {
        FetchInt();
        bool force = FetchInt();
        str.CFormat("SetState(%s)", TRUEFALSE(force));
      }
      break;
    case eDrawArray:
      {
        int32 index = FetchInt();
        nuiRenderArray* pArray = mRenderArrays[index];
        const nglChar* pMode = GetGLMode(pArray->GetMode());
        float bounds[6];
        pArray->GetBounds(bounds);
        str.CFormat("DrawArray 0x%x (size %d mode:%s) (%f , %f)->(%f, %f)", pArray, pArray->GetVertices().size(), pMode, bounds[0], bounds[1], bounds[3], bounds[4]);
      }
      break;
    case eClear:
      {
        int32 v = FetchInt();
        bool color = v & 1;
        bool depth = v & 2;
        bool stencil = v & 4;
        str.CFormat("Clear(color = %s, depth = %s, stencil = %s)", TRUEFALSE(color), TRUEFALSE(depth), TRUEFALSE(stencil));
      }
      break;
    case eBeginSession:
      str = "BeginSession";
      break;
    case eEndSession:
      str = "EndSession";
      break;
    case eDrawWidget:
      {
        nuiWidget* pS = (nuiWidget*)FetchPointer();
//        nglString clss(pS->GetObjectClass());
//        nglString name(pS->GetObjectName());
//        str.CFormat("DrawWidget 0x%x / '%s - %s'", pS, clss.GetChars(), name.GetChars());
        str.CFormat("DrawWidget %p", pS);
      }
      break;
    case eDrawLayer:
      {
        nuiLayer* pS = (nuiLayer*)FetchPointer();
//        nglString clss(pS->GetObjectClass());
//        nglString name(pS->GetObjectName());
//        str.CFormat("DrawLayer 0x%x / '%s - %s'", pS, clss.GetChars(), name.GetChars());
        str.CFormat("DrawLayer %p", pS);
      }
      break;
    case eSetSurface:
      {
        nuiSurface* pS = (nuiSurface*)FetchPointer();
        if (pS)
        {
//          nglString clss(pS->GetObjectClass());
//          nglString name(pS->GetObjectName());
          str.CFormat("SetSurface %p", pS);
        }
        else
        {
          str.CFormat("SetSurface NULL");
        }
      }
      break;
    case eCreateSurface:
    {
      nuiSurface* pS = (nuiSurface*)FetchPointer();
      NGL_ASSERT(pS);
      nglString clss(pS->GetObjectClass());
      nglString name(pS->GetObjectName());
      str.CFormat("CreateSurface 0x%x / '%s - %s'", pS, clss.GetChars(), name.GetChars());
    }
      break;
    case eLoadMatrix:
      {
        nuiMatrix m;
        FetchBuffer(m.Array, sizeof(nuiSize), 16);
        nglString v;
        m.GetValue(v);
        str = "LoadMatrix" + v;
      }
      break;
    case eMultMatrix:
      {
        nuiMatrix m;
        FetchBuffer(m.Array, sizeof(nuiSize), 16);
        nglString v;
        m.GetValue(v);
        str = "MultMatrix" + v;
      }
      break;
    case ePopMatrix:
      str = "PopMatrix";
      break;
    case ePushMatrix:
      str = "PushMatrix";
      break;
      
      
    case eLoadProjectionMatrix:
      {
        nuiMatrix m;
        nuiSize a, b, c, d;
        FetchFloat(a);
        FetchFloat(b);
        FetchFloat(c);
        FetchFloat(d);
        FetchBuffer(m.Array, sizeof(nuiSize), 16);
        nglString v;
        m.GetValue(v);
        str.CFormat("LoadProjectionMatrix(%f, %f, %f, %f) / %s", a, b, c, d, v.GetChars());
      }
      break;
    case eMultProjectionMatrix:
      {
        nuiMatrix m;
        FetchBuffer(m.Array, sizeof(nuiSize), 16);
        nglString v;
        m.GetValue(v);
        str = "MultProjectionMatrix" + v;
      }
      break;
    case ePopProjectionMatrix:
      str = "PopProjectionMatrix";
      break;
    case ePushProjectionMatrix:
      str = "PushProjectionMatrix";
      break;
      
      
      
      
    case ePushClipping:
      str = "PushClipping";
      break;
    case ePopClipping:
      str = "PopClipping";
      break;
    case eClip:
      {
        nuiSize a, b, c, d;
        FetchFloat(a);
        FetchFloat(b);
        FetchFloat(c);
        FetchFloat(d);
        str.CFormat("Clip(%f, %f, %f, %f)", a, b, c, d);
      }
      break;
    case eResetClipRect:
      str = "ResetClipRect";
      break;
    case eEnableClipping:
      str.CFormat("EnableClipping(%s)", TRUEFALSE(FetchInt()));
      break;
    case eBreak:
      str = "Break";
      break;
    case ePrint:
    {
      int len = FetchInt();
      char _str[len];
      FetchBuffer(_str, (uint32)len, 1);
      str.CFormat("Print '%s'", _str);
    }
      break;
    default:
      str.CFormat("Unknown operation %d", code);
      break;
  }
  
  return str;
}

void nuiMetaPainter::SetName(const nglString& rName)
{
  mName = rName;
}

const nglString& nuiMetaPainter::GetName() const
{
  return mName;
}

int32 nuiMetaPainter::GetOffsetFromOperationIndex(int32 index) const
{
  UpdateIndices();
  NGL_ASSERT(index < mOperationIndices.size());
  return mOperationIndices[index];
}


void nuiMetaPainter::UpdateIndices() const
{
  if (!mOperationIndices.empty()) // || mLastSize == mOperations.size())
    return;
  
  mOperationIndices.clear();
  mOperationPos = 0;
  uint size = mOperations.size();
  mLastSize = size;
  while (mOperationPos < size)
  {
    mOperationIndices.push_back(mOperationPos);
    OpCode code = FetchOpCode();
    switch (code)
    {
      case eStartRendering:
        break;
      case eSetState:
        FetchInt();
        FetchInt();
        break;
      case eDrawArray:
        FetchInt();
        break;
      case eClear:
        FetchInt();
        break;
      case eBeginSession:
        break;
      case eEndSession:
        break;
      case eDrawWidget:
        FetchPointer();
        break;
      case eDrawLayer:
        FetchPointer();
        break;
      case eSetSurface:
        FetchPointer();
        break;
      case eCreateSurface:
        FetchPointer();
        break;
      case eLoadMatrix:
      {
        nuiMatrix m;
        FetchBuffer(m.Array, sizeof(nuiSize), 16);
      }
        break;
      case eMultMatrix:
      {
        nuiMatrix m;
        FetchBuffer(m.Array, sizeof(nuiSize), 16);
      }
        break;
      case ePopMatrix:
        break;
      case ePushMatrix:
        break;
        
        
      case eLoadProjectionMatrix:
      {
        nuiMatrix m;
        nuiSize tmp;
        FetchFloat(tmp);
        FetchFloat(tmp);
        FetchFloat(tmp);
        FetchFloat(tmp);
        FetchBuffer(m.Array, sizeof(nuiSize), 16);
      }
        break;
      case eMultProjectionMatrix:
      {
        nuiMatrix m;
        FetchBuffer(m.Array, sizeof(nuiSize), 16);
      }
        break;
      case ePopProjectionMatrix:
        break;
      case ePushProjectionMatrix:
        break;
        
        
        
        
      case ePushClipping:
        break;
      case ePopClipping:
        break;
      case eClip:
      {
        nuiSize tmp;
        FetchFloat(tmp);
        FetchFloat(tmp);
        FetchFloat(tmp);
        FetchFloat(tmp);
      }
        break;
      case eResetClipRect:
        break;
      case eEnableClipping:
        FetchInt();
        break;
      case eBreak:
        break;
      case ePrint:
      {
        int len = FetchInt();
        char _str[len];
        FetchBuffer(_str, len, 1);
      }
        break;
      default:
        NGL_ASSERT(false);
        break;
    }
  }
}

void nuiMetaPainter::SetDrawChildrenImmediat(bool set)
{
  mDrawChildrenImmediat = set;
}

bool nuiMetaPainter::GetDrawChildrenImmediat() const
{
  return mDrawChildrenImmediat;
}

#ifdef _DEBUG_
void nuiMetaPainter::DBGSetReferenceObject(const nuiObject* pRef)
{
  mpDebugObjectRef = pRef;
  if (pRef)
  {
    mDebugObjectName = pRef->GetObjectName();
    mDebugObjectClass = pRef->GetObjectClass();
  }
  else
  {
    mDebugObjectName.Nullify();
    mDebugObjectClass.Nullify();
  }
}

//void nuiMetaPainter::OnAcquired() const ///< This callback is called when we are about to acquire this
//{
//  NGL_OUT("nuiMetaPainter::OnAcquire %p %d %s\n", this, GetRefCount(), GetName().GetChars());
//}
//
//void nuiMetaPainter::OnReleased() const ///< This callback is called when we are about to release this
//{
//  NGL_OUT("nuiMetaPainter::OnRelease %p %d %s\n", this, GetRefCount(), GetName().GetChars());
//}

int32 nuiMetaPainter::GetNbDrawChild() const
{
  return mNbDrawChild;
}

int32 nuiMetaPainter::GetNbDrawLayer() const
{
  return mNbDrawLayer;
}

int32 nuiMetaPainter::GetNbClearColor() const
{
  return mNbClearColor;
}



#endif

int32 nuiMetaPainter::GetNbDrawArray() const
{
  return mNbDrawArray;
}

const std::vector<nuiWidget*>& nuiMetaPainter::GetWidgets() const
{
  return mWidgets;
}

const std::vector<nuiSurface*>& nuiMetaPainter::GetSurfaces() const
{
    return mSurfaces;
}

void nuiMetaPainter::SetPriority(int64 priority)
{
  mPriority = priority;
}

int64 nuiMetaPainter::GetPriority() const
{
  return mPriority;
}

int32 nuiMetaPainter::GetCurrentWidth() const
{
  return 0;
}

int32 nuiMetaPainter::GetCurrentHeight() const
{
  return 0;
}

