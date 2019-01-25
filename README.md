### mac bochs 配置vhd格式磁盘
1. 安装sdl

```shell
    $ brew install sdl
    # 如果一直卡在homebrew的update上，可以配置关闭update
    $ export HOMEBREW_NO_AUTO_UPDATE=true
```

2. 下载最新的bochs安装包 [bochs-2.6.tar.gz](https://sourceforge.net/projects/bochs/files/bochs/)

3. 解压缩包

```shell
    $ tar -xvf bochs-2.6.tar.gz
```

4. 下载补丁安装patch

```shell
    $ wget https://raw.githubusercontent.com/Homebrew/formula-patches/e9b520dd4c/bochs/xcode9.patch
    $ cd bochs-2.6
    $ patch -p1 < ../xcode9.patch
```

5. 修改源码config.cc 第3261行

```c
    if (SIM->get_param_string("model", base)->getptr()>0){

    //修改为

    if (SIM->get_param_string("model", base)->getptr()>(char *)0){
```

6. 安装

```shell
    $ ./configure --enable-ne2000 \
            --enable-all-optimizations \
            --enable-cpu-level=6 \
            --enable-x86-64 \
            --enable-vmx=2 \
            --enable-pci \
            --enable-usb \
            --enable-usb-ohci \
            --enable-e1000 \
            --enable-debugger \
            --enable-disasm \
            --disable-debugger-gui \
            --with-sdl \
            --prefix=$HOME/tools/bochs #选择你需要安装的目录
    
    $ make && make install
```

7. 配置vhd磁盘启动文件

> bochs的安装目录下的bochs/share/doc/bochs/bochsrc-sample.txt是配置文件的模板

> 自定义的配置文件取名为bochsrc.txt, 内容如下

```
###############################################
# Configuration file for Bochs
###############################################
 
# 第一步，首先设置Bochs在运行过程中能够使用的内存，本例为32MB。
# 关键字为：megs
megs: 32
 
# 第二步，设置对应真实机器的BIOS和VGA BIOS.
# 对应两个关键字为：romimage 和 vgaromimage
romimage: file=$BXSHARE/BIOS-bochs-latest
vgaromimage: file=$BXSHARE/VGABIOS-lgpl-latest
 
# 第三步，设置Bochs所使用的磁盘，软盘的关键字为floppy。
# 若只有一个软盘，则使用floppya即可，若有多个，则为floppya，floppyb...
# floppya: 1_44=a.img, status=inserted
# 由于我们使用硬盘启动，设置不检测软盘
floppy_bootsig_check: disabled=1
# 第四步，选择启动盘符。
boot: disk

ata0: enabled=1, ioaddr1=0x1f0, ioaddr2=0x3f0, irq=14
ata0-master: type=disk, path="/Users/jasey/VirtualBox VMs/LEARN-ASM/LERAN-ASM.vhd", mode=vpc, cylinders=1003, heads=12, spt=17, model="Generic 1234", biosdetect=auto, translation=auto
ata1: enabled=1, ioaddr1=0x170, ioaddr2=0x370, irq=15
ata2: enabled=0
ata3: enabled=0
 
# 第五步，设置日志文件的输出。
log: bochsout.txt

```

> 其中`ata0-master: type=disk, path = XXX`path为指定的vhd文件路径

8. 在配置文件下运行bochs

```shell 
    $ $HOME/tools/bochs/bin/bochs
```

### mac tool

[mbr-lba使用文档](https://github.com/Jasey/c-tools)

### 8086 汇编寄存器

#### 通用寄存器
`AX, BX, CX, DX, SI, DI, SP, BP`

> 前面四个寄存器可分别访问高位和低位，后面四个不行

##### AX(Accumulator)
累加器寄存器，可用于除法，被除数放置的寄存器

16位除以8位时，结果为8位，余数为8位，商保存在AL中，余数保存在AH中

32位除以16位是，结果为16位，商为16位，被除数的低16位在AX中，结果在AX中

##### BX(Base Address Tegister)
基地址寄存器，寄存器的内容可以作为地址，访问内存

##### CX（Countor)
计数寄存器，该寄存器可以用于循环，CX非零，则重复执行循环
包括loop指令，rep指令

##### DX(Data)
数据寄存器

32位除以16位是，被出示的高16位位于DX中，余数在DX中

##### SI(Source Index)
源索引寄存器

数据传送指令中，源数据内存地址的段偏移存储在SI中，段基地址从DS中获取，逻辑地址 `DS:SI`

##### DI(Destination Index)
目标索引寄存器，和SI一样，都可以用于数据传送操作

数据传送指令中，目的数据内存地址的段偏移存储在DI中，段基地址从ES中获取，逻辑地址 `ES:DI`



#### 段寄存器
`DS, CS, ES, SS`

DS 数据段基地址寄存器，指令中如果没有指定段地址，则使用该寄存器的值

CS 代码段基地址寄存器，cpu获取指令计算IP时，如果使用的是绝对偏移地址，则基地址从CS寄存器中获取

ES 扩展段基地址寄存器，可以根据这个寄存器基地址访问数据，但是需要指定段寄存器
