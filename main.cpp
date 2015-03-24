#include<iostream>
#include<fstream>
#include<string>
#include<ctime>
#include<vector>
#include"Point.h"
#include"STLContainer.h"
#include"MyContainer.h"


using namespace std;

#define STLon 1

#ifdef  STLon	
	typedef STLContainer<Point*> Container;
#else 
	template<class T>
	typedef MyContainer<Point*>  Container<T>;
#endif




//This method prints the solution matrix to the file as matrix
void printMatrix(char** matrix,int n,string filename){
	ofstream of(filename);  //opening file
	for(int i=0;i<n;i++){ 
		for(int j=0;j<n;j++){
			of << matrix[i][j];
		}
		of<<endl;
	}
	of.close();
}

//this method prints the solution to the file in coordinate form
void printMatrixAsCoordinate(char** matrix,int n,string filename){
	ofstream of(filename);  //opening file
	for(int i=0;i<n;i++){
		for(int j=0;j<n;j++){
			if(matrix[i][j]!=' '){
				of<<matrix[i][j]<<" "<<i<<" "<<j<<endl;
			}
		}
	}
	of.close();
}

//This method used for queue based solution
void queueBasedSolution(char** matrix,bool** enqueueMatrix,int n,bool coordinateSolution,string outputFileName){
	bool solutionFound = false;
	int positionExplored = 0;
	int remainingPosition = 0;
	Container c;
	Point* startPoint;
	

	//finding start point
	startPoint = (Point*)malloc(sizeof(Point));
	for (int i = 0; i<n; i++){
		for (int j = 0; j<n; j++){
			if (matrix[i][j] == 'R'){
				startPoint->x = i;
				startPoint->y = j;
			}
		}
	}

	startPoint->from = NULL;

	c.queue.push(startPoint); //adding start point to queue
	Point* x = NULL;
	while (!c.queue.empty()){
		x = c.queue.front();
		positionExplored++;
		c.queue.pop();
		int i = x->x;
		int j = x->y;
		if (matrix[i][j] == 'G'){
			solutionFound = true;
			break;
		}
		else{
			//checking north
			if (i - 1 >= 0 && !enqueueMatrix[i - 1][j] && (matrix[i - 1][j] == ' ' || matrix[i - 1][j] == 'G')){
				Point* y = (Point*)malloc(sizeof(Point));
				y->x = i - 1;
				y->y = j;
				y->from = x;
				c.queue.push(y);
				enqueueMatrix[i - 1][j] = true;
			}
			//checking west
			if (j - 1 >= 0 && !enqueueMatrix[i][j - 1] && (matrix[i][j - 1] == ' ' || matrix[i][j - 1] == 'G')){
				Point* y = (Point*)malloc(sizeof(Point));
				y->x = i;
				y->y = j - 1;
				y->from = x;
				c.queue.push(y);
				enqueueMatrix[i][j - 1] = true;
			}
			//checking south
			if (i + 1 <n && !enqueueMatrix[i + 1][j] && (matrix[i + 1][j] == ' ' || matrix[i + 1][j] == 'G')){
				Point* y = (Point*)malloc(sizeof(Point));
				y->x = i + 1;
				y->y = j;
				y->from = x;
				c.queue.push(y);
				enqueueMatrix[i + 1][j] = true;
			}
			//checking east
			if (j + 1 <n && !enqueueMatrix[i][j + 1] && (matrix[i][j + 1] == ' ' || matrix[i][j + 1] == 'G')){
				Point* y = (Point*)malloc(sizeof(Point));
				y->x = i;
				y->y = j + 1;
				y->from = x;
				c.queue.push(y);
				enqueueMatrix[i][j + 1] = true;
			}
		}
	}
	Point* endPoint = x;
	x = x->from;

	while (matrix[x->x][x->y] != 'R'){  //drawing path
		matrix[x->x][x->y] = '*';

		x = x->from;
	}	

	
	
	//writing solution 
	if (solutionFound && !coordinateSolution){
		printMatrix(matrix, n, outputFileName);
		cout << "Goal found! " << positionExplored << " positions explored, unexplored list contains " << c.queue.size() << " positions." << endl;
	}
	else if (solutionFound && coordinateSolution){
		printMatrixAsCoordinate(matrix, n, outputFileName);
		cout << "Goal found! " << positionExplored << " positions explored, unexplored list contains " << c.queue.size() << " positions." << endl;
	}
	else{
		cout << "Goal not found! " << positionExplored << " positions explored" << endl;
	}

	

}

