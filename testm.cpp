#include<bits/stdc++.h>
#include<ctime>
using namespace std;

struct message {
	int task_number;
	int size;
	int response;
	int period;
	int ind_jitter;
	int dir_jitter;
	int no_of_links;
	int message_time;
};

struct task {
	int number;
	int period;
	int deadline;
	//int priority;
	int comp_time;
	//int comp_time_mid;
	//int comp_time_output;
	int response;
	int path[4];
	int ind_jitter;
	int dir_jitter;
	message task_message;
	int task_time;
};

struct node {
	int number;
	vector<task> tasks_at_node;
};

struct link {
	int source;
	int destination;
	vector<message> messages_at_link;
}; 

void inputTasks(task tasks[],int no_of_tasks,int n) {
	for (int i=0;i<no_of_tasks;i++) {
		//cin >> tasks[i].number >> tasks[i].period >> tasks[i].path[0] >> tasks[i].path[1] >> tasks[i].path[2] >> tasks[i].path[3];
		tasks[i].number=i;
		tasks[i].period=(rand()%50)+90;
		tasks[i].deadline=tasks[i].period;
		tasks[i].path[0]=rand()%n;
		tasks[i].path[1]=(rand()%(n*(n-2)))+n;
		tasks[i].path[2]=(rand()%(n*n-n-1-(tasks[i].path[1]/n)*n))+((tasks[i].path[1]/n)*n);
		tasks[i].path[3]=(rand()%n)+(n*n-n);
		tasks[i].task_time=0;
		tasks[i].task_message.message_time=0;
		/*tasks[i].comp_time_input = rand()%2+1;
		tasks[i].comp_time_mid = rand()%2+2;
		tasks[i].comp_time_output = rand()%2+1;*/
		tasks[i].task_message.task_number = tasks[i].number;
		tasks[i].task_message.size=1;
		tasks[i].task_message.response=0;
		tasks[i].response = 0;
		tasks[i].dir_jitter = 1;
		tasks[i].ind_jitter = 0;
		tasks[i].task_message.period=tasks[i].period;
		tasks[i].task_message.ind_jitter=0;
		tasks[i].task_message.dir_jitter=tasks[i].dir_jitter; 
		tasks[i].task_message.no_of_links=0;
		cout << tasks[i].number << " " << tasks[i].period << " " << tasks[i].path[0]<<" " << tasks[i].path[1]<<" "<<tasks[i].path[2]<<" "<<tasks[i].path[3] <<" "<<tasks[i].task_message.size<< endl;
	}
}

node getNode(node nodes[],int size,int n) {
	for (int i=0;i<size;i++) {
		if (nodes[i].number == n) return nodes[i];
	}
}

// this function fills the tasks at each node.

void fillAtNodeTasks(task tasks[],node nodes[], int n,int m) {
	for (int i=0;i<n;i++) {
		node tempNode;
		for (int j=0;j<4;j++) {
			nodes[tasks[i].path[j]].tasks_at_node.push_back(tasks[i]);
		}
	}
}

// this function is a test of topology being correct.

void test(node nodes[],int n) {
	for (int i=0;i<n;i++) {
		if (nodes[i].tasks_at_node.empty()) {
			cout << "none";
		}
		for (int j=0;j<nodes[i].tasks_at_node.size();j++) {
			cout << nodes[i].tasks_at_node[j].number << " ";
		}
		cout << endl;
	}
}

//this function fills the possibilities of links. WORKING

