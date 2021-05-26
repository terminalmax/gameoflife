# Game of Life 
This is a simulation of John Conway's "Game of Life" cellular automata. Implemented in C using SDL as platform layer.
Made for learning and educational purpose. Written in a straight forward manner so that it's simple to understand and teach.

## Rules   
Rules that the cells follow -   
- Any live cell with fewer then 2 neighbours dies.   
- Any live cell with more than 3 neighbours dies.   
- Any dead cell with exactly 3 neighbours becomes alive.   

> In other words -   
> Neighbours N:   
> N = 8 for every cell except for those on edges and corners.
>
> - Live Cell -> Dead Cell :if (N < 2) or (N > 3)   
> - Dead Cell -> Live Cell :if (N = 3)   
> - Live Cell -> Live Cell & Dead Cell -> Dead Cell: all other cases   

Visit [Wikipedia Page](https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life) for more info.
