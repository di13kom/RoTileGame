Efs: Efs.cpp
	g++ -Wall -g Efs.cpp -o Efs -lfltk -lfltk_images -pthread --std=c++11
#	#fltk-config --compile Efs.cpp
clean:
	rm -rf Efs 
