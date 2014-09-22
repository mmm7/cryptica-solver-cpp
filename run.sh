#/bin/bash
for i in `ls levels/*in`
do
#echo '------------------------------'
#echo $i
#./solve simple < $i
echo $i, `./solve simple < $i`
done

