/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot

 licence: see nui3/LICENCE.TXT
 */


#include "nui.h"
#import <Metal/Metal.h>
#import <QuartzCore/CAMetalLayer.h>

#ifndef __NUI_NO_GL__

//#define NUI_RETURN_IF_RENDERING_DISABLED return;
#define NUI_RETURN_IF_RENDERING_DISABLED
//#define NUI_USE_GL_VERTEX_BUFFER
#define NUI_COMPLEX_SHAPE_THRESHOLD 6

//#define NUI_USE_ANTIALIASING
#ifdef NUI_USE_ANTIALIASING
#define NUI_USE_MULTISAMPLE_AA
#endif

static const char* TextureVertexColor_VTX =
SHADER_STRING
(
using namespace metal;

               
struct Transforms
{
  float4x4 SurfaceMatrix;
  float4x4 ModelViewMatrix;
  float4x4 ProjectionMatrix;
  float4 Offset;
  float2 TextureTranslate;
  float2 TextureScale;
};

 struct InputVertex
{
  float4 mPosition;
  float4 mColor;
  float4 mNormal;
  float2 mTexCoord;
};

struct Vertex
{
  float4 Position [[position]];
  float4 Color;
  float2 TexCoord;
};
               
vertex Vertex vertex_main(
                          constant Transforms &transforms [[buffer(0)]],
                          constant InputVertex *vertices [[buffer(1)]],
                          uint vid [[vertex_id]])
{
  Vertex vert;

  vert.TexCoord = vertices[vid].mTexCoord * transforms.TextureScale + transforms.TextureTranslate;
  vert.Color = vertices[vid].mColor;
  vert.Position = (transforms.SurfaceMatrix * transforms.ProjectionMatrix * transforms.ModelViewMatrix * (vertices[vid].mPosition  + transforms.Offset));

  return vert;
}
               
fragment float4 fragment_main(Vertex vert [[stage_in]],
                              constant Transforms &transforms [[buffer(0)]],
                              texture2d<float> texture [[texture(0)]],
                              sampler textureSampler [[sampler(0)]]
                              )
{
  return vert.Color * texture.sample(textureSampler, vert.TexCoord);
}
);

static const char* TextureVertexColor_FGT =
SHADER_STRING
(
);

////////////////////////////////////////////////////////////////////////////////
static const char* TextureAlphaVertexColor_VTX =
SHADER_STRING
(
 using namespace metal;
 
 
 struct Transforms
{
  float4x4 SurfaceMatrix;
  float4x4 ModelViewMatrix;
  float4x4 ProjectionMatrix;
  float4 Offset;
  float2 TextureTranslate;
  float2 TextureScale;
};
 
 struct InputVertex
{
  float4 mPosition;
  float4 mColor;
  float4 mNormal;
  float2 mTexCoord;
};

 struct Vertex
{
  float4 Position [[position]];
  float4 Color;
  float2 TexCoord;
};
 
 vertex Vertex vertex_main(
                           constant Transforms &transforms [[buffer(0)]],
                           constant InputVertex *vertices [[buffer(1)]],
                           uint vid [[vertex_id]])
{
  Vertex vert;
  
  vert.TexCoord = vertices[vid].mTexCoord * transforms.TextureScale + transforms.TextureTranslate;
  vert.Color = vertices[vid].mColor;
  vert.Position = (transforms.SurfaceMatrix * transforms.ProjectionMatrix * transforms.ModelViewMatrix * (vertices[vid].mPosition  + transforms.Offset));
  
  return vert;
}
 
 fragment float4 fragment_main(Vertex vert [[stage_in]],
                               constant Transforms &transforms [[buffer(0)]],
                               texture2d<float> texture [[texture(0)]],
                               sampler textureSampler [[sampler(0)]]
                               )
{
  return vert.Color * texture.sample(textureSampler, vert.TexCoord)[3];
}
);

static const char* TextureAlphaVertexColor_FGT =
SHADER_STRING (
               );

//////////////////////////////////////////////////////////////////////////////////
static const char* TextureDifuseColor_VTX =
SHADER_STRING
(
using namespace metal;
 
struct Transforms
{
  float4x4 SurfaceMatrix;
  float4x4 ModelViewMatrix;
  float4x4 ProjectionMatrix;
  float4 Offset;
  float2 TextureTranslate;
  float2 TextureScale;
  float4 DifuseColor;
};
 
 struct InputVertex
{
  float4 mPosition;
  float4 mColor;
  float4 mNormal;
  float2 mTexCoord;
};

 struct Vertex
{
  float4 Position [[position]];
  float2 TexCoord;
};
 
vertex Vertex vertex_main(
                           constant Transforms &transforms [[buffer(0)]],
                          constant InputVertex *vertices [[buffer(1)]],
                           uint vid [[vertex_id]])
{
  Vertex vert;
  
  vert.TexCoord = vertices[vid].mTexCoord * transforms.TextureScale + transforms.TextureTranslate;
  vert.Position = (transforms.SurfaceMatrix * transforms.ProjectionMatrix * transforms.ModelViewMatrix * (vertices[vid].mPosition  + transforms.Offset));
  
  return vert;
}
 
fragment float4 fragment_main(Vertex vert [[stage_in]],
                              constant Transforms &transforms [[buffer(0)]],
                              texture2d<float> texture [[texture(0)]],
                              sampler textureSampler [[sampler(0)]]
                              )
{
  return transforms.DifuseColor * texture.sample(textureSampler, vert.TexCoord);
}

);

static const char* TextureDifuseColor_FGT =
SHADER_STRING
(
);

//////////////////////////////////////////////////////////////////////////////////
static const char* TextureAlphaDifuseColor_VTX =
SHADER_STRING
(
using namespace metal;
 
struct Transforms
{
  float4x4 SurfaceMatrix;
  float4x4 ModelViewMatrix;
  float4x4 ProjectionMatrix;
  float4 Offset;
  float2 TextureTranslate;
  float2 TextureScale;
  float4 DifuseColor;
};
 
 struct InputVertex
{
  float4 mPosition;
  float4 mColor;
  float4 mNormal;
  float2 mTexCoord;
};

struct Vertex
{
  float4 Position [[position]];
  float2 TexCoord;
};
 
vertex Vertex vertex_main(
                           constant Transforms &transforms [[buffer(0)]],
                          constant InputVertex *vertices [[buffer(1)]],
                           uint vid [[vertex_id]])
{
  Vertex vert;
  
  vert.TexCoord = vertices[vid].mTexCoord * transforms.TextureScale + transforms.TextureTranslate;
  vert.Position = (transforms.SurfaceMatrix * transforms.ProjectionMatrix * transforms.ModelViewMatrix * (vertices[vid].mPosition  + transforms.Offset));
  
  return vert;
}
 
fragment float4 fragment_main(Vertex vert [[stage_in]],
                               constant Transforms &transforms [[buffer(0)]],
                               texture2d<float> texture [[texture(0)]],
                               sampler textureSampler [[sampler(0)]]
                               )
{
  float v = texture.sample(textureSampler, vert.TexCoord)[3];
  return transforms.DifuseColor * float4(v, v, v, v);
}
 
);

static const char* TextureAlphaDifuseColor_FGT =
SHADER_STRING (
               );

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// No texture cases:
static const char* VertexColor_VTX =
SHADER_STRING
(
using namespace metal;

struct Transforms
{
  float4x4 SurfaceMatrix;
  float4x4 ModelViewMatrix;
  float4x4 ProjectionMatrix;
  float4 Offset;
};
               
 struct InputVertex
{
  float4 mPosition;
  float4 mColor;
  float4 mNormal;
  float2 mTexCoord;
};
 
struct Vertex
{
  float4 Position [[position]];
  float4 Color;
};
               
vertex Vertex vertex_main(
                           constant Transforms &transforms [[buffer(0)]],
                          constant InputVertex *vertices [[buffer(1)]],
                           uint vid [[vertex_id]])
{
  Vertex vert;
  
  vert.Color = vertices[vid].mColor;
  vert.Position = (transforms.SurfaceMatrix * transforms.ProjectionMatrix * transforms.ModelViewMatrix * (vertices[vid].mPosition  + transforms.Offset));
  
  return vert;
}
               
fragment float4 fragment_main(Vertex vert [[stage_in]])
{
  return vert.Color;
}
               
);

static const char* VertexColor_FGT =
SHADER_STRING (
);

//////////////////////////////////////////////////////////////////////////////////
static const char* DifuseColor_VTX =
SHADER_STRING
(
 using namespace metal;
 
 struct Transforms
{
  float4x4 SurfaceMatrix;
  float4x4 ModelViewMatrix;
  float4x4 ProjectionMatrix;
  float4 Offset;
  float4 DifuseColor;
};
 
 struct InputVertex
{
  float4 mPosition;
  float4 mNormal;
  float2 mTexCoord;
  uchar4 mColor;
};
 
 struct Vertex
{
  float4 Position [[position]];
};
 
 vertex Vertex vertex_main(
                           constant Transforms &transforms [[buffer(0)]],
                           constant InputVertex *vertices [[buffer(1)]],
                           uint vid [[vertex_id]])
{
  Vertex vert;
  
  vert.Position = (transforms.SurfaceMatrix * transforms.ProjectionMatrix * transforms.ModelViewMatrix * (vertices[vid].mPosition  + transforms.Offset));
  
  return vert;
}
 
 fragment float4 fragment_main(Vertex vert [[stage_in]],
                               constant Transforms &transforms [[buffer(0)]]
                               )
{
  return transforms.DifuseColor;
}
 
 );

static const char* DifuseColor_FGT =
SHADER_STRING (
               );

// Stats
static uint32 mins = 30000;
static uint32 maxs = 0;
static uint32 totalinframe = 0;
static uint32 total = 0;
static nglCriticalSection gStats(nglString(__FILE__).Add(":").Add(__LINE__).GetChars());

