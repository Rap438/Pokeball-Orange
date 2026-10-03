D="-DENABLE_DEBUGGERS -DENABLE_DIRECTORIES -DENABLE_GDB_STUB -DENABLE_SCRIPTING -DENABLE_VFS -DENABLE_VFS_FD -DM_CORE_GBA -DUSE_PNG -DUSE_PTHREADS -DUSE_ZLIB -DUSE_LZMA -DUSE_MINIZIP -DHAVE_CRC32 -D_GNU_SOURCE -std=c11"
gcc -O2 $D -o runner runner.c -I/home/claude/mgba/include -I/home/claude/mgba/build/include -L/home/claude/mgba/build -lmgba -Wl,-rpath,/home/claude/mgba/build
