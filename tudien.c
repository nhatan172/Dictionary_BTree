#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <termio.h>
#include "btree.h"
FILE *f;
char filename[20];
BTA* readfile(BTA *btfile);
BTA* addData(BTA *btfile);
BTA *AddWord(BTA *btfile);
void Search(BTA *btfile);
void Edit(BTA* btfile);
BTA* Delete(BTA *btfile);
int getch( void );
void add_code(const char *s, int c);
void init();
const char* soundex(const char *s);
int autocomplete(BTA* btfile,char *s);
int fsoundex(BTA *btfile,char *s);
void printfile(BTA *btfile);
void convert_word(char *s);
//*************************************************************
menu()
{
    printf("-------------------MENU--------------------------\n");
    printf("1. Them tu\n");
    printf("2. Tim kiem\n");
    printf("3. Cap nhat nghia cho 1 tu\n");
    printf("4. Xoa tu\n");
    printf("5. Thoat\n");
    printf("Lua chon: ");
}
main()
{
    BTA *btfile;
    char fID[]="data";
    int c;
    char data[10];
    btinit();
    btfile= btopn(fID,0,0);
    if (btfile==NULL)
    {
        btfile = btcrt(fID,0,0);
        //open file + add data
        btfile=addData(btfile);
    }
    do
    {
        menu();
        scanf("%d",&c);
        while(getchar()!='\n');
        switch(c)
        {
        case 1:
            btfile=AddWord(btfile);
            break;
        case 2:
            Search(btfile);
            break;
        case 3:
            Edit(btfile);
            break;
        case 4:
            btfile=Delete(btfile);
            break;
        case 5:
            break;
        }
    } while(c!=5);
//	printfile(btfile);    
    btcls(btfile);
}
//*************************************************************
void convert_word(char *s)
{
    int i;
    for (i=0;i<strlen(s);i++)
    {
        if (s[i]>='A'&&s[i]<='Z') s[i]+=32;
    }
}
//*************************************************************
BTA* readfile(BTA *btfile)
{
    char eng[20],viet[101];
    char line[500];
    char *p;
    int k;
    while(1)
    {
        fgets(line,100,f);
        if (line[0]=='[') break;
    }
    while(1)
    {
        fgets(line,500,f);
        if (line[strlen(line)-1]='\n') line[strlen(line)-1]='\0';
        if (line[0]==']') break;
        if (line[0]=='/') continue;
        p=strtok(line,"{}:,");
        p=strtok(NULL,"{}:,");
        strcpy(eng,p);
        p=strtok(NULL,"{}:,");
        strcpy(viet,p);
        convert_word(eng);
        k=btins(btfile,eng,viet,101);
        if (k==46) btupd(btfile,eng,viet,101);
    }
    return btfile;
}
//*************************************************************
BTA* addData(BTA *btfile)
{
    printf("nhap ten tu dien: ");
    scanf("%s", filename);
    while(getchar()!='\n');
    f=fopen(filename,"r");
    if (f!=NULL)
    {
        btfile=readfile(btfile);
    }
    else printf("file khong ton tai\n");
    fclose(f);
    return btfile;
}
//*************************************************************
BTA *AddWord(BTA *btfile)
{
    char eng[20],viet[101];
    int k,h;
    printf("TIENG ANH: ");
    scanf("%s",eng);
    convert_word(eng);
    while(getchar()!='\n');
    printf("TIENG VIET: ");
    scanf("%[^\n]",viet);
    while(getchar()!='\n');
    k=btins(btfile,eng,viet,101);
    if (k==46)
    {
        printf("Da co tu nay roi\n");
        return btfile;
    }
    printf("Da them thanh cong\n");
    return btfile;
}
//*************************************************************
void Search(BTA *btfile)
{
    char eng[20];
    char viet[101];
    char c;
    char *temp;
    int rsize,k=0,i,n,h,count=0;
    while(1)
    {
        i=0;
        printf("\nWord: ");
        c=getch();
        printf("%c",c);
        while(1)
        {
            eng[i]=c;
            i++;
            c=getch();
            if(c=='\t' || c=='\n')
                break;
            printf("%c",c);
        }
        eng[i]='\0';
        convert_word(eng);
        if(c=='\n')
        {
            n=btsel(btfile,eng,viet,101,&rsize);
            if(n==47)
            {
                printf("\nKhong tim thay\n");
                return;
            }
            printf("\n%s: %s\n",eng,viet);
            return;

        }
        if (c=='\t')
        {
            h=strlen(eng);
            k = autocomplete(btfile,eng);
            if (k==1)
            {
                temp=eng+h;
                printf("%s\n",temp);
                btsel(btfile,eng,viet,101,&rsize);
                printf("%s :   %s\n",eng,viet);
                return;
            }
            if (k==0)
            {
                k=fsoundex(btfile,eng);
                if (k==0)
                {
                    printf("\nKhong tim thay\n");
                    return;
                }
            }
        }
    }
}
//*************************************************************
void Edit(BTA* btfile)
{
    char eng[20],viet[101];
    int h;
    printf("Nhap tu can cap nhat: ");
    scanf("%s",eng);
    convert_word(eng);
    while(getchar()!='\n');
    printf("Nghia: ");
    scanf("%[^\n]",viet);
    while(getchar()!='\n');
    h=btupd(btfile,eng,viet,101);
    if (h!=0) printf("Cap nhat khong thanh cong. Khong ton co tu nay\n");
    else printf("Cap nhat thanh cong\n");
}
//*************************************************************
BTA* Delete(BTA *btfile)
{
    char eng[20];
    char viet[101];
    int k,rsize;
    printf("Tieng anh: ");
    scanf("%s",eng);
    convert_word(eng);
    k=btdel(btfile,eng);
    if (k==47) printf("Khong co tu nay\n");
    else printf("Xoa thanh cong\n");
    return btfile;
}
//*************************************************************
int getch( void )
{
    char ch;
    int fd = fileno(stdin);
    struct termio old_tty, new_tty;
    ioctl(fd, TCGETA, &old_tty);
    new_tty = old_tty;
    new_tty.c_lflag &= ~(ICANON | ECHO | ISIG);
    ioctl(fd, TCSETA, &new_tty);
    fread(&ch, 1, sizeof(ch), stdin);
    ioctl(fd, TCSETA, &old_tty);
    return ch;
}
//*************************************************************
static char code[128] = { 0 };
void add_code(const char *s, int c)
{
	while (*s) {
		code[(int)*s] = code[0x20 ^ (int)*s] = c;
		s++;
	}
}
//*************************************************************
void init()
{
	static const char *cls[] =
		{ "AEIOU", "", "BFPV", "CGJKQSXZ", "DT", "L", "MN", "R", 0};
	int i;
	for (i = 0; cls[i]; i++)
		add_code(cls[i], i - 1);
}

