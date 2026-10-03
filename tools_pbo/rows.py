import sys,pickle; sys.path.insert(0,'/home/claude/tools')
from buudec import decompress
from show8 import tiles8
from PIL import Image, ImageDraw
rom=open('/home/claude/buu/buu.gba','rb').read(); pal=open('/home/claude/buu/rips/g3.pal','rb').read()
b=pickle.load(open('/home/claude/buu/npc512.pk','rb'))
def rows(starts,out,n=20,pre=2,scale=2):
    S=Image.new('RGBA',((n)*18+40,len(starts)*36),(90,90,120,255)); d=ImageDraw.Draw(S)
    for j,(name,s) in enumerate(starts):
        d.text((1,j*36+2),name[:7],fill=(255,255,255,255))
        for k in range(n):
            i=s-pre+k
            dd,_=decompress(rom,b[i][0]); x=40+k*18; y=j*36
            S.alpha_composite(tiles8(dd,2,4,pal),(x+1,y+3))
            if k==pre: d.rectangle((x,y,x+17,y+35),outline=(255,0,0,255))
            d.text((x+1,y-1),str(i%1000),fill=(255,255,0,180))
    S.resize((S.width*scale,S.height*scale),0).save(out)
if __name__=='__main__':
    import ast
    rows(ast.literal_eval(sys.argv[1]),sys.argv[2])
