#include <stdio.h>
#include <stdlib.h>
#include <string.h>










int count_numbers_in_line(char *line);
int count_lines(FILE *file);
int count_whitespace(FILE *file);
void check_command_type(const char *line);
void allocate_resources(int customer, int request[]);
void release_resources(int customer, int release[]);
void get_numbers(FILE *file, int count, int numbers[][count]); 
int is_safe_state(int customer, int request[], int count_resources, int available[], int customer_resources[][count_resources]);



int main(int argc, char *argv[]) {
    char line[1024];
    int count_resources = 0;
    int resources[argc-1];

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
        resources[i] = atoi(argv[i+1]);
    }
    rewind(customer_file);
    int customer = count_lines(customer_file);
    printf("Customer: %d\n", customer);
    int whitespace = count_whitespace(commands_file);
    printf("Commands resources: %d\n", whitespace-1);
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
        printf("Resource %d: %d\n", i, resources[i]);
    }


    for (int i = 0; i < customer; i++) {
        printf("Customer %d resources:", i);
        for (int j = 0; j < count_resources; j++) {
            printf(" %d", customer_resources[i][j]);
        }
        printf("\n");
    }
    rewind(commands_file);
    int request[customer][count_resources];

    get_numbers(commands_file, count_resources, request);

    for(int i = 0; i < customer; i++){
        for(int j = 0; j < count_resources; j++){
            printf("%d ", request[i][j]);
        }
        printf("\n");
    }

    while(fgets(line, 1024, commands_file) != NULL) {
        if (strstr(line, "RQ") != NULL) {
            int flag = 0;
            int customer_number;
            sscanf(line, "RQ %d", &customer_number);


            // Criar uma cópia da linha original
            char line_copy[1024];
            strcpy(line_copy, line);

            for (int i = 0; i < count_resources; i++) {
                printf("%d\n", request[customer_number][i]);
                if (request[customer_number][i] > customer_resources[customer_number][i]) {
                    flag = 1;
                } else if (request[customer_number][i] > resources[i]) {
                    flag = 2;
                }
            }

            if(is_safe_state(customer_number, request, count_resources, resources, customer_resources) == 0){
                flag = 3;
            }

            switch(flag){
                case 1:
                    printf("The customer %d request was denied because exceed its maximum need\n", customer_number);
                    break;
                case 2:
                    printf("The resources are not enough to customer %d request\n", customer_number);
                    break;
                case 3:
                    printf("The customer %d request was denied because result in an unsafe state\n", customer_number);
                    break;
                default:
                    printf("The customer %d request was accepted\n", customer_number);
                    for(int i = 0; i < count_resources; i++){
                        resources[i] -= request[customer_number][i];
                        customer_resources[customer_number][i] += request[customer_number][i];
                    }
            }
        } else if (strstr(line, "RL") != NULL) {
            int customer_number;
            sscanf(line, "RL %d", &customer_number); // Corrigir aqui
            for(int i = 0; i < count_resources; i++){
                sscanf(line, "%d", &request[customer_number][i]);
                resources[i] += request[customer_number][i]; // Atualizar recursos disponíveis
                customer_resources[customer_number][i] -= request[customer_number][i]; // Atualizar recursos alocados para o cliente
            }
        } else if (strstr(line, "*") != NULL) {
       
        
        } else {
        
        
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



void get_numbers(FILE *file, int count, int numbers[][count]) {
    char line[1024];

    // Lê linha por linha do arquivo
    for (int i = 0; i < count; i++) {
        if (fgets(line, 1024, file) == NULL) {
            fprintf(stderr, "Error reading numbers from file.\n");
            exit(EXIT_FAILURE);
        }

        char *token = strtok(line, " ");
        int count_numbers = 0;

        // Lê os números separados por espaços
        while (token != NULL && count_numbers < count) {
            numbers[i][count_numbers] = atoi(token);
            count_numbers++;

            token = strtok(NULL, " ");  // Próximos números
        }
    }

    // Impressão de debug
    for (int i = 0; i < count; i++) {
        for (int j = 0; j < count; j++) {
            printf("%d ", numbers[i][j]);
        }
        printf("\n");
    }
}




int is_safe_state(int customer, int request[], int count_resources, int available[], int customer_resources[][count_resources]) {
    int work[count_resources];
    int finish[customer];

    // Inicialização dos arrays
    for (int i = 0; i < count_resources; i++) {
        work[i] = available[i];
    }

    for (int i = 0; i < customer; i++) {
        finish[i] = 0;
    }

    // Simulação da alocação
    for (int i = 0; i < customer; i++) {
        if (finish[i] == 0) {
            int j;
            for (j = 0; j < count_resources; j++) {
                if (customer_resources[i][j] > work[j]) { // Correção 1
                    break;
                }
            }

            if (j == count_resources) {
                for (int k = 0; k < count_resources; k++) {
                    work[k] -= request[k]; // Correção 2
                }
                finish[i] = 1;
                i = -1;  // Reinicia a verificação desde o início
            }
        }
    }

    // Verifica se todos os processos foram concluídos
    for (int i = 0; i < customer; i++) {
        if (finish[i] == 0) {
            return 0;  // Não seguro (unsafe) // Correção 3
        }
    }

    return 1;  // Seguro (safe)
}
