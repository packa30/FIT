#!/bin/bash

#Projekt c.1 IOS
#Autor : Peter Grofčík
#login : xgrofc00

	if [ -z  $1 ]
	then
	DIR=$(pwd)
	else

		if [[ $1 == *"-i"* ]]
		then
			if [[ $2 =~ ^[a-zA-Z0-9]+$ ]]
			then
			ignore=$2
			else
			echo Chybny argument ignorovanych suborov
			exit 1
			fi

			if [ -z $3 ]
        		then
                		DIR=$(pwd)
			else
                		DIR=$3
			fi

		else
			if [ -z $2  ]
			then
			DIR=$1
			else
			echo Chybny prepinac
			exit 1
			fi
		fi
	fi

zakaz=`find $DIR ! -perm -g+r,u+r,o+r -prune | wc -l`
if [ $zakaz  == '0' ]
then

p=0
pk=1
while [ $P1 != $P2 ]
do
if [ -z $ignore ]
then
P1=`find $DIR -maxdepth $p -type d -print | wc -l`
P2=`find $DIR -maxdepth $pk -type d -print | wc -l`
else
P1=`find $DIR -maxdepth $p -type d -print | grep -v "$ignore" | wc -l`
P2=`find $DIR -maxdepth $pk -type d -print | grep -v "$ignore" | wc -l`
fi
p=$((p+1))
pk=$((pk+1))
done
DD=$p

if [ -z $ignore ]
then
FILES=`find $DIR -type f -exec du -b {} \; |sort -n`
ND=`find $DIR -type d | wc -l`
NF=`find $DIR -type f | wc -l`
ELp1=`find $DIR -type f | grep -E '[a-zA-Z0-9]+\.[a-zA-Z0-9]*$' | awk -F . '{print $NF}'`
else
FILES=`find $DIR -type f -exec du -b {} \; | grep -Fv "$ignore." |sort -n`
ND=`find $DIR -type d | grep -wv "$ignore" | wc -l`
NF=`find $DIR -type f | grep -Fv "$ignore/" | grep -Fv "$ignore." | wc -l`
ELp1=`find $DIR -type f | grep -Fv "$ignore/" | grep -Fv "$ignore." | grep -E '[a-zA-Z0-9]+\.[a-zA-Z0-9]*$' | awk -F . '{print $NF}'`
fi
LF=`echo "$FILES" | awk '{max=$1} END {print max}'`
AF=$((NF/ND))

if [ $NF == '0' ]
then
AS=N/A
MS=N/A
else
AS=`echo "$FILES" | awk '{sum+=$1;n++} END {print sum/n}'|cut -f1 -d"."`
HP=$((NF/2))
MS1=`echo "$FILES" | nl | sed -n "$((HP+1))"p | awk '{ff=$2} END {print ff}'`
if [ $((NF%2)) -gt 0 ];
then
	MS=$MS1
else
	MS2=`echo "$FILES"| nl | sed -n "$((HP))"p | awk '{ff=$2} END {print ff}'`
	MS=$(((MS1+MS2)/2))
fi
fi

ELp2=`echo "$ELp1" | sort -u`
EL=`echo "$ELp2"| paste -s -d,`

if [ -z $LF ] || [ $LF == '0' ]
then
LF=N/A
else
:
fi

echo Root directory: $DIR
echo Directories: $ND
echo Max depth: $DD
echo Average no. of files: $AF
echo All files: $NF
echo "  Largest file: "$LF
echo "  Average file size: "$AS
echo "  File size median: "$MS
echo File extensions: $EL



if [ -z $EL ]
then
:
else
for EXT in $ELp2
do

if [ -z $ignore ]
then
EXTY=`find $DIR -type f -exec du -b {} \; |grep -E '\.+['$EXT']*$'| grep -w "$EXT" | sort -n`
else
EXTY=`find $DIR -type f -exec du -b {} \; | grep -Fv "test/" | grep -Fv "$ignore." |grep -E '\.+['$EXT']*$'| grep -w "$EXT" | sort -n`
fi

NEXT=`echo "$ELp1" | grep -w "$EXT" | wc -l `
LEXT=`echo "$EXTY" | awk '{max=$1} END {print max}'`
AEXT=`echo "$EXTY" | awk '{sum+=$1;n++} END {print sum/n}'|cut -f1 -d"."`


NEXTHP=$((NEXT/2))
mext1=`echo "$EXTY"| nl | sed -n "$((NEXTHP+1))"p | awk '{ff=$2} END {print ff}'`

if [ $((NEXT%2)) -gt 0 ];
then
MEXT=$mext1
else
mext2=`echo "$EXTY"| nl | sed -n "$((NEXTHP))"p | awk '{ff=$2} END {print ff}'`
MEXT=$(((mext1+mext2)/2))
fi

echo Files .$EXT: $NEXT
echo "  Largest file ".$EXT: $LEXT
echo "  Average file size ".$EXT: $AEXT
echo "  File size median ".$EXT: $MEXT
done
fi

else
echo Nepovoleny pristup
exit 1
fi