vector<link> fillLinks(vector<link> links,int n) {
	int i=0;
	while(i!=(n*n)-n) {
		link temp;
		temp.source=i;
		if (i%n==0) {
			temp.destination=i+1;
			links.push_back(temp);
			temp.destination=i+n;
			links.push_back(temp);
		}
		else if (i%n==n-1) {
			temp.destination=i-1;
			links.push_back(temp);
			temp.destination=i+n;
			links.push_back(temp);
		}
		else {
			temp.destination=i-1;
			links.push_back(temp);
			temp.destination=i+1;
			links.push_back(temp);
			temp.destination=i+n;
			links.push_back(temp);
		}
		i++;
	}
	while(i!=n*n) { 
		link temp; 
		temp.source=i;
		if (i%n==0) {
			temp.destination=i+1;
			links.push_back(temp);
		}
		else if (i%n==n-1) {
			temp.destination=i-1;
			links.push_back(temp);
		}
		else {
			temp.destination=i-1;
			links.push_back(temp);
			temp.destination=i+1;
			links.push_back(temp);
		}
		i++;
	}
	//for (int j=0;j<links.size();j++) {
	//	cout << links[j].source << " " << links[j].destination << endl;
	//}
	return links;
}

// this function primitively checks for utilisation at each node. If it exceeds 1, then return negative.

bool primitiveCheck(node nodes[],int i) {
	float checksum=0.0;
	for (int j=0;j<nodes[i].tasks_at_node.size();j++) {
		checksum = checksum + ((float)nodes[i].tasks_at_node[j].comp_time/(float)nodes[i].tasks_at_node[j].period);
	}
	if (checksum<=1.0) return true;
	else return false;
}

bool higherPriority(task t1,task t2) {
	if (t2.deadline < t1.deadline) return true;
	else return false;
}

bool meetsDeadline(task t) {
	if (t.deadline >= t.response) return true;
	else return false;
}

int ceil(int a, int b) {
	if (a%b == 0) return a/b;
	else return (a/b)+1;
}

// this function replaces the highest priority task from k to end at k.
 
void highestPriority(node nodes[],int i, int k) {
	task temp=nodes[i].tasks_at_node[k];
	int tempIndex=0;
	for (int j=k;j<nodes[i].tasks_at_node.size();j++) {
		if (higherPriority(temp,nodes[i].tasks_at_node[j])) {
		 	temp = nodes[i].tasks_at_node[j];
			tempIndex = j;
		}
	}
	nodes[i].tasks_at_node[tempIndex] = nodes[i].tasks_at_node[k];
	nodes[i].tasks_at_node[k] = temp;
}

// this function calculates the summation in the response time formula

int max(int a,int b) {
	if (a>b)
		return a;
	else return b;
}

int timeCalculator(node nodes[],int i,int k,int temp[],int index) {
	int ans=0;
	for (int j=0;j<k;j++) {
		nodes[i].tasks_at_node[j].ind_jitter = max(0,nodes[i].tasks_at_node[j].response - nodes[i].tasks_at_node[j].comp_time);
		ans = ans + (ceil((temp[index] + nodes[i].tasks_at_node[j].dir_jitter + nodes[i].tasks_at_node[j].ind_jitter),nodes[i].tasks_at_node[j].period)) * nodes[i].tasks_at_node[j].comp_time; 
	}
	return ans;
}

int timeCalculatorMessages(vector<link> links, int i,int k,int temp[],int index) {
	int ans=0;
	for (int j=0;j<k;j++) {
		links[i].messages_at_link[j].ind_jitter = max(0,links[i].messages_at_link[j].response - links[i].messages_at_link[j].size);
		ans = ans + (ceil((temp[index] + links[i].messages_at_link[j].dir_jitter + links[i].messages_at_link[j].ind_jitter),links[i].messages_at_link[j].period)) * links[i].messages_at_link[j].size;
	}
	return ans;
}

int highestIndex(node topology[],int i,int k) {
	int index=k;
	for (int a=k;a<topology[i].tasks_at_node.size();a++) {
		if (topology[i].tasks_at_node[a].deadline<topology[i].tasks_at_node[index].deadline) index=a;
	}
	return index;
}

int highestIndex0(vector<link> links,int i, int j) {
	int index=j;
	for (int a=j;a<links[i].messages_at_link.size();a++) {
		if (links[i].messages_at_link[a].period<links[i].messages_at_link[index].period) index=a;
	}
	return index;
}

//this function sorts tasks on the basis of priority.