template <typename T>
static T MakePOT(T v)
{
  uint i;
  double val = 1;
  for (i = 0; i < 64; i++)
  {
    if (v <= val)
    {
      v = ToBelow(val);
      break;
    }
    val *= 2;
  }
  return v;
}



uint32 nuiMetalPainter::mActiveContexts = 0;

nuiMetalPainter::nuiMetalPainter(nglContext* pContext)
: nuiPainter(pContext), mRenderingCS("mRenderingCS")
{
  gpPainters.insert(this);

  mTwoPassBlend = false;
  mDefaultFramebuffer = 0;
  mDefaultRenderbuffer = 0;
  mForceApply = false;
  mClientVertex = false;
  mClientColor = false;
  mClientTexCoord = false;
  mMatrixChanged = true;
  mR = -1;
  mG = -1;
  mB = -1;
  mA = -1;
  mTexEnvMode = 0;

  mpContext = pContext;
  mActiveContexts++;

  mpShader_TextureVertexColor = nuiShaderProgram::GetProgram(mpContext, "TextureVertexColor");
  if (!mpShader_TextureVertexColor)
  {
    mpShader_TextureVertexColor = new nuiShaderProgram(mpContext, "TextureVertexColor");
    mpShader_TextureVertexColor->AddShader(eVertexShader, TextureVertexColor_VTX);
    mpShader_TextureVertexColor->AddShader(eFragmentShader, TextureVertexColor_FGT);
    mpShader_TextureVertexColor->Link();
//    mpShader_TextureVertexColor->GetCurrentState()->Set("Offset", 0.0f, 0.0f);
//    mpShader_TextureVertexColor->GetCurrentState()->Set("texture", 0);
  }

  mpShader_TextureAlphaVertexColor = nuiShaderProgram::GetProgram(mpContext, "TextureAlphaVertexColor");
  if (!mpShader_TextureAlphaVertexColor)
  {
    mpShader_TextureAlphaVertexColor = new nuiShaderProgram(mpContext, "TextureAlphaVertexColor");
    mpShader_TextureAlphaVertexColor->AddShader(eVertexShader, TextureAlphaVertexColor_VTX);
    mpShader_TextureAlphaVertexColor->AddShader(eFragmentShader, TextureAlphaVertexColor_FGT);
    mpShader_TextureAlphaVertexColor->Link();
//    mpShader_TextureAlphaVertexColor->GetCurrentState()->Set("Offset", 0.0f, 0.0f);
//    mpShader_TextureAlphaVertexColor->GetCurrentState()->Set("texture", 0);
  }

  mpShader_TextureDifuseColor = nuiShaderProgram::GetProgram(mpContext, "TextureDiffuseColor");
  if (!mpShader_TextureDifuseColor)
  {
    mpShader_TextureDifuseColor = new nuiShaderProgram(mpContext, "TextureDiffuseColor");
    mpShader_TextureDifuseColor->AddShader(eVertexShader, TextureDifuseColor_VTX);
    mpShader_TextureDifuseColor->AddShader(eFragmentShader, TextureDifuseColor_FGT);
    mpShader_TextureDifuseColor->Link();
//    mpShader_TextureDifuseColor->GetCurrentState()->Set("DifuseColor", nuiColor(255, 255, 255, 255));
//    mpShader_TextureDifuseColor->GetCurrentState()->Set("Offset", 0.0f, 0.0f);
//    mpShader_TextureDifuseColor->GetCurrentState()->Set("texture", 0);
  }

  mpShader_TextureAlphaDifuseColor = nuiShaderProgram::GetProgram(mpContext, "TextureAlphaDifuseColor");
  if (!mpShader_TextureAlphaDifuseColor)
  {
    mpShader_TextureAlphaDifuseColor = new nuiShaderProgram(mpContext, "TextureAlphaDifuseColor");
    mpShader_TextureAlphaDifuseColor->AddShader(eVertexShader, TextureAlphaDifuseColor_VTX);
    mpShader_TextureAlphaDifuseColor->AddShader(eFragmentShader, TextureAlphaDifuseColor_FGT);
    mpShader_TextureAlphaDifuseColor->Link();
//    mpShader_TextureAlphaDifuseColor->GetCurrentState()->Set("DifuseColor", nuiColor(255, 255, 255, 255));
//    mpShader_TextureAlphaDifuseColor->GetCurrentState()->Set("Offset", 0.0f, 0.0f);
//    mpShader_TextureAlphaDifuseColor->GetCurrentState()->Set("texture", 0);
  }

  mpShader_VertexColor = nuiShaderProgram::GetProgram(mpContext, "VertexColor");
  if (!mpShader_VertexColor)
  {
    mpShader_VertexColor = new nuiShaderProgram(mpContext, "VertexColor");
    mpShader_VertexColor->AddShader(eVertexShader, VertexColor_VTX);
    mpShader_VertexColor->AddShader(eFragmentShader, VertexColor_FGT);
    mpShader_VertexColor->Link();
//    mpShader_VertexColor->GetCurrentState()->Set("Offset", 0.0f, 0.0f);
  }

  mpShader_DifuseColor = nuiShaderProgram::GetProgram(mpContext, "DifuseColor");
  if (!mpShader_DifuseColor)
  {
    mpShader_DifuseColor = new nuiShaderProgram(mpContext, "DifuseColor");
    mpShader_DifuseColor->AddShader(eVertexShader, DifuseColor_VTX);
    mpShader_DifuseColor->AddShader(eFragmentShader, DifuseColor_FGT);
    mpShader_DifuseColor->Link();
//    mpShader_DifuseColor->GetCurrentState()->Set("DifuseColor", nuiColor(255, 255, 255, 255));
//    mpShader_DifuseColor->GetCurrentState()->Set("Offset", 0.0f, 0.0f);
  }
  
  mpCurrentRenderArrayInfo = NULL;
  mpLastArray = NULL;

}

nuiMetalPainter::~nuiMetalPainter()
{
  gpPainters.erase(this);

  mActiveContexts--;

  nglCriticalSectionGuard fag(mFrameArraysCS);
  for (auto pArray : mFrameArrays)
    pArray->Release();
  mFrameArrays.clear();

  mpShader_TextureVertexColor->Release();
  mpShader_TextureDifuseColor->Release();
  mpShader_TextureAlphaVertexColor->Release();
  mpShader_TextureAlphaDifuseColor->Release();
  mpShader_DifuseColor->Release();
  mpShader_VertexColor->Release();
}

void nuiMetalPainter::SetViewport()
{
  if (!mViewportChanged)
    return;

  GLint Width = GetCurrentWidth();
  GLint Height = GetCurrentHeight();
  const nuiRect& rViewport(mProjectionViewportStack.top());
  NGL_ASSERT(rViewport.Left() >= 0);
  NGL_ASSERT(rViewport.Top() >= 0);

  int32 x, y, w, h;
  
  nuiRect r(rViewport);
  if (r.GetWidth() == 0 || r.GetHeight() == 0)
    r.SetSize((float)Width, (float)Height);
//  NGL_DEBUG(NGL_OUT("nuiMetalPainter::SetViewPort(%d, %d) (%s)\n", Width, Height, r.GetValue().GetChars());)


  x = ToNearest(r.Left());
  y = Height - ToNearest(r.Bottom());
//  NGL_ASSERT(x >= 0);
//  NGL_ASSERT(y >= 0);

//  x = nuiClamp(x, 0, Width - 1);
//  y = nuiClamp(y, 0, Height - 1);

  w = ToNearest(r.GetWidth());
  h = ToNearest(r.GetHeight());

//  w = nuiClamp(w, 0, Width - x);
//  h = nuiClamp(h, 0, Height - y);

  {
    const float scale = mpContext->GetScale();
    x *= scale;
    y *= scale;
    w *= scale;
    h *= scale;
  }

//  NGL_ASSERT(x >= 0);
//  NGL_ASSERT(y >= 0);
//  Set Metal ViewPort to (x, y, w, h);
//  NGL_DEBUG(NGL_OUT("nuiMetalPainter::SetViewPort Actual(%d, %d, %d, %d)\n", x, y, w, h);)

  if (mpSurface)
  {
    mSurfaceMatrix.SetScaling(1.0f, -1.0f, 1.0f);
  }
  else
  {
    mSurfaceMatrix.SetIdentity();
  }
  
  float angle = GetAngle();
  //  if (angle != 0.0f)
  //  {
  //    mSurfaceMatrix.Rotate(angle, 0 ,0, 1);
  //  }

  mViewportChanged = false;
}




void nuiMetalPainter::StartRendering()
{
  nuiPainter::StartRendering();
  nglCriticalSectionGuard fag(mFrameArraysCS);
  for (auto pArray : mFrameArrays)
    pArray->Release();
  mFrameArrays.clear();
  
  ResetMetalState();
}

void nuiMetalPainter::ResetMetalState()
{
  BeginSession();
#ifdef DEBUG
  nuiMetalDebugGuard g("nuiMetalPainter::ResetMetalState");
#endif
  
  mScissorX = -1;
  mScissorY = -1;
  mScissorW = -1;
  mScissorH = -1;
  mScissorOn = false;
  
  
  //SetViewport();
  
//  glDisable(GL_DEPTH_TEST);
//  glDisable(GL_SCISSOR_TEST);
//  glDisable(GL_STENCIL_TEST);
//  glDisable(GL_BLEND);
//  glDisable(GL_ALPHA_TEST);
//  glDisable(GL_CULL_FACE);

//  glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
//  BlendFuncSeparate(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

  mMatrixChanged = true;
  mR = -1;
  mG = -1;
  mB = -1;
  mA = -1;
  mTexEnvMode = 0;
  
  //  mTextureTranslate = nglVector2f(0.0f, 0.0f);
  //  mTextureScale = nglVector2f(1, 1);
  
  mFinalState = nuiRenderState();
  mpState = &mDefaultState;
  mpLastArray = NULL;
}



