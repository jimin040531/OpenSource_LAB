i=1
until [ $i -gt 9 ]
do
	echo "$i 단: "
	j=1
	until [ $j -gt 9 ]
	do
		echo "$i x $j = $((i*j))"
		j=$((j+1))
	done
	echo
	i=$((i+1))
done
