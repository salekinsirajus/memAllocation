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
Next, the fucntion where the existing first fit algorithm was defined is called 
`findbit`. We looked into the function definitions and tried to figure out what 
variable does which task, and where are they defined. 

Let's look into what we found in the first fit implementation by MINIX.
The function looks like this:
```
static int findbit(int low, int startscan, int pages, int memflags, int *len)
```
