#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "fat16.h"

static void fat16_printbootsector(struct fat16_bsec *bsec) {
	char oemname[9];
	memcpy(oemname, bsec->oemname, 8);
	oemname[8] = '\0';

	printf("FAT 16 bootsector\n");
	printf("-----------------\n");	
	printf("oemname: %s\n", oemname);
	printf("bps: %hi\n", bsec->bps);
	printf("spc: %c\n", bsec->spc);
	printf("rsc: %hi\n", bsec->rsc);
	printf("fatc: %c\n", bsec->fatc);
	printf("rdemax: %hi\n", bsec->rdemax);
	printf("totsec: %hi\n", bsec->totsec);
	printf("md: %c\n", bsec->md);
	printf("spfat: %hi\n", bsec->spfat);
	printf("spcdg: %hi\n", bsec->spcdg);
	printf("nhcdg: %hi\n", bsec->nhcdg);
	printf("hsc: %d\n", bsec->hsc);
	printf("tsc: %d\n", bsec->tsc);
}

static void fat16_printebpb(struct fat16_ebpb *ebpb) {
	char vl[12], fst[9], bcode[449];
	memcpy(vl, ebpb->vl, 11);
	vl[11] = '\0';,t,T,T
	memcpy(fst, ebpb->fst, 8);
	fst[8] = '\0';
	memcpy(bcode, ebpb->bcode, 449);
	bcode[449] = '\0';

	printf("pdn: %c\n", ebpb->pdn);
	printf("res: %c\n", ebpb->res);
	printf("ebs: %c\n", ebpb->ebs);
	printf("id: %d\n", ebpb->id);
	printf("vl: %s\n", vl);
	printf("fst: %s\n", fst);
	printf("bcode: %s\n", bcode);
	printf("bsig: %hi\n", ebpb->bsig);
}

int main(int argc, char *argv[]) {
	FILE *imagefd;
	struct fat16_bsec bsector;
	struct fat16_ebpb ebpb;
	size_t rcount;
	int closestatus;

	/* Load the image from disk. */
	if(argc >= 2) {
		imagefd = fopen(argv[1], "r");
		if(imagefd == NULL) {
			printf("could not read specified image\n");
			return -1;
		}
	}
	else {
		printf("specify fat image file\n");
		return -1;
	}

	/* Read the bootsector of the image into the bootsector structure. */
	rcount = fread(&bsector, 1, sizeof(bsector), imagefd);
	if(rcount != sizeof(bsector)) {
		printf("could not read bootsector into structure %d/%d\n",
				(int)rcount, (int)sizeof(bsector));
		return -1;
	}

	/* Read the extented bios shit. */
	rcount = fread(&ebpb, 1, sizeof(ebpb), imagefd);
	if(rcount != sizeof(ebpb)) {
		printf("could not read extended bios partition\n");
		return -1;
	}

	fat16_printbootsector(&bsector);
	fat16_printebpb(&ebpb);

	closestatus = fclose(imagefd);
	return 0;
}
