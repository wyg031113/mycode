#include <stdio.h>

#include "config.h"
#include "../debug.h"
char serip[17];
int serport;
int beok;
char dbname[128];
char passwd[128];
struct config config_table[]=
{
    {"serip", serip, CFG_STR, 17, "s:"},
    {"serport", &serport, CFG_INT, sizeof(int),"p:"},
    {"ok", &beok, CFG_INT, sizeof(int), "o"},
    {"dbname", dbname, CFG_STR, 128, "d:"},
    {"passwd", passwd, CFG_STR, 128, "e:"},
    {NULL, NULL, 0, 0, 0}
};
int main(int argc, char *argv[])
{
//    show_usage();
    DEBUG("Hello, test config.\n");
//   show_config_table();
    load_config("tst.cfg");
    parse_cmdline(argc, argv);
    INFO("After load config file\n");
    show_config();
    return 0;
}
