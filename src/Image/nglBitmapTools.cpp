/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "nglBitmapTools.h"


NGL_API void nglInvertLineSwap32 (char* pDst, char* pSrc, uint pixelcount)
{
  uint count = pixelcount * 4;
  pSrc += count - 1;
  while (--count)
    *pDst++ = *pSrc--;
}

NGL_API void nglInvertLineSwap24 (char* pDst, char* pSrc, uint pixelcount)
{
  uint count = pixelcount * 3;
  pSrc += count - 1;
  while (--count)
    *pDst++ = *pSrc--;
}

NGL_API void nglLineSwap24 (char* pDst, char* pSrc, uint pixelcount)
{
  char p1,p2,p3;
  uint count = pixelcount;
  while (--count)
  {
    p3 = *pSrc++;
    p2 = *pSrc++;
    p1 = *pSrc++;
    *pDst++ = p1;
    *pDst++ = p2;
    *pDst++ = p3;
  }
}

NGL_API void nglInvertLine32 (unsigned int* pDst, unsigned int* pSrc, uint pixelcount)
{
  uint count = pixelcount;
  pSrc += count - 1;
  while (--count)
  {
    *pDst++ = *pSrc--;
  }
}

NGL_API void nglInvertLine24 (char* pDst, char* pSrc, uint pixelcount)
{
  char p1,p2,p3;
  uint count = pixelcount;
  pSrc += count - 1;
  while (--count)
  {
    p3 = *pSrc--;
    p2 = *pSrc--;
    p1 = *pSrc--;
    *pDst++ = p1;
    *pDst++ = p2;
    *pDst++ = p3;
  }
}

NGL_API void nglInvertLine16 (short* pDst, short* pSrc, uint pixelcount)
{
  pSrc+=pixelcount - 1;
  while (--pixelcount)
    *pDst++ = *pSrc--;
}


/// From 32 BPP
NGL_API void nglCopyLine8To8 (void* pDst, void* pSrc, int PixelCount, bool Invert)
{                                          
  if (Invert)
  {
    int i;
    char* pSource = (char*) pSrc;
    char* pDest   = (char*) pDst + (PixelCount - 1);
    for (i=0; i<PixelCount; i++)
      *pDest-- = *pSource++;
  }
  else
  {
    memcpy(pDst,pSrc,PixelCount*4);
  }
}                                          
                                           
NGL_API void nglCopyLine8To15 (void* pDst, void* pSrc, int PixelCount, bool Invert)
{                                          
  int i;
  unsigned short temp;
  char* pSource = (char*)pSrc;
  if (Invert)
  {
    for (i=0; i<PixelCount; i++)
    {
      temp  = ((*pSource) & 0xF8) >> 3;
      temp |= ((*pSource) & 0xF8) << 2;
      temp |= ((*pSource) & 0xF8) << 7;
      pSource++;
      ((short*)pDst)[PixelCount-i-1] = temp;
    }
  }
  else
  {
    for (i=0; i<PixelCount; i++)
    {
      temp  = ((*pSource) & 0xF8) >> 3;
      temp |= ((*pSource) & 0xF8) << 2;
      temp |= ((*pSource) & 0xF8) << 7;
      pSource++;
      ((short*)pDst)[i] = temp;
    }
  }
}                                          
                                           
NGL_API void nglCopyLine8To16 (void* pDst, void* pSrc, int PixelCount, bool Invert)
{
  int i;
  unsigned short temp;
  char* pSource = (char*)pSrc;
  if (Invert)
  {
    for (i=0; i<PixelCount; i++)
    {
      temp  = ((*pSource)&0xF8) >> 3;
      temp |= ((*pSource)&0xF8) << 3;
      temp |= ((*pSource)&0xF8) << 8;
      pSource++;
      ((short*)pDst)[PixelCount-i-1] = temp;
    }
  }
  else
  {
    for (i=0; i<PixelCount; i++)
    {
      temp  = ((*pSource) & 0xF8) >> 3;
      temp |= ((*pSource) & 0xF8) << 3;
      temp |= ((*pSource) & 0xF8) << 8;
      pSource++;
      ((short*)pDst)[i] = temp;
    }
  }
}

