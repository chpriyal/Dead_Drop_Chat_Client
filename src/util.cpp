#include "util.hpp"

/* Measure the time it takes to access a block with virtual address addr. */
CYCLES measure_one_block_access_time(ADDR_PTR addr)
{
	CYCLES cycles;

	asm volatile("mov %1, %%r8\n\t"
	"lfence\n\t"
	"rdtsc\n\t"
	"mov %%eax, %%edi\n\t"
	"mov (%%r8), %%r8\n\t"
	"lfence\n\t"
	"rdtsc\n\t"
	"sub %%edi, %%eax\n\t"
	: "=a"(cycles) /*output*/
	: "r"(addr)
	: "r8", "edi");	

	return cycles;
}





extern void send_bit(bool bit, struct config *config)
{

	CYCLES start_t = cc_sync();
	if (bit) {
		ADDR_PTR addr = config->addr;
		while ((get_time() - start_t) < config->interval) {
			clflush(addr);
		}	

	} else {
		while (get_time() - start_t < config->interval) {}
	}
}



extern CYCLES rdtscp(void) {
	CYCLES cycles;
	asm volatile ("rdtscp"
	: "=a" (cycles));

	return cycles;
}


CYCLES get_time() {
    return rdtscp();
}


extern CYCLES cc_sync() {
    while((get_time() & CHANNEL_SYNC_TIMEMASK) > CHANNEL_SYNC_JITTER) {}
    return get_time();
}


extern void clflush(ADDR_PTR addr)
{
    asm volatile ("clflush (%0)"::"r"(addr));
}


void init_config(struct config *config)
{
	int offset = 0x0;
	config->interval = 0x00008000;
	const char *filename = "/home/priyal/projects/Dead_Drop/src/shared";

	
	if (filename != NULL) {
		int inFile = open(filename, O_RDONLY);
		if(inFile == -1) {
			printf("Failed to Open File\n");
			exit(1);
		}

		void *mapaddr = mmap(NULL,DEFAULT_FILE_SIZE,PROT_READ,MAP_SHARED,inFile,0);

		if (mapaddr == (void*) -1 ) {
			printf("Failed to Map Address\n");
			exit(1);
		}

		config->addr = (ADDR_PTR) mapaddr + offset;
	}
}



char *conv_char(char *data, int size, char *msg)
{
    for (int i = 0; i < size; i++) {
        char tmp[8];
        int k = 0;

        for (int j = i * 8; j < ((i + 1) * 8); j++) {
            tmp[k++] = data[j];
        }

        char tm = strtol(tmp, 0, 2);
        msg[i] = tm;
    }

    msg[size] = '\0';
    return msg;
}


/*coverting char array to binary*/
char *string_to_binary(char *s){
    if (s == NULL) return 0; 
    size_t len = strlen(s) - 1;//Last element is the terminating char
    char *binary = (char *)malloc(len * 8 + 1);
    binary[0] = '\0';

    for (size_t i = 0; i < len; ++i) {
        char ch = s[i];
        for (int j = 7; j >= 0; --j) {
            if (ch & (1 << j)) {
                strcat(binary, "1");
            } else {
                strcat(binary, "0");
            }
        }
    }
    return binary;
}