void nuiMetalPainter::ApplyState(const nuiRenderState& rState, bool ForceApply)
{
#ifdef DEBUG
  nuiMetalDebugGuard g("nuiMetalPainter::ApplyState()");
#endif

  //TEST_FBO_CREATION();
  NUI_RETURN_IF_RENDERING_DISABLED;
  //ForceApply = true;

  id<MTLDevice> device = (id<MTLDevice>)mpContext->GetMetalDevice();
  
  id<MTLRenderCommandEncoder> encoder = (id<MTLRenderCommandEncoder>)mpContext->GetMetalCommandEncoder();

  // blending
  if (ForceApply || mFinalState.mBlending != rState.mBlending)
  {
    mFinalState.mBlending = rState.mBlending;
    if (mFinalState.mBlending)
    {
//      glEnable(GL_BLEND);
    }
    else
    {
//      glDisable(GL_BLEND);
    }
  }

  if (ForceApply || mFinalState.mBlendFunc != rState.mBlendFunc)
  {
    mFinalState.mBlendFunc = rState.mBlendFunc;
    GLenum src, dst;
//    nuiGetBlendFuncFactors(rState.mBlendFunc, src, dst);
//    BlendFuncSeparate(src, dst);
  }

  if (ForceApply || mFinalState.mDepthTest != rState.mDepthTest)
  {
    mFinalState.mDepthTest = rState.mDepthTest;
//    if (mFinalState.mDepthTest)
//      glEnable(GL_DEPTH_TEST);
//    else
//      glDisable(GL_DEPTH_TEST);
  }

  if (ForceApply || mFinalState.mDepthWrite != rState.mDepthWrite)
  {
    mFinalState.mDepthWrite = rState.mDepthWrite;
//    glDepthMask(mFinalState.mDepthWrite ? GL_TRUE : GL_FALSE);
  }

  if (ForceApply || mFinalState.mCulling != rState.mCulling)
  {
    mFinalState.mCulling = rState.mCulling;
//    if (mFinalState.mCulling)
//    {
//      glEnable(GL_CULL_FACE);
//    }
//    else
//    {
//      glDisable(GL_CULL_FACE);
//    }
  }

  if (ForceApply || mFinalState.mCullingMode != rState.mCullingMode)
  {
    mFinalState.mCullingMode = rState.mCullingMode;
//    glCullFace(mFinalState.mCullingMode);
  }


  // We don't care about the font in the lower layer of rendering
  //nuiFont* mpFont;
  //

  for (int i = 0; i < NUI_MAX_TEXTURE_UNITS; i++)
  {
    ApplyTexture(rState, ForceApply, i);
  }

  if (ForceApply || mFinalState.mpShader != mpShader)
  {
    if (mpShader)
      mpShader->Acquire();
    if (mFinalState.mpShader)
      mFinalState.mpShader->Release();

    mFinalState.mpShader = mpShader;
#if DEBUG
    mpShader->Validate();
#endif
//    glUseProgram(mpShader->GetProgram());
  }

  if (mpShaderState)
    mpShaderState->Acquire();
  if (mFinalState.mpShaderState)
    mFinalState.mpShaderState->Release();
  mFinalState.mpShaderState = mpShaderState;

  // Rendering buffers:
  if (ForceApply || mFinalState.mColorBuffer != rState.mColorBuffer)
  {
    mFinalState.mColorBuffer = rState.mColorBuffer;
    GLboolean m = mFinalState.mColorBuffer ? GL_TRUE : GL_FALSE;
//    glColorMask(m, m, m, m);
  }

  if (mpClippingStack.top().mEnabled || ForceApply)
  {
    uint32 width = GetCurrentWidth();
    uint32 height = GetCurrentHeight();

    nuiRect clip(mpClippingStack.top());

    int x,y,w,h;
    uint angle = (mpSurface && mpSurface->GetRenderToTexture()) ? 0 : mAngle;
    x = ToNearest(clip.Left());
    y = ToNearest(height - clip.Bottom());
    w = ToNearest(clip.GetWidth());
    h = ToNearest(clip.GetHeight());

    if (!mScissorOn || ForceApply)
    {
//      printf("Enable scissor test\n");
//      glEnable(GL_SCISSOR_TEST);
      mScissorOn = true;
    }

    if (mScissorX != x || mScissorY != y || mScissorW != w || mScissorH != h || ForceApply)
    {
      mScissorX = x;
      mScissorY = y;
      mScissorW = w;
      mScissorH = h;

      if (mpSurface)
      {
        y = mpSurface->GetHeight() - (y + h);
      }

      {
        float scale = mpContext->GetScale();
        x *= scale;
        y *= scale;
        w *= scale;
        h *= scale;
      }
      
//      if (mpSurface)
//      {
//        NGL_OUT("%p scissor: %d %d %d %d (%s)\n", mpSurface, x, y, w, h, mpSurface->GetObjectName().GetChars());
//      }
//      NGL_OUT("scissor: %d %d %d %d\n", x, y, w, h);
//      glScissor(x, y, w, h);
    }
  }
  else
  {
    if (mScissorOn || ForceApply)
    {
//      printf("Disable scissor test\n");
//      glDisable(GL_SCISSOR_TEST);
      mScissorOn = false;
    }
  }

  mFinalState.mClearColor = rState.mClearColor;
  mFinalState.mStrokeColor = rState.mStrokeColor;
  mFinalState.mFillColor = rState.mFillColor;

  mForceApply = false;
}

void nuiMetalPainter::SetState(const nuiRenderState& rState, bool ForceApply)
{
  NUI_RETURN_IF_RENDERING_DISABLED;

  mpState = &rState;
  mForceApply |= ForceApply;
}

void nuiMetalPainter::SetSize(uint32 w, uint32 h)
{
  NUI_RETURN_IF_RENDERING_DISABLED;
  if (mWidth == w && mHeight == h)
    return;

  NGL_DEBUG(NGL_LOG("painter", NGL_LOG_DEBUG, "nuiMetalPainter::SetSize(%d, %d)\n", w, h);)
//  NGL_OUT("GLPainter::SetSize %d x %d -> %d x %d\n", mWidth, mHeight, w, h);
  mWidth = w;
  mHeight = h;
  mViewportChanged = true;
}

void nuiMetalPainter::ApplyTexture(const nuiRenderState& rState, bool ForceApply, int slot)
{
#ifdef DEBUG
  nuiMetalDebugGuard g("nuiMetalPainter::ApplTexture()");
#endif

  NGL_ASSERT(slot < NUI_MAX_TEXTURE_UNITS);
  NGL_ASSERT(slot >= 0);
  
  // 2D Textures:
  nglCriticalSectionGuard tg(mTexturesCS);
  auto it = mTextures.end();
  nuiTexture* pTexture = rState.mpTexture[slot];
  if (pTexture)
  {
    it = mTextures.find(pTexture);
    if (pTexture->GetTrace())
    {
      NGL_OUT("Applying texture %s\n", pTexture->GetSource().GetChars());
    }
 }

  bool uptodate = false;
  if (it != mTextures.end())
  {
    if (!it->second.mReload && it->second.mTexture != nullptr)
    {
      uptodate = true;
    }
  }

  if (pTexture && !pTexture->IsUptoDate())
  {
    uptodate = false;
    if (it != mTextures.end())
    {
      it->second.mReload = true;
    }
  }

  if (ForceApply || (mFinalState.mpTexture[slot] != pTexture) || (mFinalState.mpTexture[slot] && !uptodate))
  {
    if (mFinalState.mpTexture[slot])
    {
      mFinalState.mpTexture[slot]->Release();
    }

    //NGL_OUT("Change texture to 0x%x (%s)\n", pTexture, pTexture?pTexture->GetSource().GetChars() : nglString::Empty.GetChars());
    mFinalState.mpTexture[slot] = pTexture ;

    if (mFinalState.mpTexture[slot])
    {
      mFinalState.mpTexture[slot]->Acquire();
      UploadTexture(mFinalState.mpTexture[slot], slot);
    }

    id<MTLCommandEncoder> commandEncoder = (id<MTLCommandEncoder>) mpContext->GetMetalCommandEncoder();
    [commandEncoder setFragmentTexture:(id<MTLTexture>)it->second.mTexture atIndex:slot];

    //NGL_OUT("Change texture type from 0x%x to 0x%x\n", outtarget, intarget);
  }
  else
  {
    mFinalState.mTexturing = rState.mTexturing;
  }
}



void nuiMetalPainter::Clear(bool color, bool depth, bool stencil)
{
  mRenderOperations++;
  NUI_RETURN_IF_RENDERING_DISABLED;

  SetViewport();
  ApplyState(*mpState, mForceApply);

//  glClearColor(mpState->mClearColor.Red(),mpState->mClearColor.Green(),mpState->mClearColor.Blue(),mpState->mClearColor.Alpha());
//#ifdef _OPENGL_ES_
//    glClearDepthf(mFinalState.mClearDepth);
//#else
//    glClearDepth(mFinalState.mClearDepth);
//#endif

//  GLint v = 0;
//  if (color)
//    v |= GL_COLOR_BUFFER_BIT;
//  if (depth)
//    v |= GL_DEPTH_BUFFER_BIT;
//  if (stencil)
//    v |= GL_STENCIL_BUFFER_BIT;

//  glClear(v);
}

MTLPrimitiveType nuiMTLPrimitiveTypeFromGL(GLenum type)
{
  switch (type)
  {
    case GL_POINTS:
      return MTLPrimitiveTypePoint;
    case GL_LINES:
      return MTLPrimitiveTypeLine;
    case GL_TRIANGLES:
      return MTLPrimitiveTypeTriangle;
    case GL_TRIANGLE_FAN:
      NGL_ASSERT(0); // Fans not supported in Metal
      break;
    case GL_TRIANGLE_STRIP:
      return MTLPrimitiveTypeTriangleStrip;
  }
  
  return MTLPrimitiveTypePoint;
}

#define LOGENUM(XXX) case XXX: { NGL_OUT("%s\n", #XXX); } break;

