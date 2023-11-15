// #include "libs/net.h"

// void error(char *msg)
// {
// 	perror(msg);
// 	exit(0);
// }

// static void print_hex(void *data, int num_bytes)
// {
// 	uint8_t *bits = data;
// 	for (int i = 0; i < num_bytes; i++) {
// 		printf("%x ", bits[i]);
// 	}
// }

// void print_msg(int_msg *msg)
// {
// 	printf("type: %d\n", msg->type);
// 	printf("modifier: %d\n", msg->modifier);
// 	printf("data_sz: %d\n", msg->data_sz);
// 	printf("data: %s\n", msg->data);
// }

// fwd_data *parse_send(char *data)
// {
// 	if (!data) {
// 		return NULL;
// 	}

// 	fwd_data *f = malloc(sizeof(*f));
// 	if (!f) {
// 		return NULL;
// 	}
// 	memcpy(f, data, FWD_D_STATIC_LEN);

// 	char *ip = malloc(f->ip_sz);
// 	if (!ip) {
// 		free(f);
// 		return NULL;
// 	}

// 	char *tmp_data = malloc(f->data_sz);
// 	if (!tmp_data) {
// 		free(f);
// 		free(ip);
// 		return NULL;
// 	}

// 	memcpy(ip, data + FWD_D_STATIC_LEN, f->ip_sz);
// 	memcpy(tmp_data, data + FWD_D_STATIC_LEN + f->ip_sz, f->data_sz);
// 	f->ip	= ip;
// 	f->data = tmp_data;
// 	return f;
// }

// int forward_data(char *data)
// {
// 	fwd_data *fwd = parse_send(data);
// 	conn *c	      = conn_init(fwd->ip, fwd->port);
// 	send(c->sock, fwd->data, fwd->data_sz, 0);
// 	return 0;
// }

// void parse_msg(char *buff)
// {
// 	int msg_sz   = sizeof(int_msg) - sizeof(void *);
// 	int_msg *msg = malloc(msg_sz);
// 	memcpy(msg, buff, msg_sz);
// 	char *data = malloc(msg->data_sz);
// 	memcpy(data, buff + msg_sz, msg->data_sz);
// 	msg->data = data;
// 	if (msg->type == SEND) {
// 		forward_data(msg->data);
// 	}
// 	print_msg(msg);
// }
#include "implant.h"

// void print_item(FILE *fd, Elf64_Ehdr eh, Elf64_Shdr sh_table[])
// {
// 	int i;
// 	char *sh_str;
// 	char *buff;

// 	buff = malloc(sh_table[eh.e_shstrndx].sh_size);

// 	if (buff != NULL) {
// 		fseek(fd, sh_table[eh.e_shstrndx].sh_offset, SEEK_SET);
// 		fread(buff, 1, sh_table[eh.e_shstrndx].sh_size, fd);
// 	}
// 	sh_str = buff;

// 	for (i = 0; i < eh.e_shnum; i++) {
// 		if (!strcmp(".mydata", (sh_str + sh_table[i].sh_name))) {
// 			printf("Found section\t\".mydata\"\n");
// 			printf("at offset\t0x%08x\n", (unsigned int)sh_table[i].sh_offset);
// 			printf("of size\t\t0x%08x\n", (unsigned int)sh_table[i].sh_size);
// 			break;
// 		}
// 	}

// 	/*Code to print or store string data*/
// 	if (i < eh.e_shnum) {
// 		char *mydata = malloc(sh_table[i].sh_size);
// 		fseek(fd, sh_table[i].sh_offset, SEEK_SET);
// 		fread(mydata, 1, sh_table[i].sh_size, fd);
// 		puts(mydata);
// 	} else {
// 		// .mydata section not found
// 	}
// }

// int main()
// {
// 	print_item(fp, elf_header, sh_table);

// 	return 0;
// }
#define _GNU_SOURCE
#include <link.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <elf.h>