/* returns a static buffer; user must copy if want to save
   result across calls */
//*************************************************************
const char* soundex(const char *s)
{
	static char out[5];
	int c, prev, i;

	out[0] = out[4] = 0;
	if (!s || !*s) return out;

	out[0] = *s++;

	/* first letter, though not coded, can still affect next letter: Pfister */
	prev = code[(int)out[0]];
	for (i = 1; *s && i < 4; s++) {
		if ((c = code[(int)*s]) == prev) continue;

		if (c == -1) prev = 0;	/* vowel as separator */
		else if (c > 0) {
			out[i++] = c + '0';
			prev = c;
		}
	}
	while (i < 4) out[i++] = '0';
	return out;
}
//*************************************************************
int autocomplete(BTA* btfile,char *s)
{
    char viet[101];
    char key[20];
    int rsize,n;
    int i;
    char tmp[20],tmp2[20];
    int sum=0;
    btpos(btfile,1);
    n=btseln(btfile,key,viet,101,&rsize);
    while(strcmp(key,s)<0)
    {
        n=btseln(btfile,key,viet,101,&rsize);
        if (n==47) return 0;
    }
    for (i=0;i<strlen(s);i++)
        tmp[i]=key[i];
    tmp[i]='\0';
    while (strcmp(s,tmp)==0)
    {
        sum++;
        if (sum==1) strcpy(tmp2,key);
        if (sum==2) printf("\n%s\t%s\t",tmp2,key);
        if (sum>2) printf("%s\t",key);
        if ((sum%5)==0) printf("\n");
        n=btseln(btfile,key,viet,101,&rsize);
        for (i=0;i<strlen(s);i++)
            tmp[i]=key[i];
        tmp[i]='\0';
        if (n==47) break;
    }
    if (sum==1) strcpy(s,tmp2);
    return sum;
}
//*************************************************************
int fsoundex(BTA *btfile,char *s)
{
    char sdx[5],tmp[5],eng[20],viet[101];
    int i,n,rsize,sum=0;
    init();
    strcpy(sdx,soundex(s));
    for (i=3;i>=0;i--)
    {
        if (sdx[i]!='0') break;
        if (sdx[i]=='0') sdx[i]='\0';
    }
    btpos(btfile,1);
    n=btseln(btfile,eng,viet,101,&rsize);
    strcpy(tmp,soundex(eng));
    tmp[strlen(sdx)]='\0';
    while(sdx[0]>eng[0])
    {
        if(n==47)
            return sum;
        n=btseln(btfile,eng,viet,101,&rsize);
    }
    printf("\n");
    while (n!=47)
    {
        if (strcmp(tmp,sdx)==0)
        {
            printf("%s\t",eng);
            sum++;
            if ((sum%5)==0) printf("\n");
        }
        n=btseln(btfile,eng,viet,101,&rsize);
        strcpy(tmp,soundex(eng));
        tmp[strlen(sdx)]='\0';
        if (eng[0]>sdx[0]) break;
    }
    printf("\n");
    return sum;
}
//*************************************************************
void printfile(BTA *btfile)
{
    int n;
    char eng[20],viet[101];
    int rsize;
    f=fopen(filename,"w");
    btpos(btfile,1);
    n=btseln(btfile,eng,viet,101,&rsize);
    while(n!=47)
    {
        fprintf(f,"%s\t%s",eng,viet);
        n=btseln(btfile,eng,viet,101,&rsize);
        if (n!=47) fprintf(f,"\n");
    }
    fclose(f);
}