#!/bin/bash
function rmtmp {
  if [[ -e $MNT_INFO ]];then rm $MNT_INFO;fi
  unset MNT_INFO
}
function getmnt {
  if [[ -e $MNT_INFO ]];then rmtmp;fi
  MNT_INFO=`mktemp`
  gio mount -li > $MNT_INFO
  export MNT_INFO
}
function errmsg() {
  echo $1 1>&2
  exit 1
}
#if [[ -e ./.mbed ]];then PROJ=`basename $(realpath .)`;fi
if [[ $# -eq 0 ]] && [[ -e ./.mbed ]];then PROJ=`realpath .`;BIN=`basename $(realpath .)`
elif [[ $# -lt 1 ]] || [[ $# -gt 2 ]] ;then errmsg "Usage: $0 <projectdir> [project-name]";fi
if [[ $# -eq 1 ]];then PROJ=`realpath $1`;BIN=`basename \`realpath $1\``;fi
if [[ $# -eq 2 ]];then PROJ=`realpath $1`;BIN=$2;fi
cd $PROJ
if [[ $? -ne 0 ]];then errmsg "Error entering project directory!";fi
if [[ -e ./.mbed ]];then source ./.mbed;fi
if [[ -z $TOOLCHAIN ]] || [[ -z $TARGET ]];then errmsg "configuration environment variables for target or toolchain have not been set, they have to be set.";fi
#MBED_TOOLCHAIN=`mbed toolchain 2>&1 | tail -c +8`
#MBED_TARGET=`mbed target 2>&1 | tail -c +8`
#if [ -z $MBED_TOOLCHAIN ] || [ -z $MBED_TARGET ];then
#  echo "The mbed configuration for target or toolchain has not been set, it has to be set." 2>&1
#  exit 1
#fi
getmnt
MBED_DEV=`sed -ne '/MBED/,+3 { s~\s*unix-device: \x27\(/dev/\S*\)\x27~\1~p }' $MNT_INFO`
if [[ -z $MBED_DEV ]];then errmsg "Error finding MBED microcontroller!";fi
unset IS_MNT
IS_MNT=`sed -ne '/MBED/,$ { /Mount([0-9])/p };' $MNT_INFO`
if [[ -n $IS_MNT ]];then
  MBED_MNT=`sed -ne '/MBED/,$ { s~.*-> file://\(.*\)$~\1~p }' $MNT_INFO`
fi
if [[ -n $IS_MNT ]];then
  gio mount -u $MBED_MNT
  sleep 0.2
fi
gio mount -m -d $MBED_DEV
sleep 0.2
getmnt
unset MBED_MNT
MBED_MNT=`sed -ne '/MBED/,$ { s~.*-> file://\(.*\)$~\1~p }' $MNT_INFO`
if [[ $? -eq 1 ]] || [[ -z $MBED_MNT ]];then errmsg "Something went wrong somewhere! Couldn't mount MBED microcontroller.";fi
mbedflsh -f ./BUILD/$TARGET/$TOOLCHAIN/$BIN.bin -d $MBED_MNT
rmtmp
