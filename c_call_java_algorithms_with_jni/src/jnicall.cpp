//============================================================================
// Name        : jnicall.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>

#include "JNI_CALL_JAVA_ALGORITHMs.h"

using namespace std;

int main() {
	cout << "!!!Hello World!!!" << endl; // prints !!!Hello World!!!
	JNI_CALL_JAVA_ALGORITHMs * algs = new JNI_CALL_JAVA_ALGORITHMs ();
	algs->JNI_CALL_JAVA_Constructor_KShortestPath_Instance("SubNW_20Nodes_3links.brite");
	cout << " OK 1" << endl;
	//algs->JNI_CALL_JAVA_Get_KShortestPaths("0", "18");
	//cout << " OK 2" << endl;
//	//algs->JNI_CALL_JAVA_Get_KShortestPathsWith_KLimit("0", "18", 9);
//	cout << " OK 3" << endl;
////	algs->JNI_CALL_JAVA_Get_KShortestPathsWith_HopMinimumLimit("0", "1", 4);
//	cout << " OK 4" << endl;
//	algs->JNI_CALL_JAVA_Get_KShortestPathsWith_HopMaximumLimit("0", "1", 2);
	cout << " OK 5" << endl;
	algs->JNI_CALL_JAVA_Get_KShortestPathsWith_HopRangeLimit("0", "1", 3, 4);
	return 0;
}
