#!/bin/sh
chmod +x ./autogen.sh

echo '===== Begin [./autogen.sh] ====='
if /bin/sh ./autogen.sh
then
    echo '===== [./autogen.sh] ... OK ====='
else
    echo '===== [./autogen.sh] ... FAILED ====='
fi

echo '===== Begin [./configure] ====='
if ./configure $*
then 
    echo '===== [./configure] ... OK ====='
else 
    echo '===== [./configure] ... FAILED ====='
fi

filename="libtool.tmp"
if [ -e $filename ]
then
    rm $filename
fi
sed s/'$echo'/'$ECHO'/g libtool > libtool.tmp
mv libtool.tmp libtool

cd ./contrib

if [ -e $filename ]
then
    rm $filename
fi

sed s/'$echo'/'$ECHO'/g libtool > libtool.tmp
mv libtool.tmp libtool
cd ../

echo '===== Begin [./make] ====='
if make
then 
    echo '===== [./make] ... OK ====='
else 
    echo '===== [./make] ... FAILED ====='
fi


