# Memory Allocation on MINIX3
Maxwell Meier and Sirajus Salekin
--------
### Environment 
This project is developed for MINIX version 3.4.
It might not run in another environment.
*****
### Where
This program is structured into two directories. The one called
`testing` has only `testMemAlloc.c`, and produces a systemcall that
cycles through all the different algorithm we described in this document.

The other directory called `implementation` has the files we have actually
modified in order to implement the algorithm. The following list contains
the file name and where should they be placed
```
-Makefile	(minix/servers/pm/Makefile)
-alloc.c	(minix/servers/vm/alloc)
-callnr.h	(minix/include/minix/callnr.h)
-memheader.c	(minix/servers/pm/memheader.c)
-mempolicy.c	(minix/servers/pm/mempolicy.c)
-proto.h	(minix/servers/pm/proto.h)
-table.c	(minix/servers/pm/table.c)

# Unless otherwise specified, all of these files can be found under
# /usr/src/ path
```
Specifically, `alloc.c` has the algorithm implementation. We also wrote a
cycle-through-all-algorithms function, and it is implemented in `memheader.c` 
and `mempolicy.c` file.
The `proto.h`, `callnr.h`, `table.c` and `Makefile` were modified to implement
that system call.

*******
### How To Run:

```
1. Do a Make build as described in the assignment
2. After the build is successful, you can compile the testMemAlloc.c with
	Clang -o test testMemAlloc.c
	or
	cc -o test testMemAlloc.c
3. run ./test
```
*******
### On getsysinfo()
It appears that the last documentation on using `getsysinfo` system calls was
made onsolete by the current kernel update. They introduced a lot changes in 
the structure, and we searched for hours to no avail. We are convinced that
informations such as `SI_MEM_ALLOC` not available to user programs anymore
(which is what we need to use for getting memory usage info)

*******
### Algorithm Descriptions

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
Let's break it down into peices and see what each parameter does.
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

