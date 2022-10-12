#include <iostream>
#include <ctime>
#include <stdlib.h>
#include <conio.h>
#include <algorithm>

using namespace std;

#define NumberObjetivos 6

void GenerateDimensions(int dim, int *map, int size, int depth){
    if(dim){
        float parts[12] = {
            .05, 0, 0, 0,
            .1, 0, 0, 0,
            .05, 0, 0, 0
        };
        int items[12] = {
            6, 0, 0, 0,
            6, 0, 0, 0,
            7, 0, 0, 0
        };
        int index = (rand() % size);
        for(int j = 0; j < depth; j++){
            for(int k = 4*j; k < 4*(j+1); k++){
                int part = size*parts[k];
                for(int i = 0; i < part; i++){
                    while((map[index] > 0 && map[index] < 8)){
                        index = (rand() % size);
                        index += size*j;
                    }
                    map[index] = items[k];
                }
            }
        }
        index = (rand() % size);
        index += size*2;
        while(map[index] > 0 && map[index] < 6){
            index = (rand() % size);
            index += size*2;
        }
        map[index] = 8;
    }else{
        float parts[12] = {
            .2, .1, .1, .05,
            .3, 0, 0, 0,
            .3, .2, 0, 0
        };
        int items[12] = {
            1, 2, 3, 4,
            2, 0, 0, 0,
            2, 5, 0, 0
        };
        int index = (rand() % size);
        for(int j = 0; j < depth; j++){
            for(int k = 4*j; k < 4*(j+1); k++){
                int part = size*parts[k];
                for(int i = 0; i < part; i++){
                    while((map[index] > 0 && map[index] < 8)){
                        index = (rand() % size);
                        index += size*j;
                    }
                    map[index] = items[k];
                }
            }
        }
        index = (rand() % size);
        index += size*2;
        while(map[index] > 0 && map[index] < 6){
            index = (rand() % size);
            index += size*2;
        }
        map[index] = 8;
    }
}

void PrintMap(char *map, int width, int heigth){
    for(int i = 0; i < heigth; i++){
        for(int j = 0; j < width; j++){
            cout << map[width*i + j] << ' ';
        }
        cout << '\n';
    }
}

void PrintInventory(int inventory[]){
    string items[8] = {"Graveto(s)", "Pedra(s)", "Animal(is)", "Madeira(s)", "Ferro(s)", "Mithril(s)", "Ebony(s)", "Fruta(s)"};
    bool isFirst = true;
    for(int i = 0; i < 8; i++){
        if(inventory[i]){
            if(isFirst){
                cout << "Inventario:" << '\n';
                isFirst = false;

            }
            cout << items[i] << ": " << inventory[i] << '\n';
        }
    }
}

void PrintObjetivos(){
    string objetivos[NumberObjetivos] = {
        "Luz e calor: 4 gravetos e 2 pedras",
        "Comida: 1 fruta e 1 animal",
        "Construcao: 8 pedras e 4 madeiras",
        "Mineracao: 1 ferro e 2 pedras",
        "Mineracao dimensional: 1 mithril e 1 ebony",
        "Viagem dimensional: entrar no portal"
    };
    for(int i = 0; i < NumberObjetivos; i++){
        cout << i+1 << ": " << objetivos[i] << '\n';
    }
}

int objetivos = 0;
int CheckObjetivos(int *inventory, int dim){
    cout << objetivos << '\n';
    int requirements[NumberObjetivos-1][4] = {
        //Qnt, id Qnt, id
        {4, 0, 2, 1},//4 gravetos 2 pedras
        {1, 7, 1, 2},//1 fruta 1 animal
        {8, 1, 4, 3},//8 pedras 4 madeiras
        {1, 4, 2, 2},//1 ferro 2 pedras
        {1, 5, 1, 6} //1 mithril 1 ebony
    };
    for(int i = 0; i < NumberObjetivos-1; i++){
        int id = (objetivos >> i);
        if(id & 1){
            int count = 0;
            for(int j = 0; j < 4; j += 2){
                if(inventory[requirements[i][j+1]] >= requirements[i][j]){
                    count++;
                }
            }
            if(count == 2){
                inventory[requirements[i][1]] -= requirements[i][0];
                inventory[requirements[i][3]] -= requirements[i][2];
                objetivos -= 1 << i;
                count = 0;
            }
        }
    }
    if(dim){
        if(objetivos >> 5 & 1){
            objetivos -= 1 << 5;
        }
    }
    if(objetivos == 0){
        return 1;
    }
    return 0;
}

