#!/usr/bin/env python3
"""PokeBall Orange battle UI palettes: navy panels with orange/gold frames and white text (Buu's Fury look).
Recolours the battle text box / menus (textbox_0/1.pal, text.pal, text_pp.pal) and the healthboxes
(ball_status_bar.png palette). Originals in /home/claude/restyle/orig_ui."""
from PIL import Image
D = '/home/claude/pokeemerald/graphics/battle_interface'

def jasc(path, cols):
    with open(path, 'w', newline='\r\n') as f:
        f.write('JASC-PAL\n0100\n16\n')
        for c in cols:
            f.write('%d %d %d\n' % c)

NAVY = (20, 28, 72)
# message box / action prompt (palette 0)
jasc(f'{D}/textbox_0.pal', [
    (0, 0, 0), (248, 200, 96), (248, 248, 248), (200, 96, 0), (248, 160, 32), NAVY,
    (8, 8, 32), (40, 48, 104), (24, 24, 56), (12, 12, 24), (200, 104, 16), (120, 56, 0),
    (248, 192, 64), (248, 128, 0), (176, 72, 0), NAVY])
# menu frames (palette 1)
jasc(f'{D}/textbox_1.pal', [
    (0, 0, 0), (120, 56, 0), (200, 96, 0), (248, 160, 32), (12, 12, 24), NAVY,
    (40, 40, 88), (32, 40, 96), (16, 16, 32), (56, 64, 112), (248, 208, 72), (96, 48, 0),
    (0, 0, 0), (0, 0, 0), (0, 0, 0), (0, 0, 0)])
# menu text (palette 5): fill 14, text 13, shadow 15, PP 12/11
jasc(f'{D}/text.pal', [
    (0, 0, 0), (255, 0, 0), (131, 0, 0), (255, 164, 98), (131, 82, 49), (0, 0, 0), (0, 0, 0), (0, 0, 0),
    (0, 0, 0), (0, 0, 0), (0, 0, 0), (8, 8, 32), (248, 248, 248), (248, 248, 248), NAVY, (8, 8, 32)])
# PP colours by amount left: (text, shadow) pairs: low, lower, empty, normal
jasc(f'{D}/text_pp.pal', [
    (248, 224, 64), (8, 8, 32), (248, 144, 0), (8, 8, 32), (248, 56, 32), (8, 8, 32), (248, 248, 248), (8, 8, 32),
    (0, 0, 0), (0, 0, 0), (0, 0, 0), (0, 0, 0), (0, 0, 0), (0, 0, 0), (0, 0, 0), (0, 0, 0)])
# healthboxes
im = Image.open('/home/claude/restyle/orig_ui/ball_status_bar.png')
pal = im.getpalette()[:48]
cols = [tuple(pal[i * 3:i * 3 + 3]) for i in range(16)]
cols[1] = (248, 248, 248)   # text
cols[2] = NAVY              # fill
cols[3] = (10, 14, 40)      # shade / text shadow
cols[4] = (248, 144, 0)     # accent
cols[5] = (200, 96, 0)
cols[6] = (240, 160, 32)    # frame
cols[7] = (168, 88, 0)
cols[8] = (96, 48, 0)
flat = [v for c in cols for v in c] + [0] * (768 - 48)
im.putpalette(flat)
im.save(f'{D}/ball_status_bar.png')
print('ok')
