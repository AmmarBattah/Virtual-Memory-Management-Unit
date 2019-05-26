using namespace std;
#include <iostream>
#include <string>
int cr(int psize, int size, int strat);
void print();
bool deleteblock(int pid2);
int convert(int pid2, int va);
struct block{
	int id,base,limit1;
	char type;
	block* next;
	block* prev;
};
int insertblock(block* ptr, int psize);
block* ListData = NULL;
block* nextb = ListData;
int Size, strat, req, psize, pid1 = 1, va;
int main(){
	string cmnd;
	ListData = new block;
	ListData->id = 0;
	ListData->type = 'H';
	ListData->next = NULL;
	ListData->prev = NULL; 
	cout << "Enter the size of memory needed (in KB)\n";
	cin >> Size;
	cout << "Choose the allocation strategy from the following\n1: First Fit\n2: Next Fit\n3: Best Fit\n4: Worst Fit\n";
	cin>> strat;
	ListData->base = 0;
	ListData->limit1 = Size;
	int pid3;
	while (true){
		cout << "What command would you like the MMU to execute (cr , dl , cv , pm or quit)\n";
		cin >> cmnd;
		if (cmnd == "quit")
			break;
		if (cmnd == "cr"){
			cout << "please enter size of process\n";
			cin >> psize;
			int z = cr(psize, Size, strat);
			if (z == -1)
				cout << "Process couldnt be allocated\n";
			else{
				cout << "process created with id " << pid1 - 1 << ", base of " << z << " and a limit of " << psize << endl;
				Size -= psize;
				cout << "memory left:" << Size << endl;
			}
		}
		else if (cmnd == "dl"){
			cout << "Please provide the Process ID\n";
			cin >> pid3;
			if (deleteblock(pid3)){
				cout << "Process "<<pid3<< " deleted successfully\n";
				cout << "memory left:" << Size << endl;
			}
			else
				cout << "Process doesnt exist\n";
	
		}
		else if (cmnd == "cv"){
			cout << "Please enter Process ID\n";
			cin >> pid3;
			cout << "Please enter the virtual address\n";
			cin	>> va;
			if (convert(pid3, va) == 0)
				cout << "the process does not exist\n";
			else if (convert(pid3, va) == -1)
				cout << "The virtual address is out of process address space\n";
			else
				cout << "The physical memory address is " << convert(pid3, va) << endl;
		}
		else if (cmnd == "pm"){
			print();
		}	
	}
	system("pause");
	return 0;
}
	

int cr(int psize, int size, int strat){
	if (psize <= size){
		block *ptr = ListData;
	
		if(strat==1){
			while (ptr != NULL) {
				if (ptr->type == 'H' && ptr->limit1 >= psize){
					return insertblock(ptr, psize);
				}
				ptr = ptr->next;
			}
		}
		if(strat==2){
			block *ptr2 = nextb;
			block  *ptr4 = nextb;
			do{
				if (ptr2->type == 'H' && ptr2->limit1 >= psize){
					nextb = ptr2;
					return insertblock(ptr2, psize);
				}
				if (ptr2->next == NULL)
					ptr2 = ListData;
				else
					ptr2 = ptr2->next;
			} while (ptr2 != ptr4);
		}
		if(strat==3){
			block *ptr5 = ListData;
			int hsize = size;
			block *best = ListData;
			if (ptr5->type == 'H' && ptr5->limit1 >= psize)
				hsize = ptr5->limit1 - psize;
			ptr5 = ptr5->next;
			while (ptr5 != NULL) {
				if (ptr5->type == 'H' && ptr5->limit1 >= psize && ptr5->limit1 - psize < hsize){
					best = ptr5;
					hsize = ptr5->limit1 - psize;
				}
				ptr5 = ptr5->next;
			}
			if (hsize == size)
				return -1;
			else
				return insertblock(best, psize);
		}
		if(strat==4){
			int hsize2 = 0;
			block* ptr6 = ListData;
			block *worst = ListData;
			if (ptr6->type == 'H'&&ptr6->limit1 >= psize)
				hsize2 = ptr6->limit1 - psize;
			ptr6 = ptr6->next;
			while (ptr6 != NULL) {
				if (ptr6->type == 'H' && ptr6->limit1 >= psize && ptr6->limit1 - psize > hsize2){
					worst = ptr6;
					hsize2 = ptr6->limit1 - psize;
				}
				ptr6 = ptr6->next;
			}
			if (hsize2 == 0)
				return -1;
			else
				return insertblock(worst, psize);
		}
	}
	return -1;
}
int insertblock(block* ptr, int psize){
			block* proc = ptr;
			int base = 0;
			if (ptr->limit1 != psize){
				block* hole = new block;
				hole->type = 'H';
				hole->id = 0;
				hole->base = ptr->base+psize;
				hole->limit1 = ptr->limit1-psize;
				hole->next = ptr->next;
				hole->prev = proc;
				proc->next = hole;
				if (pid1 == 1){
					ListData = proc;
				}
				//cout << " HBAse " << hole->base << "Hlimit " << hole->limit1;
			}
			proc->limit1 = psize;
			proc->type = 'P';
			proc->id = pid1++;
			proc->base = ptr->base;
			proc->prev = ptr->prev;
			base = proc->base;
			 //cout<< " Pbase " << proc->base << "Plimit " << proc->limit1;
			 if (base==0){
				ListData = proc;
			 }
			 return base;
}
bool deleteblock(int pid2){
	block* search = ListData;
	int procsize;
	bool found = false;														
	if (pid2 == 0)
		return false;
	while (search != NULL){													
		if (search->id == pid2){												
			found = true;
			procsize = search->limit1;
			break;														
		}
		search = search->next;
	}
	if (found){															
		block* holes = NULL;													
		search->type = 'H';												
		search->id = 0;
		if (search->prev != NULL){										
			holes = search->prev;
			if (holes->type == 'H'){										
				if (nextb == search)										
					nextb = holes;
				holes->limit1 += search->limit1;								
				holes->next = search->next;									
				if (search->next != NULL)									
					search->next->prev = holes;								
				delete search;												
				search = holes;											
			}
		}
		if (search->next != NULL){											
			holes = search->next;
			if (holes->type == 'H'){											
				if (nextb == holes)										
					nextb = search;
				search->limit1 += holes->limit1;
				search->next = holes->next;
				if (holes->next != NULL)
					holes->next->prev = search;
				delete holes;
			}
		}
		Size += procsize;
		return true;														
	}
	else																	
		return false;
}
int convert(int pid2, int va){
	block* search=ListData;
	bool found = false;
	if (pid2 == 0)
		return 0;//the process doesnt exist
	while (search != NULL){
		if (search->id == pid2){
			found = true;
			break;
		}
		search = search->next;
	}
	if (!found)
		return 0;//the process doesnt exist
	if (va > search->limit1 || va<0)
		return -1;//virtual address out of process address space
	
	return (search->base + va);

}
void print(){
		block* ptr=ListData;
		while (ptr != NULL) {
			cout << ptr->type << ptr->id << " | " << ptr->base << " | " << ptr->limit1 << " | ---> ";
			ptr = ptr->next;
		}
		cout << "End of List\n";
}