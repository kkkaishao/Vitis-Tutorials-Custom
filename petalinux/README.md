# Xilinx Versal Common Images

Download the pre-built PetaLinux images for Xilinx Versal devices from the following link:

[Common Images for Embedded Vitis™ Platforms - 2025.1  Full Product Installation](https://www.xilinx.com/support/download/index.html/content/xilinx/en/downloadNav/embedded-design-tools.html)

It needs license agreement to download the images, so it's unable to provide a download script here.

After downloading the images, extract all the files to the current directory.

For example, execute the following command in current directory:
```bash
tar -xzvf /path/to/xilinx-versal-common-<version>.tar.gz --strip-components=1 -C .
```

Also, current folder contains a pre-compiled device tree file `system.dtb`, which has a workaround for the SD card issue. When you try to boot the PetaLinux from SD card on ALINX VD100 board, you may encounter a mmc issue as follows:

```
[    2.632483] mmc1: Tuning failed, falling back to fixed sampling clock
[    2.638943] mmc1: new ultra high speed SDR104 SDHC card at address 0001
[    2.645976] mmcblk1: mmc1:0001 MSSD0 29.2 GiB
[    2.699708] mmc1: Tuning failed, falling back to fixed sampling clock
[    2.828642] mmc1: Tuning failed, falling back to fixed sampling clock
[    2.883455] mmc1: Tuning failed, falling back to fixed sampling clock
[    2.889911] I/O error, dev mmcblk1, sector 0 op 0x0:(READ) flags 0x0 phys_seg 1 prio class 0
[    2.899929] Buffer I/O error on dev mmcblk1, logical block 0, async page read
[    2.955549] mmc1: Tuning failed, falling back to fixed sampling clock
[    3.010411] mmc1: Tuning failed, falling back to fixed sampling clock
[    3.065242] mmc1: Tuning failed, falling back to fixed sampling clock
[    3.120020] mmc1: Tuning failed, falling back to fixed sampling clock
[    3.126475] I/O error, dev mmcblk1, sector 0 op 0x0:(READ) flags 0x0 phys_seg 1 prio class 0
[    3.136510] Buffer I/O error on dev mmcblk1, logical block 0, async page read
[    3.143667]  mmcblk1: unable to read partition table
[    3.157939] /dev/root: Can't open blockdev
[    3.162055] VFS: Cannot open root device "/dev/mmcblk1p2" or unknown-block(179,2): error -6
[    3.170409] Please append a correct "root=" boot option; here are the available partitions:
```

You can use this file directly in your PetaLinux project to fix the issue. It limits the maximum clock frequency of the SD card to 50MHz.

However, customize device tree file for you own project is also recommended. Finding out the solution of a specific issue do help you understand the hardware and software better.