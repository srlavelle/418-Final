# Numbrix Solver
Samantha Lavelle

## Summary
We are going to implement and parallelize an algorithm to solve Numbrix puzzles. We will complete performance analyses on different optimizations of our algorithm on varying sizes and difficulties of puzzles. 

## Background
Numbrix is a grid based logic puzzle in which we are trying to create a consecutive path of numbers 1-n^2 (for an n x n grid) using only horizontal and vertical lines. Some numbers of the grid will already be given as clues to the overall path and the player must use these clues to fill in the rest of the grid.

Similar to games like Sudoku, some clues will need to be considered before others to lead to the correct overall solution or other times there may be no clue that gives away a number for sure, and in these cases we must proceed using assumptions. In the cases the player considers a wrong clue or assumption and runs into an unreachable or unfeasible solution, they have to backtrack to what parts of the grid are known to be 100% correct. This is also where parallelism can come into play as if there are many clues and possible paths, we can consider multiple solutions at once.

This game has three factors affecting difficulty and speed. The first is the size of grid. Larger grids have more numbers that must be placed and thus will generally take longer to solve than smaller puzzles. Second, the amount of revealed numbers plays a huge role in the difficulty as though each puzzle has a single solution, a 100 number path that has 50 numbers given will generally be easier for both a human and computer to solve than one with 10 numbers given.

## The Challenge
As this is a lesser known game than some other popular number and logic puzzles such as Sudoku, we will not have as many resources to work with. Though we will have help with our sequential version, we will have to think about what methods of parallelization will lead most effective and which parts of our algorithm will be most susceptible to parallelization.

In terms of the puzzle itself, this can scale to be very large in size as even a simple 10 x 10 grid can have thousands of arrangements of the numbers. This can become a problem as we are considering how we want to parallelize our algorithm as there could be too many paths and turns we must consider (think about having unlimited bends as opposed to the max 2 bends for the Assignment 3 and 4 wires).

Another aspect of our optimizations that we must consider is the data storing and communicating. We must consider how we are going to store the data we already know about the grid so that multiple threads or processes can work with this grid if they are all working on the same space. Otherwise, we must consider how to share data among threads and processes if we are working with independent areas of the grid and successfully fill in more numbers in a specific section of the grid.

A third possible challenge would be possible workload imbalance based on what our method of parallelization is. We must work to figure out what work or computations will be done in parallel and how we should divide up these computations to not repeatedly go after the same clues or to consider more plausible clues versus those with hundreds of possible paths.

## Resources
There is an existing sequential C++ Numbrix solver at https://rosettacode.org/wiki/Solve_a_Numbrix_puzzle that we will base our starting algorithm on. We haven't found any information or research about parallelizations of the Numbrix solver algorithm or any similar algorithms but we will continue looking for relevant resources to allow us to compare our algorithm and speedups with existing solvers or parallelism methods.

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
UPDATE: Completed all above tasks.

Week of 3/28: Create and code sequential algorithm. Create checker script and use to check algorithm's accuracy, and print a rough visual representation to the terminal to visually check the results. 
UPDATE: During this week, I did not know if Kavya was still an enrolled student in the class. I met with the professor, but no progress was made as there was no communication.

Week of 4/4: Research which method of parallelization would best suit the program, and think about what barriers/calls will be needed. Begin to implement parallelization, monitoring speedup tradeoffs and bottlenecks. Prepare project checkpoint, due the following Monday.
UPDATE: I assume Kavya has dropped based on her last message (and lack of communication after). I will look into other topics that I will be able to complete alone. Obviously very behind at this point.

Week of 4/11: Turn in project checkpoint, due on 4/11. Coninue parallelization. Exam 2 is this week, so time will be dedicated to taking and studying for the exam.
UPDATE: I literally got Covid, since there weren't enough hiccups in the schedule I guess. I was able to lock in Numbrix as the project topic going forward, and create the milestone report. I have a plan to start work, whenever I feel well enough.
I started looking into different methods of parallelization, and narrowed it down to OpenMP or using pthreads directly. OpenMP is simpler to implement, and I just used it for assignment 3, while pthreads might give some more control while launching new threads and rejoining them.

Week of 4/18: Finish parallelization, and tweak to achieve optimal speedup. Compare effects of synchronization, load balancing, etc. Determine bottleneck.
UPDATE: I studied the existing sequential solution so that I understand exactly how the algorithm works, and how to parallelize it. I had to research how classes are created and used in C++, as they appear a lot in the code, but I've only ever worked with classes in Python. I then created a Makefile for the file, and made sure everything runs as it should.

Week of 4/25: Prepare project report, due on Friday 4/29. Explore any last avenues of parallelization, and hopefully get to 125% goals.
UPDATE: I ran the code with different inputs, testing the accuracy visually. I'm not sure that I'll end up making a checker file since the boards are so easy to check with a quick visual inspection. I cleaned up the code, which had some weird... design choices. I then made it so that the unsolved board and solved board are printed with good formatting. I added timing code and more boards, testing the program with both larger and smaller boards. Even on a 9x9, the solver is really quick: only ~0.15 ms. The largest board I could find is 15x15, which did increase the computation time to ~0.5 ms, but I worry that the overhead of just adding OpenMP/pthreads will outweigh this, throwing off speedup values. Things did always run slower on PSC, so maybe that'll be to my advantage.

Week of 5/2: Parallelism Competition is on Thursday, May 5. Create and polish speedup graphs and visuals for the presentation, and rehearse what we will say. Any last-minute visual representations can be created in the beginning of the week.


## Milestone
This project hit a lot of hiccups in the beginning - not hearing anything from my partner, trying to get in touch with and get info from professors, and getting very sick for a week. At this point, I have a plan of how to proceed and am eager to do so once I feel well enough to do work. I will read and understand the sequential version of the Numbrix solver, and then parallelize it using threads. I have done some preliminary research and narrowed my options to using pthreads, or OpenMP or Cilk, which themselves use pthreads. I'm leaning towards using pthreads, but if the code seems like it will lend itself well to OpenMP, that may be a good option since I have experience with it.
Right now, my goal is simply to parallelize the code and note speedup, reasoning about any tradeoffs and displaying my results graphically. I'm really behind because of everything that has happened, so I know I need to focus on the basics and do more if I find myself with additional time at the end of the project. It is my hope and expectation that I will have a quality presentation for the poster session.
