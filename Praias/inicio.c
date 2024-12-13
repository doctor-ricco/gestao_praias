#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LOCALIDADES 100 // Qtde máxima de localidades
#define MAX_PRAIAS 100      // Qtde máxima de praias

// Localidade
typedef struct
{
    int IDLocalidade;
    char nome[50];
} Localidade;

// Praia
typedef struct
{
    int IDPraia;
    char nome[50];
    char tipo; // 'O' ou 'F'
    int IDLocalidade;
} Praia;

// Arrays para armazenar localidades e praias
Localidade localidades[MAX_LOCALIDADES];
int contadorLocalidades = 0; // Contador de localidades

Praia praias[MAX_PRAIAS];
int contadorPraias = 0; // Para limitar até o máximo de praias

// Declaração das funções mas sem implementação
void inserirPraia();
void inserirLocalidade();
void listarPraias();
void listarLocalidades();
void procurarPraia();
void alterarPraia();
void alterarLocalidade();
void exportarPraiasCSV();

void listarLocalidades()
{
    FILE *f = fopen("localidades.txt", "r"); // Abrir o ficheiro
    if (f == NULL)
    {
        printf("Erro ao abrir o ficheiro de localidades!\n");
        return;
    }

    printf("Lista de Localidades:\n");
    printf("----------------------------\n");

    int id;
    char nome[100];

    // Leitura linha a linha para exibir
    while (fscanf(f, "%d,%99[^\n]\n", &id, nome) != EOF)
    {
        printf("ID: %d | Nome: %s\n", id, nome);
    }

    printf("----------------------------\n");

    fclose(f);
}

void inserirLocalidade()
{
    if (contadorLocalidades < MAX_LOCALIDADES)
    {
        Localidade novaLocalidade;
        novaLocalidade.IDLocalidade = contadorLocalidades + 1; // Incremento o ID para tornar automática a geração

        printf("Digite o nome da localidade: ");
        getchar(); // Dica do professor para limpar o buffer do teclado
        fgets(novaLocalidade.nome, sizeof(novaLocalidade.nome), stdin);
        novaLocalidade.nome[strcspn(novaLocalidade.nome, "\n")] = 0; // Dica do professor para remover o \n

        // Armazenar a nova localidade
        localidades[contadorLocalidades] = novaLocalidade;
        contadorLocalidades++;

        // Gravar no ficheiro
        FILE *f = fopen("localidades.txt", "a"); // Abre o ficheiro para escrita no final (append)
        if (f == NULL)
        {
            printf("Erro ao abrir o ficheiro de localidades para escrita!\n");
            return;
        }

        // Escreve no ficheiro
        fprintf(f, "%d,%s\n", novaLocalidade.IDLocalidade, novaLocalidade.nome);

        fclose(f);

        printf("Localidade '%s' inserida com sucesso! (ID = %d)\n", novaLocalidade.nome, novaLocalidade.IDLocalidade);
    }
    else
    {
        printf("Erro: Limite de localidades atingido!\n");
    }
}

void listarPraias()
{
    printf("Lista de Praias:\n");
    printf("----------------------------\n");

    for (int i = 0; i < contadorPraias; i++)
    {
        printf("ID: %d | Nome: %s | Tipo: %c | ID Localidade: %d\n",
               praias[i].IDPraia,
               praias[i].nome,
               praias[i].tipo,
               praias[i].IDLocalidade);
    }
    printf("----------------------------\n");
}

