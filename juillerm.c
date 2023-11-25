#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

int contarColunasCommands(FILE *arquivo) {
    int colunas = 0;
    char caractere;

    while ((caractere = fgetc(arquivo)) != '\n' && caractere != EOF) {
        if (caractere == ' ') {
            colunas++;
        }
    }

    rewind(arquivo); // Volta para o início do arquivo
    return colunas;
}

int contarLinhas(FILE *arquivo) {
    int linhas = 0;
    char caractere;
    bool linhaNaoVazia = false;

    while ((caractere = fgetc(arquivo)) != EOF) {
        if (caractere == '\n') {
            if (linhaNaoVazia) {
                linhas++;
                linhaNaoVazia = false;
            }
        } else if (caractere != ' ' && caractere != '\t' && caractere != '\r') {
            linhaNaoVazia = true;
        }
    }

    if (linhaNaoVazia) {
        linhas++;
    }

    return linhas+1;
}


int contarColunas(FILE *arquivo) {
    int virgulas = 0;
    char caractere;

    // Ler caracteres até encontrar uma nova linha ou o final do arquivo
    while ((caractere = fgetc(arquivo)) != '\n' && caractere != EOF) {
        // Verificar se o caractere é uma vírgula
        if (caractere == ',') {
            virgulas++;
        }
    }

    return virgulas+1;
}


int verSeTaSafe(int n_processos, int n_recursos,int available[], int matrixAllocation[n_processos][n_recursos], int matrizResultado[n_processos][n_recursos]){
  int i,j,work[100],count=0;
  int isFinished[n_processos];
          for(i=0;i<n_processos;i++)
              work[i]=available[i];
          for(i=0;i<100;i++)
              isFinished[i]=false;
          while(count<n_recursos)
          {
                  bool canAllot=false;
                  for(i=0;i<n_recursos;i++)
                  {
                     if(isFinished[i]==false)
                     {

                          for(j=0;j<n_processos;j++)
                          {
                              if(work[j]<matrizResultado[i][j])
                              {
                                  break;
                              }
                          }
                          if(j==n_processos)
                          {
                              for(j=0;j<n_processos;j++)
                              {
                                  work[j]+=matrixAllocation[i][j];
                              }

                              isFinished[i]=true;
                              canAllot=true;
                          }
                     }
                  }
                  if(canAllot==false)
                  {
                      return 1;
                  }
          }
        return 0;
  }

void processarRequisicao(int processo, int n_recursos, int available[], int matrixCostumer[][n_recursos], int matrixAllocation[][n_recursos], int arrayAcao[]) { 
  
  for (int i = 0; i < n_recursos; i++) {
      matrixAllocation[processo][i] += arrayAcao[i];
      available[i] -= arrayAcao[i];
  }

  FILE *original_stdout = stdout;

  FILE *arquivo_existente = fopen("result.txt", "a");

  if (arquivo_existente == NULL) {
      perror("Erro ao abrir ou criar o arquivo existente");
      return;
  }

  stdout = arquivo_existente;

  printf("Allocate to customer %d the resources %d %d %d \n", processo, arrayAcao[0], arrayAcao[1], arrayAcao[2]);

  stdout = original_stdout;

  fclose(arquivo_existente);
}

void processarLiberacao(int processo, int n_recursos, int available[], int matrixCostumer[][n_recursos], int matrixAllocation[][n_recursos], int arrayAcao[]) { 
    for (int i = 0; i < n_recursos; i++) {
        matrixAllocation[processo][i] -= arrayAcao[i];
        available[i] += arrayAcao[i];
    }

  FILE *original_stdout = stdout;

  FILE *arquivo_existente = fopen("result.txt", "w");

  if (arquivo_existente == NULL) {
      perror("Erro ao abrir ou criar o arquivo existente");
      return;
  }

  stdout = arquivo_existente;
  
  printf("Release from customer %d the resources %d %d %d \n", processo, arrayAcao[0], arrayAcao[1], arrayAcao[2]);

  stdout = original_stdout;

  fclose(arquivo_existente);
  
}

void verificarRequisicao(int processo,int n_processos,int n_recursos, int available[], int matrixCostumer[][n_recursos], int matrixAllocation[][n_recursos], int arrayAcao[], int matrizResultado[n_processos][n_recursos]){
int flag_safe = 0;

FILE *original_stdout = stdout;

FILE *arquivo_existente = fopen("result.txt", "a");

if (arquivo_existente == NULL) {
    perror("Erro ao abrir ou criar o arquivo existente");
    return;
}

  stdout = arquivo_existente;
  
    for (int i =0; i < n_recursos; i++){
    
      if ((matrixAllocation[processo][i] + arrayAcao[i]) > matrixCostumer[processo][i]) {
        printf("The customer %d request %d %d %d was denied because exceed its maximum need\n", processo, arrayAcao[0], arrayAcao[1], arrayAcao[2]);
              flag_safe = 1;
              break;
            }
      if (arrayAcao[i] > available[i]){
        flag_safe = 1;
        printf("The resources %d %d %d are not enough to customer %d request %d %d %d \n", available[0], available[1], available[2], processo, arrayAcao[0], arrayAcao[1], arrayAcao[2]);
        break;
      }

      //for (int i = 0; i < n_recursos; i++) {
      //    matrixAllocation[processo][i] += arrayAcao[i];
      //    available[i] -= arrayAcao[i];
      //}
//
      ////flag_safe = verSeTaSafe(n_processos,n_recursos,available, matrixAllocation,matrizResultado);
//
      //for (int i = 0; i < n_recursos; i++) {
      //    matrixAllocation[processo][i] -= arrayAcao[i];
      //    available[i] += arrayAcao[i];
      //}

      
      if (!flag_safe){
        printf("The customer %d request %d %d %d was denied because result in an unsafe state\n",processo, arrayAcao[0], arrayAcao[1], arrayAcao[2]); 
        break;
      }
      
        
      }
       
  if (flag_safe == 0) {
    processarRequisicao(processo, n_recursos, available, matrixCostumer, matrixAllocation, arrayAcao);
  }
  
  stdout = original_stdout;

  fclose(arquivo_existente);
  
}

