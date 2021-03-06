#!/bin/bash
binDir="./bin/"

resultDir="./result/"

resultFile=$resultDir"dataMotor.csv"

make motor
touch $resultFile | cat >$resultFile

Te="0.01"

K="0.009"
I="13.5"
tMax="0.5"

$binDir"ResetState" L

$binDir"DCMotor" L 1.8 0.02 0.004 0.02 3.2e-5 6.5e-6 $Te $tMax > /dev/null &
pidDCMotor = $!
$binDir"RegPID" L $K $I 0.0 $Te $tMax >> $resultFile &
pidRegPID = $!
$binDir"SetTv" L 1.0 &

wait $pidDCMotor
wait $pidRegPID
python3 ./graphPloter.py $resultFile 