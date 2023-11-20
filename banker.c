#include <stdio.h>
#include <stdlib.h>
#include <string.h>





typedef struct Node {
    int value;
    struct Node *next;
} Node;


Node* append(Node *head, int value);
void freeList(Node *head);
int count_numbers_in_line(char *line);
int count_lines(FILE *file);
int count_whitespace(FILE *file);
void check_command_type(const char *line);
void allocate_resources(int customer, int request[]);
void release_resources(int customer, int release[]);
int is_safe_state(int customer, int request[]);



int main(int argc, char *argv[]) {
    char line[1024];
    int count_resources = 0;


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

    Node *head = NULL;
    for (int i = 1; i < argc; i++) {
        int value = atoi(argv[i]);
        head = append(head, value);
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

    for (int i = 0; i < customer; i++) {
        printf("Customer %d resources:", i);
        for (int j = 0; j < count_resources; j++) {
            printf(" %d", customer_resources[i][j]);
        }
        printf("\n");
    }
    rewind(commands_file);
    int request[count_resources];

    while(fgets(line, 1024, commands_file) != NULL) {
        if (strstr(line, "RQ") != NULL) {
            int flag = 0;
            int customer_number;
            sscanf(line, "RQ %d", &customer_number);
            Node *current = head;
            for(int i = 0; i < count_resources; i++){
                sscanf(line, "%d", &request[i]);
                if(request[i] > customer_resources[customer_number][i]){
                    flag = 1;
                }
                else if(request[i] > current->value){
                    flag = 2;
                }else if(i == count_resources - 1 && is_safe_state(customer_number, request) == 1){
                    flag = 3;
                }
                current = current->next;
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
            }
            
            

        } else if (strstr(line, "RL") != NULL) {
            int customer_number;
            sscanf(line, "RQ %d", &customer_number);
            for(int i = 0; i < count_resources; i++){
                sscanf(line, "%d", &request[i]);
            }
            
        
        } else if (strstr(line, "*") != NULL) {
       
        
        } else {
        
        
        }
    }



    freeList(head);

    fclose(customer_file);
    fclose(commands_file);

    return 0;
}



Node* append(Node *head, int value) {
    Node *newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }

    newNode->value = value;
    newNode->next = NULL;

    if (head == NULL) {
        return newNode;
    }

    Node *current = head;
    while (current->next != NULL) {
        current = current->next;
    }

    current->next = newNode;
    return head;
}

void freeList(Node *head) {
    Node *current = head;
    Node *next;

    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }
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


int is_safe_state(int customer_number, int request[]){

}