All the algorithms are implemented in the `alloc.c` file, with some comments on it.
So we are going to breifly explain how each algorithms implemented. Let's look at
the first few line of First Fit algorihtm (under the `findbit` function:

First Fit
-------
```
  int run_length = 0, i;
  # run_length is the number of consecutive free pages we have found in the current
    block. And i is a counter
  int freerange_start = startscan;
  # freerange is the range of free holes, so the variable freerange_start indicates
  # where that range starts.
  # we are going to start looking for potential holes from backwards (scoll up as we
  # said what each of parameters means
    
 ```
P.S.: we are using the `freerange=startscan` for all the algorithms to: start
scanning for holes from the high point to the low point.

Moving on, we enter into a `for` loop starting the actual searching process. There is
a bit of code after `if(!page_isfree(i))` part, and we are assuming it does some kind
of chunking, but we don't know why it does what it does. More importnantly,
it doesn't affect our algorithm, so we are not going down the rabbit hole. Part of 
programming an operating system is to have a working one, so we left it there so that
nothing breaks. We will just ignore this snippet! :D 
```
if(!page_isfree(i)) {//if the page at that address is free
			int pi;
			int chunk = i/BITCHUNK_BITS, moved = 0;
			run_length = 0;
			pi = i;
			while(chunk > 0 &&
			   !MAP_CHUNK(free_pages_bitmap, chunk*BITCHUNK_BITS)) {
				chunk--;
				moved = 1;
			}
			if(moved) { i = chunk * BITCHUNK_BITS + BITCHUNK_BITS; }
			continue;
		}
```
The followinf piece of code, however, we don't ignore. This is where the meat of 
the algorithm goes. Let's see how it works.
```
if(!run_length) { freerange_start = i; run_length = 1; }
		else { freerange_start--; run_length++; }
		assert(run_length <= pages);
		if(run_length == pages) {
		/* good block found! */
			*len = run_length;
			return freerange_start;
```
The `if(!run_length)` conditional means when we do NOT have ANY consecutive free pages
in this memory block, we set the `freerange_start = i` (at whatever point we are
in the range of free pages) and `run_length` to 1. This basically tells that since
there are no free pages in this block, we reset the range from here. (decrese the
width of the range)

But when we *do* have some free pages in this block, we widen the range (`freerange_start--`)
and increase the number of free pages in the block (`run_length++`).
Next, `if (run_length == pages)`, that is, we found that the number of consecutive 
free blocks is the same as the size of the hole we are looking for, 
our search stops there. This is the first
candidate and we sent the address of the page and go home. The line above it 
`assert(run_length <= pages);` ensures that we ARE indeed selecting the first one that we
find. If we are super unlucky and no hole big enough is found after looping through 
the available hole, we return `NO_MEMORY`.

Best Fit
------
First Fit isn't alway the most efficient algorithm. More often than not, when you 
wait, better things come along. The Best Fit algorithm, therefore, does not settle 
whenever it finds an eligible candidate. Let's take a look how it is implemented. 

Similar to the first fit algorithm, ignore the irrelevant part, whcih is:
```
for(i = startscan; i > low; i--) {
		if(!page_isfree(i)) {
			
			int pi;
			int chunk = i/BITCHUNK_BITS, moved = 0;
			run_length = 0;
			pi = i;
			while(chunk > 0 &&
				!MAP_CHUNK(free_pages_bitmap, chunk*BITCHUNK_BITS)) {
				chunk--;
				moved = 1;
			}
			if(moved) { i = chunk * BITCHUNK_BITS + BITCHUNK_BITS; }
			continue;
		}
```
Since we need to keep track of the best one so far, we initiliaze a variable
```
int best = INT_MAX;//Maximum value of an int
```

Our actual algorithm is contained here:
```
if((!page_isfree(i)) && (page_isfree(i-1)) && (run_length >= pages) && (run_length < best)) {
			best = run_length;
			bestaddress = freerange_start;
		}
		
		if(!run_length) { freerange_start = i; run_length = 1; }
		else { freerange_start--; run_length++; }
		
	}
	
	if(best >= pages) {
		*len = pages;
		return bestaddress;
```
At the following line we check a few conditions:
```
if((!page_isfree(i)) && (page_isfree(i-1)) && (run_length >= pages) && (run_length < best))
```
1. if the current page is free 
2. the page at the before that is NOT free 
3. the number of free pages in the block is at least as big as what we need, AND
4. smaller than the previous best we found 
We set that as the new best! 
```
			best = run_length;
			bestaddress = freerange_start;
```
It is big enough to hold but the smallest the wastes the least amount of memory(so far).
Next part, we do the same as what we did earlier:
```
if(!run_length) { freerange_start = i; run_length = 1; }
```
We have zero consecutive pages in this block, so we shorten the range to start from here.
But if we have some free pages, we widen the range.
```
else { freerange_start--; run_length++; }
```
After looking through all the available holes, we send the address of the best hole we
ended up with and send the address. 
```
if(best >= pages) {
		*len = pages;
		return bestaddress;
```
Worst Fit
-----
Worst Fit very similar to the First Fit algorithm, except we keep track of the worst candidate
with `int worst` variable (intialized at 0).

The important condition is the same as well, except:
```
if((!page_isfree(i)) && (page_isfree(i-1)) && (run_length >= pages) && (run_length > worst))
```
we have found this block is worse than the current worst (`run_length > worst`).

Random Fit
-----
The idea here is to keep track of eligible blocks, and pick one at random.
However, it was the most difficult to implement, due to difficulties with 
keeping an appropriatly sized list 
of memory holes and with picking one at random. So we decided to keep an array 
of 500 memory holes with enough free pages (`int addresslist[500];`)
with an integer (`int addressnumber`) tracking the number of holes present 
in the list.

Since `rand()` wasn't working, we used an uninitialzed variable to get some 
kind of random number. In C, an uninitialized variable can have any value, 
from the last time this address was used, and it turned out to be a good place
to find a random number.
```
if(addressnumber >= 1) {
		*len = pages;
		int bestaddress = addresslist[rando % addressnumber];
		return bestaddress;
```
Next Fit
-----
The next fit algorithm does not have its own function.  The original `findbit` 
function (best fit) has its start and end points passed to it as parameters, and the 
\only difference between first fit and next fit is in the start points.  
With this in mind we decided to call the first fit algorithm with the address of 
the last allocated page as its starting point, rather than having it start at 
the beginning every time, if the memory policy is set to next fit.