//this method compare two point's distance from goal
bool distanceComparator(Point* x, Point* y){
	return x->distanceToGoal > y->distanceToGoal;
}


//This method used for optimum based solution
//In this method we use vector instead of queue or stack. Every time we take a point from vector we choose point that is nearest to the destination
void optimumBasedSolution(char** matrix,bool** enqueueMatrix,int n,bool coordinateSolution,string outputFileName){
	bool solutionFound = false;
	int positionExplored = 0;
	int remainingPosition = 0;

	Point* startPoint;
	Point* endPoint;

	//finding start point and end point
	startPoint = (Point*)malloc(sizeof(Point));
	endPoint = (Point*)malloc(sizeof(Point));
	for (int i = 0; i<n; i++){
		for (int j = 0; j<n; j++){
			if (matrix[i][j] == 'R'){
				startPoint->x = i;
				startPoint->y = j;
			}
			if (matrix[i][j] == 'G'){
				endPoint->x = i;
				endPoint->y = j;
			}
		}
	}

	//defining distance matrix which stores all points distance to destination
	int **distanceMatrix;
	distanceMatrix = (int**)malloc(sizeof(int*)*n);
	for (int i = 0; i < n; i++){
		distanceMatrix[i] = (int*)malloc(sizeof(int)*n);
	}
	for (int i = 0; i < n; i++){
		for (int j = 0; j < n; j++){
			distanceMatrix[i][j] = abs(endPoint->x - i) + abs(endPoint->y - j);
		}
	}


	startPoint->from = NULL;
	vector<Point*> v;
	startPoint->distanceToGoal = distanceMatrix[startPoint->x][startPoint->y]; //setting startPoints distance from destination
	v.push_back(startPoint);
	Point *current = NULL;
	while (!v.empty()){

		sort(v.begin(), v.end(), distanceComparator);//sorting vector according to distance
		current = v[0];
		positionExplored++;
		v.erase(v.begin());
		int i = current->x;
		int j = current->y;
		if (matrix[i][j] == 'G'){
			solutionFound = true;
			break;
		}
		else{
			//checking north
			if (i - 1 >= 0 && !enqueueMatrix[i - 1][j] && (matrix[i - 1][j] == ' ' || matrix[i - 1][j] == 'G')){
				Point* y = (Point*)malloc(sizeof(Point));
				y->x = i - 1;
				y->y = j;
				y->from = current;
				v.push_back(y);
				enqueueMatrix[i - 1][j] = true;
			}
			//checking west
			if (j - 1 >= 0 && !enqueueMatrix[i][j - 1] && (matrix[i][j - 1] == ' ' || matrix[i][j - 1] == 'G')){
				Point* y = (Point*)malloc(sizeof(Point));
				y->x = i;
				y->y = j - 1;
				y->from = current;
				v.push_back(y);
				enqueueMatrix[i][j - 1] = true;
			}
			//checking south
			if (i + 1 <n && !enqueueMatrix[i + 1][j] && (matrix[i + 1][j] == ' ' || matrix[i + 1][j] == 'G')){
				Point* y = (Point*)malloc(sizeof(Point));
				y->x = i + 1;
				y->y = j;
				y->from = current;
				v.push_back(y);
				enqueueMatrix[i + 1][j] = true;
			}
			//checking east
			if (j + 1 <n && !enqueueMatrix[i][j + 1] && (matrix[i][j + 1] == ' ' || matrix[i][j + 1] == 'G')){
				Point* y = (Point*)malloc(sizeof(Point));
				y->x = i;
				y->y = j + 1;
				y->from = current;
				v.push_back(y);
				enqueueMatrix[i][j + 1] = true;
			}
		}
	}
	if (!solutionFound)
		return;
	Point *x = current;
	x = x->from;
	while (matrix[x->x][x->y] != 'R'){  //drawing route
		matrix[x->x][x->y] = '*';
		x = x->from;
	}
	
	
	//writing solution
	if (solutionFound && !coordinateSolution){
		printMatrix(matrix, n, outputFileName);
		cout << "Goal found! " << positionExplored << " positions explored, unexplored list contains " << remainingPosition << " positions." << endl;
	}
	else if (solutionFound && coordinateSolution){
		printMatrixAsCoordinate(matrix, n, outputFileName);
		cout << "Goal found! " << positionExplored << " positions explored, unexplored list contains " << remainingPosition << " positions." << endl;
	}
	else{
		cout << "Goal not found! " << positionExplored << " positions explored" << endl;
	}



}

