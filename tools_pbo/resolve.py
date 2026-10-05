#!/usr/bin/env python3
"""resolve.py FILE choice1 choice2 ...   choices: ours | theirs | both | theirs_both(=theirs then ours) | @file (custom text from file)"""
import sys
f = sys.argv[1]; choices = sys.argv[2:]
L = open(f, encoding='utf-8').read().split('\n'); out = []; i = 0; k = 0
while i < len(L):
    if L[i].startswith('<<<<<<<'):
        ours, theirs = [], []; i += 1
        while not L[i].startswith('======='): ours.append(L[i]); i += 1
        i += 1
        while not L[i].startswith('>>>>>>>'): theirs.append(L[i]); i += 1
        c = choices[k] if k < len(choices) else choices[-1]; k += 1
        if c == 'ours': out += ours
        elif c == 'theirs': out += theirs
        elif c == 'both': out += ours + theirs
        elif c == 'theirs_both': out += theirs + ours
        elif c.startswith('@'): out += open(c[1:]).read().rstrip('\n').split('\n')
        i += 1; continue
    out.append(L[i]); i += 1
open(f, 'w', encoding='utf-8').write('\n'.join(out))
print(f, k, 'hunks')
