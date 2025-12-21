#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/timer.h>
#include <linux/jiffies.h>
#include <asm/io.h>
#include "meu_driver.h"

static struct timer_list timer0;

/* --- PROTÓTIPO DA FUNÇÃO --- */
int ler_rtc(char *h, char *m, char *s);

/* --- CALLBACK DO TIMER --- */
static void timer_callback(struct timer_list *data)
{
    static int counter = 0;
    char h, m, s;

    if (ler_rtc(&h, &m, &s) == 0)
    {
        printk(KERN_INFO "(%s) Evento %d — Hora RTC: [%02x:%02x:%02x]\n",
               DRIVER_NAME, counter++, h, m, s);
    }

    mod_timer(&timer0, jiffies + msecs_to_jiffies(MY_TIMEOUT));
}

/* --- FUNÇÃO PARA LER O RTC (CMOS) --- */
int ler_rtc(char *h, char *m, char *s)
{
    if (!h || !m || !s)
        return -1;

    outb(RTC_HOUR_REG, CMOS_ADDRESS_PORT);
    *h = inb(CMOS_DATA_PORT);

    outb(RTC_MIN_REG, CMOS_ADDRESS_PORT);
    *m = inb(CMOS_DATA_PORT);

    outb(RTC_SEC_REG, CMOS_ADDRESS_PORT);
    *s = inb(CMOS_DATA_PORT);

    return 0;
}

/* --- INIT DO MÓDULO --- */
static int __init meu_driver_init(void)
{
    printk(KERN_INFO "Meu driver iniciou!\n");

    timer_setup(&timer0, timer_callback, 0);
    mod_timer(&timer0, jiffies + msecs_to_jiffies(MY_TIMEOUT));

    return 0;
}

/* --- EXIT DO MÓDULO --- */
static void __exit meu_driver_exit(void)
{
    del_timer(&timer0);
    printk(KERN_INFO "Meu driver encerrou!\n");
}

module_init(meu_driver_init);
module_exit(meu_driver_exit);


MODULE_LICENSE("GPL");
MODULE_AUTHOR("Ana Alicy");
MODULE_DESCRIPTION("Driver de exemplo com RTC e Timer");