void sortTasks(node topology[],int i) {
	for (int j=0;j<topology[i].tasks_at_node.size();j++) {
		if (j!=topology[i].tasks_at_node.size()-1) {
			int index=highestIndex(topology,i,j);
			task temp=topology[i].tasks_at_node[j];
			topology[i].tasks_at_node[j]=topology[i].tasks_at_node[index];
			topology[i].tasks_at_node[index]=temp;
		}
	} 
}

vector<link> sortMessages(vector<link> links,int i) {
	for (int j=0;j<links[i].messages_at_link.size();j++) {
		if (j!=links[i].messages_at_link.size()-1) {
			int index=highestIndex0(links,i,j);
			message temp=links[i].messages_at_link[j];
			links[i].messages_at_link[j]=links[i].messages_at_link[index];
			links[i].messages_at_link[index]=temp;
		}
	}
	return links;
}

//this function returns the index of the corresponding link.
//WORKING

int getIndex(int s, int d, vector<link> links) {
	int j=0;
	for (int j=0;j<links.size();j++) {
		if (links[j].source==s) {
			if (links[j].destination==d) return j;
		}
	}
}

//this function selects a path for the message transfer and adds the message to the corresponding link.
//WORKING

vector<link> path(int s,int d, int n,task tasks[],int i, vector<link> links) {
	if (s==d) return links;
	int temp_s=s;
	int temp_d;
	int count=(d/n)-(s/n);
	for (int j=0;j<count;j++) {
		temp_d=temp_s+n;
		//cout << temp_s << " " << temp_d << endl;
		int temp_n = getIndex(temp_s,temp_d,links);
		//cout << temp_n << endl;
		tasks[i].task_message.no_of_links++;
		links[temp_n].messages_at_link.push_back(tasks[i].task_message);
		temp_s=temp_d;
	}
	while(temp_d!=d) {
		if (temp_d<d)
			temp_d=temp_s+1;
		if (temp_d>d) 
			temp_d=temp_s-1;
		//cout << temp_s << " " << temp_d << endl;
		int temp_n=getIndex(temp_s,temp_d,links);
		//cout << temp_n << endl;
		tasks[i].task_message.no_of_links++;
		links[temp_n].messages_at_link.push_back(tasks[i].task_message);
		temp_s=temp_d;
	}
	return links;
}
	