void verificarLiberacao(int processo, int n_recursos, int available[], int matrixCostumer[][n_recursos], int matrixAllocation[][n_recursos], int arrayAcao[]){
  
  int flag_safe = 0;

  FILE *original_stdout = stdout;

  FILE *arquivo_existente = fopen("result.txt", "a");

  if (arquivo_existente == NULL) {
      perror("Erro ao abrir ou criar o arquivo existente");
      return;
  }

  stdout = arquivo_existente;

  for (int i = 0; i < n_recursos; i++){
    if (arrayAcao[i] > matrixAllocation[processo][i]){
      flag_safe = 1;
      printf("The customer %d release %d %d %d was denied because exceed its maximum allocation\n", processo, arrayAcao[0], arrayAcao[1], arrayAcao[2]);
    }
  }
    

  if (flag_safe == 0) {
    processarLiberacao(processo, n_recursos, available, matrixCostumer, matrixAllocation, arrayAcao);
  }  

  stdout = original_stdout;

  fclose(arquivo_existente);
  
}

void printaMatrizes(FILE *file, int n_processos, int n_recursos, int available[], int matrixCostumer[n_processos][n_recursos], int matrixAllocation[n_processos][n_recursos], int matrizResultado[n_processos][n_recursos]) {
    fprintf(file, "MAXIMUM | ALLOCATION | NEED\n");
    for (int i = 0; i < n_processos; i++) {
        for (int j = 0; j < n_recursos; j++) {
            fprintf(file, "%d", matrixCostumer[i][j]);
            if (j == n_recursos - 1) {
                fprintf(file, "   | ");
            } else {
                fprintf(file, " ");
            }
        }

        for (int j = 0; j < n_recursos; j++) {
            fprintf(file, "%d", matrixAllocation[i][j]);
            if (j == n_recursos - 1) {
                fprintf(file, "      | ");
            } else {
                fprintf(file, " ");
            }
        }

        for (int j = 0; j < n_recursos; j++) {
            fprintf(file, "%d ", matrizResultado[i][j]);
        }
        fprintf(file, "\n");
    }
    fprintf(file,"AVAILABLE ");       
    for (int i = 0; i < n_recursos; i++) {
        fprintf(file,"%d ", available[i]);
    }
    fprintf(file,"\n");
}


int main(int argc, char *argv[]) {
  if (argc <= 1) {
    printf("Insira argumentos da linha de comando.\n");
    return 1;
  }

  int num_argumentos = argc - 1;
  int available[num_argumentos];

  for (int i = 1; i < argc; i++) {
    available[i - 1] = atoi(argv[i]);
  }

  FILE *arquivo;
  arquivo = fopen("customer.txt", "r");

  if (arquivo == NULL) {
    printf("Fail to read customer.txt\n");
    return 1;
  }
  
  int n_recursos = contarColunas(arquivo);
  int n_processos = contarLinhas(arquivo);

  if (n_recursos != num_argumentos) {
    printf("Incompatibility between customer.txt and command line\n");
    return 1;
  }

  int matrixCostumer[n_processos][n_recursos];
  int matrixAllocation[n_processos][n_recursos]; 
  
  for (int i = 0;i<n_processos;i++){
    for (int j = 0; j<n_recursos;j++){
      matrixAllocation[i][j] = 0;
    }
  }

  
  FILE *arquivo_commands;
  arquivo_commands = fopen("commands.txt", "r");

  if (arquivo_commands == NULL) {
    printf("Fail to read commands.txt\n");
    return 1;
  }

  
  int n_recursos_commands = contarColunasCommands(arquivo_commands);
  if (n_recursos != n_recursos_commands - 1) {
    printf("Incompatibility between commands.txt and command line\n");
    return 1;
  }

  int matrizResultado[n_processos][n_recursos];



  for (int i = 0; i < n_processos; i++) {
      for (int j = 0; j < n_recursos; j++) {
          matrizResultado[i][j] = matrixCostumer[i][j] - matrixAllocation[i][j];
      }
  }
  
  char linha[100]; // Ajuste o tamanho conforme necessário

  while (fgets(linha, sizeof(linha), arquivo_commands) != NULL) {
    
    int arrayAcao[n_recursos];
    int processo;

    // Separar os números da linha e colocá-los em arrayAcao
    sscanf(linha, "%*s %d %d %d %d", &processo, &arrayAcao[0], &arrayAcao[1], &arrayAcao[2]);
    
        printf("chego aqui!\n");

      if (strncmp(linha, "RQ", 2) == 0) {
        verificarRequisicao(processo, n_processos, n_recursos, available, matrixCostumer, matrixAllocation, arrayAcao, matrizResultado);

            printf("chego aqui!\n");

               
      } else if (strncmp(linha, "RL", 2) == 0) {
        verificarLiberacao(processo, n_recursos, available, matrixCostumer, matrixAllocation, arrayAcao);

         printf("chego aqui!\n");

      } else if(strncmp(linha, "*", 1) == 0) {
        FILE *arquivo_resultado = fopen("result.txt", "a");
        printaMatrizes(arquivo_resultado, n_processos, n_recursos, available, matrixCostumer, matrixAllocation,matrizResultado);
        fclose(arquivo_resultado);
      }
  }
            



  fclose(arquivo_commands);
        
  
  fclose(arquivo);


    return 0;
}