void nuiMetalPainter::DrawArray(nuiRenderArray* pArray)
{
  mRenderOperations++;
  mBatches++;
  
  if (!mEnableDrawArray)
  {
    pArray->Release();
    return;
  }

  SetViewport();

  if (pArray->GetDebug())
  {
    NGL_OUT("Texturing %s (%p)\n", YESNO(mFinalState.mTexturing), mFinalState.mpTexture[0]);
    pArray->Dump();
  }
  
#ifdef DEBUG
  nuiMetalDebugGuard g("nuiMetalPainter::DrawArray");
#endif

  static uint32 ops = 0;
  static uint32 skipped_ops = 0;
  {
    nglCriticalSectionGuard g(gStats);
    ops++;
  }

  const nuiMatrix& rM(GetMatrix());
  float bounds[6];
  pArray->GetBounds(bounds);
  
  if (rM.Elt.M11 == 1.0f
      && rM.Elt.M22 == 1.0f
      && rM.Elt.M33 == 1.0f
      && rM.Elt.M44 == 1.0f
      
      && rM.Elt.M12 == 0.0f
      && rM.Elt.M13 == 0.0f
      //&& rM.Elt.M14 == 0.0f
      
      && rM.Elt.M21 == 0.0f
      && rM.Elt.M23 == 0.0f
      //&& rM.Elt.M24 == 0.0f
      
      && rM.Elt.M31 == 0.0f
      && rM.Elt.M32 == 0.0f
      && rM.Elt.M34 == 0.0f
      
      && rM.Elt.M41 == 0.0f
      && rM.Elt.M42 == 0.0f
      && rM.Elt.M43 == 0.0f)
  {
    bounds[0] += rM.Elt.M14;
    bounds[1] += rM.Elt.M24;
    //bounds[2] += rM.Elt.M34;
    bounds[3] += rM.Elt.M14;
    bounds[4] += rM.Elt.M24;
    //bounds[5] += rM.Elt.M34;

    nuiClipper clip = mpClippingStack.top();
    if (clip.mEnabled && (
        (bounds[0] > clip.Right()) ||
        (bounds[1] > clip.Bottom()) ||
        (bounds[3] < clip.Left()) ||
        (bounds[4] < clip.Top())
        ))
    {
      pArray->Release();
      {
        nglCriticalSectionGuard g(gStats);
        skipped_ops++;

      //      #ifdef _DEBUG
      //      if (!(skipped_ops % 100))
      //        printf("optim (%d / %d) - %2.2f%%\n", skipped_ops, ops, (float)skipped_ops * 100.0f / (float)ops);
      //      #endif
      }
      return;
    }
  }

  // Shader selection:
  mpShader = mpState->mpShader;
  mpShaderState = mpState->mpShaderState;
  
  if (mpShader == NULL)
  {
    nuiShaderProgram* pShader = NULL;
    if (pArray->IsArrayEnabled(nuiRenderArray::eColor))
    {
      // Vertex Colors is on
      if (pArray->IsArrayEnabled(nuiRenderArray::eTexCoord))
      {
        // texture on
        if (mpState->mpTexture[0]->GetPixelFormat() == eImagePixelAlpha)
          pShader = mpShader_TextureAlphaVertexColor;
        else
          pShader = mpShader_TextureVertexColor;
      }
      else
      {
        pShader = mpShader_VertexColor;
      }
    }
    else
    {
      // Vertex color off -> Difuse Color
      if (pArray->IsArrayEnabled(nuiRenderArray::eTexCoord) && mpState->mpTexture[0])
      {
        // texture on
        if (mpState->mpTexture[0]->GetPixelFormat() == eImagePixelAlpha)
          pShader = mpShader_TextureAlphaDifuseColor;
        else
          pShader = mpShader_TextureDifuseColor;
      }
      else
      {
        pShader = mpShader_DifuseColor;
      }
    }
    
    pShader->Acquire();
    mpShader = pShader;
  }
  if (!mpShaderState)
    mpShaderState = mpShader->NewState();
  else
    mpShaderState->Acquire();

  NGL_ASSERT(mpShader != NULL);
  
  ApplyState(*mpState, mForceApply);
  pArray->Acquire();
  {
    nglCriticalSectionGuard fag(mFrameArraysCS);
    mFrameArrays.push_back(pArray);
  }
  
  
  if (mFinalState.mpTexture[0] && pArray->IsArrayEnabled(nuiRenderArray::eTexCoord))
  {
    if (mTextureScale[1] < 0)
    {
      NGL_ASSERT(mFinalState.mpTexture[0]->GetSurface() != NULL);
      //      printf("REVERSED SURFACE TEXTURE");
    }
//    mFinalState.mpShaderState->SetTextureTranslate(mTextureTranslate);
    //mTextureScale = nglVector2f(1,1);
//    mFinalState.mpShaderState->SetTextureScale(mTextureScale);
  }
  
//  mFinalState.mpShaderState->SetSurfaceMatrix(mSurfaceMatrix);
//  mFinalState.mpShaderState->SetProjectionMatrix(GetProjectionMatrix());
//  mFinalState.mpShaderState->SetModelViewMatrix(GetMatrix());
  
  uint32 s = pArray->GetSize();

  {
  nglCriticalSectionGuard g(gStats);
  total += s;
  totalinframe += s;
  mins = MIN(mins, s);
  maxs = MAX(maxs, s);
  }

  if (!s)
  {
    pArray->Release();
    return;
  }
  
  if (mpClippingStack.top().GetWidth() <= 0 || mpClippingStack.top().GetHeight() <= 0)
  {
    pArray->Release();
    return;
  }
  
  mVertices += s;
  GLenum mode = pArray->GetMode();
  
  NUI_RETURN_IF_RENDERING_DISABLED;
  
  if (mMatrixChanged)
  {
    mMatrixChanged = false;
  }
  
  
  bool NeedTranslateHack = pArray->IsShape() || ((mode == GL_POINTS || mode == GL_LINES || mode == GL_LINE_LOOP || mode == GL_LINE_STRIP) && !pArray->Is3DMesh());
  float hackX = 0;
  float hackY = 0;
  if (NeedTranslateHack)
  {
    const float ratio = mpContext->GetScaleInv() / 2.f;
#ifdef _UIKIT_
    hackX = ratio;
    hackY = ratio;
#else
    //    if (mAngle == 0)
    {
      hackX = ratio;
      hackY = ratio;
    }
    //    else if (mAngle == 90)
    //    {
    //      hackX = 0;
    //      hackY = ratio;
    //    }
    //    else if (mAngle == 180)
    //    {
    //      hackX = 0;
    //      hackY = 0;
    //    }
    //    else/*mAngle == 270*/
    //    {
    //      hackX = ratio;
    //      hackY = 0;
    //    }
#endif
  }
  
//  if (mpSurface)
//    mFinalState.mpShaderState->SetOffset(-hackX, -hackY);
//  else
//    mFinalState.mpShaderState->SetOffset(hackX, hackY);
  
  
  if (!pArray->IsArrayEnabled(nuiRenderArray::eColor))
  {
    nuiColor c;
    switch (pArray->GetMode())
    {
      case GL_POINTS:
      case GL_LINES:
      case GL_LINE_LOOP:
      case GL_LINE_STRIP:
        c = mFinalState.mStrokeColor;
        break;
        
      case GL_TRIANGLES:
      case GL_TRIANGLE_STRIP:
      case GL_TRIANGLE_FAN:
        c = mFinalState.mFillColor;
        break;
    }
    
    mR = c.Red();
    mG = c.Green();
    mB = c.Blue();
    mA = c.Alpha();
    
//    mFinalState.mpShaderState->SetDifuseColor(nuiColor(mR, mG, mB, mA));
  }
  
//  mFinalState.mpShader->SetState(*mFinalState.mpShaderState);
  
  if (pArray->IsStatic())
  {
    nglCriticalSectionGuard rag(mRenderArraysCS);
    auto it = mRenderArrays.find(pArray);
    if (it == mRenderArrays.end())
    {
      mRenderArrays[pArray] = RenderArrayInfo::Create(pArray);
      it = mRenderArrays.find(pArray);
    }
    NGL_ASSERT(it != mRenderArrays.end());
    mpCurrentRenderArrayInfo = it->second;
    RenderArrayInfo& rInfo(*it->second);
    
    if (mpLastArray != pArray)
    {
      SetVertexBuffersPointers(*pArray, rInfo);
      mpLastArray = pArray;
    }
  }
  else
  {
    if (mpLastArray != pArray)
    {
      if (mpCurrentRenderArrayInfo)
      {
        mpCurrentRenderArrayInfo = NULL;
      }
      SetVertexPointers(*pArray);
      mpLastArray = pArray;
    }
  }

  id<MTLDevice> device = (id<MTLDevice>)mpContext->GetMetalDevice();
  
  id<MTLRenderCommandEncoder> encoder = (id<MTLRenderCommandEncoder>)mpContext->GetMetalCommandEncoder();
  [encoder setVertexBytes:mFinalState.mpShaderState->GetStateData() length:mpShaderState->GetStateDataSize() atIndex:0];
  [encoder setVertexBytes:&pArray->GetVertex(0).mX length:pArray->GetSize()  atIndex:1];
  [encoder setFragmentBytes:mFinalState.mpShaderState->GetStateData() length:mpShaderState->GetStateDataSize() atIndex:0];

  NSError* err = nil;
  MTLRenderPipelineDescriptor* pipelineDesc = (MTLRenderPipelineDescriptor*)mFinalState.mpShader->NewMetalPipelineDescriptor(mFinalState);
  id<MTLRenderPipelineState> pipelineState = [device newRenderPipelineStateWithDescriptor:pipelineDesc error:&err];
  [encoder setRenderPipelineState:pipelineState];
  
  if (mpSurface && mTwoPassBlend && mFinalState.mBlending)
  {
//    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_FALSE);
    uint32 arraycount = pArray->GetIndexArrayCount();
    
    if (!arraycount)
    {
      MTLPrimitiveType primitiveType = nuiMTLPrimitiveTypeFromGL(mode);
      [encoder drawPrimitives:primitiveType vertexStart:0 vertexCount:s];
//      glDrawArrays(mode, 0, s);
    }
    else
    {
      for (uint32 i = 0; i < arraycount; i++)
      {
        nuiRenderArray::IndexArray& array(pArray->GetIndexArray(i));
//#if (defined _UIKIT_) || (defined _ANDROID_)
//        glDrawElements(array.mMode, (GLsizei)array.mIndices.size(), GL_UNSIGNED_SHORT, &(array.mIndices[0]));
//#else
//        glDrawElements(array.mMode, array.mIndices.size(), GL_UNSIGNED_INT, &(array.mIndices[0]));
//#endif
        MTLPrimitiveType primitiveType = nuiMTLPrimitiveTypeFromGL(array.mMode);
#if (defined _UIKIT_)
        id<MTLBuffer> indexes = [device newBufferWithBytes:&(array.mIndices[0]) length:array.mIndices.size()*2 options:0];
        MTLIndexType indexType = MTLIndexTypeUInt16;
#else
        id<MTLBuffer> indexes = [device newBufferWithBytes:&(array.mIndices[0]) length:array.mIndices.size()*4 options:0];
        MTLIndexType indexType = MTLIndexTypeUInt32;
#endif
        [encoder drawIndexedPrimitives:primitiveType indexCount:array.mIndices.size() indexType:indexType indexBuffer:indexes indexBufferOffset:0];
      }
    }

//    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_TRUE);
//    glBlendFunc(mSrcAlpha, mDstAlpha);
    if (!arraycount)
    {
//      glDrawArrays(mode, 0, s);
      MTLPrimitiveType primitiveType = nuiMTLPrimitiveTypeFromGL(mode);
      [encoder drawPrimitives:primitiveType vertexStart:0 vertexCount:s];
    }
    else
    {
      for (uint32 i = 0; i < arraycount; i++)
      {
        nuiRenderArray::IndexArray& array(pArray->GetIndexArray(i));
//#if (defined _UIKIT_) || (defined _ANDROID_)
//        glDrawElements(array.mMode, (GLsizei)array.mIndices.size(), GL_UNSIGNED_SHORT, &(array.mIndices[0]));
//#else
//        glDrawElements(array.mMode, array.mIndices.size(), GL_UNSIGNED_INT, &(array.mIndices[0]));
//#endif
        MTLPrimitiveType primitiveType = nuiMTLPrimitiveTypeFromGL(array.mMode);
#if (defined _UIKIT_)
        id<MTLBuffer> indexes = [device newBufferWithBytes:&(array.mIndices[0]) length:array.mIndices.size()*2 options:0];
        MTLIndexType indexType = MTLIndexTypeUInt16;
#else
        id<MTLBuffer> indexes = [device newBufferWithBytes:&(array.mIndices[0]) length:array.mIndices.size()*4 options:0];
        MTLIndexType indexType = MTLIndexTypeUInt32;
#endif
        [encoder drawIndexedPrimitives:primitiveType indexCount:array.mIndices.size() indexType:indexType indexBuffer:indexes indexBufferOffset:0];
      }
    }
//    glBlendFunc(mSrcColor, mDstColor);
//    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
  }
  else
  {
    uint32 arraycount = pArray->GetIndexArrayCount();
    
    if (!arraycount)
    {
//      glDrawArrays(mode, 0, s);
      MTLPrimitiveType primitiveType = nuiMTLPrimitiveTypeFromGL(mode);
      [encoder drawPrimitives:primitiveType vertexStart:0 vertexCount:s];
    }
    else
    {
      for (uint32 i = 0; i < arraycount; i++)
      {
        nuiRenderArray::IndexArray& array(pArray->GetIndexArray(i));
//#if (defined _UIKIT_) || (defined _ANDROID_)
//        glDrawElements(array.mMode, (GLsizei)array.mIndices.size(), GL_UNSIGNED_SHORT, &(array.mIndices[0]));
//#else
//        glDrawElements(array.mMode, array.mIndices.size(), GL_UNSIGNED_INT, &(array.mIndices[0]));
//#endif
        MTLPrimitiveType primitiveType = nuiMTLPrimitiveTypeFromGL(array.mMode);
#if (defined _UIKIT_)
        id<MTLBuffer> indexes = [device newBufferWithBytes:&(array.mIndices[0]) length:array.mIndices.size()*2 options:0];
        MTLIndexType indexType = MTLIndexTypeUInt16;
#else
        id<MTLBuffer> indexes = [device newBufferWithBytes:&(array.mIndices[0]) length:array.mIndices.size()*4 options:0];
        MTLIndexType indexType = MTLIndexTypeUInt32;
#endif
        [encoder drawIndexedPrimitives:primitiveType indexCount:array.mIndices.size() indexType:indexType indexBuffer:indexes indexBufferOffset:0];
      }
    }
  }
  
//  glBindVertexArray(0);
  
  //  ResetVertexPointers(*pArray);
  pArray->Release();
}


