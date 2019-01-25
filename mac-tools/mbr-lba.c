#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MBR_LEN 521

int main(int argc, char const *argv[])
{
    if (argv[1] == NULL)
    {
        printf("please enter binary file!\n");
        return 0;
    }

    if (argv[2] == NULL)
    {
        printf("please enter vhd file!\n");
        return 0;
    }

    const char* asmFileName = argv[1];

    const char* diskFileName = argv[2];

    FILE* fd = fopen(asmFileName, "rb");
    if (fd == NULL)
    {
        printf("open file \"%s\" failed!\n", asmFileName);
        exit(1);
    }
    char binArray[MBR_LEN];

    fread(binArray, sizeof(char), MBR_LEN, fd);

    fclose(fd);

    FILE* desFd = fopen(diskFileName, "rb+");
    if (desFd == NULL)
    {
        printf("open file \"%s\" failed!\n", diskFileName);
        exit(1);
    }
    fseek(desFd, 0, SEEK_SET);
    fwrite(binArray, sizeof(char), MBR_LEN, desFd);

    fclose(desFd);

    printf("write mbr success!!\n");

    return 0;

}