NGL_API void nglCopyLine8To24 (void* pDst, void* pSrc, int PixelCount, bool Invert)
{                                          
  int i;
  char* pSource = (char*) pSrc;
  if (Invert)
  {
    char r,g,b;
    char* pDest   = (char*) pDst + 3 * (PixelCount - 1) - 1;
    for (i=0; i<PixelCount; i++)
    {
      r = *pSource;
      g = *pSource;
      b = *pSource;
      pSource++;
      *pDest-- = b;
      *pDest-- = g;
      *pDest-- = r;
    }
  }
  else
  {
    char* pDest   = (char*) pDst;
    for (i=0; i<PixelCount; i++)
    {
      *pDest++ = *pSource;
      *pDest++ = *pSource;
      *pDest++ = *pSource;
      pSource++;
    }
  }
}                                          
                                           
NGL_API void nglCopyLine8To32 (void* pDst, void* pSrc, int PixelCount, bool Invert)
{                                          
  if (Invert)
  {
    int i;
    unsigned char r,g,b,a;
    unsigned char* pSource = (unsigned char*) pSrc;
    unsigned char* pDest   = (unsigned char*) pDst + 4 * (PixelCount - 1) - 1;
    for (i=0; i<PixelCount; i++)
    {
      r = *pSource;
      g = *pSource;
      b = *pSource;
      a = 255;
      *pDest-- = a;
      *pDest-- = b;
      *pDest-- = g;
      *pDest-- = r;
    }
  }
  else
  {
    memcpy(pDst,pSrc,PixelCount*4);
  }
}    

NGL_API void nglCopyLineL8To32ARGB (void* pDst, void* pSrc, int PixelCount, bool Invert)
{                                          
  if (Invert)
  {
    int i;
    unsigned char src_val;
    unsigned char* pSource = (unsigned char*) pSrc;
    unsigned char* pDest   = (unsigned char*) pDst + 4 * (PixelCount - 1) - 1;
    for (i=0; i<PixelCount; i++)
    {
      src_val = *pSource++;
      *pDest-- = 255;
      *pDest-- = src_val;
      *pDest-- = src_val;
      *pDest-- = src_val;
    }
  }
  else
  {
    char* pSource = (char*) pSrc;
    char* pDest   = (char*) pDst;
    for (int i=0; i<PixelCount; ++i)
    {
      pDest[i*4+0] = pSource[i];
      pDest[i*4+1] = pSource[i];
      pDest[i*4+2] = pSource[i];
      pDest[i*4+3] = 255;
    }
  }
}  

NGL_API void nglCopyLineA8To32ARGB (void* pDst, void* pSrc, int PixelCount, bool Invert)
{                                          
  if (Invert)
  {
    int i;
    unsigned char src_val;
    unsigned char* pSource = (unsigned char*) pSrc;
    unsigned char* pDest   = (unsigned char*) pDst + 4 * (PixelCount - 1) - 1;
    for (i=0; i<PixelCount; i++)
    {
      src_val = *pSource++;
      *pDest-- = src_val;
      *pDest-- = 0;
      *pDest-- = 0;
      *pDest-- = 0;
    }
  }
  else
  {
    char* pSource = (char*) pSrc;
    char* pDest   = (char*) pDst;
    for (int i=0; i<PixelCount; ++i)
    {
      pDest[i*4+0] = 0;
      pDest[i*4+1] = 0;
      pDest[i*4+2] = 0;
      pDest[i*4+3] = pSource[i];
    }
  }
}  



