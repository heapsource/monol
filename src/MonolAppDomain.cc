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

#include "MonolAppDomain.h"
#include "MonolAssembly.h"
#include "MonolType.h"
#include "MonolObject.h"

Handle<Value> MonolAppDomain::Constructor(const Arguments& args)
{
	if (args.Length() < 1) {
		return ThrowException(Exception::Error(String::New("Must provide the name of the AppDomain to the constructor of MonolAppDomain")));
	}
	else if(!args[0]->IsString()) {
		return ThrowException(Exception::Error(String::New("The name of AppDomain provided to the constructor of MonolAppDomain must be a String")));
	}
	HandleScope scope;
	Local<Value> domainNameVal = (Local<Value>)args[0];
	Local<String> domainNameStr = domainNameVal->ToString();
	//char * domainName = (char*)malloc(sizeof(char) * domainNameStr->Length());
	//domainNameStr->WriteAscii(domainName);
	String::AsciiValue domainName(domainNameStr);

	MonolAppDomain* md = new MonolAppDomain();
	md->domainName = Persistent<String>::New(domainNameStr);
	md->Wrap(args.This());
	md->domain = mono_jit_init (*domainName);
	if(!md->domain) {
		return ThrowException(Exception::Error(String::New("An error has occurred while initializing the AppDomain")));
	}
	return args.This();
}
void MonolAppDomain::InitDomainFunctions(Handle<Object> target) {
	MonolAssembly::Init(target);
	MonolType::Init(target);
	MonolObject::Init(target);
	Local<FunctionTemplate> createDomainCtorFunc = FunctionTemplate::New(Constructor);
	MonolAppDomainConstructor = Persistent<FunctionTemplate>::New(createDomainCtorFunc);
	MonolAppDomainConstructor->InstanceTemplate()->SetInternalFieldCount(1);
	MonolAppDomainConstructor->SetClassName(String::NewSymbol("MonolAppDomain"));
	MonolAppDomainConstructor->PrototypeTemplate()->SetAccessor(v8::String::New("name"), NameGetter, NULL);
	NODE_SET_PROTOTYPE_METHOD(MonolAppDomainConstructor, "shutdown", Shutdown);
	NODE_SET_PROTOTYPE_METHOD(MonolAppDomainConstructor, "openAssembly", OpenAssembly);
	target->Set(String::NewSymbol("MonolAppDomain"),
	                MonolAppDomainConstructor->GetFunction());
}

Handle<Value> MonolAppDomain::NameGetter(Local<v8::String> name, 
                  const v8::AccessorInfo& info) {
	MonolAppDomain* md = ObjectWrap::Unwrap<MonolAppDomain>(info.This());
  return md->domainName;
}

Handle<Value> MonolAppDomain::Shutdown(const Arguments& args) {
	MonolAppDomain* md = ObjectWrap::Unwrap<MonolAppDomain>(args.This());
	mono_jit_cleanup (md->domain);
	md->domain = NULL;
	return v8::Null();
}

Handle<Value> MonolAppDomain::OpenAssembly(const Arguments& args) {
	if (args.Length() < 1) {
		return ThrowException(Exception::Error(String::New("openAssembly method requires the fullpath of the assembly file to load")));
	}
	else if(!args[0]->IsString()) {
		return ThrowException(Exception::Error(String::New("openAssembly method requires the fullpath of the assembly to be a String")));
	}
	Local<String> assemblyPathValue = args[0]->ToString();
	String::AsciiValue assemblyPath(assemblyPathValue);

	MonolAppDomain* md = ObjectWrap::Unwrap<MonolAppDomain>(args.This());
	MonoAssembly * assembly = mono_domain_assembly_open (md->domain, *assemblyPath);
	if (!assembly)
		return ThrowException(Exception::Error(String::New("An error has ocurred while opening assembly")));
	Handle<Value>  monolAssembly =  MonolAssembly::Create(assembly, md->domain);
	return monolAssembly;
}
Persistent<FunctionTemplate> MonolAppDomain::MonolAppDomainConstructor;