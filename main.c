#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>




//Constraints
#define Goal 5000
#define Num_Rats 20
#define Initial_Min_Wt 200
#define Initial_Max_Wt 600
#define Initial_Mode_Wt 300
#define Mutate_Odds 0.01
#define Mutate_Min 0.1
#define Mutate_Max 1.25
#define Litter_Size 16
#define Litters_Per_Year 10
#define Generation_Limit 500

//triangular distribution
int rand_triangular(int min, int max, int mode) {
    int range = max - min + 1;
    int base = rand() % range + min;
    int offset = mode - min;

    if (base <= offset) {
        return min + (int)sqrt(base * offset);
    } else {
        return max - (int)sqrt((range - base) * (range - offset));
    }
}
//probability using triangular distribution
int prob_triangular(int min, int max, int mode, int value) {
    
    
}


//population
int* populate(int num_Rats, int initial_Min_Wt, int initial_Max_Wt, int initial_Mode_Wt) {
    // Dynamically allocate memory for the array of weights
    int* rats = malloc(num_Rats * sizeof(int));
    if (rats == NULL) {
        // Handle memory allocation failure
        printf("Memory allocation failed.\n");
        return NULL;
    }

    // Populate the array with random weights
    for (int i = 0; i < num_Rats; i++) {
        rats[i] = rand_triangular(initial_Min_Wt, initial_Max_Wt, initial_Mode_Wt);
    }

    // Return the array
    return rats;
}


//mean
double mean(int num_Rats, int* rats) {
    int sum = 0;
    for (int i = 0; i < num_Rats; i++) {
        sum += rats[i];
    }
    return (double)sum / num_Rats;
}

//fitness
double fitness(int goal, int* rats) {
    double average = mean(Num_Rats, rats);
    printf("Average: %f\n", average);
    double ratio = average / goal;
    return ratio;
}
//compare function
int cmpfunc (const void * a, const void * b) {
   return (*(int *)b - *(int *)a);
}
//select the best rats
int* select(int *rats, int num_rats, int retain) {
    // Sort the array of weights in  order
    qsort(rats, num_rats, sizeof(int), cmpfunc);

    /*/print the sorted array
    printf("Sorted array: ");
    for (int i = 0; i < num_rats; i++) {
        printf("%d ", rats[i]);
    }
    */

    // Allocate memory for the selected rats
    int *selected_rats = malloc(retain * sizeof(int));
    if (selected_rats == NULL) {
        printf("Memory allocation failed.\n");
        return NULL;
    }

    // Copy the largest rats to the new array
    for (int i = 0; i < retain; i++) {
        selected_rats[i] = rats[i];
    }

    return selected_rats;
}



//breed
int* breed(int num_Rats2, int* rats, int litter_Size) {
    // Allocate memory for the entire population size plus the newly bred rats. amount = num_Rats + (litter_Size*(num_rats/2))
    int* new_Rats = malloc((num_Rats2 + litter_Size * num_Rats2 / 2) * sizeof(int));
    if (new_Rats == NULL) {
        // Handle memory allocation failure
        printf("Memory allocation failed.\n");
        return NULL;
    }
    
    // Copy the existing rats to the new array
    memcpy(new_Rats, rats, num_Rats2 * sizeof(int));

    //pair the parents in a 2d array
    int parents[num_Rats2][2];
    for (int i = 0; i < num_Rats2; i++) {
        parents[i][0] = rats[i];
        parents[i][1] = rats[i + 1];
    }

    // Breed the rats
    for (int i = 0; i < num_Rats2; i++) {
        for (int j = 0; j < litter_Size; j++) {
            int parent1 = i;
            int parent2 = i + 1;
            if (parent2 == num_Rats2) {
                parent2 = 0;
            }
            int child = (parents[parent1][rand() % 2] + parents[parent2][rand() % 2]) / 2;
            new_Rats[num_Rats2 + (i * litter_Size) + j] = child;
        }
    }
    
    
    // Return the new array
    return new_Rats;
}



//mutate
int* mutate(int* children, double mutate_odds, double mutate_min, double mutate_max) {
    for (int i = 0; i < Num_Rats; i++) {
        if ((rand() % 100) < mutate_odds * 100) {
            double mutation = (rand() % (int)((mutate_max - mutate_min) * 100) + mutate_min * 100) / 100;
            children[i] = (int)(children[i] * mutation);
        }
    }
   
    return children;
}