/// From 15 BPP
NGL_API void nglCopyLine15To8 (void* pDst, void* pSrc, int PixelCount, bool Invert)
{
  int i;
  unsigned short temp;
  char* pDest = (char*)pDst;
  if (Invert)
  {
    for (i=0; i<PixelCount; i++)
    {
      temp = ((short*)pSrc)[PixelCount - i -1];
      *pDest++ = (((temp >> 7) & 0xF8) + ((temp >> 2) & 0xF8) + ((temp << 3) & 0xF8)) / 3;
    }
  }
  else
  {
    for (i=0; i<PixelCount; i++)
    {
      temp = ((short*)pSrc)[i];
      *pDest++ = (((temp >> 7) & 0xF8) + ((temp >> 2) & 0xF8) + ((temp << 3) & 0xF8)) / 3;
    }
  }
}

NGL_API void nglCopyLine15To15 (void* pDst, void* pSrc, int PixelCount, bool Invert)
{
  if (Invert)
  {
    int i;
    for (i=0; i<PixelCount; i++)
      ((short*)pDst)[i] = ((short*)pSrc)[PixelCount-i-1];
  }
  else
  {
    memcpy(pDst,pSrc,PixelCount*2);
  }
}

NGL_API void nglCopyLine15To16 (void* pDst, void* pSrc, int PixelCount, bool Invert)
{
  int i;
  unsigned char r,g,b;
  unsigned short temp;
  if (Invert)
  {
    for (i=0; i<PixelCount; i++)
    {
      temp = ((short*)pSrc)[i];
      r = (temp >> 10) & 0x1F;
      g = (temp >> 5 ) & 0x1F;
      b = (temp >> 0 ) & 0x1F;
      temp = (r << 11) + (g <<6) + b;
      ((short*)pDst)[PixelCount - i -1] = temp;
    }
  }
  else
  {
    for (i=0; i<PixelCount; i++)
    {
      temp = ((short*)pSrc)[i];
      r = (temp >> 10) & 0x1F;
      g = (temp >> 5 ) & 0x1F;
      b = (temp >> 0 ) & 0x1F;
      temp = (r << 11) + (g <<6) + b;
      ((short*)pDst)[i] = temp;
    }
  }
}

NGL_API void nglCopyLine15To24 (void* pDst, void* pSrc, int PixelCount, bool Invert)
{
  int i;
  unsigned short temp;
  char* pDest = (char*)pDst;
  if (Invert)
  {
    for (i=0; i<PixelCount; i++)
    {
      temp = ((short*)pSrc)[PixelCount - i -1];
      *pDest++ = (temp >> 7) & 0xF8;
      *pDest++ = (temp >> 2) & 0xF8;
      *pDest++ = (temp << 3) & 0xF8;
    }
  }
  else
  {
    for (i=0; i<PixelCount; i++)
    {
      temp = ((short*)pSrc)[i];
      *pDest++ = (temp >> 7) & 0xF8;
      *pDest++ = (temp >> 2) & 0xF8;
      *pDest++ = (temp << 3) & 0xF8;
    }
  }
}

NGL_API void nglCopyLine15To32 (void* pDst, void* pSrc, int PixelCount, bool Invert)
{
  int i;
  unsigned short temp;
  char* pDest = (char*)pDst;
  if (Invert)
  {
    for (i=0; i<PixelCount; i++)
    {
      temp = ((short*)pSrc)[PixelCount - i -1];
      *pDest++ = (temp >> 7) & 0xF8;
      *pDest++ = (temp >> 2) & 0xF8;
      *pDest++ = (temp << 3) & 0xF8;
      *pDest++ = 0;
    }
  }
  else
  {
    for (i=0; i<PixelCount; i++)
    {
      temp = ((short*)pSrc)[i];
      *pDest++ = (temp >> 7) & 0xF8;
      *pDest++ = (temp >> 2) & 0xF8;
      *pDest++ = (temp << 3) & 0xF8;
      *pDest++ = 0;
    }
  }
}