//This method used for stack based solution
void stackBasedSolution(char** matrix,bool** enqueueMatrix,int n,bool coordinateSolution,string outputFileName){
	bool solutionFound = false;
	int positionExplored = 0;
	int remainingPosition = 0;
	Container c;


	//finding start point
	Point* startPoint;
	startPoint = (Point*)malloc(sizeof(Point));
	for (int i = 0; i<n; i++){
		for (int j = 0; j<n; j++){
			if (matrix[i][j] == 'R'){
				startPoint->x = i;
				startPoint->y = j;
			}
		}
	}

	startPoint->from = NULL;

	c.stack.push(startPoint);  //start point added to stack
	Point* x = NULL;
	while (!c.stack.empty()){
		x = c.stack.top();
		c.stack.pop();
		positionExplored++;
		int i = x->x;
		int j = x->y;
		if (matrix[i][j] == 'G'){
			solutionFound = true;
			break;
		}
		else{
			//checking north
			if (i - 1 >= 0 && !enqueueMatrix[i - 1][j] && (matrix[i - 1][j] == ' ' || matrix[i - 1][j] == 'G')){
				Point* y = (Point*)malloc(sizeof(Point));
				y->x = i - 1;
				y->y = j;
				y->from = x;
				c.stack.push(y);
				enqueueMatrix[i - 1][j] = true;
			}
			//checking west
			if (j - 1 >= 0 && !enqueueMatrix[i][j - 1] && (matrix[i][j - 1] == ' ' || matrix[i][j - 1] == 'G')){
				Point* y = (Point*)malloc(sizeof(Point));
				y->x = i;
				y->y = j - 1;
				y->from = x;
				c.stack.push(y);
				enqueueMatrix[i][j - 1] = true;
			}
			//checking south
			if (i + 1 <n && !enqueueMatrix[i + 1][j] && (matrix[i + 1][j] == ' ' || matrix[i + 1][j] == 'G')){
				Point* y = (Point*)malloc(sizeof(Point));
				y->x = i + 1;
				y->y = j;
				y->from = x;
				c.stack.push(y);
				enqueueMatrix[i + 1][j] = true;
			}
			//checking east
			if (j + 1 <n && !enqueueMatrix[i][j + 1] && (matrix[i][j + 1] == ' ' || matrix[i][j + 1] == 'G')){
				Point* y = (Point*)malloc(sizeof(Point));
				y->x = i;
				y->y = j + 1;
				y->from = x;
				c.stack.push(y);
				enqueueMatrix[i][j + 1] = true;
			}
		}
	}
	Point* endPoint = x;
	x = x->from;
	while (matrix[x->x][x->y] != 'R'){  //drawing solution
		matrix[x->x][x->y] = '*';
		x = x->from;
	}

	//Stack base solution will be here in PART2

	//writing solution
	if (solutionFound && !coordinateSolution){
		printMatrix(matrix, n, outputFileName);
		cout << "Goal found! " << positionExplored << " positions explored, unexplored list contains " << c.stack.size() << " positions." << endl;
	}
	else if (solutionFound && coordinateSolution){
		printMatrixAsCoordinate(matrix, n, outputFileName);
		cout << "Goal found! " << positionExplored << " positions explored, unexplored list contains " << c.stack.size() << " positions." << endl;
	}
	else{
		cout << "Goal not found! " << positionExplored << " positions explored" << endl;
	}

	

}



