import sys, pickle, struct
sys.path.insert(0,'/home/claude/tools')
from buudec import decompress
from show8 import tiles8
from PIL import Image, ImageDraw
rom=open('/home/claude/buu/buu.gba','rb').read()
def sheet(blocks, pal, out, cols=16, scale=2, dims=None):
    ims=[]
    for (o,ln,n) in blocks:
        d,_=decompress(rom,o)
        if dims: w,h=dims(ln)
        else:
            w,h={64:(8,8),128:(8,16),256:(16,16),512:(16,32),1024:(32,32),2048:(32,64),4096:(64,64),8192:(64,128),16384:(128,128)}.get(ln,(32,len(d)//32))
        ims.append((o,tiles8(d,w//8,h//8,pal)))
    cw=max(i.width for _,i in ims); ch=max(i.height for _,i in ims)+8
    rows=(len(ims)+cols-1)//cols
    S=Image.new('RGBA',(cols*(cw+2),rows*ch),(70,70,100,255)); dr=ImageDraw.Draw(S)
    for k,(o,im) in enumerate(ims):
        x=(k%cols)*(cw+2); y=(k//cols)*ch
        S.alpha_composite(im,(x,y+8)); dr.text((x,y-2),f'{o:x}'[-5:],fill=(255,255,0,255))
    S=S.resize((S.width*scale,S.height*scale),Image.NEAREST); S.save(out)
if __name__=='__main__':
    r=pickle.load(open('/home/claude/buu/all.pk','rb'))
    pal=open(sys.argv[1],'rb').read()
    lo,hi=int(sys.argv[2],16),int(sys.argv[3],16)
    sizes=set(int(x) for x in sys.argv[4].split(','))
    bl=[x for x in r if lo<=x[0]<hi and x[1] in sizes]
    print(len(bl))
    sheet(bl[:int(sys.argv[6]) if len(sys.argv)>6 else 160],pal,sys.argv[5])
