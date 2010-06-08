#ifndef _QNDS_H
#define _QNDS_H

enum cmd {
    ERR, IAM, BYE, NOP, PNG,
    TDT, BDT,
    EXC, MBX, SCN, UPD,
};

struct cmd_packet {
    uint8_t cmd;
    uint32_t pload_size;
};

#endif
