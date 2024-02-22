#include <stdio.h>

void merge(int arr[], int l, int m, int r)
{
    int n1 = m+1-l;
    int n2 = r-m;

    int i = 0, j = 0, k = 0;
    int L[n1], R[n2];

    for (i = 0; i < n1; i++)
    {
       L[i] = arr[l+i]; 
    }
    for (j = 0; j < n2; j++)
    {
        R[j] = arr[m+1+j];
    }

    i = 0, j = 0, k = l;

    while (i < n1 && j < n2)
    {
        if (L[i] <= R[j])
        {
            arr[k] = L[i];
            i++;
        }
        else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1)
    {
        arr[k] = L[i];
        i++; k++;
    }

    while (j < n1)
    {
        arr[k] = R[j];
        j++; k++;
    }
}
void merge_sort(int arr[], int l, int r)
{
    if (l >= r)
        return;
    int m = l+(r-l)/2;
    merge_sort(arr, l, m);
    merge_sort(arr, m+1, r);
    merge(arr, l, m, r);
}

int main()
{
    FILE *file = fopen("data.txt", "r");
    int n = 6;
    int array[n];
    int size = 0;
    while(fscanf(file, "%d", &array[size]) == 1) {
        size++;
    }
    

    for (int i = 0; i < size; i++) {
        printf("%d ", array[i]);
    }
    printf("\n");
    
    merge_sort(array, 0, n);   
    file = fopen("data.txt", "a");
    fprintf(file, "\n");
    fprintf(file, "Sorted Array: ");
    printf("Sorted Array: ");
    for (int i = 0; i < n; i++)
    {
        fprintf(file,"%d ", array[i]);
        printf("%d ", array[i]);
    }
    fclose(file);
}