void inserirPraia()
{

    if (contadorLocalidades == 0)
    {
        printf("\nAinda não há localidades registadas. Insira localidades antes de inserir praias. \n");
    }
    else
    {
        if (contadorPraias < MAX_PRAIAS)
        {
            Praia novaPraia;
            novaPraia.IDPraia = contadorPraias + 1;

            printf("Digite o nome da praia: ");
            getchar();
            fgets(novaPraia.nome, sizeof(novaPraia.nome), stdin);
            novaPraia.nome[strcspn(novaPraia.nome, "\n")] = 0;

            printf("Digite o tipo da praia (O para Oceânica, F para Fluvial): ");
            scanf(" %c", &novaPraia.tipo);

            // Evita que outro tipo de praia seja inserido por engano
            if (novaPraia.tipo != 'O' && novaPraia.tipo != 'F')
            {
                printf("Erro: Tipo de praia inválido! Use 'O' para Oceânica ou 'F' para Fluvial.\n");
                return;
            }
            
            // Pedir o ID da Localidade e verificar se existe
            printf("Digite o ID da localidade: ");
            scanf("%d", &novaPraia.IDLocalidade);

            int localidadeEncontrada = 0;
            for (int i = 0; i < contadorLocalidades; i++)
            {
                if (localidades[i].IDLocalidade == novaPraia.IDLocalidade)
                {
                    localidadeEncontrada = 1;
                    break;
                }
            }

            if (!localidadeEncontrada)
            {
                printf("Erro: Localidade com ID %d não encontrada! Por favor, insira uma localidade válida.\n", novaPraia.IDLocalidade);
                return;
            }

            // GUardar a nova praia no array
            praias[contadorPraias] = novaPraia;
            contadorPraias++;

            // Gravar no ficheiro
            FILE *f = fopen("praias.txt", "a");
            if (f == NULL)
            {
                printf("Erro ao abrir o ficheiro de praias para escrita!\n");
                return;
            }

            // Gravar a praia no ficheiro
            fprintf(f, "Id: %d, Nome: %s, Tipo: %c, Id_Localidade: %d\n", novaPraia.IDPraia, novaPraia.nome, novaPraia.tipo, novaPraia.IDLocalidade);

            fclose(f);

            printf("Praia '%s' inserida com sucesso! (ID = %d)\n", novaPraia.nome, novaPraia.IDPraia);
        }
        else
        {
            printf("Erro: Limite de praias atingido!\n");
        }
    }
}

void carregarLocalidades()
{
    FILE *f = fopen("localidades.txt", "r");
    if (f == NULL)
    {
        printf("Erro ao abrir o ficheiro de localidades!\n");
        return;
    }

    while (fscanf(f, "%d,%49[^\n]\n", &localidades[contadorLocalidades].IDLocalidade, localidades[contadorLocalidades].nome) != EOF)
    {
        contadorLocalidades++;
    }

    fclose(f);
}

void carregarPraias()
{
    FILE *f = fopen("praias.txt", "r");
    if (f == NULL)
    {
        printf("Erro ao abrir o ficheiro de praias!\n");
        return;
    }

    while (fscanf(f, "%d,%49[^,],%c,%d\n", &praias[contadorPraias].IDPraia, praias[contadorPraias].nome, &praias[contadorPraias].tipo, &praias[contadorPraias].IDLocalidade) != EOF)
    {
        contadorPraias++;
    }

    fclose(f);
}

void procurarPraia()
{
    char nomeProcurado[50];

    printf("Digite o nome da praia que deseja procurar: ");
    getchar();
    fgets(nomeProcurado, sizeof(nomeProcurado), stdin);
    nomeProcurado[strcspn(nomeProcurado, "\n")] = 0;

    int praiaEncontrada = 0;

    for (int i = 0; i < contadorPraias; i++)
    {
        if (strcmp(praias[i].nome, nomeProcurado) == 0)
        {
            printf("Praia encontrada:\n");
            printf("ID: %d | Nome: %s | Tipo: %c | ID Localidade: %d\n",
                   praias[i].IDPraia,
                   praias[i].nome,
                   praias[i].tipo,
                   praias[i].IDLocalidade);
            praiaEncontrada = 1;
            break;
        }
    }

    if (!praiaEncontrada)
    {
        printf("Praia '%s' não encontrada!\n", nomeProcurado);
    }
}

