#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

int count_numbers_in_line(char *line);
int count_lines(FILE *file);
int count_whitespace(FILE *file);
void allocate_resources(int customer, int request[]);
void release_resources(int customer, int release[]);
void get_request(char *line, int **request); 
bool run_bankers_algorithm(int n, int m, int alloc[n][m], int max[n][m], int avail[m]);



int main(int argc, char *argv[]) {
    char line[1024];
    int count_resources = 0;
    int avaliable[argc-1];

    FILE *customer_file = fopen("customer.txt", "r");
    if (customer_file == NULL) {
        fprintf(stderr, "Fail to read customer.txt\n");
        exit(EXIT_FAILURE);
    }

    
    FILE *commands_file = fopen("commands.txt", "r");
    if (commands_file == NULL) {
        fprintf(stderr, "Fail to read commands.txt\n");
        exit(EXIT_FAILURE);
    }

    while(fgets(line, 1024, customer_file) != NULL) {
        count_resources = count_numbers_in_line(line);
        
    }
    printf("Customer resource: %d\n", count_resources);

    if(argc-1 != count_resources){
        fprintf(stderr, "Incompatibility between customer.txt and command line\n");
        exit(EXIT_FAILURE);

    }

    for (int i = 0; i < argc - 1; i++) {
        avaliable[i] = atoi(argv[i+1]);
    }
    rewind(customer_file);
    int customer = count_lines(customer_file);
    printf("Customer: %d\n", customer);
    int whitespace = count_whitespace(commands_file);
    printf("Commands avaliable: %d\n", whitespace-1);
    if(count_resources != (whitespace - 1) ){
        fprintf(stderr, "Incompatibility between customer.txt and commands.txt\n");
        exit(EXIT_FAILURE);
    }
    
    rewind(customer_file);
    int customer_resources[customer][count_resources];        
    int current_customer = 0;
    while (fgets(line, 1024, customer_file) != NULL) {
        char *token = strtok(line, ",");
        int current_resource = 0;

        while (token != NULL && current_resource < count_resources) {
            customer_resources[current_customer][current_resource] = atoi(token);
            token = strtok(NULL, ",");
            current_resource++;
        }

        current_customer++;
    }


    for (int i = 0; i < count_resources; i++) {
        printf("Resource %d: %d\n", i, avaliable[i]);
    }


    for (int i = 0; i < customer; i++) {
        printf("Customer %d avaliable:", i);
        for (int j = 0; j < count_resources; j++) {
            printf(" %d", customer_resources[i][j]);
        }
        printf("\n");
    }
    rewind(commands_file);




    int alloc[customer][count_resources];
    for(int i = 0; i < customer; i++){
        for(int j = 0; j < count_resources; j++){
            alloc[i][j] = 0;
        }
    }
    int customer_number;
    int count_line = 0;
    int count_line_release = 0;
    while(fgets(line, 1024, commands_file) != NULL) {
        int request[count_resources];
        int release[count_resources];
        if (strstr(line, "RQ") != NULL) {
            
            int flag = 0;

            get_request(line, request);
            
            sscanf(line, "RQ %d", &customer_number);

            char line_copy[1024];
            strcpy(line_copy, line);

            for (int i = 0; i < count_resources; i++) {

                if (request[i] > customer_resources[customer_number][i]) {
                    flag = 1;
                } else if (request[i] > avaliable[i]) {
                    flag = 2;
                }
            }


            if(flag == 0 && run_bankers_algorithm(customer, count_resources, alloc, customer_resources, avaliable) == false){
                flag = 3;
            }

            switch(flag){
                case 1:
                    printf("The customer %d request was denied because exceed its maximum need\n", customer_number);
                    break;
                case 2:
                    printf("The avaliable are not enough to customer %d request\n", customer_number);
                    break;
                case 3:
                    printf("The customer %d request was denied because result in an unsafe state\n", customer_number);
                    break;
                default:
                    printf("The customer %d request was accepted\n", customer_number);
                    for(int i = 0; i < count_resources; i++){
                        avaliable[i] -= request[i]; 
                        alloc[customer_number][i] += request[i];
                        printf("Request %d: %d\n", i, request[i]);
                        printf("Resource %d: %d\n", i, avaliable[i]);
                        printf("Alloc[%d][%d]: %d\n",customer_number,i, alloc[customer_number][i]);
                    }
            }
            count_line++;
        } else if (strstr(line, "RL") != NULL) {
            int customer_number;
            get_request(line, &release);
            sscanf(line, "RL %d", &customer_number); 
            for(int i = 0; i < count_resources; i++){
                avaliable[i] += release[i]; 
                alloc[customer_number][i] -= release[i];
            }
            count_line_release++;

        } else if (strstr(line, "*") != NULL) {
            printf("Avaliable: ");
            for(int i = 0; i < count_resources; i++){
                printf("%d ", avaliable[i]);
            }
            printf("\n");
            printf("Alloc: \n");
            for(int i = 0; i < customer; i++){
                printf("Customer %d: ", i);
                for(int j = 0; j < count_resources; j++){
                    printf("%d ", alloc[i][j]);
                }
                printf("\n");
            }
            printf("Need: \n");
            for(int i = 0; i < customer; i++){
                printf("Customer %d: ", i);
                for(int j = 0; j < count_resources; j++){
                    printf("%d ", customer_resources[i][j] - alloc[i][j]);
                }
                printf("\n");
            }
            printf("\n");
        
        } 
    }




    fclose(customer_file);
    fclose(commands_file);

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


void get_request(char *line, int **request){
    char *token = strtok(line, " ");
    token = strtok(NULL, " ");
    int count = 0;
    while (token != NULL) {
        request[count] = atoi(token);
        token = strtok(NULL, " ");
        count++;
    }
}




bool run_bankers_algorithm(int n, int m, int alloc[n][m], int max[n][m], int avail[m]) {
    int i, j, k;
    int f[n];
    for (k = 0; k < n; k++) {
        f[k] = 0;
    }
    int need[n][m];
    for (i = 0; i < n; i++) {
        for (j = 0; j < m; j++)
            need[i][j] = max[i][j] - alloc[i][j];
    }
    int y = 0;
    for (k = 0; k < 5; k++) {
        for (i = 0; i < n; i++) {
            if (f[i] == 0) {
                int flag = 0;
                for (j = 0; j < m; j++) {
                    if (need[i][j] > avail[j]){
                        flag = 1;
                        break;
                    }
                }
                if (flag == 0) {
                    for (y = 0; y < m; y++)
                        avail[y] += alloc[i][y];
                    f[i] = 1;
                }
            }
        }
    }
    int flag = 1;
    for(int i=0;i<n;i++) {
        if(f[i]==0) {
            flag=0;
            return false;
        }
    }
    if(flag==1) {
        return true;
    }
    return false;
}
