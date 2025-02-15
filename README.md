# TSP
use genetic algorithm to solve travelling salesman problem problem

test file `test-12.txt`: shortest distance is `47.6887`
when the tour order is `[2 6 4 1 11 10 9 0 5 7 3 8]` or any rotation thereof

`vlsi-131.txt`: shortest distance is `564`

more test data available at https://www.math.uwaterloo.ca/tsp/data/index.html

## `mutate` operation:
mutates the genome by swapping two randomly selected values in the order vector

## `crosslink` operation:
given two genomes g1 & g2, produce their offspring g3 by:
1. choosing a random index `i` and setting the first `i` entries of g3 to be the first `i` entries of g1; and
2. iterating through g2 and appending any entry not present in g3 after step 1 to g3.

## parameters
- argv[2]: `pop` is a positive integer specifying the population size
- argv[3]: `gen` is a positive integer specifying how many generations to run the GA for
- argv[4]: `keep` is a FP value in the range 0-1 that specifies the percentage of the population that should be preserved from generation to generation. For example, 0.3 would specify that the top 30% of the population should be preserved to the next generation.
- argv[5]: `mut` is a nonnegative FP value that specifies how many mutations to apply to each member of the population, on average. For example, a value of 1.5 specifies that each member will have about 1.5 mutations applied. The total number of mutations to apply to the population is simply `mut * pop` at the end of each generation. (*Note: Do not mutate the genome at index 0, since it is the best solution found so far.)
