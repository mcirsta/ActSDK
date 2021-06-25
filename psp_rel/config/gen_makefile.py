#coding=cp936
def find_addr_from_makefile(makefile_content):
    import re
    lib_path = re.search(r'^SO_LIB_PATH\s*=\s*([^\r\n]+)\s*$',makefile_content,re.M)
    out_path = re.search(r'.*^OUT_DIR\s*=\s*([^\r\n]+)\s*$',makefile_content,re.M)
    start_addr= re.compile(r'.*^START_ADDR\s*=\s*(0x\d+)\s*$',re.M|re.DOTALL)
    end_addr = re.compile(r'.*^END_ADDR\s*=\s*(0x\d+)\s*$',re.M|re.DOTALL)
    addr_interval = re.compile(r'.*^ADDR_INTERVAL\s*=\s*(0x\d+)\s*$',re.M|re.DOTALL)
    #print start_addr.match(makefile_content)
    #print end_addr.match(makefile_content)
    #print addr_interval.match(makefile_content)
    #print lib_path.group(1)
    #print out_path.group(1)
    return (start_addr.match(makefile_content).group(1),
            end_addr.match(makefile_content).group(1),
            addr_interval.match(makefile_content).group(1),
            lib_path.group(1),
            out_path.group(1))
def find_target_from_dependcfg(dep_content):
    return re.findall(r'^\s*(\w+\.so)\s*=([\w .]+)\s*$',dep_content,re.M)


if __name__ == "__main__":
    import codecs
    import sys
    import re
    import string
    mk_file = open(sys.argv[1])
    mkfile_content = mk_file.read();
    #all_keys=[[bug[0].split(), bug[1], bug[2], bug[3], bug[0]] for target in dep_targets]
    #print start_addr.match(mkfile_content).group(1)
    #find_addr_from_makefile(mkfile_content)
    (start_addr, end_addr, addr_interval,lib_dir, out_dir) = find_addr_from_makefile(mkfile_content)

    dep_file=open(sys.argv[2])
    dep_content=dep_file.read()
    dep_all = re.search(r'^\s*all\s*=\s*(.*)\s*$',dep_content,re.M).group(1)
    dep_targets = find_target_from_dependcfg(dep_content)
    addr = string.atoi(start_addr, base=16)
    interval = string.atoi(addr_interval, base=16)
    end_addr = string.atoi(end_addr, base=16)
    ld_str = 'all:\n'
    for (target,depend) in dep_targets:
        if(re.search(target, dep_all)):
            ld_str += '\t$(LD)  '
            depend_files=depend.split(' ')
            #print depend_files
            for file in depend_files:
                if(file):
                    ld_str +=  ' ' + lib_dir + '/'+ file
            ld_str += ' $(LIBCRT) $(LIBS) $(LD_OPTS) --section-start .init='
            ld_str += hex(addr) + ' -o ' + out_dir + '/' + target + '\n'
            ld_str += '\t$(OBJDUMP)' + ' -D ' + out_dir + '/' + target + ' > ' + out_dir + '/' + target + '.lst' + '\n'
        addr += interval
        if ( addr > end_addr):
             sys.stderr.writelines("addr out of range ")
             sys.exit(1)

    target_all_in_mkfile = re.compile(r'.*(^\s*all\s*:.*)^\s*\w*\s*:',re.M|re.DOTALL).match(mkfile_content).group(1)
    #print target_all_in_mkfile
    print string.replace(mkfile_content,target_all_in_mkfile,ld_str)