void readMatrixBaseMaze(char** &matrix, bool** &enqueueMatrix, string inputFile, int &n){
	ifstream in(inputFile);	
	string fileLine="";
	getline(in,fileLine);  //reading first line of file expected number(size)
	n=atoi(fileLine.c_str());
	if (n<=0) {  //checking first line of file contain number or not
		cout<<"Input file's first line is not contain a number(size)" << endl;
		system("pause");
		exit(0);
	} 
	
	//allocation of matrix end enqueue matrix 
	matrix=(char**)malloc(n*sizeof(char*));  
	enqueueMatrix=(bool**)malloc(n*sizeof(bool*));
	for(int i=0;i<n;i++){
		matrix[i]=(char*)malloc(n*sizeof(char*));
		enqueueMatrix[i]=(bool*)malloc(n*sizeof(bool*));
	}

	//making matrix as empty and enqueue matrix false
	for(int i=0;i<n;i++){
		for(int j=0;j<n;j++){
			matrix[i][j]=' ';
			enqueueMatrix[i][j]=false;
		}
	}


	for(int i=0;i<n;i++){
		if (in.eof()){  //checking required number of line exists
			cout << "Input file does not contain " << n << " lines" << endl;
		}
		getline(in,fileLine);  //reading line from file
		for(int j=0;j<n && j<fileLine.length();j++){
			if(fileLine[j]=='X' || fileLine[j]==' ' || fileLine[j]=='G' ||fileLine[j]=='R')  //checking invalid characters
				matrix[i][j]=fileLine[j];
			else{
				cout<<fileLine[j] << "  in input file bad character"<<endl;				
				exit(1);
			}
		}
	}

}

void readCoordinateBaseMaze(char** &matrix,bool** &enqueueMatrix,string inputFile,int &n){
	ifstream in(inputFile);	 //opening input file
	string fileLine="";
	getline(in,fileLine); //reading first line of file expected number(size)
	n=atoi(fileLine.c_str());

	if (n <= 0) { //checking first line of file contain number or not
		cout << "Input file's first line is not contain a number(size)" << endl;
		exit(0);
	}



	//allocation of matrix end enqueue matrix
	matrix=(char**)malloc(n*sizeof(char*));
	enqueueMatrix=(bool**)malloc(n*sizeof(bool*));
	for(int i=0;i<n;i++){
		matrix[i]=(char*)malloc(n*sizeof(char*));
		enqueueMatrix[i]=(bool*)malloc(n*sizeof(bool*));
	}

	//making matrix as empty and enqueue matrix false
	for(int i=0;i<n;i++){
		for(int j=0;j<n;j++){
			matrix[i][j]=' ';
			enqueueMatrix[i][j]=false;
		}
	}
	char symbol;
	int xCoordinate;
	int yCoordinate;
	while(in>>symbol>>xCoordinate>>yCoordinate){  //reading file line by line
		getline(in,fileLine);
		for(int j=0;j<n && j<fileLine.length();j++){
			if(symbol=='X' || symbol==' ' || symbol=='G' ||symbol=='R')  //checking invalid characters
				matrix[xCoordinate][yCoordinate]=symbol;
			else{
				cout<<symbol << " in input file bad character"<<endl;				
				exit(1);
			}
			
		}
	}
}


