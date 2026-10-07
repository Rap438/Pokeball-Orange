// Headless mGBA runner for automated testing of PokeBall Orange.
// Usage: runner rom.gba script.txt   (script "-" reads commands from stdin, for interactive driving)
// Script commands (one per line):
//   run N               - run N frames with no input
//   press KEYS N        - hold KEYS (comma list: A,B,SELECT,START,RIGHT,LEFT,UP,DOWN,R,L) for N frames, then release 1 frame
//   hold KEYS N         - hold KEYS for N frames without release
//   shot FILE.ppm       - write screenshot
//   save FILE / load FILE - savestate
//   peek8/peek16/peek32 ADDR - print memory
//   echo TEXT           - print TEXT (lets a driver wait for the commands before it to finish)
#include <mgba/flags.h>
#include <mgba/core/core.h>
#include <mgba/core/config.h>
#include <mgba/core/interface.h>
#include <mgba/core/log.h>
#include <stdarg.h>
static void noLog(struct mLogger* l, int c, enum mLogLevel v, const char* f, va_list a) {(void)l;(void)c;(void)v;(void)f;(void)a;}
static struct mLogger quiet = { .log = noLog, .filter = NULL };
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static struct mCore* core;
// address of gSaveBlock1Ptr (pret build: 0x03005dac); override with env SB1PTR for other builds
static unsigned sb1Ptr(void) { const char* e = getenv("SB1PTR"); return e ? (unsigned)strtoul(e, NULL, 16) : 0x03005dac; }
static mColor* buf;
static unsigned W, H;

static int parseKeys(const char* s) {
	int k = 0;
	char tmp[128]; strncpy(tmp, s, 127); tmp[127] = 0;
	for (char* t = strtok(tmp, ","); t; t = strtok(NULL, ",")) {
		if (!strcmp(t, "A")) k |= 1;
		else if (!strcmp(t, "B")) k |= 2;
		else if (!strcmp(t, "SELECT")) k |= 4;
		else if (!strcmp(t, "START")) k |= 8;
		else if (!strcmp(t, "RIGHT")) k |= 16;
		else if (!strcmp(t, "LEFT")) k |= 32;
		else if (!strcmp(t, "UP")) k |= 64;
		else if (!strcmp(t, "DOWN")) k |= 128;
		else if (!strcmp(t, "R")) k |= 256;
		else if (!strcmp(t, "L")) k |= 512;
		else if (!strcmp(t, "NONE")) k |= 0;
	}
	return k;
}

static void frames(int keys, int n) {
	core->setKeys(core, keys);
	for (int i = 0; i < n; i++) core->runFrame(core);
}

static void shot(const char* f) {
	FILE* o = fopen(f, "wb");
	fprintf(o, "P6 %u %u 255\n", W, H);
	for (unsigned i = 0; i < W * H; i++) {
		uint32_t c = buf[i];
		unsigned char px[3] = { c & 0xFF, (c >> 8) & 0xFF, (c >> 16) & 0xFF };
		fwrite(px, 1, 3, o);
	}
	fclose(o);
}

