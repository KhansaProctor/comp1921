#!/bin/bash

# removing read permissions from this file
chmod -r data/bad_perms.csv

echo -e "\n~~ File Handling ~~"

echo -n "Testing bad filename - "
./maze fake.csv > out
if grep -q "Error: Bad filename" tmp;
then
    echo "PASS"
else
    echo "FAIL"
fi

echo -n "Testing bad permissions - "
timeout 0.2s ./maze data/bad_perms.csv > out
if grep -q "Error: Bad filename" tmp;
then
    echo "PASS"
else
    echo "FAIL"
fi 

echo -e "\n~~ Maze Generation~~"


echo -n "Testing bad maze generation (low width)"
timeout 0.2s ./maze bad_generation_low_width.csv > out
if grep -q "Error: Maze does not have the expected format" out;
then   
    echo "PASS"
else    
    echo "FAIL"
fi

echo -n "Testing bad maze generation (width too high)"
timeout 0.2s ./maze bad_generation_high_width.csv > out
if grep -q "Error: Maze does not have the expected format" out;
then   
    echo "PASS"
else    
    echo "FAIL"
fi

echo -n "Testing bad maze generation (low height)"
timeout 0.2s ./maze bad_generation_low_height.csv > out
if grep -q "Error: Maze does not have the expected format" out;
then   
    echo "PASS"
else    
    echo "FAIL"
fi

echo -n "Testing bad maze generation (height too high)"
timeout 0.2s ./maze bad_generation_high_height.csv > out
if grep -q "Error: Maze does not have the expected format" out;
then   
    echo "PASS"
else    
    echo "FAIL"
fi

echo -n "Testing successfull generation, width and height in range"
timeout 0.2s ./maze successfull_generation.csv > out
if grep -q "Maze successfully generated" out;
then   
    echo "PASS"
else    
    echo "FAIL"
fi

echo -n "Testing successfull generation, missing wall horizontally"
timeout 0.2s ./maze successfull_generation_width_no_wall.csv > out
if grep -q "Maze successfully generated" out;
then   
    echo "PASS"
else    
    echo "FAIL"
fi

echo -n "Testing successfull generation, missing wall vertically"
timeout 0.2s ./maze successfull_generation_height_no_wall.csv > out
if grep -q "Maze successfully generated" out;
then   
    echo "PASS"
else    
    echo "FAIL"
fi

echo -e "\n~~ User Inputs ~~"


# testing left movement by using grep to see if there's a row of the maze where the player is left of the start
echo -n "Testing movement left"
timeout 0.2s ./maze successfull_left_move.csv > out
if grep -q "#  XS   #" out;
then
    echo "PASS"
else
    echo "FAIL"
fi

# testing right movement by using grep to see if there's a row of the maze where the player is right of the start
echo -n "Testing movement right"
timeout 0.2s ./maze successfull_right_move.csv > out
if grep -q "#  SX   #" out;
then
    echo "PASS"
else
    echo "FAIL"
fi

# testing up movement by using grep to see if there's 2 rows of the maze where the player is above the start
echo -n "Testing movement up"
timeout 0.2s ./maze successfull_up_move.csv > out
if grep -A1 "#  X   #" | grep -B1 "#  S   #" out;
then
    echo "PASS"
else
    echo "FAIL"
fi

# testing down movement by using grep to see if there's 2 rows of the maze where the player is below the start
echo -n "Testing movement down"
timeout 0.2s ./maze successfull_down_move.csv > out
if grep -A1 "#  X   #" | grep -B1 "#  S   #" out;
then
    echo "PASS"
else
    echo "FAIL"
fi

echo -n "Testing map opening"
timout 0.2s ./maze successfull_map_opening.csv > out
if grep -q "Map open" out;
then
    echo "PASS"
else
    echo "FAIL"
fi

echo -n "Testing no movement or map opening"
timeout 0.2s ./maze unsuccessfull_user_input.csv > out
if grep -q "Error: input not asigned to a movement or action" out;
then 
    echo "PASS"
else
    echo "FAIL"
fi


echo -e "\n~~ Movement in the maze ~~"

echo -n "Testing movement in spaces"
timeout 0.2s ./maze successfull_move_in_space.csv > out
if grep -q "Successfull move in a space" out;
then
    echo "PASS"
else
    echo "FAIL"
fi

echo -n "Testing movement into a wall"
timeout 0.2s ./maze move_into_wall.csv > out
if grep -q "Movement through a wall is not allowed" out;
then
    echo "PASS"
else
    echo "FAIL"
fi

echo -n "Testing movement off the edge"
timeout 0.2s ./maze move_off_the_edge.csv > out
if grep -q "Movement off the edge of the map is not allowed" out;
then
    echo "PASS"
else
    echo "FAIL"
fi

echo -e "\n~~ Start and End of the maze ~~"

echo -n "Testing bad start point generation"
timeout 0.2s ./maze bad_start_point.csv > out
if grep -q "Error: bad start point generation" out;
then
    echo "PASS"
else
    echo "FAIL"
fi

echo -n "Testing bad end point generation"
timeout 0.2s ./maze bad_end_point.csv > out
if grep -q "Error: bad end point generation" out;
then
    echo "PASS"
else
    echo "FAIL"
fi

echo -n "Testing unsuccesfull end game closure"
if pgrep -x ./maze unsuccessfull_end_game.csv >/dev/null
then
    echo "PASS"
else
    echo "FAIL"
fi

echo -n "Testing succesfull end game message"
timeout 0.2s ./maze successfull_end_game.csv > out
if grep -q "Game Over, you have won" out;
then
    echo "PASS"
else
    echo "FAIL"
fi

# adding read perms back
chmod +r data/bad_perms.csv