const char *type_str(ElfW(Word) type)
{
	switch (type) {
	case PT_NULL:
		return "PT_NULL"; // should not be seen at runtime, only in the file!
	case PT_LOAD:
		return "PT_LOAD";
	case PT_DYNAMIC:
		return "PT_DYNAMIC";
	case PT_INTERP:
		return "PT_INTERP";
	case PT_NOTE:
		return "PT_NOTE";
	case PT_SHLIB:
		return "PT_SHLIB";
	case PT_PHDR:
		return "PT_PHDR";
	case PT_TLS:
		return "PT_TLS";
	case PT_GNU_EH_FRAME:
		return "PT_GNU_EH_FRAME";
	case PT_GNU_STACK:
		return "PT_GNU_STACK";
	case PT_GNU_RELRO:
		return "PT_GNU_RELRO";
	case PT_SUNWBSS:
		return "PT_SUNWBSS";
	case PT_SUNWSTACK:
		return "PT_SUNWSTACK";
	default:
		if (PT_LOOS <= type && type <= PT_HIOS) {
			return "Unknown OS-specific";
		}
		if (PT_LOPROC <= type && type <= PT_HIPROC)
			return "Unknown processor-specific";
	}
	return "Unknown";
}

const char *flags_str(ElfW(Word) flags)
{
	switch (flags & (PF_R | PF_W | PF_X)) {
	case 0 | 0 | 0:
		return "none";
	case 0 | 0 | PF_X:
		return "x";
	case 0 | PF_W | 0:
		return "w";
	case 0 | PF_W | PF_X:
		return "wx";
	case PF_R | 0 | 0:
		return "r";
	case PF_R | 0 | PF_X:
		return "rx";
	case PF_R | PF_W | 0:
		return "rw";
	case PF_R | PF_W | PF_X:
		return "rwx";
	}
	__builtin_unreachable();
}

static int callback(struct dl_phdr_info *info, size_t size, void *data)
{
	int j;
	(void)data;

	printf("object \"%s\"\n", info->dlpi_name);
	printf("  base address: %p\n", (void *)info->dlpi_addr);
	if (size > offsetof(struct dl_phdr_info, dlpi_adds)) {
		printf("  adds: %lld\n", info->dlpi_adds);
	}
	if (size > offsetof(struct dl_phdr_info, dlpi_subs)) {
		printf("  subs: %lld\n", info->dlpi_subs);
	}
	if (size > offsetof(struct dl_phdr_info, dlpi_tls_modid)) {
		printf("  tls modid: %zu\n", info->dlpi_tls_modid);
	}
	if (size > offsetof(struct dl_phdr_info, dlpi_tls_data)) {
		printf("  tls data: %p\n", info->dlpi_tls_data);
	}
	printf("  segments: %d\n", info->dlpi_phnum);

	for (j = 0; j < info->dlpi_phnum; j++) {
		const ElfW(Phdr) *hdr = &info->dlpi_phdr[j];
		printf("    segment %2d\n", j);
		printf("      type: 0x%08X (%s)\n", hdr->p_type, type_str(hdr->p_type));
		// printf("      file offset: 0x%08zX\n", hdr->p_offset);
		// printf("      virtual addr: %p\n", (void *)hdr->p_vaddr);
		// printf("      physical addr: %p\n", (void *)hdr->p_paddr);
		printf("      file size: 0x%08zX\n", hdr->p_filesz);
		printf("      memory size: 0x%08zX\n", hdr->p_memsz);
		// printf("      flags: 0x%08X (%s)\n", hdr->p_flags, flags_str(hdr->p_flags));
		// printf("      align: %zd\n", hdr->p_align);
		if (hdr->p_memsz) {
			printf("      derived address range: %p to %p\n", (void *)(info->dlpi_addr + hdr->p_vaddr), (void *)(info->dlpi_addr + hdr->p_vaddr + hdr->p_memsz));
		}
	}
	return 0;
}

extern char __etext;
extern char __executable_start;