NGL_API void nglCopyLine15To32ARGB (void* pDst, void* pSrc, int PixelCount, bool Invert)
{
  int i;
  unsigned short temp;
  char* pDest = (char*)pDst;
  if (Invert)
  {
    for (i=0; i<PixelCount; i++)
    {
      temp = ((short*)pSrc)[PixelCount - i -1];
      *pDest++ = (temp << 3) & 0xF8;
      *pDest++ = (temp >> 2) & 0xF8;
      *pDest++ = (temp >> 7) & 0xF8;
      *pDest++ = 0; 
    }
  }
  else
  {
    for (i=0; i<PixelCount; i++)
    {
      temp = ((short*)pSrc)[i];
      *pDest++ = (temp << 3) & 0xF8;
      *pDest++ = (temp >> 2) & 0xF8;
      *pDest++ = (temp >> 7) & 0xF8;
      *pDest++ = 0;
    }
  }
}

/// From 16 BPP
NGL_API void nglCopyLine16To8 (void* pDst, void* pSrc, int PixelCount, bool Invert)
{
  int i;
  unsigned short temp;
  char* pDest = (char*)pDst;
  if (Invert)
  {
    for (i=0; i<PixelCount; i++)
    {
      temp = ((short*)pSrc)[PixelCount - i -1];
      *pDest++ = (((temp >> 8) & 0xF8) + ((temp >> 3) & 0xF8) + ((temp << 3) & 0xF8)) / 3;
    }
  }
  else
  {
    for (i=0; i<PixelCount; i++)
    {
      temp = ((short*)pSrc)[i];
      *pDest++ = (((temp >> 8) & 0xF8) + ((temp >> 3) & 0xF8) + ((temp << 3) & 0xF8)) / 3;
    }
  }
}


NGL_API void nglCopyLine16To15 (void* pDst, void* pSrc, int PixelCount, bool Invert)
{
  int i;
  unsigned char r,g,b;
  unsigned short temp;
  if (Invert)
  {
    for (i=0; i<PixelCount; i++)
    {
      temp = ((short*)pSrc)[i];
      r = (temp >> 11) & 0x1F;
      g = (temp >> 6 ) & 0x1F;
      b = (temp >> 0 ) & 0x1F;
      temp = (r << 10) + (g <<5) + b;
      ((short*)pDst)[PixelCount - i -1] = temp;
    }
  }
  else
  {
    for (i=0; i<PixelCount; i++)
    {
      temp = ((short*)pSrc)[i];
      r = (temp >> 11) & 0x1F;
      g = (temp >> 6 ) & 0x1F;
      b = (temp >> 0 ) & 0x1F;
      temp = (r << 10) + (g <<5) + b;
      ((short*)pDst)[i] = temp;
    }
  }
}
              
NGL_API void nglCopyLine16To16 (void* pDst, void* pSrc, int PixelCount, bool Invert)
{
  if (Invert)
  {
    int i;
    for (i=0; i<PixelCount; i++)
      ((short*)pDst)[i] = ((short*)pSrc)[PixelCount-i-1];
  }
  else
  {
    memcpy(pDst,pSrc,PixelCount*2);
  }
}

NGL_API void nglCopyLine16To24 (void* pDst, void* pSrc, int PixelCount, bool Invert)
{
  int i;
  unsigned short temp;
  char* pDest = (char*)pDst;
  if (Invert)
  {
    for (i=0; i<PixelCount; i++)
    {
      temp = ((short*)pSrc)[PixelCount - i -1];
      *pDest++ = (temp >> 8) & 0xF8;
      *pDest++ = (temp >> 3) & 0xF8;
      *pDest++ = (temp << 3) & 0xF8;
    }
  }
  else
  {
    for (i=0; i<PixelCount; i++)
    {
      temp = ((short*)pSrc)[i];
      *pDest++ = (temp >> 8) & 0xF8;
      *pDest++ = (temp >> 3) & 0xF8;
      *pDest++ = (temp << 3) & 0xF8;
    }
  }
}

