#include "set.h"

/**
 * This function initializes a set without an element and returns it.
 */
Set initSet() {
	Set set = (Set)malloc(sizeof(Set_t));
	int cardinality = 0; //cardinality is zero when initilazing.

	set->elements = NULL;
	set->cardinality = cardinality;

	return set;
}

/**
 * This function creates an integer-typed element and returns it. 
 * It allocates memory for both the element and the integer.
 */
Element createIntegerElement(int data) {
	Element element = (Element)malloc(sizeof(Element_t)); // memory for element
	int* dataAdress = (int*)malloc(sizeof(int)); // memory for integer. it keep with pointer
	*dataAdress = data; // the sign * must be used for take the value.
	element->data = dataAdress;
	element->type = INTEGER;

	return element;
}

/**
 * This function creates an float-typed element and returns it. 
 * It allocates memory for both the element and the float.
 */
Element createFloatElement(float data) {
	Element element = (Element)malloc(sizeof(Element_t)); // memory for element 
	float* dataAdress = (float*)malloc(sizeof(float)); // memory for float. it keep with pointer
	*dataAdress = data; // the sign * must be used for take the value.

	element->data = dataAdress;
	element->type = FLOAT;

	return element;
}

/**
 * This function creates an string-typed element and returns it. 
 * It allocates memory for both the element and the string.
 * This function uses the `strlen` and the `strcpy` functions.
 */
Element createStringElement(char* data) {
	Element element = (Element)malloc(sizeof(Element_t)); // memory for element
	char* string = (char*)malloc(strlen(data) + 1); // strlen() is using for string instead of sizeof() funciton.
	strcpy(string, data); // strcp() is using instead of "=" sign.

	element->data = string;
	element->type = STRING;

	return element;
}

/**
 * This function creates an matrix-point-typed element and returns it. 
 * It allocates memory for both the element and the matrix point.
 */
Element createMatrixPointElement(int x, int y, int data) {
	Element element = (Element)malloc(sizeof(Element_t)); // memory for element
	int* array = (int*)malloc(sizeof(int)*3); // multiplication by 3 because we need to memory for x y values and the number value.

	array[0] = x; // x is the row value of point.
	array[1] = y; // y is the row value of point.
	array[2] = data; // the number at (x,y)
	
	element->data = array;
	element->type = MATRIX_POINT;

	return element;
}

/**
 * This function compares two elements to see if they are identical. 
 * First, it checks if the types are identical. If they are the same, 
 * it checks if the data are the same. However, this function does not 
 * compare addresses because the value from two different addresses 
 * can be the same. It returns one if the elements are the same; 
 * otherwise, it returns zero. This function uses the `strcmp` function.
 */
int isSame(Element e1, Element e2) {
	if (e1->type == e2->type) { // first, the type control
		switch (e1->type){
			case INTEGER:
				return *(int*)e1->data == *(int*)e2->data; // (int*) is keeping the value as integer. the second "*" is dereferencing the pointer.
			case FLOAT:
				return *(float*)e1->data == *(float*)e2->data; // (float*) is keeping the value as float. the second "*" is dereferencing the pointer.
			case STRING:
				return strcmp((char*)e1->data, (char*)e2->data) == 0; // (char*) is keeping the value as char. the second "*" is not used. If we use, the second "*" is dereferencing the pointer for just first char element. We want the all chars. We use strcmp() instead of "==".
			case MATRIX_POINT:{
				int* data1 = (int*)e1->data; // for first matrix point, (int*) is keeping the value as integer to a new pointer.
				int* data2 = (int*)e2->data; // for second matrix point, (int*) is keeping the value as integer to a new pointer.
				return data1[0] == data2[0] && data1[1] == data2[1] && data1[2] == data2[2];
			}
		}
    }
	return 0;
}

/**
 * This function checks if the given element is in the set or not. 
 * It does not compare the addresses. It returns one if the given 
 * element is in the set; otherwise, it returns zero.
 */
int in(Set set, Element element) {
	for (int i=0; i<set->cardinality; i++){
		if (isSame(set->elements[i],element)) { // Is element same an element in the set? Control with for loop.
			return 1;
        }
	}
	return 0;
}


