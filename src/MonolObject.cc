/* Copyright Firebase.co<http://www.firebase.co>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to permit
 * persons to whom the Software is furnished to do so, subject to the
 * following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN
 * NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE
 * USE OR OTHER DEALINGS IN THE SOFTWARE.
 */


#include "MonolObject.h"
#include "MonolType.h"
#include <stdio.h>
#include <string.h>
MonolObject::MonolObject(MonoObject * mobject, MonolType * monolType) {
	this->mobject = mobject;
	this->monolType = monolType;
}

Handle<Value> MonolObject::Create(MonoObject *mobject, MonolType * monolType) {
	HandleScope scope;
	Local<Function> function = MonolObjectConstructor->GetFunction();
	MonolObject * a = new MonolObject(mobject, monolType);
	Handle<Object> o = function->NewInstance();
	a->Wrap(o);
	return scope.Close(o);
}

void MonolObject::Init(Handle<Value> value) {
	Local<FunctionTemplate> ctor = FunctionTemplate::New();
	MonolObjectConstructor = Persistent<FunctionTemplate>::New(ctor);
	MonolObjectConstructor->InstanceTemplate()->SetInternalFieldCount(1);
	MonolObjectConstructor->SetClassName(String::NewSymbol("MonolObject"));
	NODE_SET_PROTOTYPE_METHOD(MonolObjectConstructor, "invokeMethod", InvokeMethod);
}

Handle<Value> MonolObject::InvokeMethod(const Arguments& args) {
	printf("InvokeMethod\n");
	if (args.Length() < 1) {
		return ThrowException(Exception::Error(String::New("invokeMethod method requires the method name")));
	}
	else if(!args[0]->IsString()) {
		return ThrowException(Exception::Error(String::New("invokeMethod method requires the method name to be a String")));
	}
	
	MonolObject* mo = ObjectWrap::Unwrap<MonolObject>(args.This());
	
/*	const char* className = mono_class_get_name(mo->monolType->getClass());
	printf("Classname\n");
	printf(className);
	printf("\n");*/
	String::AsciiValue methodName(args[0]->ToString());
	const char* methodNameStr = *methodName; 
	printf("methodNameStr\n");
	printf("'%s'", methodNameStr);
	printf("\n");
	/*char * fullMethodName = (char*)malloc(sizeof(char) *(strlen(methodNameStr) + strlen(className) + 1) );
	sprintf("%s.%s",fullMethodName, className, methodNameStr);
	printf("fullMethodName\n");
	printf(fullMethodName);
	printf("\n");
	*/
	MonoMethodDesc* methodDesc = mono_method_desc_new(methodNameStr, 1);
	if (!methodDesc) {
		return ThrowException(Exception::Error(String::New("invokeMethod lookup failed for method")));
	}
	mono_method_desc_search_in_class (methodDesc, mo->monolType->getClass());
	
}

Persistent<FunctionTemplate> MonolObject::MonolObjectConstructor;