NGL_API void nglCopyLine16To32 (void* pDst, void* pSrc, int PixelCount, bool Invert)
{
  int i;
  unsigned short temp;
  char* pDest = (char*)pDst;
  if (Invert)
  {
    for (i=0; i<PixelCount; i++)
    {
      temp = ((short*)pSrc)[PixelCount - i -1];
      *pDest++ = (temp << 3) & 0xF8;
      *pDest++ = (temp >> 3) & 0xF8;
      *pDest++ = (temp >> 8) & 0xF8;
      *pDest++ = 0;
    }
  }
  else
  {
    for (i=0; i<PixelCount; i++)
    {
      temp = ((short*)pSrc)[i];
      *pDest++ = (temp << 3) & 0xF8;
      *pDest++ = (temp >> 3) & 0xF8;
      *pDest++ = (temp >> 8) & 0xF8;
      *pDest++ = 0;
    }
  }
}


NGL_API void nglCopyLine16To32ARGB (void* pDst, void* pSrc, int PixelCount, bool Invert)
{
  int i;
  unsigned short temp;
  char* pDest = (char*)pDst;
  if (Invert)
  {
    for (i=0; i<PixelCount; i++)
    {
      temp = ((short*)pSrc)[PixelCount - i -1];
      *pDest++ = (temp >> 8) & 0xF8;
      *pDest++ = (temp >> 3) & 0xF8;
      *pDest++ = (temp << 3) & 0xF8;
      *pDest++ = 0;
    }
  }
  else
  {
    for (i=0; i<PixelCount; i++)
    {
      temp = ((short*)pSrc)[i];
      *pDest++ = (temp >> 8) & 0xF8;
      *pDest++ = (temp >> 3) & 0xF8;
      *pDest++ = (temp << 3) & 0xF8;
      *pDest++ = 0;
    }
  }
}


/// From 24 BPP
NGL_API void nglCopyLine24To8 (void* pDst, void* pSrc, int PixelCount, bool Invert)
{                                          
  char r,g,b;
  int i;
  char* pSource = (char*) pSrc;
  if (Invert)
  {
    char* pDest   = (char*) pDst + (PixelCount - 1) - 1;
    for (i=0; i<PixelCount; i++)
    {
      r = *pSource++;
      g = *pSource++;
      b = *pSource++;
      *pDest-- = (b + g + r)/3;
    }
  }
  else
  {
    char* pDest   = (char*) pDst;
    for (i=0; i<PixelCount; i++)
    {
      r = *pSource++;
      g = *pSource++;
      b = *pSource++;
      *pDest++ = (b + g + r)/3;
    }
  }
}                                          

NGL_API void nglCopyLine24To15 (void* pDst, void* pSrc, int PixelCount, bool Invert)
{                                          
  int i;
  unsigned short temp;
  char* pSource = (char*)pSrc;
  if (Invert)
  {
    for (i=0; i<PixelCount; i++)
    {
      temp  = ((*pSource++) & 0xF8) >> 3;
      temp |= ((*pSource++) & 0xF8) << 2;
      temp |= ((*pSource++) & 0xF8) << 7;
      ((short*)pDst)[PixelCount-i-1] = temp;
    }
  }
  else
  {
    for (i=0; i<PixelCount; i++)
    {
      temp  = ((*pSource++) & 0xF8) >> 3;
      temp |= ((*pSource++) & 0xF8) << 2;
      temp |= ((*pSource++) & 0xF8) << 7;
      ((short*)pDst)[i] = temp;
    }
  }
}                                          
                                           
NGL_API void nglCopyLine24To16 (void* pDst, void* pSrc, int PixelCount, bool Invert)
{
  int i;
  unsigned short temp;
  char* pSource = (char*)pSrc;
  if (Invert)
  {
    for (i=0; i<PixelCount; i++)
    {
      temp  = ((*pSource++)&0xF8) << 8;
      temp |= ((*pSource++)&0xF8) << 3;
      temp |= ((*pSource++)&0xF8) >> 3;
      ((short*)pDst)[PixelCount-i-1] = temp;
    }
  }
  else
  {
    for (i=0; i<PixelCount; i++)
    {
      temp  = ((*pSource++)&0xF8) << 8;
      temp |= ((*pSource++)&0xF8) << 3;
      temp |= ((*pSource++)&0xF8) >> 3;
      ((short*)pDst)[i] = temp;
    }
  }
}

