#ifndef _FAT16_H
#define _FAT16_H

#include <stdint.h>

struct fat16_ebpb {
	char pdn;
	char res;
	char ebs;
	int32_t id;
	char vl[11];
	char fst[8];
	char bcode[448];
	uint16_t bsig;
} __attribute__((packed));

struct fat16_bsec { 
	char jmp[3];
	char oemname[8];
	int16_t bps;
	char spc;
	int16_t rsc;
	char fatc;
	int16_t rdemax;
	int16_t totsec;
	char md;
	int16_t spfat;
	int16_t spcdg;
	int16_t nhcdg;
	int32_t hsc;
	int32_t tsc;
} __attribute__((packed));

#endif