void nuiMetalPainter::BeginSession()
{
  NUI_RETURN_IF_RENDERING_DISABLED;
  mpContext->MakeCurrent();

  FinalizeSurfaces();
  FinalizeTextures();
  FinalizeRenderArrays();

  {
    nglCriticalSectionGuard g(gStats);
    totalinframe = 0;
  }
}

void nuiMetalPainter::EndSession()
{
  // Bleh!
  NUI_RETURN_IF_RENDERING_DISABLED;
  //NGL_OUT("min = %d max = %d total in frame = %d total = %d\n", mins, maxs, totalinframe, total);
}


nuiMetalPainter::TextureInfo::TextureInfo()
{
  mReload = false;
  mTexture = nullptr;
  mSampler = nullptr;
}

static inline MTLSamplerMinMagFilter nuiGLToMetaltextureFilter(GLenum val)
{
  switch (val)
  {
    case GL_NEAREST_MIPMAP_NEAREST:
      return MTLSamplerMinMagFilterNearest;
    case GL_LINEAR_MIPMAP_NEAREST:
      return MTLSamplerMinMagFilterLinear;
    case GL_NEAREST_MIPMAP_LINEAR:
      return MTLSamplerMinMagFilterNearest;
    case GL_LINEAR_MIPMAP_LINEAR:
      return MTLSamplerMinMagFilterLinear;
  }
  
  return MTLSamplerMinMagFilterLinear;
}

static inline MTLSamplerAddressMode nuiGLToMetaltextureAddressMode(GLenum val)
{
  switch (val)
  {
    case GL_CLAMP_TO_EDGE:
      return MTLSamplerAddressModeClampToEdge;
    case GL_CLAMP_TO_BORDER:
      return MTLSamplerAddressModeClampToBorderColor;
    case GL_CLAMP:
      return MTLSamplerAddressModeClampToEdge;
    case GL_REPEAT:
      return MTLSamplerAddressModeRepeat;
    case GL_MIRRORED_REPEAT:
      return MTLSamplerAddressModeMirrorRepeat;
  }
  
  return MTLSamplerAddressModeClampToEdge;
}

void nuiMetalPainter::UploadTexture(nuiTexture* pTexture, int slot)
{
#ifdef DEBUG
  nuiMetalDebugGuard g("nuiMetalPainter::UploadTexture()");
#endif

  nuiTexture* pProxy = pTexture->GetProxyTexture();
  if (pProxy)
    pTexture = pProxy;
  nuiSurface* pSurface = pTexture->GetSurface();

  float Width = pTexture->GetUnscaledWidth();
  float Height = pTexture->GetUnscaledHeight();

  bool changedctx = false;

  nglCriticalSectionGuard tg(mTexturesCS);
  std::map<nuiTexture*, TextureInfo>::iterator it = mTextures.find(pTexture);
  if (it == mTextures.end())
    it = mTextures.insert(mTextures.begin(), std::pair<nuiTexture*, TextureInfo>(pTexture, TextureInfo()));
  NGL_ASSERT(it != mTextures.end());

  TextureInfo& info(it->second);

  void* _id = (void*)pTexture->GetTextureID();
  if (_id)
  {
    info.mReload = false;
    info.mTexture = _id;
  }

  //NGL_OUT("Apply Target: 0x%x\n", target);
  nglImage* pImage = pTexture->GetImage();

  {
    bool firstload = false;
    bool reload = info.mReload;

    uint i;
    if (info.mTexture == nullptr)
    { // Generate a texture
      //      if (mpSharedContext)
      //      {
      //        mpSharedContext->MakeCurrent();
      //        changedctx = true;
      //      }

      //NGL_OUT("nuiMetalPainter::UploadTexture 0x%x : '%s' / %d\n", pTexture, pTexture->GetSource().GetChars(), info.mTexture);
      firstload = true;
      reload = true;
    }

    if (!pSurface && !(pImage && pImage->GetPixelSize()) && !_id)
      return;

    if (reload)
    {
      uint32 type = 8;
      MTLPixelFormat mtlPixelFormat = MTLPixelFormatInvalid;
      GLint pixelformat = 0;
      GLbyte* pBuffer = NULL;

      if (pImage)
      {
        type = pImage->GetBitDepth();
        pixelformat = pImage->GetPixelFormat();
        pBuffer = (GLbyte*)pImage->GetBuffer();

        //#ifndef NUI_IOS
        switch (type)
        {
          case 16:
//            mtlPixelFormat = MTLPixelFormatB5G6R5Unorm;
            NGL_ASSERT(0);
            break;
          case 15:
//            mtlPixelFormat = MTLPixelFormatBGR5A1Unorm;
            NGL_ASSERT(0);
            break;
          case 8:
            mtlPixelFormat = MTLPixelFormatA8Unorm;
            break;
          case 24:
            NGL_ASSERT(0);
            break;
          case 32:
            mtlPixelFormat = MTLPixelFormatRGBA8Unorm;
            break;
            
          default:
            NGL_ASSERT(0);
        }
      }
      else
      {
        NGL_ASSERT(pSurface);
        mtlPixelFormat = MTLPixelFormatRGBA8Unorm;
      }


      {
        // Handle mipmaps and pTexture->GetAutoMipMap()
          pTexture->ResetForceReload();

        // Give a name to the texture for Metal Debugging
        
        id<MTLDevice> device = (id<MTLDevice>)mpContext->GetMetalDevice();
        id<MTLTexture> texture = (id<MTLTexture>)info.mTexture;
        if (!texture)
        {
          MTLTextureDescriptor* descriptor = [MTLTextureDescriptor texture2DDescriptorWithPixelFormat:mtlPixelFormat
                                                                                                width:ToNearest(Width) height:ToNearest(Height) mipmapped:pTexture->GetAutoMipMap()];
          [device newTextureWithDescriptor:descriptor];
        }
        
        MTLRegion region = MTLRegionMake2D(0, 0, ToNearest(Width), ToNearest(Height));
        if (pBuffer)
        {
          [texture replaceRegion:region mipmapLevel:0 withBytes:pBuffer bytesPerRow:pTexture->GetImage()->GetBytesPerLine()];
        }
        info.mTexture = texture;
        MTLSamplerDescriptor *samplerDescriptor = [MTLSamplerDescriptor new]; samplerDescriptor.minFilter = nuiGLToMetaltextureFilter(pTexture->GetMinFilter());
        samplerDescriptor.magFilter = nuiGLToMetaltextureFilter(pTexture->GetMinFilter());
        samplerDescriptor.sAddressMode = nuiGLToMetaltextureAddressMode(pTexture->GetWrapS());
        samplerDescriptor.tAddressMode = nuiGLToMetaltextureAddressMode(pTexture->GetWrapT());

        id<MTLSamplerState> samplerState = [device newSamplerStateWithDescriptor:samplerDescriptor];
        info.mSampler = samplerState;
      }

      info.mReload = false;

#if (TARGET_IPHONE_SIMULATOR || TARGET_OS_IPHONE)
      if (!pTexture->IsBufferRetained())
      {
        pTexture->ReleaseBuffer();
      }
#endif

    }
  }

  mTextureTranslate = nglVector2f(0.0f, 0.0f);
//  mTextureScale = nglVector2f(rx, ry);
}

