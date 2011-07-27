var path = require('path')
var monol = require('./../build/default/monol')
var domain = new monol.MonolAppDomain("DomainZero")

console.log(domain.name)
console.log(domain)
var assemblyPath = path.join(__dirname,"app.exe")
console.log(assemblyPath)
var asm = domain.openAssembly(assemblyPath)
var appClass = asm.getType('MyNamespace','App')
console.log(appClass)
console.log(appClass.constructor.name)

var appInstance = appClass.newInstance()
console.log(appInstance)
console.log(appInstance.constructor.name)
appInstance.invokeMethod("MyNamespace.App:Run()")

domain.shutdown()