#include "os/NetworkSocket.h"
#include "utl/Str.h"

NetAddress
NetworkSocket::SetIPPortFromHostPort(const char *c1, const char *c2, unsigned short port) {
    NetAddress addr;
    String str(c1);
    unsigned int idx = str.find(':');
    if (idx != FixedString::npos) {
        String str2 = str.substr(idx + 1);
        str = str.substr(0, idx);
        addr.mPort = strtol(str2.c_str(), 0, 0);
    } else {
        addr.mPort = port;
    }
    unsigned int ip = IPStringToInt(str);
    if (ip != -1) {
        addr.mIP = ip;
    }
    if (addr.mIP == 0) {
        if (c2) {
            str += ".";
            str += c2;
        }
        addr.mIP = ResolveHostName(str);
    }
    return addr;
}

// void NetworkSocket::SetIPPortFromHostPort
//                (NetAddress *out,char *param_2,char *param_3,ushort param_4)

// {

//   out->mIP = 0;
//   out->mPort = 0;
//   String::String(&SStack_50,param_2);
//   uVar2 = FixedString::find(&SStack_50.super_FixedString,':');

//   uVar2 = IPStringToInt(&SStack_50);
//   if (uVar2 != 0xffffffff) {
//     out->mIP = uVar2;
//   }
//   if (out->mIP == 0) {
//     if (param_3 != 0x0) {
//       String::operator+=(&SStack_50,".");
//       String::operator+=(&SStack_50,param_3);
//     }
//     SVar1 = String::String(aSStack_40,&SStack_50);
//     uVar2 = ResolveHostName(SVar1);
//     out->mIP = uVar2;
//   }
//   String::~String(&SStack_50);
//   return;
// }
