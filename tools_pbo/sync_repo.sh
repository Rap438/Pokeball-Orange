#!/bin/bash
# Mirror the committed pokeemerald tree + generator tools into OG's GitHub repo and push.
set -e
SRC=/home/claude/pokeemerald; DST=/home/claude/pokeball-orange
MSG="$1"
TMP=$(mktemp -d); (cd $SRC && git archive HEAD | tar -x -C $TMP)
mkdir -p $TMP/tools_pbo/assets
(cd /home/claude/tools && cp *.py *.sh runner.c new_species.json $TMP/tools_pbo/ 2>/dev/null || true)
cp /home/claude/title/src.png $TMP/tools_pbo/assets/title_art.png
find $DST -mindepth 1 -maxdepth 1 ! -name .git -exec rm -rf {} +
cp -a $TMP/. $DST/
rm -rf $TMP
cd $DST
git add -A
git -c user.name="OG" -c user.email="bp3jackson@gmail.com" commit -qm "$MSG

Co-Authored-By: Claude Opus 5.5 <noreply@anthropic.com>
Claude-Session: https://claude.ai/code/session_019FzykcShgGfFmgSm3dA7Kd" || echo "nothing to commit"
timeout 900 git push -q origin main && git log --oneline | head -3
