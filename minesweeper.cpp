#include<stdio.h>
#include<stdlib.h>
#include<time.h>

char fieldArray[8][8];
int numberArray[8][8];
char numberArrayChar[8][8];
int bombArray[10][2]; //10 bombs 2 coordinates
int cursor[2] = {0, 0};
int adjArray[8][2] = {{0,1},{1,1},{1,0},{1,-1},{0,-1},{-1,-1},{-1,0},{-1,1}};
int move;
bool firstClick = true;
char underCursor;
int expCount = 0;

//returns 1 if the coords have a bomb, 0 if not
int isBomb(int X, int Y){
	for(int i=0; i<10; i++){
		if(X == bombArray[i][1] && Y == bombArray[i][0]){
			return 1;
		}
	}
	return 0;
}

//moves bomb of rank n if it has a dupelicate (place is occupied), also stops bombs from moving into 3x3 grid around the cursor
void removeDupe(int n){
	for(int m=0; m<10; m++){
		if(m==n){
			continue; //skips same bomb
		}
		while((bombArray[n][0] == bombArray[m][0] && bombArray[n][1] == bombArray[m][1]) || ((bombArray[n][0]>=cursor[1]-1 && bombArray[n][0]<=cursor[1]+1) && (bombArray[n][1]>=cursor[0]-1 && bombArray[n][1]<=cursor[0]+1))){
			bombArray[n][0] = rand() % 8;
			bombArray[n][1] = rand() % 8;
			if(n==0){
				m=1;
			}else{
				m=0;
			}
		}
	}
}

//moves the bomb at coords X Y
void moveBomb(int X, int Y){
	for(int j=0; j<10; j++){
		if(X == bombArray[j][1] && Y == bombArray[j][0]){
			bombArray[j][0] = rand() % 8;
			bombArray[j][1] = rand() % 8;
			removeDupe(j);
			break;
		}
	}
}

//moves bombs in a 3x3 area
void clearSurround(int X, int Y){
	moveBomb(X, Y);
	for(int i=0; i<8; i++){
		moveBomb(X+adjArray[i][0], Y+adjArray[i][1]);
	}
}

//fills the numberArray according to bomb placement
void fillNumArray(){
	for(int j=0; j<8; j++){
		for(int i=0; i<8; i++){
			numberArray[j][i] = 0;
		}
	}
	for(int j=0; j<10; j++){
		for(int n=0; n<8; n++){
			int row = bombArray[j][1]+adjArray[n][1];
			int col = bombArray[j][0]+adjArray[n][0];
			if(row >= 0 && row < 8 && col >= 0 && col < 8){
				numberArray[row][col]++;
			}
		}
	}
}

//exposes a single cell
void expose(int X, int Y){
	numberArrayChar[X][Y] = 48 + numberArray[X][Y];
	expCount++;
}

//exposes with cascading effect
void checkSpot(int X, int Y){
	if(numberArray[X][Y] == 0){
		expose(X, Y);
		for(int i=0; i<8; i++){
			if(numberArrayChar[X+adjArray[i][0]][Y+adjArray[i][1]] == ' '){
				int row = Y+adjArray[i][1];
				int col = X+adjArray[i][0];
				if(row >= 0 && row < 8 && col >= 0 && col < 8){
					checkSpot(col, row);
				}
			}
		}
	}else{
		expose(X, Y);
	}
}

int main(){
	printf("\n--------MINESWEEPER--------\n\n");
	//randomize seed
	srand(time(NULL));
	
	//scatter bombs
	for(int i=0; i<10; i++){
		bombArray[i][0] = rand() % 8;
		bombArray[i][1] = rand() % 8;
	}
	
	//removes dupelicates of all bombs
	for(int i=0; i<10; i++){
		removeDupe(i);
	}
	
	//initializes the numberArrayChar
	for(int j=0; j<8; j++){
		for(int i=0; i<8; i++){
			numberArrayChar[j][i] = ' ';
		}
	}
	
	while(expCount<54){	
		//initializes the field
		for(int j=0; j<8; j++){
			for(int i=0; i<8; i++){
				fieldArray[j][i] = numberArrayChar[j][i];
			}
		}
	
		//save whats under the cursor
		underCursor = fieldArray[cursor[0]][cursor[1]];
		//set cursor
		fieldArray[cursor[0]][cursor[1]] = 'X';
		
		//prints field array
		for(int j=0; j<8; j++){
			for(int i=0; i<8; i++){
				printf("[");
				printf("%c", fieldArray[j][i]);
				printf("]");
			}
			printf("\n");
		}
		
		//returns whats under the cursor to it's previous value
		fieldArray[cursor[0]][cursor[1]] = underCursor;
		
		//instructions and input
		printf("1. Right\n2. Left\n3. Up\n4. Down\n5. click\n");
		scanf("%d", &move);
		
		//cursor movement
		switch(move){
			case 1:
				if(cursor[1]<7) cursor[1]++;
				break;
			case 2:
				if(cursor[1]>0) cursor[1]--;
				break;
			case 3:
				if(cursor[0]>0) cursor[0]--;
				break;
			case 4:
				if(cursor[0]<7) cursor[0]++;
				break;
			case 5:
				//first click protection
				if(firstClick){
					clearSurround(cursor[0], cursor[1]);
					fillNumArray();
					firstClick = false;
				}
				if(isBomb(cursor[0], cursor[1])){
					printf("\n");
					printf("--------YOU LOSE--------"); // checkSpot all @ symbols
					printf("\n");
					printf("\n");
					for(int j=0; j<8; j++){
						for(int i=0; i<8; i++){
							fieldArray[j][i] = ' ';
						}
					}
					for(int i=0; i<10; i++){
						fieldArray[bombArray[i][1]][bombArray[i][0]] = '@';
					}
					for(int j=0; j<8; j++){
						for(int i=0; i<8; i++){
							printf("[");
							printf("%c", fieldArray[j][i]);
							printf("]");
						}
						printf("\n");
					}
					system("Pause");
					return 0;
				}
				checkSpot(cursor[0], cursor[1]);
				break;
		}
	}
	printf("\n");
		printf("--------YOU WIN!--------");
		printf("\n");
		printf("\n");
		for(int i=0; i<10; i++){
			numberArrayChar[bombArray[i][1]][bombArray[i][0]] = '/';
		}
		for(int j=0; j<8; j++){
			for(int i=0; i<8; i++){
				printf("[");
				printf("%c", numberArrayChar[j][i]);
				printf("]");
			}
			printf("\n");
		}
		printf("\n");
	system("Pause");
	return 0;
}