import struct
from PIL import Image
def lin8(d,W,H,pal,palbase=256,transparent0=True):
    im=Image.new('RGBA',(W,H),(0,0,0,0)); px=im.load()
    for i in range(min(len(d),W*H)):
        v=d[i]
        if v==0 and transparent0: continue
        c=struct.unpack_from('<H',pal,(palbase+v)*2)[0]
        px[i%W,i//W]=((c&31)*255//31,((c>>5)&31)*255//31,((c>>10)&31)*255//31,255)
    return im
