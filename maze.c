#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//global variables
int MAX_MAZE = 100;
int MIN_MAZE = 5;
int BUFFER_SIZE = 1000;
char filename[10];
int line_counter = 0;
int start_found = 0;
int end_found = 0;

// struct to store the maze data
typedef struct {
    //includes maze width,height,
        //width and height are used to create and index through the whole maze array
    //start location and end location as arrays representing coords
    //maze as a whole (stored as a 2D array, 
        //with elements including spaces(' '), walls(#), the start location(S), the end location(E)
    int width;
    int height;
    char *maze_array[1000];
    int start_location[1000];
    int end_location[1000];

} Maze_data;

typedef struct {
    //player struct
    //includes player coords
    int x_coords;
    int y_coords;
} Character_data;

//function to validate if the move the player wants to do is possible
//takes in predicted_player_location and Maze_date as arguments
int Validate_move(int x_coords, int y_coords, Maze_data* the_maze, Character_data* the_character){
    //checks if the player can move to the location they want to
    //done by checking if there's a wall or an edge at that location
    //if not valid, appropriate message is printed
    //either "Movement off the edge of the map is not allowed" or "Movement through a wall is not allowed"
    if (x_coords>=the_maze->width||x_coords<0||y_coords>=the_maze->height||y_coords<0){
        printf("Movement off the edge of the map is not allowed\n");
        return 0;
    }
    if (the_maze->maze_array[y_coords][x_coords]=='#'){
       printf("Movement through a wall is not allowed\n");
        return 0; 
    }
    else{
        the_character->x_coords = x_coords;
        the_character->y_coords = y_coords;
        return 0;
    }
}

//function to be able to move the player
//takes in keyboard input (WASD/wasd) Maze_data and Character_data as arguments
int Move_player(char *input, Maze_data* the_maze, Character_data* the_character){
    //assigns value to local predicted_location variables
    //depending on direction moved
    //checks if the move is valid
    //updates player location in Maze_data using predicted_player_location if move is valid
    //validated by calling the Validate_move function
    int predicted_location_y;
    int predicted_location_x;
    if (*input=='W'||*input=='w'){
        predicted_location_y = the_character->y_coords-1;
        predicted_location_x = the_character->x_coords;
    }
    if (*input=='S'||*input=='s'){
        predicted_location_y = the_character->y_coords+1;
        predicted_location_x = the_character->x_coords;
    }
    if (*input=='A'||*input=='a'){
        predicted_location_x = the_character->x_coords-1;
        predicted_location_y = the_character->y_coords;
    }
    if (*input=='D'||*input=='d'){
        predicted_location_x = the_character->x_coords+1;
        predicted_location_y = the_character->y_coords;
    }
    Validate_move(predicted_location_x,predicted_location_y,the_maze,the_character);
    return 0;
}

//function to display the map
//takes Maze_data as an argument 
int Open_map(Maze_data* the_maze, Character_data* the_character){
    //prints the 2D array of the maze from Maze_data
    printf("\n");
    for (int i=0; i<the_maze->height;i++){
        for (int j=0; j<the_maze->width; j++){
            if (the_character->x_coords == j && the_character->y_coords == i){
                printf("X");
            }
            else{
                printf("%c",the_maze->maze_array[i][j]);
            }
        }
        printf("\n");
    }
    return 0;
}

//function to check if the end of the maze has been reached
//takes Maze_data as an argument
int Reach_end(Character_data* the_character, Maze_data* the_maze){
    //compares the values in the player_location array
    //to the end location array
    //if they're the exact same, the end of the maze has been reached and 1 is returned
    //else, 0 is returned
    if (the_character->x_coords==the_maze->end_location[1]&&
    the_character->y_coords==the_maze->end_location[0]){
        return 1;
    }
    else{
        return 0;
    }

}

int Validate_width(int width){
    //takes in maze width as an argument
    //compares width of the maze line to minimum and maximum requirements
    //returns 3 if width is invalid
    if (width < MIN_MAZE || width > MAX_MAZE){
        printf("wError: Maze does not have the expected format\n");
        return 3;
    }
    return 0;
}

int Validate_height(int height){
    //takes in maze height as argument
    //comapres maze height to minimum and maximum requirements
    //returns 3 if height is invalid

    if (height < MIN_MAZE || height > MAX_MAZE){
        printf("hError: Maze does not have the expected format\n");
        return 3;
    }
    return 0;
}

