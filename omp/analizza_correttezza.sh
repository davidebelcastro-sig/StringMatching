diff $1 $2 > isCorrect.txt;
var=`cat isCorrect.txt`;
corr="";
if [ "$var" = "$corr" ]; then
    echo "Analisi correttezza tra $1 e $2:ok"
else
    echo "Analisi correttezza tra $1 e $2:fail";
    echo $var;
fi

