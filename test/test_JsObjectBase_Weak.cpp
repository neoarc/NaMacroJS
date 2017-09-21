#include "stdafx.h"
#include <catch/catch.hpp>

#include <NaMacroCommon.h>

#include <V8Wrap.h>
#include <JsObjectBase.h>

#include <NaLib/NaString.h>

TEST_CASE("JsOBjectBase.Weak")
{
	// #TODO Cannot test NaMacro classes

	/*
	auto p = V8Wrap::Initialize();
	{
		Isolate* isolate = V8Wrap::CreateNewIsolate();
		Isolate::Scope _is(isolate);
		HandleScope _hs(isolate);
		{
			JsObjectBase *pJs = new JsObjectBase;
			Local<Object> obj = JsObjectBase::WrapObject(isolate, pJs);
		}
	}
	V8Wrap::TearDown(p);
	//*/
}