void read_elf_header()
{
	// Either Elf64_Ehdr or Elf32_Ehdr depending on architecture.
	Elf64_Ehdr *elfhead = &__executable_start;

	uint64_t sectionoffset = elfhead->e_shoff;
	printf("section header offset: %ld\n", sectionoffset);
	Elf64_Shdr *sectionHeader = (Elf64_Shdr *)(&__etext + sectionoffset);
	printf("start of header:       %p\n", elfhead);
	printf("start of section:      %p\n", sectionHeader);

	Elf32_Shdr *sh_tbl = malloc(elfhead->e_shentsize * elfhead->e_shnum);
	for (uint32_t i = 0; i < elfhead->e_shnum; i++) {
		memcpy((void *)&sh_tbl[i], sectionHeader, elfhead->e_shentsize);
	}

	// uint32_t stringoffset = elfhead->e_shstrndx;

	// int stringoffset	  = header.e_shstrndx;

	// if (memcmp(header.e_ident, ELFMAG, SELFMAG) == 0) {
	// 	printf("valid elf\n");
	// 	if (header.e_shstrndx == SHN_UNDEF)
	// 		printf("no e_shstrndx");
	// 	// else
	// 	// (printf("%p\n", header.e_shstrndx));
	// 	printf("%d|%x\n", header.e_shstrndx, header.e_shstrndx);
	// 	printf("num of sections: %d\n", header.e_shnum);
	// 	printf("[%s]\n", &header + (int)header.e_shstrndx);
	// 	printf("[%s]\n", &header + (int)header.e_shstrndx + 1);
	// } else {
	// 	printf("unknown\n");
	// 	printf("0x%lx\n", (unsigned long)&__executable_start);
	// 	printf("0x%lx\n", (unsigned long)&__etext);
	// }
}

int main(void)
{
	// dl_iterate_phdr(callback, NULL);
	read_elf_header();
	exit(EXIT_SUCCESS);
}

// int main(int argc, char *argv[])
// {
// 	printf("%d\n", mynum);
// 	printf("addr = %p\n", ELF_HEADER);
// FILE *fp = NULL; // Pointer used to access current file
// char *program_name;
// Elf64_Ehdr elf_header; // Elf header
// Elf64_Shdr *sh_table; // Elf symbol table

// program_name = "/home/Testing/TEST";
// fp	     = fopen(program_name, "r");

// fseek(fp, 0, SEEK_SET);
// fread(&elf_header, 1, sizeof(Elf64_Ehdr), fp);
// sh_table = malloc(elf_header.e_shentsize * elf_header.e_shnum);

// fseek(fp, elf_header.e_shoff, SEEK_SET);
// fread(sh_table, 1, elf_header.e_shentsize * elf_header.e_shnum, fp);
// int server_fd;
// int new_socket;
// ssize_t valread;
// struct sockaddr_in address;
// int opt		  = 1;
// int addrlen	  = sizeof(address);
// char buffer[1024] = { 0 };

// // Creating socket file descriptor
// if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
// 	perror("socket failed");
// 	exit(EXIT_FAILURE);
// }

// if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
// 	perror("setsockopt");
// 	exit(EXIT_FAILURE);
// }
// address.sin_family	= AF_INET;
// address.sin_addr.s_addr = INADDR_ANY;
// address.sin_port	= htons(8001);

// // Forcefully attaching socket to the port 8080
// if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
// 	perror("bind failed");
// 	exit(EXIT_FAILURE);
// }
// if (listen(server_fd, 3) < 0) {
// 	perror("listen");
// 	exit(EXIT_FAILURE);
// }
// if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
// 	perror("accept");
// 	exit(EXIT_FAILURE);
// }
// valread = read(new_socket, buffer, 1024);
// // parse_msg(buffer);
// // closing the connected socket
// close(new_socket);
// // closing the listening socket
// shutdown(server_fd, SHUT_RDWR);
// 	return 0;
// }