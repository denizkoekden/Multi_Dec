SET BASEDIR=D:\DDK
nmake %1 /f SAA7146.mak WIN95=1
nmake %1 /f SAA7146.mak
copy winnt\MDSAA.sys ..
copy win95\MDSAA.vxd ..
