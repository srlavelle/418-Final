# Title: Numbrix Solver
Samantha Lavelle

Kavya Tummalapalli

## Summary


## Background


## The Challenge


## Resources


## Goals and Deliverables
100%

Achieve an accurate, working Numbrix solver with 8x speedup for a 64 (8 x 8) grid. This speedup is chosen as a benchmark because it is substantial, and we feel that we can attain it with thorough parallelization. The puzzle, in theory, could be parallelized over every square in the grid; what will prevent this is the existence of false paths during solving. Thus, 8x seems a reasonable speedup. An 8 x 8 grid is used as our benchmark because it is the standard size of the game; we plan to work up to this board size after we get a solution working with a smaller board. 
Our program will additionally provide a visual reprentation of the input and output boards. This will be printed in the calling terminal with the addition of the -v flag.
To prove our solution is accurate, we will also create a checker program that ensures the board produced is a valid solution.

125%

If we have more time, we will implement a solution that works not only for square/rectangular boards, but also irregularly shaped boards. These boards provide a higher level of complexity when solving, likely adding time to our routing algorithm.
It would also be cool to produce a visual representation of the input and output boards that is more stylized, with different colors for numbers that were originally on the board versus those that our program added. This can be done with a java or python script, like WireGrapher from assignment 3.

75%

In the event that we are not able to meet all of our goals, we would at least like to have a fully working solution that attains 4x speedup. The solution must still be accurate, as proven by our checker program.

Deliverables

At the poster section, we will show our program solving 8x8 grids in real time, displaying the visual representations of the input and output and checking with our script. We will display speedup graphs for different board sizes and numbers of threads/cores/processors, and justify these results. We also can bring printed versions of the puzzle so students can try it out for themselves to better understand the work our program is doing!

## Platform Choice
We plan to code our program in C++, as we have throughout this course. Java and Python will be used for checking/visualization scripts, as needed. 
As we parallelize our code, we'd like the method of parallelization to allow us certain things. We want each thread to have a private memory, but the ability to sync with others; each thread to be able to launch and die independently to account for the discovery (and possible disproving) of possible paths. This makes a fork-join model a good choice. We plan to look into the fork-join primitives in OpenMP, as well as pthreads.

## Schedule
Week of 3/21: Generate project ideas, and choose one. Create and submit project proposal. Assignment 4 is due this week, so most time will be dedicated to completing it.

Week of 3/28: Create and code sequential algorithm. Create checker script and use to check algorithm's accuracy, and print a rough visual representation to the terminal to visually check the results. 

Week of 4/4: Research which method of parallelization would best suit the program, and think about what barriers/calls will be needed. Begin to implement parallelization, monitoring speedup tradeoffs and bottlenecks. Prepare project checkpoint, due the following Monday.

Week of 4/11: Turn in project checkpoint, due on 4/11. Coninue parallelization. Exam 2 is this week, so time will be dedicated to taking and studying for the exam.

Week of 4/18: Finish parallelization, and tweak to achieve optimal speedup. Compare effects of synchronization, load balancing, etc. Determine bottleneck.

Week of 4/25: Prepare project report, due on Friday 4/29. Explore any last avenues of parallelization, and hopefully get to 125% goals.

Week of 5/2: Parallelism Competition is on Thursday, May 5. Create and polish speedup graphs and visuals for the presentation, and rehearse what we will say. Any last-minute visual representations can be created in the beginning of the week.