int main(int argc,char *argv[]){
	bool stackBase=false;
	bool queueBase=false;
	bool optimumBase=false;
	string inputFile="maze.txt";
	string outputFile="mazeout.txt";
	bool timing=false;
	bool inCoordinate=false;
	bool outCoordinate=false;
	bool helpMode=false;
	
	char** matrix;
	bool** enqueueMatrix;


	//reading command line arguments
	for(int i=0;i<argc;i++){
		if(strcmp(argv[i],"-Stack")==0){
			stackBase=true;
		}
		else if(strcmp(argv[i],"-Queue")==0){
			queueBase=true;
		}
		else if(strcmp(argv[i],"-Opt")==0){
			optimumBase=true;
		}
		else if(strcmp(argv[i],"-Infile")==0){
			i++;
			inputFile=argv[i];
			
		}
		else if(strcmp(argv[i],"-Outfile")==0){
			i++;
			outputFile=argv[i];			
		}
		else if(strcmp(argv[i],"-Time")==0){
			timing=true;
		}
		else if(strcmp(argv[i],"-Incoordinate")==0){
			inCoordinate=true;		
		}
		else if(strcmp(argv[i],"-Outcoordinate")==0){
			outCoordinate=true;		
		}
		else if(strcmp(argv[i],"-Help")==0){
			helpMode=true;		
		}

	}

	//if -Help parameter  exists or no parameter entered
	if(helpMode || argc<2 ){
		cout<<"-Stack (boolean) If this switch is set you should use the stack-based routing scheme."<<endl;
		cout<<"-Opt (boolean) If this switch is set you should use your optimal routing scheme."<<endl;
		cout<<"-Queue (boolean) If this switch is set you should use the queue-based routing scheme."<<endl;
		cout<<"-Infile (string) Use the string as the input filename. If the -Infile flag does not appear then you should use the file maze.txt as the input filename."<<endl;
		cout<<"-Outfile (string) Use the string as the output file name. If the -Outfile flag does not appear then you should use the file mazeout.txt as the output filename."<<endl;
		cout<<"-Time (boolean) If this switch is set, print the runtime of your program as described above."<<endl;
		cout<<"-Incoordinate (boolean) If this switch is set, the input file is in the coordinate format. If the switch is not set, the input file is in the map format."<<endl;
		cout<<"-Outcoordinate (boolean) If this switch is set, the output file is to be in the coordinate format. If the switch is not set, the output should be in the map format."<<endl;
		system("pause");
		exit(0);
	}
	//checking entered methods from user this should be 1
	int methodCount=0; 
	if(stackBase){
		methodCount++;
	}
	if(queueBase){
		methodCount++;
	}
	if(optimumBase){
		methodCount++;
	}
	if(methodCount!=1){
		cout<<"You should use only one of -Stack -Queue -Opt parameter as method"<<endl;
		system("pause");
		exit(1);
	}

	int n;

	if(inCoordinate){  //if input file is in coordinate form
		readCoordinateBaseMaze(matrix,enqueueMatrix,inputFile,n);
	}
	else{
		readMatrixBaseMaze(matrix,enqueueMatrix,inputFile,n);
	}
	
	double runtime=0;

	clock_t begin = clock();

	if(stackBase){
		stackBasedSolution(matrix,enqueueMatrix,n,outCoordinate,outputFile);
	}
	else if(queueBase){
		queueBasedSolution(matrix,enqueueMatrix,n,outCoordinate,outputFile);
	}
	else if(optimumBase){
		optimumBasedSolution(matrix,enqueueMatrix,n,outCoordinate,outputFile);
	}

	clock_t end = clock();
	runtime = double(end - begin) / CLOCKS_PER_SEC;


	if(timing){
		cout<<"@runtime = "<<runtime<<" sec"<<endl;
	}

	system("pause");
	return 0;
}