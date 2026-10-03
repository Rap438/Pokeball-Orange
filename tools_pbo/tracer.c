// Watchpoint tracer: tracer rom state frames min max [type: w|r] [keys]
// Prints unique (pc, source) of accesses in [min,max], with registers for first hit of each pc.
#include <mgba/flags.h>
#include <mgba/core/core.h>
#include <mgba/core/config.h>
#include <mgba/core/log.h>
#include <mgba/debugger/debugger.h>
#include <mgba/internal/arm/arm.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
static void noLog(struct mLogger* l, int c, enum mLogLevel v, const char* f, va_list a) {(void)l;(void)c;(void)v;(void)f;(void)a;}
static struct mLogger quiet = { .log = noLog };
static struct mCore* core;
static uint32_t seen[4096]; static int nseen;
static long hits;
static void entered(struct mDebuggerModule* m, enum mDebuggerEntryReason r, struct mDebuggerEntryInfo* info) {
	m->isPaused = false;
	if (r != DEBUGGER_ENTER_WATCHPOINT) return;
	hits++;
	struct ARMCore* cpu = core->cpu;
	uint32_t pc = cpu->gprs[15];
	int src = info->type.wp.accessSource;
	if (getenv("DECOMP") && (pc < 0x03000040 || pc > 0x030002e0)) return;
	uint32_t key = getenv("DECOMP") ? (cpu->gprs[1] >> 8) : getenv("BYADDR") ? (info->address >> 10) : (pc ^ (src << 28));
	for (int i = 0; i < nseen; i++) if (seen[i] == key) return;
	if (nseen < 4096) seen[nseen++] = key;
	printf("pc=%08X thumb=%d src=%d addr=%08X val=%08X w=%d |", pc, cpu->executionMode, src, info->address, info->type.wp.newValue, info->width);
	for (int i = 0; i < 15; i++) printf(" r%d=%08X", i, cpu->gprs[i]);
	printf("\n");
}
int main(int argc, char** argv) {
	mLogSetDefaultLogger(&quiet);
	core = mCoreFind(argv[1]); core->init(core); mCoreInitConfig(core, NULL);
	unsigned W, H; core->baseVideoSize(core, &W, &H);
	mColor* buf = calloc(W * H, sizeof(mColor)); core->setVideoBuffer(core, buf, W);
	mCoreLoadFile(core, argv[1]); core->reset(core);
	FILE* f = fopen(argv[2], "rb"); size_t sz = core->stateSize(core); void* st = malloc(sz); fread(st, 1, sz, f); fclose(f); core->loadState(core, st);
	int frames = atoi(argv[3]);
	uint32_t mn = strtoul(argv[4], 0, 16), mx = strtoul(argv[5], 0, 16);
	int type = (argc > 6 && argv[6][0] == 'r') ? WATCHPOINT_READ : WATCHPOINT_WRITE;
	int keys = argc > 7 ? atoi(argv[7]) : 0;
	struct mDebugger dbg; mDebuggerInit(&dbg); mDebuggerAttach(&dbg, core);
	struct mDebuggerModule mod; memset(&mod, 0, sizeof mod);
	mod.type = DEBUGGER_CUSTOM; mod.entered = entered;
	mDebuggerAttachModule(&dbg, &mod);
	struct mWatchpoint wp = { .segment = -1, .minAddress = mn, .maxAddress = mx, .type = type };
	dbg.platform->setWatchpoint(dbg.platform, &mod, &wp);
	core->setKeys(core, keys);
	for (int i = 0; i < frames; i++) mDebuggerRunFrame(&dbg);
	fprintf(stderr, "hits=%ld unique=%d\n", hits, nseen);
	return 0;
}
