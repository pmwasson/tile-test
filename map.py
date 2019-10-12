#----------------------------------------
# Data
#----------------------------------------

map = [
R"#                              #",
R"#\  $                         /#",
R"#######\ /#\ /####\ /###//### ##",
R"######## ##############//#### ##",
R"#######/ \########      #/ \# ##",
R"######/   \###/ \##\ ###//# # ##",
R"#####    $ ##//#\\##\\#//## # ##",
R"#####\ #####//### \##   ### # ##",
R"######\\###//####  #/  /### # ##",
R"#######\\#//####//##   #### # ##",
R"########   ##### #####\\### # ##",
R"#####/      \###\\##### ### # ##",
R"####/        \###\\###//### # ##",
R"#/      ##    \###\\#//####\  \#",
R"# $    ####  $ #### $ ######\$ #",
R"##### ######################## #",
R"#####\     \###/        ###### #",
R"#######\         /#\      #### #",
R"###   ###############       ## #",
R"##                     /##\  # #",
R"##        ################## # #",
R"##          \##########/     # #",
R"#                          ### #",
R"#         ######           ##  #",
R"#       ##########\        ##  #",
R"#       ###########        ##  #",
R"##\       $#######        /##  #",
R"#####################   ####   #",
R"##  X                   ###    #",
R"##      ##############         #",
R"##  $   #################$######",
R"################################",
]


#----------------------------------------
# Main
#----------------------------------------

#   character	tile 	name 	value
#
#   " " 				empty	0
#
#				###
#   "#" 		###		wall	1
#				###
#
#          		##/
#   "/"   		#/ 		upper-	2 
#          		/		left
#  
#          		\##
#   "\"   		 \#		upper-	3 
#          		  \		right
#  
#          		\
#   "\"   		#\		lower-	4 
#          		##\		left
#
#          		  /
#   "/"   		 /#		lower-	5 
#          		/##		right
#  
#	"$"			^_^		fuel	6
#
#				:::
#	":"			:::		dirt	7
#				:::

EMPTY = 0
WALL = 1
UPPERLEFT = 2
UPPERRIGHT = 3
LOWERLEFT  = 4
LOWERRIGHT = 5
FUEL = 6
DIRT = 7

def isEmpty(tile):
	return (tile == " " or tile == "X")

def isWall(tile):
	return (tile == "#")

def isFuel(tile):
	return (tile == "$")

def isDirt(tile):
	return (tile == ":")

def isForward(tile):
	return (tile == "/")

def isBackward(tile):
	return (tile == "\\")

def isSlant(tile):
	return (isForward(tile) or isBackward(tile))

def parseTile(map,x,y):
	width = len(map[0])
	height = len(map)
	tile = map[y][x]

	# unambiguous tiles
	if isEmpty(tile):
		return EMPTY
	elif isWall(tile):
		return WALL
	elif isFuel(tile):
		return FUEL
	elif isDirt(tile):
		return DIRT
	# ambiguous tiles
	elif isSlant(tile):
		above = map[y-1][x] if (y>0)      else "#"
		below = map[y+1][x] if (y<height-1) else "#"
		left  = map[y][x-1] if (x>0)      else "#"
		right = map[y][x+1] if (x<width-1)  else "#"
		if (isWall(above)):
			return UPPERLEFT if isForward(tile) else UPPERRIGHT
		if (isWall(below)):
			return LOWERRIGHT if isForward(tile) else LOWERLEFT
		if (isWall(left)):
			return UPPERLEFT if isForward(tile) else LOWERLEFT
		if (isWall(right)):
			return LOWERRIGHT if isForward(tile) else UPPERRIGHT
		assert 0,"ambiguous tile at "+str(x)+","+str(y)
	else:
		assert 0,"Unknown tile at "+str(x)+","+str(y)

def parse(map):
    width = len(map[0])
    height = len(map)
    player_x = -1
    player_y = -1

    print("  //",width,'x',height)

    for y in range(height):
    	assert len(map[y]) == width,"Inconsistent width at y="+str(y)
    	output = "  "
    	for x in range(width):
    		output += str(parseTile(map,x,y)) + ","
    		if (map[y][x] == "X"):
    			player_x = x
    			player_y = y
    	print(output)
    print("  // player_x =",player_x)
    print("  // player_y =",player_y)

def main():
	parse(map);

if __name__ == "__main__":
    main()
