#!/usr/bin/env python3
"""Resolve all conflict hunks in files by keeping both sides (ours then theirs): union.py FILE..."""
import sys
for f in sys.argv[1:]:
    L = open(f, encoding='utf-8').read().split('\n'); out = []
    for l in L:
        if l.startswith('<<<<<<<') or l.startswith('=======') or l.startswith('>>>>>>>'):
            continue
        out.append(l)
    open(f, 'w', encoding='utf-8').write('\n'.join(out))
