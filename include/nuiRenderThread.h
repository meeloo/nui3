//
//  nuiRenderThread.h
//  nui3
//
//  Created by Sebastien Metrot on 14/12/14.
//  Copyright (c) 2014 libNUI. All rights reserved.
//

#pragma once

class nuiMetaPainter;
class nuiLayer;

class nuiRenderingStat
{
public:
  nglString mName;
  double mTime;
  int64  mCount;
};

class nuiRenderThread : public nglThread
{
public:
  typedef nuiFastDelegate2<nuiRenderThread*, bool> RenderingDoneDelegate; ///< Beware! This delegate is called on the Render Thread! Not on the main thread!

  nuiRenderThread(nglContext* pContext, nuiDrawContext* pDrawContext, nuiPainter* pDestinationPainter, const RenderingDoneDelegate& RenderingDone);
  virtual ~nuiRenderThread();

  // Public API:
  void LockRendering()    { mRenderingLock.Lock(); }
  void UnlockRendering()  { mRenderingLock.Unlock(); }

  void StartRendering(uint32 x, uint32 y);
  void SetRect(const nuiRect& rRect);
  //  void AddClipRect(const nuiRect& rRect);
  void Exit();
  void SetWidgetContentsPainter(nuiWidget* pWidget, nuiRef<nuiMetaPainter> pPainter);
  void SetWidgetDrawPainter(nuiWidget* pWidget, nuiRef<nuiMetaPainter> pPainter);
  void SetLayerDrawPainter(nuiLayer* pLayer, nuiRef<nuiMetaPainter> pPainter);
  void SetLayerContentsPainter(nuiLayer* pLayer, nuiRef<nuiMetaPainter> pPainter);
  void InvalidateLayerContents(nuiLayer* pLayer);
  void SetRootWidget(nuiWidget* pRoot);
  void RunTaskOnRenderThread(nuiTask* rTask, bool OnNextFrame);
  void SetLayerTree(nuiLayer* pLayerRoot);

  nuiPainter* GetPainter() const;
  nuiDrawContext* GetDrawContext() const;
  nglContext* GetContext() const;

  void DrawWidgetContents(nuiDrawContext* pContext, nuiWidget* pKey);
  void DrawLayerContents(nuiDrawContext* pContext, nuiLayer* pKey);
  void DrawWidget(nuiDrawContext* pContext, nuiWidget* pKey);
  void DrawLayer(nuiDrawContext* pContext, nuiLayer* pKey);

  static void DestroyWidget(nuiWidget* pWidget);
  static void DestroyLayer(nuiLayer* pLayer);

  std::map<nuiLayer*, nuiRenderingStat> GetStats() const;

private:
  void Post(nuiTask* pTask);
  
  nglCriticalSection mRenderingLock;

  nuiTaskQueue mQueue;
  nuiTaskQueue mNextFrameQueue;

  nuiRect mRect;
  std::vector<nuiRect> mPartialRects;
  nuiWidget* mpRoot = nullptr;
  std::map<nuiWidget*, nuiRef<nuiMetaPainter>> mWidgetContentsPainters;
  std::map<nuiWidget*, nuiRef<nuiMetaPainter>> mWidgetDrawPainters;
  std::map<nuiLayer*, nuiRef<nuiMetaPainter>> mLayerContentsPainters;
  std::map<nuiLayer*, nuiRef<nuiMetaPainter>> mLayerDrawPainters;
  std::set<nuiLayer*> mDirtyLayers;
  nglContext* mpContext = nullptr;
  nuiDrawContext* mpDrawContext = nullptr;
  nuiPainter* mpPainter = nullptr;
  RenderingDoneDelegate mRenderingDone;
  bool mContinue = true;
  nuiLayer* mpLayerTreeRoot = nullptr;

  void _StartRendering(uint32 x, uint32 y);
  void _SetRect(const nuiRect& rRect);
  void _Exit();
  void _SetWidgetContentsPainter(nuiWidget* pWidget, nuiRef<nuiMetaPainter> pPainter);
  void _SetWidgetDrawPainter(nuiWidget* pWidget, nuiRef<nuiMetaPainter> pPainter);
  void _SetLayerDrawPainter(nuiLayer* pLayer, nuiRef<nuiMetaPainter> pPainter);
  void _SetLayerContentsPainter(nuiLayer* pLayer, nuiRef<nuiMetaPainter> pPainter);
  void _InvalidateLayerContents(nuiLayer* pLayer);
  void _SetRootWidget(nuiWidget* pWidget);
  void _SetLayerTree(nuiLayer* pRoot);
  void _RenderFrame(const nuiRef<nuiMetaPainter>& pFrame);

  virtual void OnStart();

  void RenderingDone(bool result);

  nglAtomic mRenderingTicks=0;
  int mWidgetIndentation = 0;


  static nglCriticalSection ThreadsCS;
  static std::set<nuiRenderThread*> mThreads;

  mutable nglCriticalSection StatsCS;
  std::map<nuiLayer*, nuiRenderingStat> mLayerStats;
  void DumpStats();
};