NGL_API void nglCopyLine24To24 (void* pDst, void* pSrc, int PixelCount, bool Invert)
{                                          
  if (Invert)
  {
    int i;
    char r,g,b;
    char* pSource = (char*) pSrc;
    char* pDest   = (char*) pDst + 3 * (PixelCount - 1) - 1;
    for (i=0; i<PixelCount; i++)
    {
      r = *pSource++;
      g = *pSource++;
      b = *pSource++;
      *pDest-- = b;
      *pDest-- = g;
      *pDest-- = r;
    }
  }
  else
  {
    memcpy(pDst,pSrc,PixelCount*3);
  }
}                                          
                                           
NGL_API void nglCopyLine24To32 (void* pDst, void* pSrc, int PixelCount, bool Invert)
{                                          
  int i;
  char* pSource = (char*) pSrc;
  if (Invert)
  {
    char r,g,b;
    char* pDest   = (char*) pDst + 4 * (PixelCount - 1) - 1;
    for (i=0; i<PixelCount; i++)
    {
      r = *pSource++;
      g = *pSource++;
      b = *pSource++;
      *pDest--;
      *pDest-- = b;
      *pDest-- = g;
      *pDest-- = r;
    }
  }
  else
  {
    char* pDest   = (char*) pDst;
    for (i=0; i<PixelCount; i++)
    {
      *pDest++ = *pSource++;
      *pDest++ = *pSource++;
      *pDest++ = *pSource++;
      pDest++;
    }
  }
}      

NGL_API void nglCopyLine24To32ARGB (void* pDst, void* pSrc, int PixelCount, bool Invert)
{                                          
  int i;
  char* pSource = (char*) pSrc;
  if (Invert)
  {
    char r,g,b;
    char* pDest   = (char*) pDst + 4 * (PixelCount - 1) - 1;
    for (i=0; i<PixelCount; i++)
    {
      r = *pSource++;
      g = *pSource++;
      b = *pSource++;
      *pDest--;
      *pDest-- = r;
      *pDest-- = g;
      *pDest-- = b;
    }
  }
  else
  {
    char* pSource = (char*) pSrc;
    char* pDest   = (char*) pDst;
    //memcpy(pDst,pSrc,PixelCount*4);
    for (int i=0; i<PixelCount; ++i)
    {
      pDest[i*4+0] = pSource[i*3+2];
      pDest[i*4+1] = pSource[i*3+1];
      pDest[i*4+2] = pSource[i*3+0];
      pDest[i*4+3] = 255;
    }
  }
}   


/// From 32 BPP
NGL_API void nglCopyLine32To8 (void* pDst, void* pSrc, int PixelCount, bool Invert)
{                                          
  int i;
  char* pSource = (char*) pSrc;
  if (Invert)
  {
    char r,g,b;
    char* pDest   = (char*) pDst + (PixelCount - 1) - 1;
    for (i=0; i<PixelCount; i++)
    {
      r = *pSource++;
      g = *pSource++;
      b = *pSource++;
      pSource++;
      *pDest-- = (b + g + r )/3;
    }
  }
  else
  {
    char* pDest   = (char*) pDst;
    for (i=0; i<PixelCount; i++)
    {
      int average;
      average  = *pSource++;
      average += *pSource++;
      average += *pSource++;
      pSource++;
      *pDest++ = average / 3; 
    }
  }
}                                          
                                           

