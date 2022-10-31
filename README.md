# TCP-UDP-CONN
UDP - TCP APPLICATION C++ FOR LINUX/UBUNTU  

Application that connect 4 components.  

[ TERMINAL 1 ] -> [ A ] -> [ B ] <-> [ SERVER = TERMINAL 2]

Component A and B will be connected through a PIPELINE in a [ TERMINAL 3] with its respective command.  

After downloading the folder, just run : 
```
make
```
After A and B have been created open 3 terminals inside the folder and do in this order:

[PORT 8080]  
[ SERVER = TERMINAL 2] :  
Create a server listening through a TCP connection by running the netcat command:

```
nc -l 8080 
```


[TERMINAL 3 ] :  
Run the two files ./A and ./B. I decided to connect A and B through a PIPELINE so that the input of Component B is the output of Component A.  
Run in the terminal:  
```
./A | ./B
```
(Now if we write in the [ TERMINAL 2 ] some message it will be displayed in the [ TERMINAL 3 ] )

Now the SERVER and A are connected to B with 2 different approach ( STDIN AND TCP ).  

[PORT 8081]  
[ TERMINAL 1] :  

I need to send through UDP connection messages to A therefore in the terminal I run this command:  

```
nc -w 1 -u localhost 8081 < input.txt
```

Which will send the content of input.txt to A.  
The content will be sent in the stdoutput so B can read through the PIPELINE as stdinput.  
B then will evaluate the content, if the unsigned char is 10 that means LINE FEED therefore I evaluate the parameters passed until now.  
Is possible to send through the file more than one message, it's only needed to separate the messages with 10s as in the input.txt file that I uploaded as test.  
  
  I added some checks for the lenght of the message, since they should be pair of p and v I decided that If the content lenght is an odd number that means that a volume is missing, therefore I just don't consider the last price and I make the content lenght even.
  
  
  
  



