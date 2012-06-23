SET BASEDIR=D:\DDK
nmake %1 /f BT8x8.mak WIN95=1
nmake %1 /f BT8x8.mak
copy winnt\MDBT.sys ..
copy win95\MDBT.vxd ..

