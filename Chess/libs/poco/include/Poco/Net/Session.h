//
// Session.h
//
// $Id: //poco/1.4/NetSSL_OpenSSL/include/Poco/Net/Session.h#1 $
//
// Library: NetSSL_OpenSSL
// Package: SSLCore
// Module:  Session
//
// Definition of the Session class.
//
// Copyright (c) 2010, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
// 
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//


#ifndef NetSSL_Session_INCLUDED
#define NetSSL_Session_INCLUDED


#include "Poco/Net/NetSSL.h"
#include "Poco/RefCountedObject.h"
#include "Poco/AutoPtr.h"
#include <openssl/ssl.h>


namespace Poco {
namespace Net {


class NetSSL_API Session: public Poco::RefCountedObject
	/// This class encapsulates a SSL session object
	/// used with session caching on the client side.
	///
	/// For session caching to work, a client must
	/// save the session object from an existing connection,
	/// if it wants to reuse it with a future connection.
{
public:
	typedef Poco::AutoPtr<Session> Ptr;

	SSL_SESSION* sslSession() const;
		/// Returns the stored OpenSSL SSL_SESSION object.

protected:	
	Session(SSL_SESSION* pSession);
		/// Creates a new Session object, using the given
		/// SSL_SESSION object. 
		/// 
		/// The SSL_SESSION's reference count is not changed.

	~Session();
		/// Destroys the Session.
		///
		/// Calls SSL_SESSION_free() on the stored
		/// SSL_SESSION object.

private:
	Session();

	SSL_SESSION* _pSession;
	
	friend class SecureSocketImpl;
};


//
// inlines
//
inline SSL_SESSION* Session::sslSession() const
{
	return _pSession;
}


} } // namespace Poco::Net


#endif // NetSSL_Session_INCLUDED
