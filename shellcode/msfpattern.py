#python msfpattern.py create 200>1.txt
#python msfpattern.py offset 0x64413764
#获取到的大小是返回地址前。
import sys
import struct

def generate():
    for x in xrange(0x41, 0x5b):
        for y in xrange(0x61, 0x7b):
            for z in xrange(0x30, 0x3a):
                yield "%c%c%c" % (x, y, z)

cmd = sys.argv[1]

if cmd == 'create':
    size = int(sys.argv[2])
    s = ''
    for x in generate():
        s += x
        if len(s) >= size:
            print s[:size]
            break
    else:
        raise Exception("size too large")
elif cmd == 'offset':
    value = int(sys.argv[2], 16)
    chunk = struct.pack('<I', value)
    s = ''
    for x in generate():
        s += x
        if chunk in s:
            print s.index(chunk)
            break
    else:
        raise Exception("not found")
