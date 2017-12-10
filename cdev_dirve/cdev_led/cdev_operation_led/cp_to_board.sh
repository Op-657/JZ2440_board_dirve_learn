#/bin/bash
APP_TEST="app_cdev_led.c" 
DRV=cdev_led.c 
echo "-------make---------"
make
echo ""

echo "-------arm-linux-gcc--------"
echo ""arm-linux-gcc ${APP_TEST} -o app_test.exe
arm-linux-gcc ${APP_TEST} -o app_test.exe
echo "  "

cp *.ko app_test.exe /work/nfs_root
echo "cp success"