int main(int argc, char** argv) {
	if (argc < 3) { fprintf(stderr, "usage\n"); return 1; }
	mLogSetDefaultLogger(&quiet);
	core = mCoreFind(argv[1]);
	if (!core) { fprintf(stderr, "no core\n"); return 1; }
	core->init(core);
	mCoreInitConfig(core, NULL);
	core->baseVideoSize(core, &W, &H);
	buf = calloc(W * H, sizeof(mColor));
	core->setVideoBuffer(core, buf, W);
	if (!mCoreLoadFile(core, argv[1])) { fprintf(stderr, "load fail\n"); return 1; }
	if (getenv("SAV")) { if (!mCoreLoadSaveFile(core, getenv("SAV"), getenv("SAVTMP") != NULL)) fprintf(stderr, "sav load fail\n"); }
	core->reset(core);
	FILE* s = strcmp(argv[2], "-") ? fopen(argv[2], "r") : stdin;
	if (!s) { fprintf(stderr, "no script %s\n", argv[2]); return 1; }
	char line[512];
	while (fgets(line, sizeof line, s)) {
		char cmd[32], a[256]; int n = 0; a[0] = 0;
		if (sscanf(line, "%31s", cmd) != 1 || cmd[0] == '#') continue;
		if (!strcmp(cmd, "echo")) { char* t = line + 4; while (*t == ' ') t++; fputs(t, stdout); }
		else if (!strcmp(cmd, "run")) { sscanf(line, "%*s %d", &n); frames(0, n); }
		else if (!strcmp(cmd, "press")) { sscanf(line, "%*s %255s %d", a, &n); frames(parseKeys(a), n ? n : 2); frames(0, 1); }
		else if (!strcmp(cmd, "hold")) { sscanf(line, "%*s %255s %d", a, &n); frames(parseKeys(a), n); }
		else if (!strcmp(cmd, "shot")) { sscanf(line, "%*s %255s", a); shot(a); }
		else if (!strcmp(cmd, "save")) {
			sscanf(line, "%*s %255s", a);
			size_t sz = core->stateSize(core); void* st = malloc(sz);
			core->saveState(core, st);
			FILE* o = fopen(a, "wb"); fwrite(st, 1, sz, o); fclose(o); free(st);
		} else if (!strcmp(cmd, "load")) {
			sscanf(line, "%*s %255s", a);
			size_t sz = core->stateSize(core); void* st = malloc(sz);
			FILE* i = fopen(a, "rb"); if (!i) { fprintf(stderr, "no state %s\n", a); return 1; }
			fread(st, 1, sz, i); fclose(i);
			core->loadState(core, st); free(st);
		} else if (!strcmp(cmd, "poke8") || !strcmp(cmd, "poke16")) {
			unsigned addr, v; sscanf(line, "%*s %x %x", &addr, &v);
			if (cmd[4] == '8') core->busWrite8(core, addr, v); else core->busWrite16(core, addr, v);
		} else if (!strcmp(cmd, "setvar")) {
			unsigned var, v; sscanf(line, "%*s %x %x", &var, &v);
			unsigned sb1 = core->busRead32(core, sb1Ptr());
			core->busWrite16(core, sb1 + 0x139C + (var - 0x4000) * 2, v);
		} else if (!strcmp(cmd, "getvar")) {
			unsigned var; sscanf(line, "%*s %x", &var);
			unsigned sb1 = core->busRead32(core, sb1Ptr());
			printf("var %04X = %04X\n", var, core->busRead16(core, sb1 + 0x139C + (var - 0x4000) * 2));
		} else if (!strncmp(cmd, "peek", 4)) {
			unsigned addr; sscanf(line, "%*s %x", &addr);
			if (!strcmp(cmd, "peek8")) printf("%08X: %02X\n", addr, core->busRead8(core, addr));
			else if (!strcmp(cmd, "peek16")) printf("%08X: %04X\n", addr, core->busRead16(core, addr));
			else printf("%08X: %08X\n", addr, core->busRead32(core, addr));
		} else if (!strcmp(cmd, "dump")) {
			unsigned addr, len; sscanf(line, "%*s %x %x %255s", &addr, &len, a);
			FILE* o = fopen(a, "wb");
			for (unsigned i = 0; i < len; i++) { unsigned char b = core->rawRead8(core, addr + i, -1); fwrite(&b, 1, 1, o); }
			fclose(o);
		} else if (!strcmp(cmd, "poke8")) {
			unsigned addr, v; sscanf(line, "%*s %x %x", &addr, &v); core->busWrite8(core, addr, v);
		} else if (!strcmp(cmd, "poke32")) {
			unsigned addr, v; sscanf(line, "%*s %x %x", &addr, &v); core->busWrite32(core, addr, v);
		}
		fflush(stdout);
	}
	core->deinit(core);
	return 0;
}
