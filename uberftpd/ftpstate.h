#ifndef FTP_STATE_H
#define FTP_STATE_H

void ftp_state_init();
void ftp_proc_command(char *command, int sock);

#endif
