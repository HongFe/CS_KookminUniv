
// Preorder Function
int preOrder(const char nodeChar[],int nodeNum){
		
	if (nodeChar[nodeNum] != '0'){
		cout << nodeChar[nodeNum] << " ";
	}

	if(nodeChar[nodeNum*2] != NULL){
		preOrder(nodeChar,nodeNum*2);
	}
	
	if (nodeChar[nodeNum * 2 + 1] != NULL){
		preOrder(nodeChar,nodeNum*2+1);
	}

	return 0;
}

// Inorder Function
int inOrder(const char nodeChar[],int nodeNum){
	
	if (nodeChar[nodeNum * 2] != NULL){
		inOrder(nodeChar,nodeNum*2);
	}
		
	if (nodeChar[nodeNum] != '0'){
		cout << nodeChar[nodeNum] << " ";
	}

	if (nodeChar[nodeNum * 2 + 1] != NULL){
		inOrder(nodeChar,nodeNum*2+1);
	}

	return 0;
}

// Postorder Function
int postOrder(const char nodeChar[], int nodeNum){

	if (nodeChar[nodeNum * 2] != NULL){
		postOrder(nodeChar, nodeNum * 2);
	}

	if (nodeChar[nodeNum * 2 + 1] != NULL){
		postOrder(nodeChar, nodeNum * 2 + 1);
	}

	if (nodeChar[nodeNum] != '0'){
		cout << nodeChar[nodeNum] << " ";
	}

	return 0;
}