void nuiMetalPainter::DestroyTexture(nuiTexture* pTexture)
{
  nglCriticalSectionGuard g(mRenderingCS);
  mDestroyedTextures.push_back(pTexture);
}

void nuiMetalPainter::FinalizeTextures()
{
  for (auto texture: mDestroyedTextures)
    _DestroyTexture(texture);
  mDestroyedTextures.clear();
}

void nuiMetalPainter::_DestroyTexture(nuiTexture* pTexture)
{
  nglCriticalSectionGuard tg(mTexturesCS);
  std::map<nuiTexture*, TextureInfo>::iterator it = mTextures.find(pTexture);
  if (it == mTextures.end())
    return;
  NGL_ASSERT(it != mTextures.end());

  TextureInfo info(it->second);
  mTextures.erase(it);

  //NGL_OUT("nuiMetalPainter::DestroyTexture 0x%x : '%s' / %d\n", pTexture, pTexture->GetSource().GetChars(), info.mTexture);

  // TODO Actually Delete the Texture
  id<MTLTexture> texture = (id<MTLTexture>)info.mTexture;
  id<MTLSamplerState> sampler = (id<MTLSamplerState>)info.mSampler;

  if (texture)
    [texture release];
  if (sampler)
    [sampler release];
}

nuiMetalPainter::FramebufferInfo::FramebufferInfo()
{
  mReload = true;
  mFramebuffer = 0;
  mRenderbuffer = 0;
  mTexture = 0;
  mDepthbuffer = 0;
  mStencilbuffer = 0;
}

void nuiMetalPainter::DestroySurface(nuiSurface* pSurface)
{
  nglCriticalSectionGuard g(mRenderingCS);
  mDestroyedSurfaces.push_back(pSurface);
}

void nuiMetalPainter::FinalizeSurfaces()
{
  for (auto surface : mDestroyedSurfaces)
    _DestroySurface(surface);
  mDestroyedSurfaces.clear();
}

void nuiMetalPainter::_DestroySurface(nuiSurface* pSurface)
{
  nglCriticalSectionGuard fbg(mFramebuffersCS);
  std::map<nuiSurface*, FramebufferInfo>::iterator it = mFramebuffers.find(pSurface);
  
  if (it == mFramebuffers.end())
  {
    return;
  }
  FramebufferInfo info = it->second;

//  glBindFramebufferNUI(GL_FRAMEBUFFER_NUI, 0);
//  NGL_ASSERT(info.mFramebuffer > 0);

//  glDeleteFramebuffersNUI(1, (GLuint*)&info.mFramebuffer);

//  if (info.mRenderbuffer > 0)
//  {
//    glDeleteRenderbuffersNUI(1, (GLuint*)&info.mRenderbuffer);
//  }
//  if (info.mDepthbuffer > 0)
//  {
//    glDeleteRenderbuffersNUI(1, (GLuint*)&info.mDepthbuffer);
//  }
//  if (info.mStencilbuffer > 0)
//  {
//    glDeleteRenderbuffersNUI(1, (GLuint*)&info.mStencilbuffer);
//  }

//  NGL_OUT("nuiMetalPainter::DestroySurface %p (fbo %d tx %d)\n", pSurface, info.mFramebuffer, info.mTexture);
  mFramebuffers.erase(it);
}

void nuiMetalPainter::CreateSurface(nuiSurface* pSurface)
{
  //NGL_OUT("nuiMetalPainter::CreateSurface %p\n", pSurface);
  NGL_ASSERT(pSurface != nullptr);
  NGL_ASSERT(this != nullptr);

  nglCriticalSectionGuard fbg(mFramebuffersCS);
  std::map<nuiSurface*, FramebufferInfo>::const_iterator it = mFramebuffers.find(pSurface);
  bool create = (it == mFramebuffers.end()) ? true : false;
  if (!create)
    return;

  GLint width = (GLint)pSurface->GetWidth();
  GLint height = (GLint)pSurface->GetHeight();

  float scale = mpContext->GetScale();
  width *= scale;
  height *= scale;

  nuiTexture* pTexture = pSurface->GetTexture();

#if defined DEBUG && defined _UIKIT_
  if (pTexture)
    mpContext->StartMarkerGroup(pTexture->GetSource().GetChars());
  else
    mpContext->StartMarkerGroup(pSurface->GetObjectName().GetChars());
#endif

  if (pTexture && !pTexture->IsPowerOfTwo())
  {
    switch (GetRectangleTextureSupport())
    {
      case 0:
        width  = MakePOT(width);
        height = MakePOT(height);
        break;
      case 1:
      case 2:
        break;
    }
  }

  FramebufferInfo info;

  if (pTexture && pSurface->GetRenderToTexture())
  {
    mFinalState.mpTexture[0] = nullptr;
    
    nglCriticalSectionGuard tg(mTexturesCS);
    std::map<nuiTexture*, TextureInfo>::iterator it = mTextures.find(pTexture);
    if (it == mTextures.end())
      it = mTextures.insert(mTextures.begin(), std::pair<nuiTexture*, TextureInfo>(pTexture, TextureInfo()));
    NGL_ASSERT(it != mTextures.end());
    
    TextureInfo& tinfo(it->second);
    
//    glGenTextures(1, (GLuint*)&tinfo.mTexture);
//    info.mTexture = tinfo.mTexture;

    NGL_ASSERT(width > 0);
    NGL_ASSERT(height > 0);

//    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

//    NGL_OUT("init color surface texture %d x %d --> %d\n", width, height, tinfo.mTexture);
    
  }


//  glGenFramebuffersNUI(1, (GLuint*)&info.mFramebuffer);
//  NGL_OUT("glGenFramebuffersNUI %p '%s' -> %d (%d x %d)\n", pSurface, pSurface->GetObjectName().GetChars(), info.mFramebuffer, pSurface->GetWidth(), pSurface->GetHeight());
//  NGL_ASSERT(info.mFramebuffer);

#ifdef _UIKIT_
#ifdef NGL_DEBUG
//      if (pTexture)
//        glLabelObjectEXT(GL_BUFFER_OBJECT_EXT, info.mFramebuffer, 0, pTexture->GetSource().GetChars());
//      else
//        glLabelObjectEXT(GL_BUFFER_OBJECT_EXT, info.mFramebuffer, 0, pSurface->GetObjectName().GetChars());
#endif
#endif

  ///< We definetly need a color attachement, either a texture, or a renderbuffer
  if (pTexture && pSurface->GetRenderToTexture())
  {
//    glFramebufferTexture2DNUI(GL_FRAMEBUFFER_NUI, GL_COLOR_ATTACHMENT0_NUI, GL_TEXTURE_2D, info.mTexture, 0);

//    NGL_OUT("init color surface texture %d x %d --> %d\n", width, height, info.mTexture);
  }
  else
  {
//    glGenRenderbuffersNUI(1, (GLuint*)&info.mRenderbuffer);

//    glBindRenderbufferNUI(GL_RENDERBUFFER_NUI, info.mRenderbuffer);

    GLint pixelformat = pSurface->GetPixelFormat();
    GLint internalPixelformat = pSurface->GetPixelFormat();
    internalPixelformat = GL_RGBA;

//    glFramebufferRenderbufferNUI(GL_FRAMEBUFFER_NUI,
//                                 GL_COLOR_ATTACHMENT0_NUI,
//                                 GL_RENDERBUFFER_NUI,
//                                 info.mRenderbuffer);
    //NGL_OUT("surface render buffer -> %d\n", info.mRenderbuffer);
  }
  

  ///< Do we need a depth buffer
  if (pSurface->GetDepth())
  {
//    glGenRenderbuffersNUI(1, (GLuint*)&info.mDepthbuffer);
//    glBindRenderbufferNUI(GL_RENDERBUFFER_NUI, info.mDepthbuffer);

    
    if (pSurface->GetStencil())
    {
//      glRenderbufferStorageNUI(GL_RENDERBUFFER_NUI,
//                               GL_DEPTH24_STENCIL8_OES,
//                               width, height);
    }
    else
    {
      int32 depth = pSurface->GetDepth();
//      if (depth <= 16)
//        glRenderbufferStorageNUI(GL_RENDERBUFFER_NUI, GL_DEPTH_COMPONENT16, width, height);
//      else if (depth <= 24)
//        glRenderbufferStorageNUI(GL_RENDERBUFFER_NUI, GL_DEPTH_COMPONENT24, width, height);
//      else
//        glRenderbufferStorageNUI(GL_RENDERBUFFER_NUI, GL_DEPTH_COMPONENT32, width, height);
    }

//    glBindRenderbufferNUI(GL_RENDERBUFFER_NUI, 0);

//    glFramebufferRenderbufferNUI(GL_FRAMEBUFFER_NUI,
//                                 GL_DEPTH_ATTACHMENT_NUI,
//                                 GL_RENDERBUFFER_NUI,
//                                 info.mDepthbuffer);

    if (pSurface->GetStencil())
    {
//      glFramebufferRenderbufferNUI(GL_FRAMEBUFFER_NUI,
//                                   GL_STENCIL_ATTACHMENT_NUI,
//                                   GL_RENDERBUFFER_NUI,
//                                   info.mDepthbuffer);
    }
  }

  ///< Do we need a stencil buffer
  if (pSurface->GetStencil())
  {
//    glGenRenderbuffersNUI(1, (GLuint*)&info.mStencilbuffer);
//    glBindRenderbufferNUI(GL_RENDERBUFFER_NUI, info.mStencilbuffer);

//    glRenderbufferStorageNUI(GL_RENDERBUFFER_NUI,
//                             GL_STENCIL_INDEX,
//                             width, height);
//    glBindRenderbufferNUI(GL_RENDERBUFFER_NUI, 0);
//    glFramebufferRenderbufferNUI(GL_FRAMEBUFFER_NUI,
//                                 GL_STENCIL_ATTACHMENT_NUI,
//                                 GL_RENDERBUFFER_NUI,
//                                 info.mStencilbuffer);
  }

  {
    nglCriticalSectionGuard fbg(mFramebuffersCS);
    mFramebuffers[pSurface] = info;
  }

//  glBindFramebufferNUI(GL_FRAMEBUFFER_NUI, DefaultFrameBuffer);
  //NGL_OUT("glBindFramebufferNUI -> %d\n", DefaultFrameBuffer);
}

