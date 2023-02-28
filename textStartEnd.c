#include <stdio.h>
#include <stdlib.h>
#include <elf.h>

int main(int argc, char **argv) {
    int textStart;
    int textEnd;
    
    //Usage
    if (argc < 2) {
        printf("Usage: %s <binary to analyse>\n", argv[0]);
        return 1;
    }

    //Reading the file
    FILE *fp = fopen(argv[1], "rb");
    
    if (!fp) {
        printf("Error while reading file. Exiting...\n");
        return 1;
    }

    Elf64_Ehdr header;

    //Reading the elf header
    if (fread(&header, sizeof(header), 1, fp) != 1) {
        printf("Error while reading ELF header. Exiting...\n");
        return 1;
    }

    //Checking if the given binary is an ELF binary
    if (header.e_ident[EI_MAG0] != ELFMAG0 ||
            header.e_ident[EI_MAG1] != ELFMAG1 ||
            header.e_ident[EI_MAG2] != ELFMAG2) {
        printf("Error: given file is probably not an ELF file. Exiting...\n");
        return 1;
    }

    //Starting to read the sections
    Elf64_Shdr sectionHeader;
    fseek(fp, header.e_shoff + header.e_shstrndx * sizeof(sectionHeader), SEEK_SET);
    fread(&sectionHeader, sizeof(sectionHeader), 1, fp);
    fseek(fp, sectionHeader.sh_offset, SEEK_SET);

    //Reading all the sections
    for (int i = 0; i < header.e_shnum; ++i) {
        if (fread(&sectionHeader, sizeof(sectionHeader), 1, fp) != 1) {
            printf("Error while reading ELF section header. Exiting...\n");
            return 1;
        }
        
        //Checking if the current section is the .text section
        if (sectionHeader.sh_type == SHT_PROGBITS && sectionHeader.sh_flags == (SHF_EXECINSTR | SHF_ALLOC)) {
            textStart = sectionHeader.sh_addr;
            textEnd = textStart + sectionHeader.sh_offset;
            
            printf("Found .text section: starts at 0x%x and ends at 0x%x.\n", textStart, textEnd);
        }

        //Advancing to the next section
        fseek(fp, header.e_shoff + i * sizeof(sectionHeader), SEEK_SET);
    }

    fclose(fp);
    
    return 0;
}
