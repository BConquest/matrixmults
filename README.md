# matrixmults
## Packages
- Creates and Populates input matrices from input files
- of the form
```text
(x) rows (y)cols
Nx1y1 Nx1y2 ... Nx1y
Nx2y1 Nx2y2 ... Nx2y
...   ...   ... ...
Nxy1  Nxy2  ... Nxy
```
- after reading these in will create subtasks for dotproducts (each subtask has
  a type of 1 and put onto a message queue
- read completed calculations from queue and populate the output matrix
- display output matrix to screen and output file

## Compute
- creates a pool of threads to read subtasks from message queue
- calculates the value for each position
- returns the result onto the message queue with a message type 2


## Both programs
- will capture SIGINT and print out a status of calculation of the form
```text
Jobs sent X Jobs Received 0
```
- will print a message when a message is successfully sent or recieved of the
  form
```text
Sending job id x type (1 or 2) size Y (rc=0)
Receiving job id x type (1 or 2) size Y
```

