# dynamic_linking_libraries

Week 2 Assignment 

 created all .h and .cpp files 
 
 created object files 
 g++ -Wall -fPIC -c addition.cpp subtraction.cpp division.cpp multiplication.cpp -I../inc
 
 created the libmath.so
 g++ -shared -Wl,-soname,libmath.so.1 -o libmath.so.1.0 *.o
 
 Moved the created library to opt/lib
 sudo mv libmath.so.1.0 /opt/lib
 
 created softlinks
 sudo ln -sf /opt/lib/libmath.so.1.0 /opt/lib/libmath.so.1
 sudo ln -sf /opt/lib/libmath.so.1.0 /opt/lib/libmath.so
 
 compile 
 g++ -Wall -L/opt/lib main.cpp -lmath -o out -I../inc
 
 to run set path:
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/opt/lib
./out 


 g++ -rdynamic main.cpp -ldl -I../inc
