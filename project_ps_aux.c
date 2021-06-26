#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <pwd.h>
#define MAX_LEN 20

struct ps_info *trav_dir(char dir[]);
int read_info(char d_name[],struct ps_info *p1);
void print_ps(struct ps_info *head);
int is_num(char *);
void uid_to_name();
void ps_();
void ps_ps(struct ps_info *head);
void ps_aux(struct ps_info *head);
void ps_axjf();

typedef struct ps_info
{
    char pname[MAX_LEN];
    char user[MAX_LEN];
    int  pid;
    int  ppid;
    char state;
    struct ps_info *next;
} mps;


int main()
{
	ps_();		//��cmd[0] = 'ps'ʱ�����ô˺��������޸Ĵ˺�����cmd����void ps_()����ж����
   	return 0;
}
void ps_()
{
    mps *head;
    head=trav_dir("/proc/");
    if(head==NULL)
        printf("traverse dir error\n");
    ps_ps(head);		//���cmd[1] = ''���ã�ps_ps()����
    ps_aux(head);		//���cmd[1] ='aux'������ps_aux()����
    ps_axjf(head);		//���cmd[1] = 'axjf'������  ps_axjf()��������δʵ��
}

void ps_ps(struct ps_info *head)
{
    mps *link;


    print_ps(head);

    while(head!=NULL)
    {
        link=head;
        head=head->next;
    }
    return;
}
void ps_aux(struct ps_info *head)
{
    int length;
    mps *list1=NULL;
    list1 = head;
    mps *arr[1001];

    int i=0,j=0;
    mps *temp=NULL;

    for(; list1!=NULL; list1=list1->next,i++)
    {
        arr[i]=list1;

    }
    length = i;
    for(i=0; i<length-1; i++)
    {
        for(j=0; j<length-1-i; j++)
        {
            if((arr[j]->ppid)>(arr[j+1]->ppid))
            {
                temp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1]=temp;
            }
        }
    }
    printf("USER\t\t\t\tPID\t\tPPID\t\tSTATE\t\tPNAME\n");
    for(i=0; i<length; i++)
    {
        printf("%-16s\t\t%d\t\t%d\t\t%c\t\t%s\n",arr[i]->user,arr[i]->pid,arr[i]->ppid,arr[i]->state,arr[i]->pname);

    }



    return;
}
void ps_axjf()
{
    return;
}

mps *trav_dir(char dir[])
{
    DIR *dir_ptr;
    mps *head,*p1,*p2;
    struct dirent *direntp;
    struct stat infobuf;

    if((dir_ptr=opendir(dir))==NULL)
        fprintf(stderr,"dir error %s\n",dir);
    else
    {
        head=p1=p2=(struct ps_info *)malloc(sizeof(struct ps_info));//��������
        while((direntp=readdir(dir_ptr)) != NULL)               //����/procĿ¼���н���Ŀ¼
        {
            if((is_num(direntp->d_name))==0)                   //�ж�Ŀ¼�����Ƿ�Ϊ������
            {
                if(p1==NULL)
                {
                    printf("malloc error!\n");
                    exit(0);
                }

                if(read_info(direntp->d_name,p1)!=0)         //��ȡ������Ϣ
                {
                    printf("read_info error\n");
                    exit(0);
                }
                p2->next=p1;                        //�����½ڵ�
                p2=p1;
                p1=(struct ps_info *)malloc(sizeof(struct ps_info));
            }
        }
    }
    p2->next=NULL;
    return head;
}

int read_info(char d_name[],struct ps_info *p1)
{
    FILE *fd;
    char dir[20];
    struct stat infobuf;

    sprintf(dir,"%s/%s","/proc/",d_name);
    chdir("/proc");                        //�л���/procĿ¼����Ȼstat����-1
    if(stat(d_name,&infobuf)==-1)                     //get process USER
        fprintf(stderr,"stat error %s\n",d_name);
    else
        //p1->user=uid_to_name(infobuf.st_uid);
        uid_to_name(infobuf.st_uid, p1);

    chdir(dir);                         //�л���/proc/pidĿ¼
    if((fd=fopen("stat","r"))<0)
    {
        printf("open the file is error!\n");
        exit(0);
    }
    while(4==fscanf(fd,"%d %s %c %d\n",&(p1->pid),p1->pname,&(p1->state),&(p1->ppid)))     //���ļ���ǰ�ĸ��ֶ����ݣ����������ص������Ա��
    {
        break;
    }
    fclose(fd);
    return 0;
}

void uid_to_name(uid_t uid, struct ps_info *p1)         //�ɽ���uid�õ����̵�������user
{
    struct passwd *getpwuid(), *pw_ptr;
    static char numstr[10];

    if((pw_ptr=getpwuid(uid)) == NULL)
    {
        sprintf(numstr,"%d",uid);
        strcpy(p1->user, numstr);
    }
    else
        strcpy(p1->user, pw_ptr->pw_name);
}

int is_num(char p_name[])
{
    int i,len;
    len=strlen(p_name);
    if(len==0) return -1;
    for(i=0; i<len; i++)
        if(p_name[i]<'0' || p_name[i]>'9')
            return -1;
    return 0;
}

void print_ps(struct ps_info *head)
{
    mps *list;
    printf("USER\t\t\t\tPID\t\tPPID\t\tSTATE\t\tPNAME\n");
    for(list=head; list!=NULL; list=list->next)
    {
        printf("%-16s\t\t%d\t\t%d\t\t%c\t\t%s\n",list->user,list->pid,list->ppid,list->state,list->pname);
    }

}
