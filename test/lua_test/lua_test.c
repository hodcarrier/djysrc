
#include    "djyos.h"
#include    "gdd.h"
#include    <wchar.h>

#include    "lauxlib.h"
#include    "lua.h"

/*============================================================================*/
static int average(lua_State *L)
{
/* 得到参数个数 */
int n = lua_gettop(L);
double sum = 0;
int i;
/* 循环求参数之和 */
for (i = 1; i <= n; i++)
{
/* 求和 */
sum += lua_tonumber(L, i);
}
/* 压入平均值 */
lua_pushnumber(L, sum / n);
/* 压入和 */
lua_pushnumber(L, sum);
/* 返回返回值的个数 */
return 2;
}

static int Lsleep(lua_State *L){
  int d;

  d=lua_tonumber(L, 1);
  GUI_SleepMS(d);
  lua_pushnumber(L, 1);
  return 1;
}

static int Lmyputs0(lua_State *L)
{
   char *buf;
   int len;

   buf=(char *)lua_tostring(L,1);
   len=lua_tonumber(L, 2);
   printf(buf);
  // myputs0(buf,len);

   lua_pushnumber(L, 1);
   return 1;
}

//static char buf[1024];
static char *buf;

void    lua_test(void)
{
    lua_State *L;
 //   float a,c2;

    printf("lua_test.\r\n");

    L   = lua_open();
    printf("L=%08XH.\r\n",L);

    luaopen_base(L);
    //注_册自己的c函数到lua中
    lua_register(L, "average", average);
    lua_register(L, "Lsleep", Lsleep);
    lua_register(L, "Lmyputs0", Lmyputs0);

    //这里是lua脚本， 脚本里面又调用了自己定义得c函数, Lmyputs0---串口输出函数   average --平均值函数，  Lsleep--延时函数

    strcpy(buf," Lmyputs0(\"start!\",6); c2=2; a1 = {\"one\",\"two\",\"three\"}    a=average(10, 20, 30, 40, 50); count=0;  while 1>0 do  \r\n  count=count+1;  Lmyputs0(a1[(count%3)+1],6);  Lsleep(300); \r\n end; \r\n Lmyputs0(\"end!\",4)");


//    buf=(char*)0x001F0000;
    printf("====================Start\r\n");
    printf("\r\n");
    printf("%s",buf);
    printf("\r\n");
    printf("====================End\r\n");
    //让lua运行脚本!
    luaL_dostring(L, buf);

    printf("lua_test end.\r\n");

}

/*============================================================================*/

