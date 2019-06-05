#include <vector>
#include "dsets.h"

/**
 * creates n unconnected root nodes(their values are -1)
 * at the end of the vector
 */
void DisjointSets::addelements(int num)
{
    //using while loop to achieve it
    int count = 0;
    while(count <num)
    {
        vec.push_back(-1);
        count++;
    }
}

/**
 * Find the root of the given element and also compress paths(smart union)
 * @param elem The element whose root is to be found
 * @return The index of the root of the up-tree in which the parameter element resides
 */
int DisjointSets::find(int elem)
{
    int index = 0;//the index of the root

    //if the element has no root to point
    //return itself
    if (vec[elem] < 0) index = elem;

    //else, recursively find the root of the given elem
    else
    {
        index = find( vec[elem] );
        vec[elem] = index;
    }
    return index;
}

/**
 * Combine the trees of the given two unions by using union-by-size.
 * I assume the given two arguments are not the roots of the trees
 * (e.g we need to use the find function to find the roots first).
 * if the sizes of the two unions are the same, we make the tree containing
 * the second argument point to the first tree(just for my convenient)
 * @param a Index of the first element to union
 * @param b Index of the second element to union
 */
void DisjointSets::setunion(int a, int b)
{
    //find the roots of the given two union
    a = find(a);
    b = find(b);

    //if they are in the same union, don't need to combine
    if(a == b) return;

    //combine the size
    //notice that: we take negative value of their size
    int newSize = vec[a]+vec[b];

    //As we mention above, we attach the smaller set to the bigger one
    //if their sizes are the same, we make the second argument point to the first one
    if ( size(a) >= size(b))
    {
        vec[b] = a;
        vec[a] = newSize;
    }
    else
    {
        vec[a] = b;
        vec[b] = newSize;
    }
}

/**
 * return the size of the up-tree containing elem
 * @param elem the given elem of the union
 * @return the size of the up-tree containing elem
 */
int DisjointSets::size(int elem)
{
    return -(vec[find(elem)]); //notice that we take the negative value
}
