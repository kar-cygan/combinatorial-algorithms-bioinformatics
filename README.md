
# Combinatorial Algorithms in Bioinformatics

* [Adjoint Graph Transformation](#adjoint-graph-transformation)
* [Restriction Mapping](#restriction-mapping)
* [Motif Search](#motif-search)


## Adjoint Graph Transformation

This program implements an exact algorithm with polynomial complexity that implements the following:
- loading any directed graph from a text file
- checking whether the loaded graph is an adjoint graph (*G*)
- if the graph is an adjoint graph, checking if it's a line graph
- if the graph is an adjoint graph, transforming it into its original graph (*H*)
- saving the resulting graph *H* to a text file

### Input

In the text file, the graph is represented by a list of successors. The vertices are denoted by consecutive integers from *1* to *n*, where *n* equals the number of vertices of the graph. Each vertex of the graph is represented by two lines, the first of which contains the label of the vertex, and the second contains the labels (numbers) of the direct successors of that vertex separated by a space character and arranged in ascending order. For a vertex whose set of direct successors is empty, the second line in the file contains only the number *0*.


### Example

Transforming graph *G1* into *H1*:

```
Graph G1
3 -> 10
2 -> 1, 2, 11
3 -> 4, 6
4 -> 3, 10
5 -> 7
6 -> 7
7 -> 8
8 -> 9
9 -> 3, 10
10 -> 1, 2, 11
11 -> 12
12 -> 0

Graph H2
1   7 -> 6
2   7 -> 7
3   6 -> 1
4   1 -> 6
5   2 -> 3
6   1 -> 3
7   3 -> 4
8   4 -> 5
9   5 -> 6
10  6 -> 7
11  7 -> 8
12  8 -> 9
```

![Example graph_g1](./adjoint-graph-transform/img/graph_g1.png | width=200))
![Example graph_h1](./adjoint-graph-transform/img/graph_h1.png | width=200))

***

## Restriction Mapping

The partial digest mapping problem is based on an experiment involving a single restriction enzyme. This enzyme cleaves copies of a DNA fragment at matching restriction sites but at varying time intervals. In the experiment's theoretical framework, the duration of enzyme digestion is modified in consecutive reactions to guarantee that the DNA fragment is cut by the enzyme in one, two, and subsequently all the way up to every restriction site present in that fragment. Consequently, a multiset A is generated, comprising the lengths of segments obtained from all these reactions, along with the overall length of the fragment.

This program implements an exact algorithm with exponential complexity to reconstruct the pattern (map) of restriction sites in the investigated fragment based on the outcomes of this experiment, implementing the following steps:
- loading from a file an instance of a partial digest mapping problem (multiset A)
- constructing a restrictive map compatible with the given multiset (one of the possible maps)
- outputting the result

### Input

A text file containing one line with a sequence of natural numbers separated by spaces, representing the multiset A.


### Example

`multisetA = {2, 3, 3, 4, 6, 6, 7, 8, 9, 9, 11, 12, 15, 15, 18}`  
`map = (3, 4, 2, 6, 3)`

***

## Motif Search

This program searches in a group of nucleotide sequences for a common structural motif of the length provided by the user. The sequences are passed in a single FASTA file along with a QUAL file containing the qualities of the individual nucleotides in the sequences. Nucleotides with qualities lower than the threshold passed by the user are not considered in the search. 

In this program, a heuristic algorithm with polynomial complexity is implemented that performs the following:
- loading instances
- removing from the sequences nucleotides with quality below the threshold set by the user
- creating a graph with vertices corresponding to all subsequences of the sequence with length set by the user
- searching the graph for a clique-like structure in which each input sequence will be represented by exactly one vertex
- printing the results

### Input
The input instances correspond to the results from the sequencer and consist of two files: 
- `*.fasta` - containing nucleotide sequences with identifiers 
- `*.qual` - containing nucleotide sequence quality scores along with the identifiers of these sequences


### Example

*&ast;.fasta*  
`>DOJHLOP01AN4EN length=100 xy=0157_2493 region=1 run=R_2005_09_08_15_35_38_`
`TTTTCTCCTCGAATTTGAAAAACCTCTTGTTGAATTAGAGAATCAGATTGATCAAATCAG
AGAATTAGCAAGAGAGTTGTACGCCTGGTGAAGTCAGCAA`

*&ast;.qual*  
`>DOJHLOP01AN4EN length=100 xy=0157_2493 region=1 run=R_2005_09_08_15_35_38_`
`24 24 22 12 30 26 31 28 32 28 32 29 24 28 27 19 32 22 22 21 16 7 30 27 26 31 29 26 32 31 27 31 30 25 31 28 28 32 29 32 31 27 31 29 31 29 32 29 26 27 32 31 30 25 25 18 32 27 29 30 29 32 30 27 30 27 28 31 31 31 28 24 31 30 31 31 27 31 29 21 26 20 30 31 17 31 30 29 22 31 31 27 31 29 31 31 24 25 31 27`

Result:  
`Motif: GTTGTACGCCTGGTG`  

***

## Acknowledgements

Programs implemented as part of the course "Combinatorial Algorithms in Bioinformatics".
