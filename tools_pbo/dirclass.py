import sys,pickle,struct; sys.path.insert(0,'/home/claude/tools')
from buudec import decompress
import numpy as np
rom=open('/home/claude/buu/buu.gba','rb').read(); pal=open('/home/claude/buu/rips/g3.pal','rb').read()
b=pickle.load(open('/home/claude/buu/npc512.pk','rb'))
rgb=np.array([[ (c&31)*8, ((c>>5)&31)*8, ((c>>10)&31)*8] for c in [struct.unpack_from('<H',pal,(256+i)*2)[0] for i in range(256)]])
def frame(i):
    d,_=decompress(rom,b[i][0]); return np.frombuffer(d,np.uint8).reshape(4,2,8,8).transpose(0,2,1,3).reshape(32,16)
def classify(a):
    m=a>0
    cols=np.where(m.any(0))[0]
    if len(cols)==0: return '?'
    # center mask horizontally
    l,r=cols[0],cols[-1]
    sub=m[:,l:r+1]; asym=(sub!=sub[:,::-1]).sum()/max(1,sub.sum())
    c=rgb[a]; lum=c.sum(-1)
    head=slice(2,14)
    white=((lum[head]>600)&m[head]).sum()
    skin=((c[head][...,0]>180)&(c[head][...,1]>120)&(c[head][...,2]<170)&(c[head][...,0]>c[head][...,2]+50)&m[head]).sum()
    if asym>0.32: return 'S'
    return 'F' if (white>=2 or skin>12) else 'B'
def seq(s,n=24):
    return ''.join(classify(frame(s+k)) for k in range(n))
if __name__=='__main__':
    import ast
    for name,s in ast.literal_eval(sys.argv[1]): print(f'{name:8s}',seq(s))
