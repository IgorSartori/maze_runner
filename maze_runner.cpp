#include <stdio.h>
#include <stdlib.h>
#include <stack>
#include <thread>
#include <chrono>

using namespace std;

// Matriz de char representnado o labirinto
char** maze; // Voce também pode representar o labirinto como um vetor de vetores de char (vector<vector<char>>)

// Numero de linhas e colunas do labirinto
int num_rows;
int num_cols;

// Representação de uma posição
struct pos_t {
	int i;
	int j;
};

// Estrutura de dados contendo as próximas posicões a serem exploradas no labirinto
stack<pos_t> valid_positions;

// Função que le o labirinto de um arquivo texto, carrega em memória e retorna a posição inicial
pos_t load_maze(const char* file_name) {
	pos_t initial_pos;

	// Abre o arquivo para leitura (fopen)
	FILE* file = fopen(file_name, "r"); // Abre o arquivo para leitura
	if (file == NULL) {
        perror("Erro ao abrir o arquivo");
        exit(EXIT_FAILURE);
    }

	// Le o numero de linhas e colunas (fscanf) e salva em num_rows e num_cols
    fscanf(file, "%d %d", &num_rows, &num_cols); // Lê o número de linhas e colunas

	// Aloca a matriz maze (malloc)
	maze = (char**)malloc(num_rows * sizeof(char*)); // Aloca a matriz maze
    for (int i = 0; i < num_rows; ++i) {
        maze[i] = (char*)malloc(num_cols * sizeof(char)); // Aloca cada linha da matriz
    }

	for (int i = 0; i < num_rows; ++i) {
        for (int j = 0; j < num_cols; ++j) {
			// Le o valor da linha i+1,j do arquivo e salva na posição maze[i][j]
			// Se o valor for 'e' salvar o valor em initial_pos
            fscanf(file, " %c", &maze[i][j]); // Lê o valor da linha i+1, j do arquivo

            if (maze[i][j] == 'e') {
                initial_pos.i = i;
                initial_pos.j = j;
            }
        }
    }

    fclose(file); // Fecha o arquivo
    return initial_pos;
}

// Função que imprime o labirinto
void print_maze() {
    system("clear");
	for (int i = 0; i < num_rows; ++i) {
		for (int j = 0; j < num_cols; ++j) {
			printf("%c", maze[i][j]);
		}
		printf("\n");
	}
}

// Função responsável pela navegação.
// Recebe como entrada a posição inicial e retorna um booleando indicando se a saída foi encontrada
bool walk(pos_t pos) {
    chrono::milliseconds delayDuration(40);
    int x = 0;

    print_maze();    
    this_thread::sleep_for(delayDuration);
    maze[pos.i][pos.j] = 'o';
    print_maze();
    this_thread::sleep_for(delayDuration);

    int qntX;

    while (!valid_positions.empty() || x != 1) {
        qntX = 0;
        maze[pos.i][pos.j] = 'o';
        
        print_maze();
        
        // Abaixo
        if (pos.i + 1 < num_rows && maze[pos.i + 1][pos.j] == 'x') {
            pos_t new_pos = {pos.i + 1, pos.j};
            valid_positions.push(new_pos);
            qntX++;
        } else if (pos.i + 1 < num_rows && maze[pos.i + 1][pos.j] == 's'){
            x = 1;
            break;
        }

        // Acima
        if (pos.i - 1 >= 0 && maze[pos.i - 1][pos.j] == 'x') {
            pos_t new_pos = {pos.i - 1, pos.j};            
            if (qntX >= 1) {
                thread walk(new_pos);
            }else{
                valid_positions.push(new_pos);
            }
            qntX++;
        } else if (pos.i - 1 >= 0 && maze[pos.i - 1][pos.j] == 's'){
            x = 1;
            break;
        }

        // Direita
        if (pos.j + 1 < num_cols && maze[pos.i][pos.j + 1] == 'x') {
            pos_t new_pos = {pos.i, pos.j + 1};
            if (qntX >= 1) {
                thread walk(new_pos);
            }else{
                valid_positions.push(new_pos);
            }
            qntX++;
        } else if (pos.j + 1 < num_cols && maze[pos.i][pos.j + 1] == 's'){
            x = 1;
            break;
        }

        // Esquerda
        if (pos.j - 1 >= 0 && maze[pos.i][pos.j - 1] == 'x') {
            pos_t new_pos = {pos.i, pos.j - 1};
            if (qntX >= 1) {
                thread walk(new_pos);
            }else{
                valid_positions.push(new_pos);
            }
            qntX++;
        } else if (pos.j - 1 >= 0 && maze[pos.i][pos.j - 1] == 's'){
            x = 1;
            break;
        }

        this_thread::sleep_for(delayDuration);

        maze[pos.i][pos.j] = '.';                  

        if (!valid_positions.empty()) {
            pos = valid_positions.top();
            valid_positions.pop();
        }                    
    }

    if (x==1) {
        return true;
    } else {
        return false;        
    }
}

int main(int argc, char* argv[]) {

	// carregar o labirinto com o nome do arquivo recebido como argumento
    pos_t initial_pos = load_maze(argv[1]);
    valid_positions.push(initial_pos); // Adicionar a posição inicial à pilha
	// chamar a função de navegação
    bool exit_found = walk(initial_pos);

	// Tratar o retorno (imprimir mensagem)
    if (exit_found) {
        printf("Saída encontrada!\n");
    } else {
        printf("Saída não encontrada.\n");
    }
	
	return 0;
}