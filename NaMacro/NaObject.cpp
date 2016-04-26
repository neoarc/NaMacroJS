#include "NaObject.h"

Global<ObjectTemplate> NaObject::s_NaObjectTemplate;

NaObject::NaObject()
{
	m_nRef = 0;
}


NaObject::~NaObject()
{
	if (!m_Persistent.IsEmpty())
	{
		m_Persistent.ClearWeak();
		m_Persistent.Reset();
	}
}

int NaObject::AddRef()
{
	if (!m_Persistent.IsEmpty())
		m_Persistent.ClearWeak();

	return ++m_nRef;
}

int NaObject::Release()
{
	if (--m_nRef == 0)
	{
		MakeWeak();
		delete this;
	}
	return m_nRef;
}

Local<ObjectTemplate> NaObject::MakeObjectTemplate(Isolate * isolate)
{
	EscapableHandleScope handle_scope(isolate);

	Local<ObjectTemplate> result = ObjectTemplate::New(isolate);
	result->SetInternalFieldCount(1);

	// Again, return the result through the current handle scope.
	return handle_scope.Escape(result);
}

Local<Object> NaObject::WrapObject(Isolate *isolate, NaObject * pObject)
{
	// Local scope for temporary handles.
	EscapableHandleScope handle_scope(isolate);

	// Fetch the template for creating JavaScript map wrappers.
	// It only has to be created once, which we do on demand.
	Global<ObjectTemplate> &_ObjectTemplate = pObject->GetObjectTemplate();
	if (_ObjectTemplate.IsEmpty())
	{
		Local<ObjectTemplate> RawTemplate = pObject->MakeObjectTemplate(isolate);
		_ObjectTemplate.Reset(isolate, RawTemplate);
	}

	Local<ObjectTemplate> templ = Local<ObjectTemplate>::New(isolate, _ObjectTemplate);

	// Create an empty NaObject wrapper.
	Local<Object> result = templ->NewInstance(isolate->GetCurrentContext()).ToLocalChecked();

	// Wrap the raw C++ pointer in an External so it can be referenced from within JavaScript.
	Local<External> obj_ptr = External::New(isolate, pObject);

	// Store the NaObject pointer in the JavaScript wrapper.
	result->SetInternalField(0, obj_ptr);

	pObject->AddRef();

	// Make weak
	Local<Object> handle = v8::Local<v8::Object>::New(isolate, pObject->m_Persistent);
	pObject->m_Persistent.Reset(isolate, result);
	pObject->MakeWeak();

	// Return the result through the current handle scope.  Since each
	// of these handles will go away when the handle scope is deleted
	// we need to call Close to let one, the result, escape into the
	// outer handle scope.
	return handle_scope.Escape(result);

	//pObject->AddRef();
}

NaObject * NaObject::UnwrapObject(Local<Object> obj)
{
	Local<External> field = Local<External>::Cast(obj->GetInternalField(0));
	void* ptr = field->Value();

	return static_cast<NaObject*>(ptr);
}

void NaObject::MakeWeak()
{
	m_Persistent.SetWeak(this, WeakCallback);
	m_Persistent.MarkIndependent();
}

void NaObject::WeakCallback(
	const v8::WeakCallbackData<v8::Object, NaObject>& data
	//Persistent<Value> obj, void * pData
	)
{
	NaObject *pObject = data.GetParameter();
	if (pObject)
	{
		pObject->Release();
	}
}
