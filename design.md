Design document design.pdf which should reside in the project directory with the rest of
your code. Your design document should describe the design of your project in enough detail
that a knowledgeable programmer could duplicate your work. This includes descriptions of
the data structures you use, all non-trivial algorithms and formulas, and a description of each
function including its purpose, inputs, outputs, and assumptions it makes about the inputs or
outputs

# Memory Allocation on MINIX3
Maxwell Meier and Sirajus Salekin
--------
The objective for this project was to implement five memory allocation algorithms.
Minix had a first fit algorithm implemented already (it was very close to being a next fit
though). Specifically, we implemented the following algorithms:

1. Best Fit
2. Next Fit
3. Worst Fit
4. Random Fit
5. First Fit (default)

The most difficult part of this project was to figure out the dependencies and
figure out exactly what parts of the codebase we need to make changes to. We knew
that the preexisting implementation was in `alloc.c` file. So the next thing we
needed to figure out was exactly what part of the file had that implementation of
first fit policy. This file can be found at `/minix/servers/vm/alloc.c`.
Next, we found the fucntion where the existing first fit algorithm was defined is called 
`findbit`. We found this function by looking for functions that had names that indicated 
that they implement memory allocations. Although the function `alloc_mem` described 
in the comments inferred that it was implementing the first fit algorithm, but we
had to trace back to find the culprit `findbit` function.

Then we looked into the function definitions and tried to figure out what 
variable does which task, and where are they defined. 

Let's look at what we found in the first fit implementation by MINIX.
The function looks like this:
```
static int findbit(int low, int startscan, int pages, int memflags, int *len)
```
Let's break it down into peices.
```
int startscan # The starting point where the we look for memory hole with a loop
int low # The ending point for the loop

int pages # size of the memory hole we need for our process 

int memflags  # Not important for our purposes because they didn't matter
              # for what we were trying to do.
                       
int *len # a pointer to an integer recording the size of the memory block.  
         # Due to the original algorithm stopping as soon as it found a memory hole 
         # of suitable size, this was invariably equal to the pages variable, 
         # so to keep the other algorithms from having unexpected side effects, 
         # this was set equal to pages on all our functions
```
After we found out why the first fit function is using those parameters, and what
role each of them play, we had now a way to implement other algorithms using those
(and perhaps a few other) variables.

Now it's time to implement the algorithms. Let's do a quick recap of each of them.
1. Best Fit: Finding the best hole for the requested size. So if requested size is
  150, we will try to get a hole as close as we possibly can.
2. Next Fit: Start looking for the hole from the last place we 
  stopped at. So if we stopped at memory address 1500 in the previous request cycle, 
  we will start our search for new request from 1500.
3. Worst Fit: Settling for the biggest hole (which is the worst one for the size of
  the requested memory)
4. Random Fit: Look at all the potential holes, and pick one at random.
5. First Fit (default): Doesn't look any further; settle for the first hole it finds.

