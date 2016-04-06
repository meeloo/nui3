/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot

 licence: see nui3/LICENCE.TXT
 */

#pragma once

#include "nui.h"
#include "nuiSocket.h"
#include "nglCriticalSection.h"

class nuiPipe
{
public:
  nuiPipe();
  virtual ~nuiPipe();

  size_t Write(const uint8* pBuffer, size_t size);
  size_t Write(const nglString& rString);
  size_t Read(uint8* pBuffer, size_t size);
  size_t GetSize() const;
  const uint8* LockBuffer();
  void UnlockBuffer();
  void Eat(size_t size);
  void Clear();

protected:
  mutable nglCriticalSection mCS;
  std::vector<uint8> mBuffer;
};


class nuiTCPClient : public nuiSocket
{
public:
  nuiTCPClient();
  virtual ~nuiTCPClient();

  bool Connect(const nuiNetworkHost& rHost, nuiSocketPool* pPool = nullptr, nuiSocketPool::TriggerMode triggerMode = nuiSocketPool::eStateChange);
  bool Connect(const nglString& rHost, int16 port, nuiSocketPool* pPool = nullptr, nuiSocketPool::TriggerMode triggerMode = nuiSocketPool::eStateChange);
  bool Connect(uint32 ipaddress, int16 port, nuiSocketPool* pPool = nullptr, nuiSocketPool::TriggerMode triggerMode = nuiSocketPool::eStateChange);

  size_t Send(const std::vector<uint8>& rData);
  size_t Send(const uint8* pData, size_t len);
  size_t Send(const nglString& rString);
  size_t ReceiveAvailable(std::vector<uint8>& rData); ///< Fill the rData vector with the data that is already available to read on the socket.
  size_t Receive(std::vector<uint8>& rData); ///< Fill the buffer up to its current size.
  size_t Receive(uint8* pData, size_t len); ///< Fill the buffer up to its current size.

  bool IsConnected() const;
  bool IsWriteConnected() const;
  bool IsReadConnected() const;
  int32 GetAvailable() const;
  bool GetCanWrite() const;

  void Close();

  void SetNoDelay(bool set);

  // Buffered client interface:
  size_t BufferedSend(const uint8* pBuffer, size_t size, bool BufferOnly = true);
  size_t BufferedSend(const nglString& rString, bool BufferOnly = true);
  size_t BufferedReceive(uint8* pBuffer, size_t size);

  virtual void OnCanRead();
  virtual void OnCanWrite();
  virtual void OnReadClosed();
  virtual void OnWriteClosed();

  nuiSignal0<> CanRead;
  nuiSignal0<> CanWrite;
  nuiSignal0<> ReadClosed;
  nuiSignal0<> WriteClosed;
  nuiSignal0<> Connected;
  nuiSignal0<> ConnectError;

  void SetAutoDelete(bool set); ///< Delete the client when the send buffer is empty after a call to SendWriteBuffer()
  void SendWriteBuffer();

  void SetAutoPool(nuiSocketPool* pPool);

  nglString GetDesc() const;

protected:
  friend class nuiTCPServer;
  nuiTCPClient(int sock);
  bool mReadConnected;
  bool mWriteConnected;

  nuiPipe mIn;
  nuiPipe mOut;

  // This is only used by stream handlers
  size_t WriteToInputBuffer(const uint8* pBuffer, size_t size);
  size_t ReadFromOutputBuffer(uint8* pBuffer, size_t size);

  int64 mReceived;
  int64 mSent;

  bool mAutoDelete;
  nuiSocketPool* mpAutoPool;
};


