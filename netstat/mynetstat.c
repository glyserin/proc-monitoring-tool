#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>

const char* tcp_state_to_string(int state) {
	switch (state) {
    case 0x01: return "ESTABLISHED";
    case 0x02: return "SYN_SENT";
    case 0x03: return "SYN_RECV";
    case 0x04: return "FIN_WAIT1";
    case 0x05: return "FIN_WAIT2";
    case 0x06: return "TIME_WAIT";
    case 0x07: return "CLOSE";
    case 0x08: return "CLOSE_WAIT";
    case 0x09: return "LAST_ACK";
    case 0x0A: return "LISTEN";
    case 0x0B: return "CLOSING";
    default: return "UNKNOWN";
  }
}

int main() {
	FILE* tcp_file;
	char line[1024];
	uint32_t local_addr;
	uint16_t local_port;
	uint32_t remote_addr;
	uint16_t remote_port;
	uint32_t state;
	char local_addr_buf[INET_ADDRSTRLEN];
	char remote_addr_buf[INET_ADDRSTRLEN];

	tcp_file = fopen("/proc/net/tcp", "r");
	if (tcp_file == NULL) {
		perror("Failed to open /proc/net/tcp");
		return EXIT_FAILURE;
	}

	printf("%-8s%-25s%-25s%s\n", "Proto", "Local Address", "Foreign Address", "State");

	while (fgets(line, sizeof(line), tcp_file)) {
		sscanf(line, "%*s %*2X%8X:%hX %8X:%hX %X", &local_addr, &local_port, &remote_addr, &remote_port, &state);
		
		// uint32_t nl_local_addr = htonl(local_addr);
		// uint32_t nl_remote_addr = htonl(remote_addr);
	
		inet_ntop(AF_INET, &local_addr, local_addr_buf, sizeof(local_addr_buf));
		inet_ntop(AF_INET, &remote_addr, remote_addr_buf, sizeof(remote_addr_buf));

		printf("%-8s", "TCP");
		printf("%-15s", local_addr_buf);
		printf(":%-9x", local_port);
		printf("%-15s", remote_addr_buf);
		printf(":%-9x", remote_port);
		printf("%s\n", tcp_state_to_string(state));
	}
	fclose(tcp_file);

	return EXIT_SUCCESS;
}
