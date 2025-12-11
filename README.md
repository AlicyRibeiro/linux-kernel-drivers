# linux-kernel-drivers

#  meu_driver — Módulo de Kernel com Timer e Acesso ao RTC

Este diretório contém o **módulo básico de kernel** desenvolvido na seção **5.3.1** do material *Técnicas de Programação para Sistemas Embarcados II*.

Ele apresenta três conceitos essenciais:

1. **Criação de um módulo simples de kernel**
2. **Uso de timers periódicos no kernel**
3. **Acesso direto ao hardware (RTC via portas CMOS)**

---

##  Estrutura dos Arquivos

* meu_driver/
* ├── Makefile        
* ├── meu_driver.c       
* └── meu_driver.h


---

#  1. Funcionalidades do módulo

###  Inicialização e finalização

O módulo imprime mensagens no log ao ser carregado e removido usando:

- `module_init(meu_driver_init)`
- `module_exit(meu_driver_exit)`
- `printk(KERN_INFO "...")`

---

#  2. Timer Periódico no Kernel

O módulo cria um timer usando:

- `timer_setup`
- `mod_timer`
- `del_timer`

A função `timer_callback()` é chamada periodicamente e:

- imprime um contador
- agenda o próximo evento usando `jiffies + msecs_to_jiffies(MY_TIMEOUT)`

---

#  3. Acesso ao Hardware — Leitura do RTC

O driver lê o horário do RTC via portas CMOS:

- Registro selecionado pela porta `0x70`
- Dado lido na porta `0x71`

Definições no `meu_driver.h`:

```c
#define CMOS_ADDRESS_PORT 0x70
#define CMOS_DATA_PORT    0x71

```


O módulo utiliza:

- `inb()` → leitura  
- `outb()` → escrita  

Essas funções pertencem ao cabeçalho `<asm/io.h>`.

A função `ler_rtc()` obtém **hora, minuto e segundo** diretamente da CMOS e o timer exibe o horário no log.

---

##  Como compilar

Dentro da pasta `meu_driver/` execute:

```bash
make
```

Isso gera o arquivo:

```bash
meu_driver.ko
```

---

## Como carregar o módulo

```bash
sudo insmod meu_driver.ko
```

Verifique as mensagens:

```bash
dmesg -w
```

---

## Como remover o módulo

```bash
sudo rmmod meu_driver
```

⚠️ Sempre remova o módulo antes de recompilar ou carregar novamente.

## Logs úteis

Para acompanhar as mensagens do driver:

```bash
dmesg -w
```

Ou:

```bash
tail -f /var/log/syslog
```
