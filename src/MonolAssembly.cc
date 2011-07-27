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

#include "MonolAssembly.h"
#include "MonolType.h"
#include "MonolObject.h"


MonolAssembly::MonolAssembly(MonoAssembly * assembly, MonoDomain * domain) {
	this->assembly = assembly;
	this->domain = domain;
	this->image = mono_assembly_get_image(assembly);
}

Handle<Value> MonolAssembly::Create(MonoAssembly *assembly, MonoDomain * domain) {
	HandleScope scope;
	Local<Function> function = MonolAssemblyConstructor->GetFunction();
	MonolAssembly * a = new MonolAssembly(assembly, domain);
	Handle<Object> o = function->NewInstance();
	a->Wrap(o);
	return scope.Close(o);
}

void MonolAssembly::Init(Handle<Value> value) {
	Local<FunctionTemplate> ctor = FunctionTemplate::New();
	MonolAssemblyConstructor = Persistent<FunctionTemplate>::New(ctor);
	MonolAssemblyConstructor->InstanceTemplate()->SetInternalFieldCount(1);
	MonolAssemblyConstructor->SetClassName(String::NewSymbol("MonolAssembly"));
	NODE_SET_PROTOTYPE_METHOD(MonolAssemblyConstructor, "getType", GetType);
}

Handle<Value> MonolAssembly::GetType(const Arguments& args) {
	if (args.Length() < 1) {
		return ThrowException(Exception::Error(String::New("getType method requires the namespace of the type")));
	}
	else if(!args[0]->IsString()) {
		return ThrowException(Exception::Error(String::New("getType method requires the namespace of the type to be a String")));
	}
	if (args.Length() < 2) {
		return ThrowException(Exception::Error(String::New("getType method requires the class name of the type")));
	}
	else if(!args[1]->IsString()) {
		return ThrowException(Exception::Error(String::New("getType method requires the class name of the type to be a String")));
	}
	String::AsciiValue ns(args[0]->ToString());
	String::AsciiValue name(args[1]->ToString());
	MonolAssembly* ma = ObjectWrap::Unwrap<MonolAssembly>(args.This());
	MonoClass * klass = mono_class_from_name(ma->image, *ns,*name);
	if(!klass) {
		return ThrowException(Exception::Error(String::New("getType failed. Please verify that the namespace and class name are correct.")));
	}
	return MonolType::Create(klass, ma);
}
MonoDomain * MonolAssembly::appDomain() {
	return this->domain;
}
Persistent<FunctionTemplate> MonolAssembly::MonolAssemblyConstructor;