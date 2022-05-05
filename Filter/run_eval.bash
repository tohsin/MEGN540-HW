cd BIN

FILE=../solution/Ring_Buffer.c
CLEANUP=false
if test -f "$FILE"; then
    echo "Student Provided Ring_Buffer Files"
else
    mv ../solution/Ring_Buffer_c ../solution/Ring_Buffer.c
    mv ../solution/Ring_Buffer_h ../solution/Ring_Buffer.h
    CLEANUP=true
fi

cmake ..
make clean 
make 
./filter_hw_test ->result.txt


if $CLEANUP; then
    mv ../solution/Ring_Buffer.c ../solution/Ring_Buffer_c
    mv ../solution/Ring_Buffer.h ../solution/Ring_Buffer_h
fi

rm ../solution/*.c ../solution/*.h


cd ..
