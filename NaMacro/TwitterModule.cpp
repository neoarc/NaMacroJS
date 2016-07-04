#include "stdafx.h"

#include "Common.h"
#include "TwitterModule.h"


void NaTwitterModule::Create(Isolate * isolate, Local<ObjectTemplate>& global_template)
{
#define ADD_GLOBAL_METHOD(_js_func, _c_func)	ADD_TEMPLATE_METHOD(global_template, _js_func, _c_func);

	// methods
	ADD_GLOBAL_METHOD(method, Method);
}

void NaTwitterModule::Init(Isolate * isolate, Local<ObjectTemplate>& global_template)
{
	
}

void NaTwitterModule::Release()
{
	
}

// description: 
// syntax:		
void NaTwitterModule::Method(V8_FUNCTION_ARGS)
{
	Isolate *isolate = args.GetIsolate();
	
	//twitCurl tc;

	std::string ck = std::string("qaOgNbwSlaBfZljuFbsL1TBPN");
	std::string cs = std::string("2KIzuD82ASp9IGvbSUYqCV2CeNgVtMqd4cln5VMTygrte3QhpF");
	std::string tk = std::string("713937644-atFLOQ6fFCphm9H2Tqaf29w2jfKCcnpfwSanjEd6");
	std::string ts = std::string("WJyeuClQrv9cwqtD6eP05K8Nsj0aSQXcxPU2lIosSigm4");


	//  Send the same status update as shown in this example:
	//  https://dev.twitter.com/docs/api/1.1/post/statuses/update

	//  IMPORTANT: Make sure this app has read/write access.
	//  Otherwise it cannot post an update (i.e. tweet) to the Twitter account.

	//CkHttpRequest req;
	//req.AddParam("status", "test chilkat");
	//resp = http.PostUrlEncoded("https://api.twitter.com/1.1/statuses/update.json", req);

	
}
