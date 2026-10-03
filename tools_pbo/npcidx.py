import sys,pickle; sys.path.insert(0,'/home/claude/tools')
from buudec import decompress
from show8 import tiles8
from PIL import Image, ImageDraw
rom=open('/home/claude/buu/buu.gba','rb').read(); pal=open('/home/claude/buu/rips/g3.pal','rb').read()
r=pickle.load(open('/home/claude/buu/all.pk','rb'))
b=[x for x in r if 0x2C0000<=x[0]<0x7B8000 and x[1]==512]
s,e=int(sys.argv[1]),int(sys.argv[2]); out=sys.argv[3]; cols=24
chunk=b[s:e]; rows=(len(chunk)+cols-1)//cols
S=Image.new('RGBA',(cols*22,rows*42),(70,70,100,255)); d=ImageDraw.Draw(S)
for k,(o,ln,n) in enumerate(chunk):
    dd,_=decompress(rom,o); im=tiles8(dd,2,4,pal)
    x=(k%cols)*22; y=(k//cols)*42
    S.alpha_composite(im,(x+3,y+9)); d.text((x+1,y-1),str(s+k),fill=(255,255,0,255))
S=S.resize((S.width*2,S.height*2),Image.NEAREST); S.save(out)
