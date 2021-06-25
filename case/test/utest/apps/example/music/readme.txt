注意事项：
1.对example进行代码覆盖测试时，需更改以下2点：
.更改config_main.c，让小机运行时直接加载被测应用而不加载usb，因为debug模式需占用usb资源。
.更改example中music_main.c，让应用从list界面按return键时能退出应用。

2.如果先执行了单元测试，再执行代码覆盖测试，需进\case\apps\config对config重新进行编译。因为单元测试时把所有的测试用例以及测试框架都编译成了一个config.app，所以用完后要把config.app恢复成正常编译产生的文件。