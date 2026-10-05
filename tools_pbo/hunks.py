#!/usr/bin/env python3
"""Print merge conflict hunks of files: hunks.py FILE... (with N lines of context)"""
import sys
for f in sys.argv[1:]:
    L = open(f, encoding='utf-8', errors='replace').read().split('\n')
    i = 0; k = 0
    while i < len(L):
        if L[i].startswith('<<<<<<<'):
            k += 1
            j = i
            while not L[j].startswith('>>>>>>>'): j += 1
            print(f'===== {f} hunk {k} (line {i+1})')
            for x in range(max(0, i - 3), min(len(L), j + 4)):
                print(f'{x+1:6}: {L[x]}')
            i = j
        i += 1
