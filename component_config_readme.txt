DJYOS���ں��Լ������bsp�ȵĲü������ù��ܣ���Դ���IDE�����ɣ��������͵�hacker��Ҳ�����Լ��ü���
Դ���У�c/cpp��h/hpp���з���һ���ض���ʽ�������ı���IDEɨ��õ�������Ϣ����ͼ�λ��ķ�ʽ�������á��ü���
IDE����ͼ�����ý��棬������ɺ󣬻����� projec_config.h �ļ����ڹ���Ŀ¼�У�����ǵ����������������ɵķ��ŷ����������С�
�������ĺô���
1������������ã�djysrc���ֲ��䣬����ҪΪ�ض������޸�djysrc��ʹdjysrc�ܹ�ά��һ��copy������ά����
2����ʹ���������̣�������djysrc��Դ��Ҳ�ǿɶ��ģ���������Ҳ���#define�ĳ��������´��벻�ɶ������⡣

�����ַ����������
1��ֻ����1��C�ļ��������
2��2�������ļ���ɵ������

���ڵ�һ�������c/cpp�ļ���������ÿ�֮ǰ������䣺
#include  "projec_config.h"

���ڵڶ�����������������Ŀ¼��Ψһ��Ӧ�ģ������ȫ�����������һ��Ŀ¼���С�
��Ŀ¼�У�������һ����Ϊcomponent_config_myname.h��ͷ�ļ���������������Ŀ¼�С�
component_config_myname.h�ļ������ݣ����һ�������c/cpp�ļ�һ�¡�
c/cpp�ļ���������ÿ�֮ǰ������䣺
#include component_config_myname.h

�����﷨��

1��������ÿ�

�����������ݰ����ڡ�//@#$%component configure�� �� ��//@#$%component end configure��������ǩ�м䣬��Ա�ǩÿ���ļ���ֻ�ܳ���һ��

2�������ʼ������������

���ڰ���IDE���ɳ�ʼ�����롣
��������ڱ�ǩ��//%$#@initcode���͡�//%$#@end initcode��֮�䣬������ע��״̬��IDE��ɾ��"//"��copy����ʼ���ļ��С�

3�����������

����������������ԣ��������������������ȣ������ڱ�ǩ��//%$#@describe���͡�//%$#@end describe��֮��
������԰�����
//component name:"example"      //�����������
//parent:"none"                 //������ĸ�������֣�none��ʾû�и����
//attribute:�û����            //ѡ���������������������bsp������û��������������������IDE�з���
//select:��ѡ                   //ѡ���ѡ����ѡ���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ�IDE�ü������в���ʾ
//init time:medium              //��ʼ��ʱ������ѡֵ��early��medium��later,pre-main��
                                //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"none"             //������������������������none����ʾ�������������
                                //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                //����������������������г�
//weakdependence:"none"         //��������������������������none����ʾ�������������
                                //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//mutex:"none"                  //������������������������none����ʾ�������������
                                //����������������������г�

4������������ÿ�

�����ڱ�ǩ��//%$#@configue//%$#@end configue ��֮�䣬������������Ĳ�����
DJYOS����ģ��Ĳ�������һ��ʹ���в���ֵ��ʽ��
#define     CFG_PARAM   value       //"name",������ע��
ֻ�е��������������ʹ���޲���ֵ��ʽ��
#define     CFG_PARAM               //"name",������ע��
����"name"����ʾ��IDEͼ�����ý����е����֣���������ע���ö��Ÿ�����
�ڵ�һ���������ÿ�ʼ��������һ�δ��룺��SDCARD_POWER�����������
#if ( CFG_MODULE_ENABLE_SDCARD_POWER == false )
//#warning  " board_config  �������δ���ã�ʹ��Ĭ������"
//%$#@target = header           //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#define CFG_MODULE_ENABLE_SDCARD_POWER    false //�����ѡ�˱����������DIDE��project_config.h���������ж���Ϊtrue
����д�ĺô��ǣ���ʹ�û�û�����ã����Ҳ���������롢�Ķ��������ܹ��������棬��ǿ�˿ɶ��ԣ������˴�ͳ��Դ����������makefile��configure��Щ����ʽ���ļ����ܶ�����ë����
���������������ԣ�ʹ�ñ�ǩ����˵����IDE��ͼ�����ý����У����ݴ��������飬����ǩ˵�����£�
//%$#@num,0,100,                ������������������ֵ���������Сֵ�����ֵ���ö��ŷָ�����������ֵ
//%$#@enum,true,false,          ����������������enum�����������г�ȫ����ѡֵ
//%$#@string,1,10,              �����������������ַ����������Ŵ�������Сֵ�����ֵ�������޳���
//%$#@select,                   ��������IDE�����ù�ѡ��ע�͵�ΪĬ�ϲ���ѡ������Ĭ�Ϲ�ѡ����ѡ�Ľ�����һ����ֵ�ĺ� ��ֻ���ڵ�����������������������ʹ�á�
//%$#@SYMBOL,                   ��������ֱ�Ӷ�����ֵ���ţ�ֻ���ڵ�����������������������ʹ�á�
//%$#@free,                     ���������������ã�IDE����������
//%$#@object_num,0,1,2,..,100   ������������object�����������������г�ȫ����ѡֵ,��object_para�󶨣�ѡ�����������һֵʱ��object_para��Ҫ������̬���������
//%$#@object_para,              ������������ÿ��object�Ĳ�������object_num�󶨣�Ŀǰ������������������ÿ�����

5�����ų����ļ�

��Ҫ���ڵ�������Դ���룬Ҳ�������д���ʹ�ã��Ѳ���Ҫ���ļ��ų����⡣
//%$#@exclude       ****�����ų��ļ��б�
//�г������ų��ļ����ļ��У��÷ֺš�;���������Ա��ļ�����Ŀ¼Ϊ��ǰĿ¼��������ͨ���
//%$#@end exclude   ****�ų��ļ��б�����

6���ӵ�include����·�����ļ���

���ڵ�������Դ���룬�г���ʹ�ñ���Դ����ʱ��Ҫ�ӵ�include����·����ȫ���ļ��С�
//%$#@API include path         ****APIͷ�ļ�·���б����÷ֺš�;���������Ա��ļ�����Ŀ¼Ϊ��ǰĿ¼��������ͨ�����Ӧ�ñ���ʱ��DIDE���Զ��ѱ��б��е�ͷ�ļ��ӵ� -I ����·����
//./include;
//%$#@end API include path

//%$#@self include path         ****����������ͷ�ļ�·���б����÷ֺš�;���������Ա��ļ�����Ŀ¼Ϊ��ǰĿ¼��������ͨ��������뱾��ʱ��DIDE���Զ��ѱ��б��е�ͷ�ļ��ӵ� -I ����·����
//./include;
//%$#@end self include path

7��ѡ��ĳ���������APPʱ��Ҫ���ӵĶ����ƿ�
//%$#@library path
//./ip/my_lib_name.a
//%$#@end library path

//@#$%component end configure

������ϵԭ��
1������������豸���ơ��ļ����Ƶ����������main�г�ʼ����
2�������������������bsp�е������Ҳ�����������������������֮���ԡ