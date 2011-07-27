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

#include <v8.h>
#include <node.h>
#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/mono-config.h>
using namespace v8;
using namespace node;

class MonolAppDomain : ObjectWrap {
private:
	static Persistent<FunctionTemplate> MonolAppDomainConstructor;
	MonoDomain *domain;
	Persistent<String> domainName;

public:
	static Handle<Value> Constructor(const Arguments& args);

	static void InitDomainFunctions(Handle<Object> target);

	static Handle<Value> NameGetter(Local<v8::String> name, 
		const v8::AccessorInfo& info);

	// Shutdown the AppDomain
	// Example:
	//	new monol.MonolAppDomain("DomainZero").shutdown();
	static Handle<Value> Shutdown(const Arguments& args);
	
	// Opens an Assembly
	// Example:
	//	new monol.MonolAppDomain("DomainZero").openAssembly('app.exe');
	static Handle<Value> OpenAssembly(const Arguments& args);
};