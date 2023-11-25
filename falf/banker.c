#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

int count_numbers_in_line(char *line);
int count_lines(FILE *file);
int count_whitespace(FILE *file);
void allocate_resources(int customer, int request[]);
void release_resources(int customer, int release[]);
int run_bankers_algorithm(int count_resources, int customer, int *available, int **need, int **alloc);



int main(int argc, char *argv[]) {
    char line[1024];
    int count_resources = 0;
    int *avaliable = (int *)malloc(sizeof(int) * sizeof(int));
    for(int i = 0; i < argc - 1; i++){
        avaliable[i] = atoi(argv[i+1]);
    }


    FILE *customer_file = fopen("customer.txt", "r");
    if (customer_file == NULL) {
        fprintf(stderr, "Fail to read customer.txt\n");
        exit(EXIT_FAILURE);
    }

    
    FILE *commands_file = fopen("commands.txt", "r");
    if (commands_file == NULL) {
        fprintf(stderr, "Fail to read commands.txt\n");
        fclose(customer_file);
        exit(EXIT_FAILURE);
    }

    while(fgets(line, 1024, customer_file) != NULL) {
        count_resources = count_numbers_in_line(line);
        
    }

    if(argc-1 != count_resources){
        fprintf(stderr, "Incompatibility between customer.txt and command line\n");
        exit(EXIT_FAILURE);

    }

    for (int i = 0; i < argc - 1; i++) {
        avaliable[i] = atoi(argv[i+1]);
    }

    rewind(customer_file);
    int customer = count_lines(customer_file);

    int whitespace = count_whitespace(commands_file);

    if(argc-1 != (whitespace - 1) ){
        fprintf(stderr, "Incompatibility between commands.txt and command line\n");
        exit(EXIT_FAILURE);
    }
    
    rewind(customer_file);


    int **customer_resources = (int **)malloc(sizeof(int *) * customer);
    for (int i = 0; i < customer; i++) {
        customer_resources[i] = (int *)malloc(sizeof(int) * count_resources);
    }

    int current_customer = 0;
    while (fgets(line, 1024, customer_file) != NULL && current_customer < customer) {
        char *token = strtok(line, ",");
        int current_resource = 0;

        while (token != NULL && current_resource < count_resources) {
            customer_resources[current_customer][current_resource] = atoi(token);
            token = strtok(NULL, ",");
            current_resource++;
        }

        current_customer++;
    }

    rewind(commands_file);

    int **alloc = (int **)calloc(customer, sizeof(int*));
    for(int i = 0; i < customer; i++){
        alloc[i] = (int *)calloc(count_resources, sizeof(int));
    }


    int **need = (int **)calloc(customer, sizeof(int*));
    for(int i = 0; i < customer; i++){
        need[i] = (int *)calloc(count_resources, sizeof(int));
    }

    for(int i = 0; i < customer; i++){
        for(int j = 0; j < count_resources; j++){
            need[i][j] = customer_resources[i][j] - alloc[i][j];
        }
    }

    FILE *result = fopen("result.txt", "w");
    
    while(fgets(line, 1024, commands_file) != NULL) {
        int customer_number;
        int *request = (int *)malloc(sizeof(int) * count_resources);
        int *release = (int *)malloc(sizeof(int) * count_resources);
        char line_copy[1024];
        strcpy(line_copy, line);
        if (strstr(line, "RQ") != NULL) {
            
            int flag = 0;
            sscanf(line, "RQ %d", &customer_number);

            char *token = strtok(line_copy, " ");
            token = strtok(NULL, " ");
            token = strtok(NULL, " ");
            int count = 0;
            while (token != NULL) {
                request[count] = atoi(token);
                token = strtok(NULL, " ");
                count++;
            }

            

            char line_copy[1024];
            strcpy(line_copy, line);

            for (int i = 0; i < count_resources; i++) {

                if (request[i] > customer_resources[customer_number][i]) {
                    fprintf(result, "The customer %d request ", customer_number);
                    for(int i = 0; i < count_resources; i++){
                        fprintf(result, "%d ", request[i]);
                    }


                    fprintf(result, "was denied because exceed its maximum need\n");
                    flag = 1;
                    break;
                    }
            }
            if(flag == 1){
                continue;
            }


            for(int i = 0; i < count_resources; i++){
                if(request[i] > avaliable[i]){
                    fprintf(result, "The resources ");
                    for(int i = 0; i < count_resources; i++){
                        fprintf(result, "%d ", avaliable[i]);
                    }

                    fprintf(result, "are not enough to customer %d request ", customer_number);
                    for(int i = 0; i < count_resources; i++){
                        fprintf(result, "%d ", request[i]);
                    }
                    fprintf(result, "\n");
                    flag = 1;
                    break;
                    }
            }
            if(flag == 1){
                continue;
            }
            for(int i = 0; i < count_resources; i++){
                avaliable[i] -= request[i];
                alloc[customer_number][i] += request[i];
                need[customer_number][i] = customer_resources[customer_number][i] - alloc[customer_number][i];
            }



            if(run_bankers_algorithm(count_resources, customer, avaliable, need, alloc) == 0){

                fprintf(result, "Allocate to customer %d the resources ", customer_number);
                for(int i = 0; i < count_resources; i++){
                    fprintf(result, "%d ", request[i]);
                }
                fprintf(result, "\n");
            }else{
                for(int i = 0; i < count_resources; i++){
                    avaliable[i] += request[i];
                    alloc[customer_number][i] -= request[i];
                    need[customer_number][i] = customer_resources[customer_number][i] - alloc[customer_number][i];
                }
                fprintf(result, "The customer %d request ", customer_number);
                for(int i = 0; i < count_resources; i++){
                    fprintf(result, "%d ", request[i]);
                }

                fprintf(result, "was denied because result in an unsafe state\n");
            }


        } else if (strstr(line, "RL") != NULL) {

            int flag = 0;
            int customer_number;
            sscanf(line, "RL %d", &customer_number);


            char *token = strtok(line_copy, " ");
            token = strtok(NULL, " ");
            token = strtok(NULL, " ");
            int count = 0;
            while (token != NULL) {
                release[count] = atoi(token);
                token = strtok(NULL, " ");
                count++;
            }
            for(int i = 0; i < count_resources; i++){
                if(release[i] > alloc[customer_number][i]){
                    fprintf(result, "The customer %d released", customer_number);
                    for(int i = 0; i < count_resources; i++){
                        fprintf(result, " %d", release[i]);
                    }
                    fprintf(result, " was denied because exceed its maximum allocation\n");
                    flag = 1;
                    break;
                }

            }
            if(flag == 0){
                fprintf(result, "Release from customer %d the resources ", customer_number);
                for(int i = 0; i < count_resources; i++){
                    fprintf(result, "%d ", release[i]);
                }
                fprintf(result, "\n");
                for(int i = 0; i < count_resources; i++){
                    avaliable[i] += release[i]; 
                    alloc[customer_number][i] -= release[i];
                    need[customer_number][i] = customer_resources[customer_number][i] - alloc[customer_number][i];
                }
            }


        } else if (strstr(line, "*") != NULL) {
            //print result file
            //MAXIMUM | ALLOCATION | NEED
            //7 5 3   | 0 1 0      | 7 4 3
            //3 2 2   | 2 0 0      | 1 2 2
            //9 0 2   | 3 0 2      | 6 0 0
            //2 2 2   | 2 1 1      | 0 1 1
            //4 3 3   | 0 0 2      | 4 3 1

            fprintf(result, "MAXIMUM | ALLOCATION | NEED\n");
            for(int i = 0; i < customer; i++){
                for(int j = 0; j < count_resources; j++){
                    fprintf(result, "%d ", customer_resources[i][j]);
                }
                fprintf(result, "  | ");
                for(int j = 0; j < count_resources; j++){
                    fprintf(result, "%d ", alloc[i][j]);
                }
                fprintf(result, "     | ");
                for(int j = 0; j < count_resources; j++){
                    fprintf(result, "%d ", customer_resources[i][j] - alloc[i][j]);
                }
                fprintf(result, "\n");
            }
            fprintf(result,"AVAILABLE ");
            for(int i = 0; i < count_resources; i++){
                fprintf(result, "%d ", avaliable[i]);
            }
            fprintf(result, "\n");
        } 
        free(request);
        free(release);

    }



    for(int i = 0; i < customer; i++){
        free(customer_resources[i]);
    }
    free(customer_resources);
    free(avaliable);
    for(int i = 0; i < customer; i++){
        free(alloc[i]);
    }
    free(alloc);
    for(int i = 0; i < customer; i++){
        free(need[i]);
    }
    free(need);



    fclose(customer_file);
    fclose(commands_file);
    fclose(result);

    return 0;
}





