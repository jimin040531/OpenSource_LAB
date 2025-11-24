i=1
while [ $i -le 9 ]
do 
	echo "$i 단: "
	j=1
	while [ $j -le 9 ]
	do 
		echo "$i x $j = $((i*j))"
		j=$((j+1))
	done
	echo
	i=$((i+1))
done	