int main() {
	srand(time(NULL));
	int n=5;
	//cin >> n;
	int no_of_nodes=n*n;
	//int no_of_tasks=(rand()%20)+30;
	int no_of_tasks=5;
	//cin >> no_of_tasks;
	int t=1;
	int finalCount=0;
	while(t--) {
		node topology[no_of_nodes];
		task tasks[no_of_tasks];
		vector<link> links;
		links=fillLinks(links,n);
		srand(time(NULL));
		//cout << links.size() << endl;
		/*for (int i=0;i<links.size();i++) {
			cout << links[i].source << " " << links[i].destination << endl;
		}*/
		inputTasks(tasks,no_of_tasks,n); 
		fillAtNodeTasks(tasks,topology,no_of_tasks,no_of_nodes);
		for (int i=0;i<no_of_nodes;i++) {
			sortTasks(topology,i);
		}
		
		for (int i=0;i<no_of_tasks;i++) {
			links=path(tasks[i].path[0],tasks[i].path[1],n,tasks,i,links);
			links=path(tasks[i].path[1],tasks[i].path[2],n,tasks,i,links);
			links=path(tasks[i].path[2],tasks[i].path[3],n,tasks,i,links);
		}

		for (int i=0;i<links.size();i++) {
				links=sortMessages(links,i);
		}
		
		for (int i=0;i<links.size();i++) {
			if (links[i].messages_at_link.size()==0) cout << "NULL";
			for (int j=0;j<links[i].messages_at_link.size();j++) {
				cout << links[i].messages_at_link[j].task_number << " ";
			}
			cout << endl;
		}
	
		for (int i=0;i<no_of_nodes;i++) {
			for (int j=0;j<topology[i].tasks_at_node.size();j++) {
				if (i>=0 && i<n) topology[i].tasks_at_node[j].comp_time=(rand()%2)+1;
				else if (i>=n && i<n*n-n) topology[i].tasks_at_node[j].comp_time=(rand()%2)+4;
				else topology[i].tasks_at_node[j].comp_time=(rand()%2)+1;
				cout << topology[i].tasks_at_node[j].comp_time << "(" << topology[i].tasks_at_node[j].number << ") ";
			}
			cout << endl;
		}
		//test(topology,no_of_nodes);
		int flag=1;
		for (int i=0;i<no_of_nodes;i++) {
			flag=flag*primitiveCheck(topology,i);
		}
		if (flag==0) {
			cout << "NOT SCHEDULABLE" << endl;
			return 0;
		}
		for (int i=0;i<no_of_nodes;i++) {
			
			
			for (int k=0;k<topology[i].tasks_at_node.size();k++) {
				int temp[100];
				int index=0;
				temp[index]=topology[i].tasks_at_node[k].comp_time;
				//cout << temp[index] << endl;
				//for (int a=0;a<k;a++) {
				//}
				while (1) {
					temp[index+1]=topology[i].tasks_at_node[k].comp_time + timeCalculator(topology,i,k,temp,index);
					if (temp[index]==temp[index+1]) {
						topology[i].tasks_at_node[k].response = topology[i].tasks_at_node[k].response + temp[index];
						tasks[topology[i].tasks_at_node[k].number].response = tasks[topology[i].tasks_at_node[k].number].response + topology[i].tasks_at_node[k].response;
						tasks[topology[i].tasks_at_node[k].number].task_time = tasks[topology[i].tasks_at_node[k].number].task_time + topology[i].tasks_at_node[k].response;
						break;
					}				
					index++;
				}
			}
			
		}
		for (int i=0;i<links.size();i++) {
			for (int k=0;k<links[i].messages_at_link.size();k++) {
				int temp[100];
				int index=0;
				temp[index]=links[i].messages_at_link[k].size;
				while (1) {
					temp[index+1]=links[i].messages_at_link[k].size + timeCalculatorMessages(links,i,k,temp,index);
					if (temp[index]==temp[index+1]) {
						links[i].messages_at_link[k].response = links[i].messages_at_link[k].response + temp[index];
						tasks[links[i].messages_at_link[k].task_number].response = tasks[links[i].messages_at_link[k].task_number].response + links[i].messages_at_link[k].response;
						tasks[links[i].messages_at_link[k].task_number].task_message.message_time=tasks[links[i].messages_at_link[k].task_number].task_message.message_time + links[i].messages_at_link[k].response;
						break;
					}
					index++;
				}
			}
		}
		for (int i=0;i<no_of_tasks;i++) {
			tasks[i].response=tasks[i].response-(tasks[i].task_message.no_of_links-3)*tasks[i].task_message.size;
			tasks[i].task_message.message_time = tasks[i].task_message.message_time - (tasks[i].task_message.no_of_links-3)*tasks[i].task_message.size;
		}
		cout << no_of_tasks << endl;
		int final_flag=1;
		int count=0;
		for (int i=0;i<no_of_tasks;i++) {
			if (tasks[i].response > tasks[i].deadline){
				final_flag=final_flag*0;
				count++;
			}
			cout << tasks[i].response << " " << tasks[i].deadline << endl;
			cout << "The time taken by tasks is " << tasks[i].task_time << ". The time taken by messages is " << tasks[i].task_message.message_time << endl;
		}
		cout << count << endl;
		float a=(float)no_of_tasks-(float)count;
		float b=(float)no_of_tasks;
		float sched =a/b;
		cout << sched * 100.00 << endl;
		if (final_flag==0) cout << "NOT SCHEDULABLE" << endl;
		else cout << "SCHEDULABLE" << endl;
		using namespace std::this_thread;
		using namespace std::chrono;
		sleep_for(nanoseconds(1000000000));
		//no_of_tasks++;
		if (count==0) finalCount++;
	}
	cout << "The final result is- " << finalCount << endl;
	
	return 0;
}
