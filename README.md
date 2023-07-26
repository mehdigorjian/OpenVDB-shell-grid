# OpenVDB shell grid function
## OpenVDB shell_grid function that creates the 'shell indicator' grid of a level set
	1. This function takes in a narrow band level set grid pointer as input
	2. It returns an integer grid pointer that stores the following values:
		1. voxel position xyz stores value 1 if xyz lies in the active band of the level set
		2. voxel position xyz stores value 2 if xyz stores a negative value in the level set
		3. voxel position xyz stores value 0 otherwise
## COMPILATION:
	1. to compile:
		mkdir build
		cd build
		cmake ..
		make
	2. to run:
		./solveproblem
