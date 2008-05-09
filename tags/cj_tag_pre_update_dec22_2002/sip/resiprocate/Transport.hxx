#if !defined(TRANSPORT_HXX)
#define TRANSPORT_HXX

#include <exception>

#include "sip2/util/Data.hxx"
#include "sip2/util/Fifo.hxx"
#include "sip2/util/Socket.hxx"
#include "sip2/util/BaseException.hxx"
#include "sip2/sipstack/Message.hxx"

namespace Vocal2
{

class SipMessage;
class SendData;

class Transport
{
   public:
      typedef enum 
      {
         Unknown,
         UDP,
         TCP,
         TLS,
         SCTP,
         DCCP,
      } Type;

      class Tuple
      {
         public:
            Tuple();
            Tuple(struct in_addr pipv4,
                  int pport,
                  Transport::Type ptype);
            
            bool operator<(const Tuple& rhs) const;
            bool operator==(const Tuple& rhs) const;
            
            struct in_addr ipv4;
            int port;
            Transport::Type transportType;
            Transport* transport;
      };
      
      class Exception : public BaseException
      {
         public:
            Exception(const Data& msg, const Data& file, const int line);
            const char* name() const { return "TransportException"; }
      };
      
      // sendHost is localhost (may be a dns name referring to this sip element)
      // portNum is the port to receive and/or send on
      Transport(const Data& sendHost, int portNum, const Data& nic, Fifo<Message>& rxFifo);
      // !ah! need to think about type for
      // interface specification here.
      
      virtual ~Transport();
      
      virtual void send( const Tuple& tuple, const Data& data, const Data& tid);
      virtual void process(FdSet& fdset) = 0;
      virtual void buildFdSet( FdSet& fdset);
      
      void run(); // will not return.
      void shutdown();

      void fail(const Data& tid); // called when transport failed
      void ok(const Data& tid); // called when the transport succeeds
      
      // These methods are used by the TransportSelector
      virtual const Data& hostname() const { return mHost; } 
      virtual int port() const { return mPort; } 
      virtual Type transport() const =0 ;
      virtual bool isReliable() const =0;
      
      static Type toTransport( const Data& );
      static Data toData( Type );


   protected:
      Socket mFd; // this is a unix file descriptor or a windows SOCKET
      Data mHost;
      int mPort;
      Data mInterface;
      Fifo<SendData> mTxFifo; // owned by the transport
      Fifo<Message>& mStateMachineFifo; // passed in
   private:

      bool mShutdown ;
};

std::ostream& operator<<(std::ostream& strm, const Transport::Tuple& tuple);

class SendData
{
   public:
      SendData(const Transport::Tuple& dest, const Data& pdata, const Data& tid): 
         destination(dest),
         data(pdata),
         transactionId(tid) 
      {}
      const Transport::Tuple destination;
      const Data data;
      const Data transactionId;
};

}

#if ( (__GNUC__ == 3) && (__GNUC_MINOR__ >= 1) )
#include <ext/hash_map>

namespace __gnu_cxx
{

struct hash<Vocal2::Transport::Tuple>
{
      size_t operator()(const Vocal2::Transport::Tuple& addrPort) const;
};
 
}

#endif // __GNUC__
#endif


/* ====================================================================
 * The Vovida Software License, Version 1.0 
 * 
 * Copyright (c) 2000 Vovida Networks, Inc.  All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 
 * 3. The names "VOCAL", "Vovida Open Communication Application Library",
 *    and "Vovida Open Communication Application Library (VOCAL)" must
 *    not be used to endorse or promote products derived from this
 *    software without prior written permission. For written
 *    permission, please contact vocal@vovida.org.
 *
 * 4. Products derived from this software may not be called "VOCAL", nor
 *    may "VOCAL" appear in their name, without prior written
 *    permission of Vovida Networks, Inc.
 * 
 * THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESSED OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, TITLE AND
 * NON-INFRINGEMENT ARE DISCLAIMED.  IN NO EVENT SHALL VOVIDA
 * NETWORKS, INC. OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT DAMAGES
 * IN EXCESS OF $1,000, NOR FOR ANY INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
 * USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
 * DAMAGE.
 * 
 * ====================================================================
 * 
 * This software consists of voluntary contributions made by Vovida
 * Networks, Inc. and many individuals on behalf of Vovida Networks,
 * Inc.  For more information on Vovida Networks, Inc., please see
 * <http://www.vovida.org/>.
 *
 */