import sys,pickle; sys.path.insert(0,'/home/claude/tools')
from buudec import decompress
from show8 import tiles8
from PIL import Image, ImageDraw
rom=open('/home/claude/buu/buu.gba','rb').read(); pal=open('/home/claude/buu/rips/g3.pal','rb').read()
r=pickle.load(open('/home/claude/buu/all.pk','rb'))
lo,hi=int(sys.argv[1],16),int(sys.argv[2],16); out=sys.argv[3]; cols=int(sys.argv[4]) if len(sys.argv)>4 else 12
b=[x for x in r if lo<=x[0]<hi and x[1]==512]
rows=(len(b)+cols-1)//cols
S=Image.new('RGBA',(cols*40,rows*46),(70,70,100,255)); d=ImageDraw.Draw(S)
for k,(o,ln,n) in enumerate(b):
    dd,_=decompress(rom,o); im=tiles8(dd,2,4,pal)
    x=(k%cols)*40; y=(k//cols)*46
    S.alpha_composite(im,(x+12,y+12)); d.text((x+1,y),f'{k}:{o&0xFFFFF:05x}',fill=(255,255,0,255))
S=S.resize((S.width*3,S.height*3),Image.NEAREST); S.save(out)
print(len(b))
