//
//  nuiDebugServer.cpp
//  nui3
//
//  Created by Sebastien Metrot on 02/02/16.
//  Copyright © 2016 libNUI. All rights reserved.
//

#include "nuiDebugServer.h"
#include "nuiTCPClient.h"

class nuiDebugProtocol : public nuiProtocol
{
public:
  nuiDebugProtocol(nuiTCPClient* pClient)
  : nuiProtocol(pClient)
  {
    AddMethod("UpdateWindowList",
    [=]()
    {
      
    });

    std::function<void(int32)> fn =
    [=](int32 i)
    {
      printf("Hello world2 %d!\n", i);
    };
    AddMethod("HelloWorld2", fn);

  
    AddMethod("HelloWorld3", nui_make_function([](int32 i, float j, double k, nglString s)
              {
                printf("Hello world3 %d %f %f %s!\n", i, j, k, s.GetChars());
              }));
  }
  
};

class nuiDebugClient : public nuiTCPClient
{
public:
  nuiDebugClient(nuiDebugServer *pServer, nuiSocket::SocketType sock)
  : nuiTCPClient(sock), mpServer(pServer)
  {
    mpProtocol = new nuiDebugProtocol(this);
  }
  
  void OnCanRead()
  {
    mpProtocol->HandleMessages();
  }
  
protected:
  nuiDebugServer* mpServer = nullptr;
  nuiProtocol *mpProtocol = nullptr;
};

///////////////////////////////// Debug Server
nuiDebugServer::nuiDebugServer()
{
  
}

nuiDebugServer::~nuiDebugServer()
{
  
}

void nuiDebugServer::Start(int16 port)
{
  if (Bind("127.0.0.1", port))
  {
    bool res = Listen();
    if (res)
    {
      mSocketPool.Add(this, nuiSocketPool::eStateChange);
      mRunning = true;
      
      mpThread = new nglThreadFunction([=](){
        while (mRunning)
        {
          mSocketPool.DispatchEvents(100);
        }
        
        mSocketPool.Del(this);
        Close();
        delete mpThread;
        mpThread = nullptr;
      });
      
      mpThread->Start();
    }
  }
}

void nuiDebugServer::Stop()
{
  mRunning = false;
}

nuiTCPClient* nuiDebugServer::OnCreateClient(nuiSocket::SocketType sock)
{
  return new nuiDebugClient(this, sock);
}