void alterarPraia()
{
    int idPraia;
    printf("Digite o ID da praia que deseja alterar: ");
    scanf("%d", &idPraia);

    // Verificar pelo ID se a praia existe, se já foi inserida
    int praiaEncontrada = -1;
    for (int i = 0; i < contadorPraias; i++)
    {
        if (praias[i].IDPraia == idPraia)
        {
            praiaEncontrada = i;
            break;
        }
    }

    if (praiaEncontrada == -1)
    {
        printf("Praia com ID %d não encontrada!\n", idPraia);
        return;
    }

    Praia *praia = &praias[praiaEncontrada];

    printf("Praia encontrada: \n");
    printf("ID: %d | Nome: %s | Tipo: %c | ID Localidade: %d\n",
           praia->IDPraia, praia->nome, praia->tipo, praia->IDLocalidade);

    // Opções de alterações que podem ser feitas
    int opcao;
    printf("\nO que você deseja alterar?\n");
    printf("1. Nome\n");
    printf("2. Tipo (Oceânica/Fluvial)\n");
    printf("3. Localidade\n");
    printf("4. Cancelar\n");
    printf("Escolha uma opção: ");
    scanf("%d", &opcao);
    getchar();

    switch (opcao)
    {
    case 1: // nome
        printf("Digite o novo nome da praia: ");
        fgets(praia->nome, sizeof(praia->nome), stdin);
        praia->nome[strcspn(praia->nome, "\n")] = 0; // Remover o '\n'
        break;

    case 2: // tipo
        printf("Digite o novo tipo da praia (O para Oceânica, F para Fluvial): ");
        scanf(" %c", &praia->tipo);

        if (praia->tipo != 'O' && praia->tipo != 'F')
        {
            printf("Erro: Tipo de praia inválido! Use 'O' para Oceânica ou 'F' para Fluvial.\n");
            return;
        }
        break;

    case 3: // localidade da praia
        printf("Digite o novo ID da localidade: ");
        int novoIDLocalidade;
        scanf("%d", &novoIDLocalidade);

        // Verifica se o IDLocalidade existe
        int localidadeEncontrada = 0;
        for (int i = 0; i < contadorLocalidades; i++)
        {
            if (localidades[i].IDLocalidade == novoIDLocalidade)
            {
                localidadeEncontrada = 1;
                break;
            }
        }

        if (!localidadeEncontrada)
        {
            printf("Erro: Localidade com ID %d não encontrada! Por favor, insira uma localidade válida.\n", novoIDLocalidade);
            return;
        }

        praia->IDLocalidade = novoIDLocalidade;
        break;

    case 4:
        printf("Operação cancelada.\n");
        return;

    default:
        printf("Opção inválida!\n");
        return;
    }

    // Regravar após a alteração
    FILE *f = fopen("praias.txt", "w");
    if (f == NULL)
    {
        printf("Erro ao abrir o ficheiro de praias para escrita!\n");
        return;
    }

    for (int i = 0; i < contadorPraias; i++)
    {
        fprintf(f, "%d,%s,%c,%d\n", praias[i].IDPraia, praias[i].nome, praias[i].tipo, praias[i].IDLocalidade);
    }

    fclose(f);

    printf("Praia '%s' (ID = %d) alterada com sucesso!\n", praia->nome, praia->IDPraia);
}

void alterarLocalidade()
{
    int idLocalidade;
    printf("Digite o ID da localidade que deseja alterar: ");
    scanf("%d", &idLocalidade);

    // Verificar se a localidade com o ID fornecido existe no ficheiro
    int localidadeEncontrada = -1;
    for (int i = 0; i < contadorLocalidades; i++)
    {
        if (localidades[i].IDLocalidade == idLocalidade)
        {
            localidadeEncontrada = i;
            break;
        }
    }

    if (localidadeEncontrada == -1)
    {
        printf("Localidade com ID %d não encontrada!\n", idLocalidade);
        return;
    }

    Localidade *localidade = &localidades[localidadeEncontrada];

    printf("Localidade encontrada: \n");
    printf("ID: %d | Nome: %s\n", localidade->IDLocalidade, localidade->nome);

    // novo nome
    printf("Digite o novo nome da localidade: ");
    getchar();
    fgets(localidade->nome, sizeof(localidade->nome), stdin);
    localidade->nome[strcspn(localidade->nome, "\n")] = 0;

    // Regravar
    FILE *f = fopen("localidades.txt", "w");
    if (f == NULL)
    {
        printf("Erro ao abrir o ficheiro de localidades para escrita!\n");
        return;
    }

    for (int i = 0; i < contadorLocalidades; i++)
    {
        fprintf(f, "%d,%s\n", localidades[i].IDLocalidade, localidades[i].nome);
    }

    fclose(f);

    printf("Localidade '%s' (ID = %d) alterada com sucesso!\n", localidade->nome, localidade->IDLocalidade);
}

