# 2020SpringCS572Proj

* student: Li-Chi Chang
* professor: Peter, Ng

## Algo policy

### travel map

#### DFS

1. If root is 3-way(including entry) node, push root into stact, or go to next 3 way node.
2. Pop a node from stack, if the stack is empty, return.
3. Go to this node, and find new way to go.
4. If we find new nodes, push them into stack. and repeat step.2