int Unequal_rows_columns(int first_width, int line_width){
    //if the height of a column is unequal it means the row width will be unequal
    //so only row width will need to be checked
    //compares each row to the width of the first row
    if (first_width != line_width){
        printf("ueError: Maze does not have the expected format\n");
        return 3;
    }
   return 0;
}

int Validate_contents(char *line,int width,int line_number,Maze_data* the_maze){
    //validates the contents of the maze
    //checks for a start and end point
    //also checks that only the start, end, space, wall and new line characters are used
    
    for (int i=0; i<width; i++){
        char current = *line;
        if(*line=='S'){
            start_found=1;
            //stores the location of the start within the maze array
            the_maze->start_location[0] = line_number;
            the_maze->start_location[1] = i;
        }
        if(*line=='E'){
            end_found=1;
            the_maze->end_location[0] = line_number;
            the_maze->end_location[1] = i;

        }
        if(*line!='S'&&*line!='E'&&*line!=' '&&*line!='#'&&*line!='\n'){
            printf("cError: Maze does not have the expected format\n");
            return 3;
        }
        line++;
    }
    return 0;

}

int main(){
    //initial set up
    //to read in the maze from a file
    //validate width and height using functions
    //store maze data in a struct
    //creates player struct

    Maze_data the_maze;
    Character_data the_character;
    for(int row = 0;row < 101; row++) {
    the_maze.maze_array[row] = malloc(100);
    }
    char line_buffer[BUFFER_SIZE];
    char previous_line[BUFFER_SIZE];
    int width;
    printf("Input filename: ");
    scanf("%s",filename);
    FILE *file = fopen(filename, "r");
    if(file == NULL){
        printf("Error opening file\n");
        return 2;
    }

    while(fgets(line_buffer, BUFFER_SIZE, file) != NULL){
        if (line_counter>5){
            if(Validate_height(line_counter)!=0){
            return 3;
            }
        }
        if (line_counter == 0){
            
            width = strcspn(line_buffer, "\n");
            if (Validate_width(width)!=0){
                return 3;
            }
            the_maze.width =width;
        }
        if (line_counter != 0){
            
            int line_width = strcspn(line_buffer, "\n");
            if (Unequal_rows_columns(width, line_width)!=0){
                return 3;
            }
            
        }
        if (Validate_contents(line_buffer,width,line_counter,&the_maze)!=0){
            return 3;
        }
        strcpy(the_maze.maze_array[line_counter], line_buffer);
        
        line_counter++;

    }
    if (start_found==0||end_found==0){
        printf("Error: Maze does not have the expected format\n");
        return 3;
    }
    if(Validate_height(line_counter)!=0){
        return 3;
    }
    the_maze.height = line_counter;
    the_character.x_coords=the_maze.start_location[1];
    the_character.y_coords=the_maze.start_location[0];
    fclose(file);

    //loop to run the game
    //loop ends when game is complete or game is closed
    //End_value variable keeps track of if the game has ended, initialised to 0
    //loops through checking if there's a keyboard movement input
    //if there's a keyboard movement, the Move_player function is called
    //then checking if there's an open map input
    //if there is an open map input, the Open_map function is called
    //then checking if the end point has been reached, done by calling the Reach_end function
    //End_value updated to what Reach_end returns
    //if End_value is 1 the loop breaks

    int End_value = 0;
    char player_input[10];
    while (End_value == 0){
        printf("Input an action: ");
        scanf("%s", player_input);
        if (strcmp(player_input,"W")==0||strcmp(player_input,"A")==0||strcmp(player_input,"S")==0||strcmp(player_input,"D")==0||
        strcmp(player_input,"w")==0||strcmp(player_input,"a")==0||strcmp(player_input,"s")==0||strcmp(player_input,"d")==0){
            Move_player(player_input,&the_maze,&the_character);
        }
        if (strcmp(player_input,"M")==0||strcmp(player_input,"m")==0){
            Open_map(&the_maze,&the_character);
        }
        End_value = Reach_end(&the_character,&the_maze);
    }
    //End message is printed "Game Over, you have won"
    //Program is closed
    printf("Game Over, you have won\n");
    return 0;
}