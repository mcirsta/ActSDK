API_Generator.py:
1.����c���Զ����apitbl[]��һ�½��ļ�������api.c
2.��cygwin���������нű���
cat api.c | python  ../../case/makefile/API_Generator.py SYSCALL_API_START >API.S
���У�
api.c�������ļ���
../../case/makefile/API_Generator.py��python�ű�
SYSCALL_API_START����ʼAPI��
API.S������ļ���
