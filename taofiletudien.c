#include <stdio.h>
#include <stdlib.h>
#include <string.h>
FILE *f;
char filename[]="tudien.dic";
void random_string(int length,char *out)
{
    int i;
    char xau[]="abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    for (i=0;i<length;i++)
        out[i]= xau[rand()%62];
    out[length]='\0';
}
main()
{
    int num;
    int i;
    char tmp[200];
    f=fopen(filename,"w");
    printf("nhap so tu cua file: ");
    scanf("%d",&num);
    fprintf(f,"[\n");
    for (i=1;i<=num;i++)
    {
        random_string(10,tmp);
        fprintf(f,"   {%s}:",tmp);
        random_string(100,tmp);
        fprintf(f,"{%s}",tmp);
        if (i<num) fprintf(f,",");
        fprintf(f,"\n");
    }
    fprintf(f,"]\n");
    fclose(f);
}