void nuiMetalPainter::SetSurface(nuiSurface* pSurface)
{
//  NGL_OUT("nuiMetalPainter::SetSurface %p\n", pSurface);
  mViewportChanged = true;
//  if (!mpSurface && pSurface)
//  {
//  }
//  else if (mpSurface && !pSurface)
//  {
//    SetSize(mOriginalWidth, mOriginalHeight);
//  }
//

  if (pSurface)
  {
    pSurface->Acquire();
    SetSize(pSurface->GetWidth(), pSurface->GetHeight());
  }
  
  if (mpSurface)
  {
    mpSurface->Release();
  }
  mpSurface = pSurface;
  
  if (pSurface)
  {
    nglCriticalSectionGuard fbg(mFramebuffersCS);
    std::map<nuiSurface*, FramebufferInfo>::const_iterator it = mFramebuffers.find(pSurface);
    bool create = (it == mFramebuffers.end()) ? true : false;
    
    GLint width = (GLint)pSurface->GetWidth();
    GLint height = (GLint)pSurface->GetHeight();

    //SetSize(width, height);

    float scale = mpContext->GetScale();
    width *= scale;
    height *= scale;
    
    nuiTexture* pTexture = pSurface->GetTexture();
    
#if defined DEBUG && defined _UIKIT_
    if (pTexture)
      mpContext->StartMarkerGroup(pTexture->GetSource().GetChars());
    else
      mpContext->StartMarkerGroup(pSurface->GetObjectName().GetChars());
#endif
    
    if (create)
    {
      CreateSurface(pSurface);
      it = mFramebuffers.find(pSurface);
    }

    const FramebufferInfo& info(it->second);
//    glBindFramebufferNUI(GL_FRAMEBUFFER_NUI, info.mFramebuffer);
  }
  else
  {
    /// !pSurface
//    glBindFramebufferNUI(GL_FRAMEBUFFER_NUI, mDefaultFramebuffer);
  }
}

nglCriticalSection nuiMetalPainter::RenderArrayInfo::mHeapCS("nuiMetalPainter::mHeapCS");
std::list<nuiMetalPainter::RenderArrayInfo*> nuiMetalPainter::RenderArrayInfo::mHeap;

nuiMetalPainter::RenderArrayInfo* nuiMetalPainter::RenderArrayInfo::Create(nuiRenderArray* pRenderArray)
{
  nglCriticalSectionGuard g(mHeapCS);
  if (!mHeap.empty())
  {
    RenderArrayInfo* pInfo = mHeap.front();
    mHeap.pop_front();
    pInfo->Rebind(pRenderArray);
    return pInfo;
  }
  else
  {
    return new RenderArrayInfo(pRenderArray);
  }
}

void nuiMetalPainter::RenderArrayInfo::Recycle(nuiMetalPainter::RenderArrayInfo* pInfo)
{
  pInfo->Destroy();
}


nuiMetalPainter::RenderArrayInfo::RenderArrayInfo(nuiRenderArray* pRenderArray)
{
  mpRenderArray = nullptr;
  mpVertexBuffer = nullptr;

  if (pRenderArray)
    Rebind(pRenderArray);
}

nuiMetalPainter::RenderArrayInfo::~RenderArrayInfo()
{
  Destroy();
}


