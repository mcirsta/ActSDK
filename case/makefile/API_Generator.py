

def output(one_func, index):
    h1="""LEAF("""
    h2=""")
	.set noreorder
	.text
"""
    h3=""":
    li	v1, """
    h4="""+"""
    h5="""
	syscall
END ("""
    h6=""")
"""
    return h1 + one_func + h2 + one_func + h3 + sys.argv[1] + h4 + index + h5 + one_func + h6

def procTbl(tbl_str):
    sa = tbl_str.split(',')
    api = [elem[elem.index(')')+1:] for elem in sa if elem.find(')')!=-1]
    print """#include "asm-mips/asm.h"
#include "asm-mips/regdef.h"
#include "ucos/api.h"
"""
    index=0
    for one_func in api:
        print output(one_func, str(index))
        index = index + 1        

import sys

if __name__ == "__main__":    
    procTbl(sys.stdin.read())
    
    