void listarPraiasPorLocalidade()
{
    int idLocalidade;
    printf("Digite o ID da localidade para listar as praias: ");
    scanf("%d", &idLocalidade);

    // Verificar pelo id se a localidade existe
    int localidadeEncontrada = 0;
    for (int i = 0; i < contadorLocalidades; i++)
    {
        if (localidades[i].IDLocalidade == idLocalidade)
        {
            localidadeEncontrada = 1;
            printf("Localidade: %s\n", localidades[i].nome);
            break;
        }
    }

    if (!localidadeEncontrada)
    {
        printf("Erro: Localidade com ID %d não encontrada!\n", idLocalidade);
        return;
    }

    printf("Praias da localidade (ID = %d):\n", idLocalidade);
    printf("----------------------------\n");

    int praiasEncontradas = 0;
    for (int i = 0; i < contadorPraias; i++)
    {
        if (praias[i].IDLocalidade == idLocalidade)
        {
            printf("ID: %d | Nome: %s | Tipo: %c\n",
                   praias[i].IDPraia,
                   praias[i].nome,
                   praias[i].tipo);
            praiasEncontradas = 1;
        }
    }

    if (!praiasEncontradas)
    {
        printf("Nenhuma praia encontrada para a localidade com ID %d.\n", idLocalidade);
    }

    printf("----------------------------\n");
}

void exportarPraiasCSV()
{
    FILE *f = fopen("praias.csv", "w"); // mesmo procedimento para abrir ou criar o ficheiro CSV
    if (f == NULL)
    {
        printf("Erro ao abrir o ficheiro para exportação!\n");
        return;
    }

    // Escrever o cabeçalho do CSV
    fprintf(f, "Nome Localidade,Nome Praia,Tipo Praia\n");

    // Iterar sobre as praias e buscar o nome da localidade correspondente
    for (int i = 0; i < contadorPraias; i++)
    {
        char nomeLocalidade[50] = "Desconhecida";

        // Procurar o nome da localidade correspondente ao IDLocalidade da praia
        for (int j = 0; j < contadorLocalidades; j++)
        {
            if (localidades[j].IDLocalidade == praias[i].IDLocalidade)
            {
                strcpy(nomeLocalidade, localidades[j].nome);
                break;
            }
        }

        // Escrever a linha no CSV (Nome Localidade, Nome Praia, Tipo Praia)
        fprintf(f, "%s,%s,%c\n", nomeLocalidade, praias[i].nome, praias[i].tipo);
    }

    fclose(f);
    printf("Praias exportadas com sucesso para 'praias.csv'!\n");
}

int main()
{
    carregarLocalidades();
    carregarPraias();

    int opcao;

    do
    {
        printf("Menu de Opções:\n");
        printf("1. Inserir Praia\n");
        printf("2. Inserir Localidade\n");
        printf("3. Listar Praias\n");
        printf("4. Listar Localidades\n");
        printf("5. Procurar Praia por Nome\n");
        printf("6. Alterar Praia\n");
        printf("7. Alterar Localidade\n");
        printf("8. Listar Praias por Localidade\n");
        printf("9. Exportar Praias para CSV\n");
        printf("10. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        switch (opcao)
        {
        case 1:
            inserirPraia();
            break;
        case 2:
            inserirLocalidade();
            break;
        case 3:
            listarPraias();
            break;
        case 4:
            listarLocalidades();
            break;
        case 5:
            procurarPraia();
            break;
        case 6:
            alterarPraia();
            break;
        case 7:
            alterarLocalidade();
            break;
        case 8:
            listarPraiasPorLocalidade();
            break;
        case 9:
            exportarPraiasCSV();
            break;
        case 10:
            printf("Boas férias e até a próxima.\n\n");
            break;
        default:
            printf("Opção inválida!\n\n");
        }
    } while (opcao != 10);

    return 0;
}
