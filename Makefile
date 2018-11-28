Efs: Efs.cpp EFS_Lib.cpp BFS_Lib.cpp Common.h
	g++ -Wall -g Efs.cpp EFS_Lib.cpp BFS_Lib.cpp -o Efs -lfltk -lfltk_images -pthread --std=c++14
#	#fltk-config --compile Efs.cpp
clean:
	rm -rf Efs 
