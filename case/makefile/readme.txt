API_Generator.py:
1.拷贝c语言定义的apitbl[]到一新建文件，例如api.c
2.在cygwin环境中运行脚本：
cat api.c | python  ../../case/makefile/API_Generator.py SYSCALL_API_START >API.S
其中：
api.c：输入文件名
../../case/makefile/API_Generator.py：python脚本
SYSCALL_API_START：起始API号
API.S：输出文件名
