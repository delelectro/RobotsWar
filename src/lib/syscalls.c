#include <sys/stat.h>
#include "stm32f10x_usart.h"

/* _end is set in the linker command file */
extern caddr_t _end;

/* just in case, most boards have at least some memory */
#ifndef RAMSIZE
#  define RAMSIZE             (caddr_t)0x50000
#endif

#define STACK_TOP 0x20000800

void uart_send(const char*str);

/*
 * sbrk -- changes heap size size. Get nbytes more
 *         RAM. We just increment a pointer in what's
 *         left of memory on the board.
 */
caddr_t
_sbrk(nbytes)
int nbytes;
{
    static caddr_t heap_ptr = NULL;
    caddr_t        base;

    if (heap_ptr == NULL) {
        heap_ptr = (caddr_t)&_end;
    }

    if ((STACK_TOP - (unsigned int)heap_ptr) >= 0) {
        base = heap_ptr;
        heap_ptr += nbytes;
        return (base);
    } else {
        uart_send("heap full!\r\n");
        return ((caddr_t)-1);
    }
}

int _open(const char *path, int flags, ...)
{
    return 1;
}

int _close(int fd)
{
    return 0;
}

int _fstat(int fd, struct stat *st)
{
    st->st_mode = S_IFCHR;
    return 0;
}

int _isatty(int fd)
{
    return 1;
}

int isatty(int fd)
{
    return 1;
}

int _lseek(int fd, off_t pos, int whence)
{
    return -1;
}

unsigned char getch(void)
{
    while (!(USART2->SR & USART_FLAG_RXNE));
    return USART2->DR;
}


int _read(int fd, char *buf, size_t cnt)
{
    *buf = getch();

    return 1;
}

void putch(unsigned char c)
{
    if (c == '\n') putch('\r');

    while (!(USART2->SR & USART_FLAG_TXE));
    USART2->DR = c;
}

void cgets(char *s, int bufsize)
{
    char *p;
    int c;
    int i;

    for (i = 0; i < bufsize; i++) {
        *(s+i) = 0;
    }
//    memset(s, 0, bufsize);

    p = s;

    for (p = s; p < s + bufsize-1;)
    {
        c = getch();
        switch (c)
        {
        case '\r' :
        case '\n' :
            putch('\r');
            putch('\n');
            *p = '\n';
            return;

        case '\b' :
            if (p > s)
            {
                *p-- = 0;
                putch('\b');
                putch(' ');
                putch('\b');
            }
            break;

        default :
            putch(c);
            *p++ = c;
            break;
        }
    }
    return;
}

int _write(int fd, const char *buf, size_t cnt)
{
    int i;
//    uart_send("_write\r\n");

    for (i = 0; i < cnt; i++)
        putch(buf[i]);

    return cnt;
}

/* Override fgets() in newlib with a version that does line editing */
char *fgets(char *s, int bufsize, void *f)
{
//    uart_send("fgets\r\n");
    cgets(s, bufsize);
    return s;
}