NGL_API void nglCopyLine32To15 (void* pDst, void* pSrc, int PixelCount, bool Invert)
{                                          
  int i;
  unsigned short temp;
  char* pSource = (char*)pSrc;
  if (Invert)
  {
    for (i=0; i<PixelCount; i++)
    {
      temp  = ((*pSource++) & 0xF8) >> 3;
      temp |= ((*pSource++) & 0xF8) << 2;
      temp |= ((*pSource++) & 0xF8) << 7;
      pSource++;
      ((short*)pDst)[PixelCount-i-1] = temp;
    }
  }
  else
  {
    for (i=0; i<PixelCount; i++)
    {
      temp  = ((*pSource++) & 0xF8) >> 3;
      temp |= ((*pSource++) & 0xF8) << 2;
      temp |= ((*pSource++) & 0xF8) << 7;
      pSource++;
      ((short*)pDst)[i] = temp;
    }
  }
}                                          
                                           
NGL_API void nglCopyLine32To16 (void* pDst, void* pSrc, int PixelCount, bool Invert)
{
  int i;
  unsigned short temp;
  char* pSource = (char*)pSrc;
  if (Invert)
  {
    for (i=0; i<PixelCount; i++)
    {
      temp  = ((*pSource++)&0xF8) >> 3;
      temp |= ((*pSource++)&0xF8) << 3;
      temp |= ((*pSource++)&0xF8) << 8;
      pSource++;
      ((short*)pDst)[PixelCount-i-1] = temp;
    }
  }
  else
  {
    for (i=0; i<PixelCount; i++)
    {
      temp  = ((*pSource++) & 0xF8) >> 3;
      temp |= ((*pSource++) & 0xF8) << 3;
      temp |= ((*pSource++) & 0xF8) << 8;
      pSource++;
      ((short*)pDst)[i] = temp;
    }
  }
}

NGL_API void nglCopyLine32To24 (void* pDst, void* pSrc, int PixelCount, bool Invert)
{                                          
  int i;
  char* pSource = (char*) pSrc;
  if (Invert)
  {
    char r,g,b;
    char* pDest   = (char*) pDst + 3 * (PixelCount - 1) - 1;
    for (i=0; i<PixelCount; i++)
    {
      r = *pSource++;
      g = *pSource++;
      b = *pSource++;
      pSource++;
      *pDest-- = b;
      *pDest-- = g;
      *pDest-- = r;
    }
  }
  else
  {
    char* pDest   = (char*) pDst;
    for (i=0; i<PixelCount; i++)
    {
      *pDest++ = *pSource++;
      *pDest++ = *pSource++;
      *pDest++ = *pSource++;
      pSource++;
    }
  }
}                                          
                                           
NGL_API void nglCopyLine32To32 (void* pDst, void* pSrc, int PixelCount, bool Invert)
{                                          
  if (Invert)
  {
    int i;
    char r,g,b,a;
    char* pSource = (char*) pSrc;
    char* pDest   = (char*) pDst + 4 * (PixelCount - 1) - 1;
    for (i=0; i<PixelCount; i++)
    {
      r = *pSource++;
      g = *pSource++;
      b = *pSource++;
      a = *pSource++;
      *pDest-- = a;
      *pDest-- = b;
      *pDest-- = g;
      *pDest-- = r;
    }
  }
  else
  {
    memcpy(pDst,pSrc,PixelCount*4);
  }
}     

NGL_API void nglCopyLine32To32ARGB (void* pDst, void* pSrc, int PixelCount, bool Invert)
{                                          
  if (Invert)
  {
    int i;
    char r,g,b,a;
    char* pSource = (char*) pSrc;
    char* pDest   = (char*) pDst + 4 * (PixelCount - 1) - 1;
    for (i=0; i<PixelCount; i++)
    {
      r = *pSource++;
      g = *pSource++;
      b = *pSource++;
      a = *pSource++;
      *pDest-- = a;
      *pDest-- = r;
      *pDest-- = g;
      *pDest-- = b;
    }
  }
  else
  {
    char* pSource = (char*) pSrc;
    char* pDest   = (char*) pDst;
    //memcpy(pDst,pSrc,PixelCount*4);
    for (int i=0; i<PixelCount; ++i)
    {
      pDest[i*4+0] = pSource[i*4+2];
      pDest[i*4+1] = pSource[i*4+1];
      pDest[i*4+2] = pSource[i*4+0];
      pDest[i*4+3] = pSource[i*4+3];
    }
  }
} 