void nuiMetalPainter::RenderArrayInfo::Rebind(nuiRenderArray* pRenderArray)
{
  mpRenderArray = pRenderArray;

  int32 count = pRenderArray->GetSize();
  NGL_ASSERT(mpVertexBuffer == nullptr);
//  glGenBuffers(1, &mVertexBuffer);
//  glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
//  glBufferData(GL_ARRAY_BUFFER, sizeof(nuiRenderArray::Vertex) * count, &pRenderArray->GetVertices()[0], GL_STATIC_DRAW);

  if (pRenderArray->GetIndexArrayCount() > 0)
  {
    uint32 indexcount = pRenderArray->GetIndexArrayCount();
    mIndexBuffers.resize(indexcount);

    for (uint32 i = 0; i < indexcount; i++)
    {
//      glGenBuffers(1, &mIndexBuffers[i]);
//      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffers[i]);
      const nuiRenderArray::IndexArray& indices(pRenderArray->GetIndexArray(i));
//      glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.mIndices.size() * sizeof(GLushort), &indices.mIndices[0], GL_STATIC_DRAW);
    }
  }

  int streamcount = pRenderArray->GetStreamCount();
  mStreamBuffers.resize(streamcount);
  for (int i = 0; i < streamcount; i++)
  {
    const nuiRenderArray::StreamDesc& stream(pRenderArray->GetStream(i));

//    glGenBuffers(1, &mStreamBuffers[i]);
//    glBindBuffer(GL_ARRAY_BUFFER, mStreamBuffers[i]);
    int32 s = 1;
    switch (stream.mType)
    {
      case nuiRenderArray::eFloat: s = 4; break;
      case nuiRenderArray::eInt: s = 4; break;
      case nuiRenderArray::eByte: s = 1; break;
    }
//    glBufferData(GL_ARRAY_BUFFER, count * stream.mCount * s, stream.mData.mpFloats, GL_STATIC_DRAW);
  }

//  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
//  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void nuiMetalPainter::RenderArrayInfo::Destroy()
{
  if (mpVertexBuffer == nullptr)
    return;

  for (auto vao : mVAOs)
  {
//    glDeleteVertexArrays(1, (GLuint*)&vao.second);
  }
  mVAOs.clear();

//  glDeleteBuffers(1, &mVertexBuffer);
  mpVertexBuffer = nullptr;

  for (uint32 i = 0; i < mIndexBuffers.size(); i++)
  {
//    glDeleteBuffers(1, &mIndexBuffers[i]);
  }
  mIndexBuffers.clear();

  for (uint32 i = 0; i < mStreamBuffers.size(); i++)
  {
//    glDeleteBuffers(1, &mStreamBuffers[i]);
  }

  mStreamBuffers.clear();
  {
    nglCriticalSectionGuard g(mHeapCS);
    mHeap.push_back(this);
  }
}

void nuiMetalPainter::RenderArrayInfo::BindVertices() const
{
//  glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
}

void nuiMetalPainter::RenderArrayInfo::BindStream(int index) const
{
//  glBindBuffer(GL_ARRAY_BUFFER, mStreamBuffers[index]);
}

void nuiMetalPainter::RenderArrayInfo::BindIndices(int index) const
{
//  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffers[index]);
}

void nuiMetalPainter::RenderArrayInfo::Draw() const
{
  BindVertices();

  if (mpRenderArray->GetIndexArrayCount() > 0)
  {
    for (uint32 i = 0; i < mIndexBuffers.size(); i++)
    {
      const nuiRenderArray::IndexArray& array(mpRenderArray->GetIndexArray(i));
      BindIndices(i);
      
//#ifdef _UIKIT_
//      glDrawElements(array.mMode, array.mIndices.size(), GL_UNSIGNED_SHORT, (void*)0);
//#else
//      glDrawElements(array.mMode, array.mIndices.size(), GL_UNSIGNED_INT, (void*)0);
//#endif
    }
  }
  else
  {

  }
}


void nuiMetalPainter::LoadMatrix(const nuiMatrix& rMatrix)
{
  NUI_RETURN_IF_RENDERING_DISABLED;
  
  nuiPainter::LoadMatrix(rMatrix);
  mMatrixChanged = true;
}

void nuiMetalPainter::MultMatrix(const nuiMatrix& rMatrix)
{
  NUI_RETURN_IF_RENDERING_DISABLED;
  
  nuiPainter::MultMatrix(rMatrix);
  mMatrixChanged = true;
}

void nuiMetalPainter::PushMatrix()
{
  NUI_RETURN_IF_RENDERING_DISABLED;
  
  nuiPainter::PushMatrix();
  mMatrixChanged = true;
}

void nuiMetalPainter::PopMatrix()
{
  NUI_RETURN_IF_RENDERING_DISABLED;
  
  nuiPainter::PopMatrix();
  mMatrixChanged = true;
}

void nuiMetalPainter::LoadProjectionMatrix(const nuiRect& rViewport, const nuiMatrix& rMatrix)
{
  NUI_RETURN_IF_RENDERING_DISABLED;
  
  nuiPainter::LoadProjectionMatrix(rViewport, rMatrix);
  mViewportChanged = true;
}

void nuiMetalPainter::MultProjectionMatrix(const nuiMatrix& rMatrix)
{
  NUI_RETURN_IF_RENDERING_DISABLED;
  
  nuiPainter::MultProjectionMatrix(rMatrix);
  mViewportChanged = true;
}

void nuiMetalPainter::PushProjectionMatrix()
{
  NUI_RETURN_IF_RENDERING_DISABLED;
  
  nuiPainter::PushProjectionMatrix();
  mViewportChanged = true;
}

void nuiMetalPainter::PopProjectionMatrix()
{
  NUI_RETURN_IF_RENDERING_DISABLED;
  
  nuiPainter::PopProjectionMatrix();
  mViewportChanged = true;
}



void nuiMetalPainter::SetVertexPointers(const nuiRenderArray& rArray)
{
  nuiShaderProgram* pPgm = mFinalState.mpShader;
  GLint Position = pPgm->GetVAPositionLocation();
  GLint TexCoord = pPgm->GetVATexCoordLocation();
  GLint Color = pPgm->GetVAColorLocation();
  GLint Normal = pPgm->GetVANormalLocation();
  
  if (Position != -1)
  {
//    glEnableVertexAttribArray(Position);
//    glVertexAttribPointer(Position, 3, GL_FLOAT, GL_FALSE, sizeof(nuiRenderArray::Vertex), &rArray.GetVertices()[0].mX);
  }

  if (TexCoord != -1)
  {
//    glEnableVertexAttribArray(TexCoord);
//    glVertexAttribPointer(TexCoord, 2, GL_FLOAT, GL_FALSE, sizeof(nuiRenderArray::Vertex), &rArray.GetVertices()[0].mTX);
  }

  if (Color != -1)
  {
//    glEnableVertexAttribArray(Color);
//    glVertexAttribPointer(Color, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(nuiRenderArray::Vertex), &rArray.GetVertices()[0].mR);
  }

  if (Normal != -1)
  {
//    glEnableVertexAttribArray(Normal);
//    glVertexAttribPointer(Normal, 3, GL_FLOAT, GL_FALSE, sizeof(nuiRenderArray::Vertex), &rArray.GetVertices()[0].mNX);
  }

  int stream_count = rArray.GetStreamCount();
  for (int i = 0; i < stream_count; i++)
  {
    const nuiRenderArray::StreamDesc& rDesc(rArray.GetStream(i));
//    glEnableVertexAttribArray(rDesc.mStreamID);
//    glVertexAttribPointer(rDesc.mStreamID, rDesc.mCount, rDesc.mType, rDesc.mNormalize ? GL_TRUE : GL_FALSE, 0, rDesc.mData.mpFloats);
  }
}

void nuiMetalPainter::SetVertexBuffersPointers(const nuiRenderArray& rArray, RenderArrayInfo& rInfo)
{
  static int64 created = 0;
  static int64 bound = 0;
  {
    nglCriticalSectionGuard g(gStats);
    total++;
  }
  nuiShaderProgram* pPgm = mFinalState.mpShader;
  
  // Look for VAO:
  auto it = rInfo.mVAOs.find(pPgm);
  if (it == rInfo.mVAOs.end())
  {
    created++;
    
    // Create this VAO:
    GLint vao = 0;
//    glGenVertexArrays(1, (GLuint*)&vao);
    rInfo.mVAOs[pPgm] = vao;
    
//    glBindVertexArray(vao);

    GLint Position = pPgm->GetVAPositionLocation();
    GLint TexCoord = pPgm->GetVATexCoordLocation();
    GLint Color = pPgm->GetVAColorLocation();
    GLint Normal = pPgm->GetVANormalLocation();
    
    rInfo.BindVertices();
    if (Position != -1)
    {
//      glVertexAttribPointer(Position, 3, GL_FLOAT, GL_FALSE, sizeof(nuiRenderArray::Vertex), (void*)offsetof(nuiRenderArray::Vertex, mX));
//      glEnableVertexAttribArray(Position);
    }

    if (TexCoord != -1)
    {
//      glVertexAttribPointer(TexCoord, 2, GL_FLOAT, GL_FALSE, sizeof(nuiRenderArray::Vertex), (void*)offsetof(nuiRenderArray::Vertex, mTX));
//      glEnableVertexAttribArray(TexCoord);
    }

    if (Color != -1)
    {
//      glVertexAttribPointer(Color, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(nuiRenderArray::Vertex), (void*)offsetof(nuiRenderArray::Vertex, mR));
//      glEnableVertexAttribArray(Color);
    }

    if (Normal != -1)
    {
//      glVertexAttribPointer(Normal, 3, GL_FLOAT, GL_FALSE, sizeof(nuiRenderArray::Vertex), (void*)offsetof(nuiRenderArray::Vertex, mNX));
//      glEnableVertexAttribArray(Normal);
    }

    for (int i = 0; i < rArray.GetStreamCount(); i++)
    {
      SetStreamBuffersPointers(rArray, rInfo, i);
    }
    
    return;
  }
  
  bound++;
  
  if (bound % 10000 == 0)
  {
//    NGL_DEBUG(NGL_OUT("VAO/VBO %lld created %lld bound (%f cache hit)\n", created, bound, (float)bound / (float)created););
  }
  
  NGL_ASSERT(it != rInfo.mVAOs.end());
  
  GLint vao = it->second;
//  glBindVertexArray(vao);

}

void nuiMetalPainter::SetStreamBuffersPointers(const nuiRenderArray& rArray, const RenderArrayInfo& rInfo, int index)
{
  rInfo.BindStream(index);
  const nuiRenderArray::StreamDesc& rDesc(rArray.GetStream(index));
//  glVertexAttribPointer(rDesc.mStreamID, rDesc.mCount, rDesc.mType, rDesc.mNormalize ? GL_TRUE : GL_FALSE, 0, NULL);
//  glEnableVertexAttribArray(rDesc.mStreamID);
}


void nuiMetalPainter::ResetVertexPointers(const nuiRenderArray& rArray)
{
  nuiShaderProgram* pPgm = mFinalState.mpShader;
  GLint Position = pPgm->GetVAPositionLocation();
  GLint TexCoord = pPgm->GetVATexCoordLocation();
  GLint Color = pPgm->GetVAColorLocation();
  GLint Normal = pPgm->GetVANormalLocation();
  
  if (Position != -1)
  {
//    glDisableVertexAttribArray(Position);
  }
  
  if (TexCoord != -1)
  {
//    glDisableVertexAttribArray(TexCoord);
  }
  
  if (Color != -1)
  {
//    glDisableVertexAttribArray(Color);
  }
  
  if (Normal != -1)
  {
//    glDisableVertexAttribArray(Normal);
  }
  
  int stream_count = rArray.GetStreamCount();
  for (int i = 0; i < stream_count; i++)
  {
    const nuiRenderArray::StreamDesc& rDesc(rArray.GetStream(i));
//    glDisableVertexAttribArray(rDesc.mStreamID);
  }
}



void nuiMetalPainter::DestroyRenderArray(nuiRenderArray* pArray)
{
  nglCriticalSectionGuard g(mRenderingCS);
  mDestroyedRenderArrays.push_back(pArray);
}

void nuiMetalPainter::FinalizeRenderArrays()
{
  nglCriticalSectionGuard g(mRenderingCS);
//  glBindVertexArray(0);
//  glBindBuffer(GL_ARRAY_BUFFER, 0);
//  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  for (auto array : mDestroyedRenderArrays)
    _DestroyRenderArray(array);
  mDestroyedRenderArrays.clear();
}

void nuiMetalPainter::_DestroyRenderArray(nuiRenderArray* pArray)
{
  nglCriticalSectionGuard rag(mRenderArraysCS);
  auto it = mRenderArrays.find(pArray);
  if (it == mRenderArrays.end())
    return; // This render array was not stored here
  
  RenderArrayInfo* info(it->second);
  RenderArrayInfo::Recycle(info);
  
  mRenderArrays.erase(it);
}

nglImage* nuiMetalPainter::CreateImageFromGPUTexture(const nuiTexture* pTexture) const
{
  return nullptr;
}


std::map<nuiTexture*, nuiMetalPainter::TextureInfo> nuiMetalPainter::mTextures;
std::map<nuiSurface*, nuiMetalPainter::FramebufferInfo> nuiMetalPainter::mFramebuffers;
std::vector<nuiRenderArray*> nuiMetalPainter::mFrameArrays;
std::map<nuiRenderArray*, nuiMetalPainter::RenderArrayInfo*> nuiMetalPainter::mRenderArrays;
nglCriticalSection nuiMetalPainter::mTexturesCS("mTexturesCS");
nglCriticalSection nuiMetalPainter::mFramebuffersCS("mFramebuffersCS");
nglCriticalSection nuiMetalPainter::mFrameArraysCS("mFrameArraysCS");
nglCriticalSection nuiMetalPainter::mRenderArraysCS("mRenderArraysCS");


#endif //   #ifndef __NUI_NO_GL__
