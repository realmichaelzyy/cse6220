#CSE 6220 List Ranking -- OMP
<!-- #CSE 6220 List Ranking -- Cilk Plus -->

In this lab, you will implement Wyllie's list ranking algorithm as discussed in the videos, plus another algorithm due to Helman and Jaja.  The efficiency of your code will be compared to a reference implementation to help determine your grade.

## Getting Started
Begin by obtaining the starter code from the github repository.

<pre><code>
	git clone https://github.gatech.edu/sb300/oms-hpc-lab1.git
</code></pre>

Note that this is the [GT github server](https:github.gatech.edu), so you will need to authenticate with your GT credentials.

Optionally, you may choose use a git hosting service for your code.  As always, please **do not make your repository public** on Github or another git hosting service.  Anyone will be able to see it.  If you feel the need to use a hosting service for your project, please keep your repository private.  Your GT account allows you to create free private repositories on [the GT github server](https:github.gatech.edu).


## Programming
You should only modify the function contents in listrank_wyllie.c and listrank_hj.c. Do not modify any other files, and do not modify the function signature. You may add helper functions to be used inside listrank_wyllie.c and listrank_hj.c.
<!--
First implement the parallel list ranking algorithms using Cilk Plus, an extension of C++ and C for shared memory parallel applications. Cilk Plus is available on Intel compilers and GCC 5.2.0. There is a wealth of documentation about Cilk Plus online.  Here are a few resources to help you get started:

*  [cilkplus.org](https://www.cilkplus.org/)
*  [Wikipedia](https://en.wikipedia.org/wiki/Cilk)
*  [Cilk Plus Tutorial](https://www.cilkplus.org/cilk-plus-tutorial)

You should allow Cilk Plus to decide how many threads are available rather than choosing for yourself.  Thus, for example, using 'cilk_for' should not entail tuning the number of iterations.
-->
First implement the parallel list ranking algorithms using OpenMP, an API for shared memory parallel applications.  (Intel's Cilk is a popular competitor in this category.)  There is a wealth of documentation about OpenMP online.  Here are a few resources to help you get started

*  [LLNL](https://computing.llnl.gov/tutorials/openMP/)
*  [Wikipedia](https://en.wikipedia.org/wiki/OpenMP)
*  [OpenMP.org](http://openmp.org/wp/resources/) (Check out the "Learn" heading on the right".)

You should allow OpenMP to decide how many threads are available rather than choosing for yourself.  Thus, a simple

<pre><code>
	#pragma omp parallel
</code></pre>

directive will be sufficient for the purposes of this lab.

Your first task is to implement Wyllie's algorithm, which is described in the lesson videos, in the file listrank_wyllie.c.  

Next, you are to implement the Helman-Jaja algorithm for list ranking in the listrank_hj.c file.  Your should be able to use your GT credentials to access the [the original paper](http://link.springer.com.prx.library.gatech.edu/chapter/10.1007/3-540-48518-X_3) via the GT library, though you may find that the summary below is sufficient.

1. Sequentially, partition the input list into *s* sublists, by randomly choosing *s* sublist head nodes.
2. In parallel, traverse each sublist computing the list ranking of each node within the sublists.
3. Sequentially, compute the list ranking of the head nodes overall (by doing a prefix sum of the head node ranks).
4. In parallel, traverse the sublists again to convert the sublist ranking to the complete list ranking (by adding the previously determined prefix sum values).


### Compilation
In addition to the starter code discussed above, the repository also contains a number of other files to help you test and measure the performance of your code.  The file correctness.c contains a simple test to see if your code is correct.  By default, the Makefile will compile the file listrank_wyllie.c.  To have it compile listrank_jk.c instead, use the command

<pre><code>
	make correctness IMPL=hj
</code></pre>

Note that the value for the variable IMPL given on the command line overrides the one given in the Makefile.

### Performance Testing
You should measure the performance of your code on the Deepthought cluster. The main file metrics.cc performs some the same tests on which your code will be evaluated.  Use the IMPL command line parameter to change which implementation of the parallelListRanks is compiled.  Thus, if you wanted to time your Helman-Jaja algorithm, you might implement it in a file listrank_hj.c and compile it with

<pre><code>
	make metrics IMPL=hj
</code></pre>

The file timing_hj.sub serves as an example for how to use sbatch for the Helman-Jaja algorithm.  Thus, to measure the performance of you code you should run

<pre><code>
	sbatch timing_hj.sub
</code></pre>

from your Deepthought login node.

####Sample Performance Values

The following speedup figures are based on median values of timings on Deepthought. With the Wyllie algorithm, you should expect speedups of around 0.2 or greater (slowdowns) for array sizes of 1e6 and 1e7. With the Helman-Jaja algorithm, you can obtain speedups greater than 5 for array sizes of 1e6 and 1e7.

###Submitting Your Code
Once you have completed and tested your implementations, please submit them to the [Udacity site](https://www.udacity.com/course/viewer#!/c-ud281/l-4329489524/m-4616289189), which will do a quick correctness test.  You may submit as many times as you like before the deadline.  At the deadline, the TA will download the code and perform some timing runs.  These results along with a manual inspection of the code will determine your grade.

