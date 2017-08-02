### Running the Program
To run the program, compile using 
```shell
gcc phase1.c -o phase1 
```
then specify the follow arguments at the command line:

For a forward transformation: <br>
```shell
./phase1 --infile <input_name.txt> --outfile <output_name.ph1> --forward
```
For a backward transformation: <br>
```shell
./phase --infile <input_name.ph1> --outfile <output_name.txt> --backward
```
**OR**

To automate all tests, after compiling, run 
```shell
./alltests.sh
```
