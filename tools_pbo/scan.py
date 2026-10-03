import sys; sys.path.insert(0,'/home/claude/tools')
from buudec import decompress
rom=open('/home/claude/buu/buu.gba','rb').read()
import pickle
res=[]
lo=int(sys.argv[1],16); hi=int(sys.argv[2],16)
o=lo
while o<hi:
    try:
        d,n=decompress(rom,o,maxlen=0x20000)
        if len(d)>=64:
            res.append((o,len(d),n)); o+=(n+3)&~3; continue
    except Exception: pass
    o+=4
pickle.dump(res,open(sys.argv[3],'wb'))
print(len(res))
