
# monol: Node.JS and V8 getting along with the CIL universe(Mono Runtime)  
monol is a progressive effort to build a Node.JS module that allows calls into CIL universe(Mono Runtime) from Node.JS and V8 scripts.

## Features
 - Initialize the Runtime
 - Load Assembly File from Disk
 - Discover Types in Assembly
 - Create Object Instances

## Usage

###app.cs

    using System;
    namespace MyNamespace {
        public class App {
            public static void Main() {
                Console.WriteLine("Hello from C#");	
            }
            public void Run() {
                Console.WriteLine("Printed from CIL Universe");
            }
        }
    }
    // Compile with: mcs app.cs

###app.js

    var path = require('path')
    var monol = require('monol')
    var domain = new monol.MonolAppDomain("DomainZero")
    var asm = domain.openAssembly('app.js')
    var appClass = asm.getType('MyNamespace','App')
    var appInstance = appClass.newInstance()
    appInstance.invokeMethod("MyNamespace.App:Run()")
    domain.shutdown()

###Configuring and Compiling

You need to pre-configure the following components:
 - Node.JS
 - Mono Runtime
 - Mono Development Libraries and Headers( Mono CSDK in MacOSX or mono-dev packages in Debian/Ubuntu)


    git clone git://github.com/firebaseco/monol.git
    cd monol
    node-waf configure build && node example/app.js

##Status
This project has been suspended due lack of interest and time. If you are interested in a similar solution or you want to contribute to this project, I can help you coordinate efforts to make this happen.

	
## Author
Johan Hernandez: johan@firebase.co
Drop me an email if you need anything, thanks!

## License

Copyright (c) 2011 Firebase.co

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
