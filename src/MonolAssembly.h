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
class MonolType;
class MonolObject;

using namespace v8;
using namespace node;

class MonolAssembly : ObjectWrap {
private:
	MonoAssembly *assembly;
	MonoImage * image;
	MonoDomain * domain;
	static Persistent<FunctionTemplate> MonolAssemblyConstructor;
public:
	MonolAssembly(MonoAssembly *assembly, MonoDomain * domain);
	
	static Handle<Value> Create(MonoAssembly *assembly, MonoDomain * domain);
	
	static void Init(Handle<Value> value);
	
	// Get a Type to Inspect or Instantiate
	// Example:
	//	new monol.MonolAppDomain("DomainZero").openAssembly('app.exe').getType('TheNamespace','TheClassName');
	static Handle<Value> GetType(const Arguments& args);
	
	MonoDomain * appDomain();
};