/// Stubber:
NGL_API nglCopyLineFn nglGetCopyLineFn (int DstBPP, int SrcBPP)
{
  switch (SrcBPP)
  {
  case 8:
    switch (DstBPP)
    {
    case 8:
      return &nglCopyLine8To8;
      break;
    case 15:
      return &nglCopyLine8To15;
      break;
    case 16:
      return &nglCopyLine8To16;
      break;
    case 24:
      return &nglCopyLine8To24;
      break;
    case 32:
      return &nglCopyLine8To32;
      break;
    }
    break;
  case 15:
    switch (DstBPP)
    {
    case 8:
      return &nglCopyLine15To8;
      break;
    case 15:
      return &nglCopyLine15To15;
      break;
    case 16:
      return &nglCopyLine15To16;
      break;
    case 24:
      return &nglCopyLine15To24;
      break;
    case 32:
      return &nglCopyLine15To32;
      break;
    }
    break;
  case 16:
    switch (DstBPP)
    {
    case 8:
      return &nglCopyLine16To8;
      break;
    case 15:
      return &nglCopyLine16To15;
      break;
    case 16:
      return &nglCopyLine16To16;
      break;
    case 24:
      return &nglCopyLine16To24;
      break;
    case 32:
      return &nglCopyLine16To32;
      break;
    }
    break;
  case 24:
    switch (DstBPP)
    {
    case 8:
      return &nglCopyLine24To8;
      break;
    case 15:
      return &nglCopyLine24To15;
      break;
    case 16:
      return &nglCopyLine24To16;
      break;
    case 24:
      return &nglCopyLine24To24;
      break;
    case 32:
      return &nglCopyLine24To32;
      break;
    }
    break;
  case 32:
    switch (DstBPP)
    {
    case 8:
      return &nglCopyLine32To8;
      break;
    case 15:
      return &nglCopyLine32To15;
      break;
    case 16:
      return &nglCopyLine32To16;
      break;
    case 24:
      return &nglCopyLine32To24;
      break;
    case 32:
      return &nglCopyLine32To32;
      break;
    }
    break;
  }
  return NULL; // There is no line copy function for the given pixel types...
}

NGL_API void nglCopyImage(void* pDst, int dstwidth, int dstheight, int dstbpp, void* pSrc, int srcwidth, int srcheight, int srcbpp, bool vmirror, bool hmirror)
{
  int i;
  int sizex = 0, sizey = 0;
  int slinesize = srcwidth * ((srcbpp+1)/8);
  int dlinesize = dstwidth * ((dstbpp+1)/8);

  nglCopyLineFn pCpFn = nglGetCopyLineFn(dstbpp,srcbpp);

  if (!pCpFn)
    return;

  if ( dstwidth > srcwidth )
  {
    sizex = srcwidth;
  }
  else if ( dstwidth < srcwidth )
  {
    sizex = dstwidth;
  }
  else
    sizex = dstwidth;

  if ( dstheight > srcheight )
    sizey = srcheight;
  else
    sizey = dstheight;

  char* pSource = (char*) pSrc;
  char* pDest = (char*) pDst;
  if (vmirror)
  {
    pSource += slinesize * (sizey-1);
    for (i = 0; i< sizey; i++)
    {
      pCpFn(pDest,pSource, sizex, hmirror);
      pSource -= slinesize;
      pDest += dlinesize;
    }
  }
  else
  {
    for (i = 0; i< sizey; i++)
    {
      pCpFn(pDest,pSource, sizex, hmirror);
      pSource += slinesize;
      pDest += dlinesize;
    }
  }
}







