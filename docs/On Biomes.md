# Documentation on Biomes
## MAP		

The following map explains the biomes. 
Please view on a text editor.

```
	 1    2    3    4    5    6    7    8    9  10
	_____________________________________________	
1	|-------------------- |---------------------|
2	|------Tentacle------ |-----Forest----------|
3	|---------------------|---------------------|
4	|-------Biome---------|-----Biome-----------|
5	|--_---_---_---_-- _|_--_--_--_--_--_--_--_-|
6	|---------------------|---------------------|
7	|------General--------|-----Alien-----------|
8	|---------------------|---------------------|
9	|-------Biome---------|-----Biome---------- |
10	|_____________________|_____________________|
```
# Details
Tentacle biome is lots of rocks with tentacles
Heavy render is only Trees A and C to maximize GPU intensity.
General biome has everything with even mix of rocks.
Alien biome uses position based coloring. No textures. Even number of rocks

Biomes alternate based on current X and Y position via modulo.
Each biome is a certain square size
