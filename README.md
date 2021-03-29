# FileHandlingTest
File Handling Test(with C Library and System call function) In Ubuntu 20.04
## Precondition
File data and Users Arguments are permitted only numbers(int), Alpabets.
In arguemnt example <originFile> -> must exist in Your storage
  
## copy.c
make new coped file. read form originfile and write data copied file 
reading units are 10 bytes.

ex) a.out <originFile> <copidedFile>
  
## read.c
In origin File, including Offset data , read offset ~ offset + bytes and print in Terminal
if no more data, read up to end of file

ex) a.out <originFile> <offset> <bytes>
  
## merge.c
