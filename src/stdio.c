#include <stdio.h>
#include <stdarg.h>
#include <typedef.h>

extern void m_putc(char chr);

void m_putc(char chr)
{

;
}

// 计算m^n
unsigned long m_pow_n(unsigned long m, unsigned long n)
{
    unsigned long i = 0, ret = 1;
    if (n < 0) return 0;
    for (i = 0; i < n; i++)
    {
        ret *= m;
    }
    return ret;
}

// 返回值为打印字符的个数
// 支持%d，%o, %x，%s，%c，%f（只打印6位数字）
int print(const char* str, ...)
{
    if (str == NULL) return -1;

	unsigned int ret_num = 0;// 返回打印字符的个数
    char* pStr = (char*)str;// 指向str
    int ArgIntVal = 0;  // 接收整型
    unsigned long ArgHexVal = 0;// 接十六进制
    char* ArgStrVal = NULL;  // 接收字符型
    double ArgFloVal = 0.0; // 接受浮点型
    unsigned long val_seg = 0;   // 数据切分
    unsigned long val_temp = 0;  // 临时保存数据
    int cnt = 0;       // 数据长度计数
    int i = 0;
    
    va_list pArgs; // 定义va_list类型指针，用于存储参数的地址
    va_start(pArgs, str); // 初始化pArgs
    while (*pStr != '\0')
    {
        switch (*pStr)
        {
        case ' ':
            m_putc(*pStr); ret_num++; break;
        case '\t':
            m_putc(*pStr); ret_num += 4; break;
        case '\r':
            m_putc(*pStr); ret_num++; break;
        case '\n':
            m_putc(*pStr); ret_num++; break;
        case '%':
            pStr++;
            // % 格式解析
            switch (*pStr)
            {
            case '%':
                m_putc('%');// %%，输出%
                ret_num++;
                pStr++;
				continue;
            case 'c':
                ArgIntVal = va_arg(pArgs, int);// %c，输出char
                m_putc((char)ArgIntVal);
                ret_num++;
                pStr++;
				continue;
            case 'd':
                // 接收整型
                ArgIntVal = va_arg(pArgs, int);
                if (ArgIntVal < 0)// 如果为负数打印，负号
                {
                    ArgIntVal = -ArgIntVal;// 取相反数

                    m_putc('-');
                    ret_num++;
                }
                val_seg = ArgIntVal;// 赋值给 val_seg处理数据
                // 计算ArgIntVal长度
                if (ArgIntVal)
                {
                    while (val_seg) {
                        cnt++;
                        val_seg /= 10;
                    }
                }
                else cnt = 1;// 数字0的长度为1

                ret_num += cnt;// 字符个数加上整数的长度

                // 将整数转为单个字符打印
                while (cnt)
                {
                    val_seg = ArgIntVal / m_pow_n(10, cnt - 1);
                    ArgIntVal %= m_pow_n(10, cnt - 1);
                    m_putc((char)val_seg + '0');
                    cnt--;
                }
                pStr++;
                continue;
            case 'o':
                // 接收整型
                ArgIntVal = va_arg(pArgs, int);
                if (ArgIntVal < 0)// 如果为负数打印，负号
                {
                    ArgIntVal = -ArgIntVal;// 取相反数

                    m_putc('-');
                    ret_num++;
                }
                val_seg = ArgIntVal;// 赋值给 val_seg处理数据
                // 计算ArgIntVal长度
                if (ArgIntVal)
                {
                    while (val_seg) {
                        cnt++;
                        val_seg /= 8;
                    }
                }
                else cnt = 1;// 数字0的长度为1

                ret_num += cnt;// 字符个数加上整数的长度

                // 将整数转为单个字符打印
                while (cnt)
                {
                    val_seg = ArgIntVal / m_pow_n(8, cnt - 1);
                    ArgIntVal %= m_pow_n(8, cnt - 1);
                    m_putc((char)val_seg + '0');
                    cnt--;
                }
                pStr++;
				continue;
            case 'x':
                // 接收16进制
                ArgHexVal = va_arg(pArgs, unsigned long);
                val_seg = ArgHexVal;
                // 计算ArgIntVal长度
                if (ArgHexVal)
                {
                    while (val_seg) {
                        cnt++;
                        val_seg /= 16;
                    }
                }
                else cnt = 1;// 数字0的长度为1

                ret_num += cnt;// 字符个数加上整数的长度
                // 将整数转为单个字符打印
                while (cnt)
                {
                    val_seg = ArgHexVal / m_pow_n(16, cnt - 1);
                    ArgHexVal %= m_pow_n(16, cnt - 1);
                    if (val_seg <= 9)
                        m_putc((char)val_seg + '0');
                    else
                    {
						//m_putc((char)val_seg - 10 + 'a'); //小写字母
                        m_putc((char)val_seg - 10 + 'A');
                    }
                    cnt--;
                }
                pStr++;
				continue;
            case 'b':
                // 接收整型
                ArgIntVal = va_arg(pArgs, int);
                val_seg = ArgIntVal;
                // 计算ArgIntVal长度
                if (ArgIntVal)
                {
                    while (val_seg) {
                        cnt++;
                        val_seg /= 2;
                    }
                }
                else cnt = 1;// 数字0的长度为1

                ret_num += cnt;// 字符个数加上整数的长度
                // 将整数转为单个字符打印
                while (cnt)
                {
                    val_seg = ArgIntVal / m_pow_n(2, cnt - 1);
                    ArgIntVal %= m_pow_n(2, cnt - 1);
                    m_putc((char)val_seg + '0');
                    cnt--;
                }
                pStr++;
				continue;
            case 's':
                // 接收字符
                ArgStrVal = va_arg(pArgs, char*);
                ret_num += (unsigned int)strlen(ArgStrVal);
                while (*ArgStrVal)
                {
                    m_putc(*ArgStrVal);
                    ArgStrVal++;
                }

                pStr++;
				continue;

            case 'f':
                // 接收浮点型 保留6为小数，不采取四舍五入
                ArgFloVal = va_arg(pArgs, double);
                val_seg = (unsigned long)ArgFloVal;// 取整数部分
                val_temp = val_seg;      // 临时保存整数部分数据
                ArgFloVal = ArgFloVal - val_seg;// 得出余下的小数部分
                // 计算整数部分长度
                if (val_seg)
                {
                    while (val_seg) {
                        cnt++;
                        val_seg /= 10;
                    }
                }
                else cnt = 1;// 数字0的长度为1
                ret_num += cnt;// 字符个数加上整数的长度
                // 将整数转为单个字符打印
                while (cnt)
                {
                    val_seg = val_temp / m_pow_n(10, cnt - 1);
                    val_temp %= m_pow_n(10, cnt - 1);
                    m_putc((char)val_seg + '0');
                    cnt--;
                }
                // 打印小数点
                m_putc('.');
                ret_num++;
                // 开始输出小数部分
                ArgFloVal *= 1000000;
                // printf("\r\n %f\r\n", ArgFloVal);
                cnt = 6;
                val_temp = (int)ArgFloVal;// 取整数部分
                while (cnt)
                {
                    val_seg = val_temp / m_pow_n(10, cnt - 1);
                    val_temp %= m_pow_n(10, cnt - 1);
                    m_putc((char)val_seg + '0');
                    cnt--;
                }
                ret_num += 6;
                pStr++;
				continue;
            default:// % 匹配错误，暂输出空格
				m_putc(' '); ret_num++;
				continue;
            }


        default:
            m_putc(*pStr); ret_num++;
            break;
        }
        pStr++;
    }
    va_end(pArgs);// 结束取参数

    return ret_num;
}
