import sys, struct
from PIL import Image
def tiles8(data, wt, ht, pal, remap=None, palbase=256):
    im = Image.new('RGBA', (wt*8, ht*8), (0,0,0,0)); px = im.load()
    for t in range(wt*ht):
        tx, ty = t % wt, t // wt
        for y in range(8):
            for x in range(8):
                i = t*64 + y*8 + x
                if i >= len(data): continue
                v = data[i]
                if remap: v = remap[v] if v else 0
                if v:
                    c = struct.unpack_from('<H', pal, (palbase+v)*2)[0]
                    px[tx*8+x, ty*8+y] = ((c&31)*255//31, ((c>>5)&31)*255//31, ((c>>10)&31)*255//31, 255)
    return im
