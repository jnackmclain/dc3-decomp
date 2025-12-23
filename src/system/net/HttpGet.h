#pragma once
#include "os/NetworkSocket.h"
#include "os/Timer.h"
#include "utl/MemMgr.h"
#include "utl/Str.h"

class HttpGet {
public:
    enum State {
    };

    HttpGet(unsigned int, unsigned short, const char *, const char *);
    HttpGet(unsigned int, unsigned short, const char *, unsigned char, const char *);
    virtual ~HttpGet();
    virtual void SetContent(const char *) {}
    virtual void SetContentLength(unsigned int) {}

    bool IsDownloaded();
    bool HasFailed();
    char *DetachBuffer();
    void Send();
    void Poll();
    void SetTimeout(float);
    unsigned int GetBufferSize();

    MEM_OVERLOAD(HttpGet, 0x1C);

private:
    void AddRequiredHeaders();

    static const float kDefaultTimeoutMs;
    static const int kMaxRetries;
    static const int kRecvBufSize;

protected:
    virtual bool CanRetry();
    virtual void StartSending();
    virtual void Sending() {
        MILO_FAIL("HttpGet::Sending() - shouldn't be calling this");
    }

    void StartReceiving();
    void SafeDisconnect();
    void SafeShutdown();
    void StartConnection();
    bool HasTimedOut();
    void SetState(State);

    NetworkSocket *mSocket; // 0x8
    String unkc; // 0xc
    unsigned short unk14; // 0x14
    int unk18;
    bool unk1c;
    Timer unk20;
    float unk50;
    unsigned int unk54;
    String unk58;
    void *unk60;
    int mRecvBufPos; // 0x64
    u32 unk68;
    char *mFileBuf; // 0x6c
    int mFileBufSize; // 0x70
    int mFileBufRecvPos; // 0x74
    int unk78;
    int unk7c;
    int unk80;
};

class HttpPost : public HttpGet {
public:
    HttpPost(unsigned int, unsigned short, const char *, unsigned char);
    virtual ~HttpPost();
    virtual void SetContent(const char *content) { mContent = content; }
    virtual void SetContentLength(unsigned int);

protected:
    virtual bool CanRetry();
    virtual void StartSending();
    virtual void Sending();

    const char *mContent; // 0x88
    unsigned int mContentLength; // 0x8c
    unsigned int unk90; // 0x90
    String unk94; // 0x94
    int unk9c;
};
