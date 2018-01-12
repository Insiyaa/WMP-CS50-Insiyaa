/**
 * helpers.c
 *
 * Helper functions for Problem Set 3.
 */

#include <cs50.h>

#include "helpers.h"

/**
 * Returns true if value is in array of n values, else false.
 */

bool search(int value, int values[], int n)
{
    //immediately return false if n is negative...
    int mid;
    int left = 0;
    int right = n-1;
    if(n <= 0)
        return false;
    //O(logn)...that is implementing Binary search...
    while(left <= right)
	{
		mid = (left + right)/2;
    	if(value == values[mid])
    		return true;
    	if(value < values[mid])
    		right = mid - 1;
    	if(value > values[mid])
    		left = mid + 1;
    }
    return false;

}

/**
 * Sorts array of n values.
 */
void sort(int values[], int n)
{
    // TODO: implement a sorting algorithm...O(n*n)
    //bubble sort
    int i,j,temp;
    for(i = 1; i < n; i++) //starting from second element, considering the list of one element on left to be sorted
	{
		//taking the first unsorted element...
		temp = values[i]; //first unsorted element
		j = i - 1; //starting with element just before temp
		while(j >= 0 && values[j] > temp) //WHILE index is non-negative AND jth element is greater than temp, keep shifting!
		{
			values[j+1] = values[j];
			j--;
		}
		//when jth element is no longer larger than temp, shifting stops! and arr[j] becomes the first element smaller than temp
		values[j+1] = temp; //hence we place temp just after that element!
	}



    return;
}
