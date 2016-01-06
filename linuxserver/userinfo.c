#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <sys/utsname.h>
 
void main()
{
    printf("当前用户 %s\n", getlogin());
    printf("uid=%d, gid=%d ugid=%d egid=%d\n", getuid(), getgid(), geteuid(), getegid());
    printf("pid=%d, pgid=%d ppid=%d sid=%d\n", getpid(), getpgid(0), getppid(), getsid(getpid()));
 
    struct passwd *pw;
 
    //当前用户信息
    pw = getpwuid(getuid());
    printf("name=%s, uid=%d, gid=%d, home=%s, shell=%s\n",
        pw -> pw_name, pw -> pw_uid, pw -> pw_gid, pw -> pw_dir, pw -> pw_shell);
 
    //wyg用户信息
    pw = getpwnam("wyg");
    printf("name=%s, uid=%d, gid=%d, home=%s, shell=%s\n",
        pw -> pw_name, pw -> pw_uid, pw -> pw_gid, pw -> pw_dir, pw -> pw_shell);
     
    //root用户信息
    pw = getpwnam("root");
    printf("name=%s, uid=%d, gid=%d, home=%s, shell=%s\n",
        pw -> pw_name, pw -> pw_uid, pw -> pw_gid, pw -> pw_dir, pw -> pw_shell);    
 
    printf("==============\n");
 
    //获取主机信息
    char computer[256];
    struct utsname uts;
 
    if (gethostname(computer, 255) != 0 || uname(&uts) < 0)
    {
        fprintf(stderr, "无法获取主机信息\n");
        exit(EXIT_FAILURE);
    }
 
    printf("HOST NAME：[%s]\n", computer);
    printf("操作系统： [%s]\n", uts.sysname);
    printf("硬件类型： [%s]\n", uts.machine);
    printf("主机名：   [%s]\n", uts.nodename);
    printf("VERSION：  [%s]\n", uts.version);
    printf("RELEASE：  [%s]\n", uts.release);
	sleep(20);
    exit(EXIT_SUCCESS);
}