//main
int main()
{
    int generation = 0;

    int* parents = populate(Num_Rats, Initial_Min_Wt, Initial_Max_Wt, Initial_Mode_Wt);

    if (parents == NULL) {
        return 1; // Exit if memory allocation failed
    }

    // Print the weights of the rats and the population number
    printf("Weights of population:\n");
    //use fitness function to check the fitness of the population
    double fit = fitness(Goal, parents);
    //check fitness
    printf("Fitness: %f\n", fit);

    // Select the best rats
    int* selected_parents = select(parents, Num_Rats, Num_Rats / 2);

    if (selected_parents == NULL) {
        // Handle memory allocation failure
        return 1;
    }

    // Print the weights of the new rats
    printf("Weights of selected parents:\n");
    for (int i = 0; i < Num_Rats / 2; i++) {
        printf("%d\n", selected_parents[i]);
    }

    // Breed the new rats
    int* children = breed(Num_Rats, selected_parents, Litter_Size);
    if (children == NULL) {
        // Handle memory allocation failure
        return 1;
    }
    //mutate the children
    children = mutate(children, Mutate_Odds, Mutate_Min, Mutate_Max);
    //check fitness
    double fit2 = fitness(Goal, children);
    //print the fitness
    printf("Fitness: %f\n", fit2);
    //Show all the weights of the children
    printf("Weights of children:\n");
    for (int i = 0; i < Num_Rats + (Litter_Size * (Num_Rats / 2)); i++) {
        printf("%d\n", children[i]);
    }


    // Breed the new rats in generations
    /*while (generation < 1) {

        //print the generation number
        printf("Generation: %d\n", generation);

        // Breed the new rats
        int* children = breed(Num_Rats / 2, selected_parents, Litter_Size);
        if (children == NULL) {
            // Handle memory allocation failure
            return 1;
        }
        //mutate the children
        children = mutate(children, Mutate_Odds, Mutate_Min, Mutate_Max);
        //check fitness
        double fit2 = fitness(Goal, children);
        //print the fitness
        printf("Fitness: %f\n", fit2);
        // Select the best rats
        int* selected_children = select(children, Num_Rats, Num_Rats / 2);
        if (selected_children == NULL) {
            // Handle memory allocation failure
            return 1;
        }
        // Print the weights of the new rats
        printf("Weights of selected children:\n");
        for (int i = 0; i < Num_Rats / 2; i++) {
            printf("%d\n", selected_children[i]);
        }
        // Free the dynamically allocated memory for the array
        free(selected_parents);
        selected_parents = selected_children;
        generation++;
    }






    /*
    int* rats=populate(Num_Rats, Initial_Min_Wt, Initial_Max_Wt, Initial_Mode_Wt);
   
    if (rats == NULL) {
        return 1; // Exit if memory allocation failed
    }

    // Print the weights of the rats and the population number
    printf("Weights of rats:\n");
    int i;
    int count = 0;

    for ( i = 0; i < Num_Rats; i++) {
        printf("%d\n", rats[i]);
        count++;
    }
    printf("Population: %d\n", count);
    // fitness
    double fit = fitness(Goal, rats);
    //check fitness
    printf("Fitness: %f\n", fit);

    // Select the best rats
    int* new_Rats = select(rats,count, Num_Rats);

    if (new_Rats == NULL) {
        // Handle memory allocation failure
        return 1;
    }

    // Print the weights of the new rats
    printf("Weights of new rats:\n");
    for (int i = 0; i < Litter_Size; i++) {
        printf("%d\n", new_Rats[i]);
    }

    // Breed the new rats
    int *new_Rats2 = breed(Num_Rats, new_Rats, Litter_Size);
    if (new_Rats2 == NULL) {
        // Handle memory allocation failure
        return 1;
    }

    // Print the weights of the new rats and the population size
    printf("Weights of new rats:\n");
    int count2 = 0;
    int lim =Num_Rats + (Litter_Size * (Num_Rats / 2));
    for (int i = 0; i < lim; i++) {
        printf("%d\n", new_Rats2[i]);
        count2++;
    }
    printf("Population: %d\n", count2);

    //cull the population
    int* new_Rats3 = select(new_Rats2, count2, Num_Rats);

    if (new_Rats3 == NULL) {
        // Handle memory allocation failure
        return 1;
    }

    // Print the weights of the new rats
    printf("Weights of new rats:\n");
    for (int i = 0; i < Litter_Size; i++) {
        printf("%d\n", new_Rats3[i]);
    }

    //fitness of new rats
    double fit2 = fitness(Goal, new_Rats3);
    //check fitness
    printf("Fitness: %f\n", fit2);



    // Free the dynamically allocated memory for the array
    free(rats);
    free(new_Rats);

    */




}


