#include <vector>
#include <thread>
#include <chrono>
#include <iostream>
#include <fstream>
#include <unistd.h>

using namespace std;

bool saidaEncontrada = false;

// Matriz de char representando o labirinto
vector<vector<char>> maze;

// Numero de linhas e colunas do labirinto
int num_rows;
int num_cols;

// Representação de uma posição
struct pos_t {
    int i;
    int j;
};

pos_t load_maze(const char* file_name) {
    pos_t initial_pos = {-1, -1}; 
    FILE* file = fopen(file_name, "r");
    fscanf(file, "%d %d", &num_rows, &num_cols);

    maze.resize(num_rows, std::vector<char>(num_cols));
    for (int i = 0; i < num_rows; ++i) {
        for (int j = 0; j < num_cols; ++j) {
            char c;
            fscanf(file, " %c", &c);
            maze[i][j] = c;
            if (c == 'e') {
                initial_pos.i = i;
                initial_pos.j = j;
            }
        }
    }

    fclose(file);
    
    return initial_pos;
}

void print_maze()
{
	while (!saidaEncontrada)
	{
		system("clear");
		for (int i = 0; i < num_rows; ++i) {
            for (int j = 0; j < num_cols; ++j) {
                printf("%c", maze[i][j]);
            }
		    printf("\n");
	    }
	    fflush(stdout); // Garante que a saída seja exibida imediatamente
	    usleep(80000);  // Pausa o programa por 80 milissegundos
	}
}


// Função responsável pela navegação.
// Recebe como entrada a posição inicial e retorna um booleano indicando se a saída foi encontrada
void walk(pos_t pos) {
    vector<pos_t> valid_positions;
    valid_positions.push_back(pos);
    
    while (!valid_positions.empty()) {
        if (saidaEncontrada) return;

        pos_t current_pos = valid_positions.back();
        valid_positions.pop_back();
        
        maze[current_pos.i][current_pos.j] = 'o'; // Marcar posição como atual
        this_thread::sleep_for(chrono::milliseconds(40));

        vector<thread> new_threads;

        // Abaixo
        if (current_pos.i + 1 < num_rows && maze[current_pos.i + 1][current_pos.j] == 'x') {
            pos_t new_pos = {current_pos.i + 1, current_pos.j};
            new_threads.emplace_back(walk, new_pos);
        } else if (current_pos.i + 1 < num_rows && maze[current_pos.i + 1][current_pos.j] == 's') {
            saidaEncontrada = true;
            return;
        }

        // Acima
        if (current_pos.i - 1 >= 0 && maze[current_pos.i - 1][current_pos.j] == 'x') {
            pos_t new_pos = {current_pos.i - 1, current_pos.j};
            new_threads.emplace_back(walk, new_pos);
        } else if (current_pos.i - 1 >= 0 && maze[current_pos.i - 1][current_pos.j] == 's') {
            saidaEncontrada = true;
            return;
        }

        // Direita
        if (current_pos.j + 1 < num_cols && maze[current_pos.i][current_pos.j + 1] == 'x') {
            pos_t new_pos = {current_pos.i, current_pos.j + 1};
            new_threads.emplace_back(walk, new_pos);
        } else if (current_pos.j + 1 < num_cols && maze[current_pos.i][current_pos.j + 1] == 's') {
            saidaEncontrada = true;
            return;
        }

        // Esquerda
        if (current_pos.j - 1 >= 0 && maze[current_pos.i][current_pos.j - 1] == 'x') {
            pos_t new_pos = {current_pos.i, current_pos.j - 1};
            new_threads.emplace_back(walk, new_pos);
        } else if (pos.j - 1 >= 0 && maze[current_pos.i][current_pos.j - 1] == 's') {
            saidaEncontrada = true;
            return;
        }

        maze[current_pos.i][current_pos.j] = '.';
        for (thread& thread : new_threads) {
            thread.join(); 
        }
    }
}

int main(int argc, char *argv[]) {
    pos_t initial_pos = load_maze(argv[1]);

    std::thread walk_thread(walk, initial_pos);

    print_maze();

    walk_thread.join(); // Aguarda a conclusão da thread walk

    if (saidaEncontrada) {
        std::cout << "Saída encontrada!" << std::endl;
    } else {
        std::cout << "Saída não encontrada." << std::endl;
    }

    return 0;
}