int main(){
    srand(time(NULL));

    int width, height, depth = 3;

    cout << "Digite a largura e a altura do mapa" << '\n';
    cin >> width >> height;

    (!width && !height) ? width = height = 8 : width = width;
    char grid[depth][height][width];
    int map[depth][height][width];

    char dimensionGrid[depth][height][width];
    int dimensionMap[depth][height][width];

    for(int i = 0; i < depth; i++){
        for(int j = 0; j < height; j++){
            for(int k = 0; k < width; k++){
                grid[i][j][k] = '_';
                map[i][j][k] = 0;
                
                dimensionGrid[i][j][k] = '_';
                dimensionMap[i][j][k] = 0;
            }
        }
    }
    GenerateDimensions(0, *map[0], width*height, depth);
    GenerateDimensions(1, *dimensionMap[0], width*height, depth);

    char currGrid[depth][height][width];
    int currMap[depth][height][width];
    copy(*map[0], *map[0]+(width*height*depth), *currMap[0]);
    copy(*grid[0], *grid[0]+(width*height*depth), *currGrid[0]);

    int currDim = 0;

    int objetivo = 1;
    cout << "Escolha os objetivos" << '\n';
    PrintObjetivos();
    while(cin >> objetivo && objetivo <= NumberObjetivos && objetivo){
        if(!(objetivos >> objetivo-1)){
            objetivos += 1 << objetivo-1;
        }
    }

    int inventory[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    string items[8] = {"graveto", "pedra", "animal", "arvore", "ferro", "mithril", "ebony", "portal"};
    int player[3] = {width >> 1, height >> 1, 0};
    int itemPos[3] = {-1, -1, -1};
    currGrid[player[2]][player[1]][player[0]] = 'P';

    PrintMap(*currGrid[0], width, height);

    int posX = -1, posY = -1, posZ = -1, completed = 0;
    cout << "Use WASD para se mexer, x e z para subir e descer" << '\n';
    //Jogar
    while(!completed){
        int ch = getch();
        switch(ch){
            case 119://W
                posY = player[0];
                posX = player[1]-1;
                posZ = player[2];
                break;
            case 97://A
                posY = player[0]-1;
                posX = player[1];
                posZ = player[2];
                break;
            case 115://S
                posY = player[0];
                posX = player[1]+1;
                posZ = player[2];
                break;
            case 100://D
                posY = player[0]+1;
                posX = player[1];
                posZ = player[2];
                break;
            case 120://UP
                posY = player[0];
                posX = player[1];
                posZ = player[2]-1;
                break;
            case 122://DOWN
                posY = player[0];
                posX = player[1];
                posZ = player[2]+1;
                break;
            case 3:
                exit(1);
                break;
            default:
                break;
        }
        system("cls");
        if((posX >= height || posX < 0) || (posY >= width || posY < 0) || (posZ >= depth || posZ < 0)){
            cout << "Fora dos limites" << '\n';
        }else{
            currGrid[player[2]][player[1]][player[0]] = '-';

            //Movimento
            player[0] = posY;
            player[1] = posX;
            player[2] = posZ;

            //Icone no mapa
            if(itemPos[0] + 1){
                currGrid[itemPos[2]][itemPos[1]][itemPos[0]] = 'I';
            }else{
                currGrid[itemPos[2]][itemPos[1]][itemPos[0]] = '-';
            }
            currGrid[player[2]][player[1]][player[0]] = 'P';
            //Item
            int item = currMap[player[2]][player[1]][player[0]];
            if(item > 0 && item < 9){
                cout << "Voce encontrou um(a) " << items[item-1] << '\n';
                if(item < 8){
                    cout << "Deseja coletar? Y/N" << '\n';
                    int choice = getch();
                    if(choice == 121){
                        if(item == 4){
                            inventory[7]++;
                        }
                        inventory[item-1]++;
                        currMap[player[2]][player[1]][player[0]] = 0;
                        itemPos[0] = -1;
                        itemPos[1] = -1;
                        itemPos[2] = -1;
                    }else{
                        itemPos[0] = player[0];
                        itemPos[1] = player[1];
                        itemPos[2] = player[2];
                    }
                }else{
                    cout << "Deseja entrar? Y/N" << '\n';
                    int choice = getch();
                    if(choice == 121){
                        if(currDim){
                            currDim = 0;
                            //copy currMap to dimensionMap and copy currGrid to dimensionGrid
                            copy(*currMap[0], *currMap[0]+(width*height*depth), *dimensionMap[0]);
                            copy(*currGrid[0], *currGrid[0]+(width*height*depth), *dimensionGrid[0]);
                            //copy map to currMap and copy grid to currGrid
                            copy(*map[0], *map[0]+(width*height*depth), *currMap[0]);
                            copy(*grid[0], *grid[0]+(width*height*depth), *currGrid[0]);
                        }else{
                            currDim = 1;
                            //copy currMap to map and copy currGrid to grid
                            copy(*currMap[0], *currMap[0]+(width*height*depth), *map[0]);
                            copy(*currGrid[0], *currGrid[0]+(width*height*depth), *grid[0]);
                            //copy dimensionMap to currMap and copy dimensionGrid to currGrid
                            copy(*dimensionMap[0], *dimensionMap[0]+(width*height*depth), *currMap[0]);
                            copy(*dimensionGrid[0], *dimensionGrid[0]+(width*height*depth), *currGrid[0]);
                        }
                        itemPos[0] = -1;
                        itemPos[1] = -1;
                        itemPos[2] = -1;
                        player[2] = 0;
                        player[1] = width >> 1;
                        player[0] = height >> 1;
                        currGrid[player[2]][player[1]][player[0]] = 'P';
                    }else{
                        itemPos[0] = player[0];
                        itemPos[1] = player[1];
                        itemPos[2] = player[2];
                    }
                }
                completed = CheckObjetivos(inventory, currDim);
            }
        }
        PrintMap(*currGrid[player[2]], width, height);
        PrintInventory(inventory);
    }
    cout << '\n' << "Objetivo(s) completo" << '\n';
    cout << "Aperte qualquer tecla pra fechar..." << '\n';
    getch();

    return 0;
}