/**
 * This function inserts an element into the given set. 
 * The same element cannot be twice in the same set.
 * It returns one if the inserting element is successful; 
 * otherwise, it returns zero. This function uses the 
 * `realloc` function.
 */
int insertElement(Set set, Element element) {
	if(!in(set,element)){ // if element in set dont insert and return 0.
		set->elements = realloc(set->elements,sizeof(Element_t)*(set->cardinality+1)); // the memory increase by 1 with realloc() function.
		set->elements[set->cardinality] = element; // insert the element in pop of set.
		set->cardinality++; // the cardinality increased after inserting.
		return 1;
	}
	return 0;
}

/**
 * This function removes the given element in the set. 
 * It returns one if removal is successful; otherwise, 
 * it returns zero. This function uses the `realloc` function.
 */
int removeElement(Set set, Element element) {
	for (int i=0; i<set->cardinality; i++){
		if (isSame(set->elements[i], element)){ // Is current element wanted element? When found the removing will start.

			set->cardinality--; // the cardinality will decrease by 1 after removing. If we do that now, cardinality value will be more usable in next.
			for(int j=i; j<set->cardinality; j++){ // it starts on choosen element (i=j). cardinality changed, so j will go until (old cardinality - 1).
				set->elements[j] = set->elements[j+1]; // shift left the all elements after choosen element. 
			}
			set->elements = realloc(set->elements,sizeof(Element_t)*(set->cardinality)); // create memory by new cadinality with realloc function.
			return 1;
		}
	}
	return 0;
}

/**
 * This function creates and returns a new set, which is united of the given sets.
 */
Set unite(Set s1, Set s2) {
	Set newSet = initSet();
	for (int i=0; i<s1->cardinality; i++){
		insertElement(newSet,s1->elements[i]); // insert first sets elements
	}
	for (int j=0; j<s2->cardinality; j++){
		insertElement(newSet,s2->elements[j]); // insert first sets elements. same elements automaticly reject in insertElement() function.
	}
	return newSet;
}

/**
 * This function creates and returns a new set, which is intersected of the given sets.
 */
Set intersect(Set s1, Set s2) {
	Set newSet = initSet();
	for (int i=0; i<s1->cardinality; i++){
			if (in(s2,s1->elements[i])){ // it need to same elements in both of two sets.
				insertElement(newSet,s1->elements[i]); // insert that elements.
			}
	}
	return newSet;
}

/**
 * This function creates and returns a new set, which is 
 * subtracted from the first given set by the second one.
 */
Set substract(Set s1, Set s2) {
	Set newSet = initSet();
	for (int i=0; i<s1->cardinality; i++){
		if(!in(s2,s1->elements[i])){
			insertElement(newSet,s1->elements[i]);
		}
	}
	return newSet;
}

/**
 * This function prints the given element depending on its type.
 */
void printElement(Element element) {
	switch(element->type){
		case INTEGER:
			printf("The integer element is %d\n" , *(int*)element->data); // print integer. the pointer information gave on isSame() function.
			break;
		case FLOAT:
			printf("The float element is %f\n" , *(float*)element->data); // print float
			break;
		case STRING:
			printf("The string element is %s\n" , (char*)element->data); // print string
			break;
		case MATRIX_POINT: {
			int* data = (int*)element->data;
			printf("The matrix point element is at (%d, %d) with value %d\n", data[0], data[1], data[2]); // print coordinates and the value of point.
			break;
		}
	}
}

/**
 * This function prints the given set element by element depending on its type.
 */
void printSet(Set set) {
	for (int i = 0; i < set->cardinality; i++) {
        if (set->cardinality != 0) { // if the set is empty give a warning.
            printElement(set->elements[i]); // use own print function for all element types with printElement() function.
        } 
		else {
            printf("The set is empty"); // the warning
        }
    }

}

/**
 * This element deallocates all data in the given set and the set itself.
 */
void freeSet(Set set) {
	for (int i = 0; i < set->cardinality; i++) {
		if (set->elements[i] != NULL) {
            if (set->elements[i]->data != NULL) { // null control for free
                free(set->elements[i]->data); // free the data
            }
            free(set->elements[i]); // free element
        }	
	}
}