int count_numbers_in_line(char *line) {
    int count = 0;
    char *token = strtok(line, ",");
    
    while (token != NULL) {
        count++;
        token = strtok(NULL, ",");
    }
    
    return count;
}

int count_lines(FILE *file) {
    int count = 0;
    char ch;

    while ((ch = fgetc(file)) != EOF) {
        if (ch == '\n') {
            count++;
        }
    }

    if (count > 0 && ch != '\n') {
        count++;
    }

    return count;
}

int count_whitespace(FILE *file) {
    int count = 0;
    char ch;

    while ((ch = fgetc(file)) != '\n' && ch != EOF) {
        if (ch == ' ' || ch == '\t') {
            count++;
        }
    }

    return count;
}







int run_bankers_algorithm(int count_resources, int customer, int *available, int **need, int **alloc){
    int work[count_resources];
    int finish[customer];

    for(int i = 0; i < customer; i++){
        finish[i] = 0;
    }


    for(int i = 0; i < count_resources; i++){
        work[i] = available[i];
    }


    int sequence_count = 0;  


    while(sequence_count < customer){
        int current_customer = -1;


        for(int i = 0; i < customer; i++){

            if(finish[i] == 0){
                int j;
                for(j = 0; j < count_resources; j++){
                    if(need[i][j] > work[j]){
                        break;
                    }
                }


                if(j == count_resources){
                    for(j = 0; j < count_resources; j++){
                        work[j] += alloc[i][j];
                    }


                    finish[i] = 1;

  
                    sequence_count++;

                    current_customer = i;
                }
            }
        }


        if(current_customer == -1){
            break;
        }
    }


    if(sequence_count == customer){
        return 0;  
    } else {
        return 1; 
    }
}
