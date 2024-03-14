#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>

using namespace std;

class Task {
public:
    string title;
    string description;
    int priority;
    bool completed;
    Task* dependency;
    Task* next;

    Task(string taskTitle, string taskDesc, int taskPriority)
    {
    	title=taskTitle;
    	description=taskDesc;
    	priority=taskPriority;
    	dependency=NULL;
    	next=NULL;
    	completed=NULL;
	}
        
    Task() 
	{
		dependency=NULL;
    	next=NULL;
    	completed=NULL;
	} 
};

class TaskManager {
private:
    Task* head;

public:
    TaskManager() 
	{
        head = NULL;
    }

    void addTask()
	{
    
    string title, description;
    int priority;

    cout << "Enter Task Title: ";
    cin.ignore();
    getline(cin, title);

    // Check for duplicate task titles
    Task* temp = head;
    while (temp != NULL) {
        if (temp->title == title) {
            cout << "Task with this title already exists. Please enter a different title.\n";
            return;
        }
        temp = temp->next;
    }

    cout << "Enter Task Description: ";
    getline(cin, description);

    // Input validation for priority levels
    do {
        cout << "Enter Task Priority (1-5): ";
        cin >> priority;
        if (priority < 1 || priority > 5) {
            cout << "Invalid priority level. Please enter a number between 1 and 5.\n";
        }
    } while (priority < 1 || priority > 5);

    Task* newTask = new Task(title, description, priority);

    // If the list is empty or the new task has higher priority than the head
    if (head == NULL || priority > head->priority) {
        newTask->next = head;
        head = newTask;
    } else {
        Task* current = head;

        // Traverse the list to find the correct position based on priority
        while (current->next != NULL && priority <= current->next->priority) {
            current = current->next;
        }

        // Insert the new task at the appropriate position in the sorted list
        newTask->next = current->next;
        current->next = newTask;
    }
}

    void printTasks() {
        if (head == NULL) {
            cout << "No tasks added yet.\n";
            return;
        }
        Task* temp = head;
        while (temp != NULL) {
            cout << "Title: " << temp->title << endl;
            cout << "Description: " << temp->description << endl;
            cout << "Priority: " << temp->priority << endl;
            cout << "Completed: " << (temp->completed ? "Yes" : "No") << endl;
            if (temp->dependency != NULL) {
                cout << "Dependency: " << temp->dependency->title << endl;
            } else {
                cout << "Dependency: None\n";
            }
            cout << endl;
            temp = temp->next;
        }
    }

    void markTaskCompleted() {
        string title;
        cout << "Enter the title of the task to mark as completed: ";
        cin.ignore();
        getline(cin, title);

        Task* temp = head;
        while (temp != NULL && temp->title != title) {
            temp = temp->next;
        }
        if (temp == NULL) {
            cout << "Task not found.\n";
            return;
        }
        temp->completed = true;
        cout << "Task marked as completed.\n";
    }

    void addDependency() {
        string title, depTitle;
        cout << "Enter the title of the task: ";
        cin.ignore();
        getline(cin, title);

        cout << "Enter the title of the task it depends on: ";
        getline(cin, depTitle);

        Task* task = head;
        Task* depTask = head;
        while (task != NULL && task->title != title) {
            task = task->next;
        }
        while (depTask != NULL && depTask->title != depTitle) {
            depTask = depTask->next;
        }
        if (task == NULL || depTask == NULL) {
            cout << "Task or its dependency not found.\n";
            return;
        }
        task->dependency = depTask; // Set the dependency pointer
        cout << "Dependency added.\n";
    }
    
    void deleteTask(string taskTitle) {
        Task* current = head;
        Task* prev = NULL;

        while (current != NULL && current->title != taskTitle) {
            prev = current;
            current = current->next;
        }

        if (current == NULL) {
            cout << "Task not found.\n";
            return;
        }

        // If the task to be deleted is the head
        if (prev == NULL) {
            head = current->next;
        } else {
            prev->next = current->next;
        }

        // Update dependencies: remove task as a dependency
        Task* temp = head;
        while (temp != NULL) {
            if (temp->dependency == current) {
                temp->dependency = NULL;
            }
            temp = temp->next;
        }

        delete current;
        cout << "Task '" << taskTitle << "' deleted successfully.\n";
    }

    void displayDependencyChain(string taskTitle) {
        Task* current = head;
        while (current != NULL && current->title != taskTitle) {
            current = current->next;
        }

        if (current == NULL) {
            cout << "Task not found.\n";
            return;
        }

        cout << "Dependency chain for task '" << taskTitle << "':\n";
        displayDependencyHelper(current);
    }

    void displayDependencyHelper(Task* task) {
        if (task == NULL) {
            return;
        }

        cout << task->title << " ";

        // Recursively display dependencies
        displayDependencyHelper(task->dependency);
    }

    
    void taskCount() {
    int count = 0;
    Task* temp = head;
    while (temp != NULL) {
        count++;
        temp = temp->next;
    }
    cout << "Total number of tasks: " << count << endl;
}

void statusSummary() {
    int completedCount = 0, pendingCount = 0, inProgressCount = 0;
    Task* temp = head;
    while (temp != NULL) {
        if (temp->completed) {
            completedCount++;
        } else {
            if (temp->dependency != NULL) {
                inProgressCount++;
            } else {
                pendingCount++;
            }
        }
        temp = temp->next;
    }
    cout << "Completed tasks: " << completedCount << endl;
    cout << "Pending tasks: " << pendingCount << endl;
    cout << "Tasks in progress: " << inProgressCount << endl;
}

    
    void saveTasksToFile() {
    ofstream file("tasks.txt"); // Create or open a file named tasks.txt

    if (file.is_open()) {
        Task* temp = head;
        while (temp != NULL) {
            file << "Task=" << temp->title << endl;
            file << "Task_Description=" << temp->description << endl;
            file << "Priority=" << temp->priority << endl;
            file << "Completed=" << (temp->completed ? "1" : "0") << endl;
            if (temp->dependency != NULL) {
                file << "Dependencies=" << temp->dependency->title << endl;
            } else {
                file << "Dependencies=None\n";
            }
            file << endl;
            temp = temp->next;
        }
        file.close();
        cout << "Tasks saved to file.\n";
    } else {
        cout << "Unable to open file.\n";
    }
}

    void loadTasksFromFile()
	 {
        ifstream file("tasks.txt");
        if (file.is_open()) {
            string taskTitle, taskDesc, depTitle;
            int taskPriority;
            bool taskCompleted;

            while (file >> taskTitle >> taskDesc >> taskPriority >> taskCompleted) {
                Task* newTask = new Task(taskTitle, taskDesc, taskPriority);
                newTask->completed = taskCompleted;

                string dependencyTitle;
                file >> dependencyTitle;
                if (dependencyTitle != "None") {
                    // Find dependency task in the list and assign it
                    Task* temp = head;
                    while (temp != NULL && temp->title != dependencyTitle) {
                        temp = temp->next;
                    }
                    if (temp != NULL) {
                        newTask->dependency = temp;
                    }
                }

                newTask->next = head;
                head = newTask;
            }
            file.close();
            cout << "Tasks loaded from file.\n";
        } else {
            cout << "Unable to open file.\n";
        }
    }
    
    void clearScreen() {
    system("cls"); 
    
}
};

int main() {
    TaskManager manager;
    manager.loadTasksFromFile();

    while (true) {
        int choice;
        cout << "\nTASK MANAGEMENT SYSTEM\n";
        cout << "-----------------------------\n";
        cout << "1. Add Task\n";
        cout << "2. Print Tasks\n";
        cout << "3. Mark Task as Completed\n";
        cout << "4. Add Dependency\n";
        cout << "5. Save Tasks to File\n";
        cout << "6. Task Count\n";
        cout << "7. Status Summary\n";
        cout << "8. Delete Task\n";
        cout << "9. Display Dependency Chain\n";
        cout << "10. Exit\n";
        cout << "-----------------------------\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                manager.clearScreen();
                manager.addTask();
                break;
            case 2:
                manager.clearScreen();
                manager.printTasks();
                break;
            case 3:
                manager.clearScreen();
                manager.markTaskCompleted();
                break;
            case 4:
                manager.clearScreen();
                manager.addDependency();
                break;
            case 5:
                manager.clearScreen();
                manager.saveTasksToFile();
                break;
            case 6:
                manager.clearScreen();
                manager.taskCount();
                break;
            case 7:
                manager.clearScreen();
                manager.statusSummary();
                break;
            case 8: {
                manager.clearScreen();
                string title;
                cout << "Enter the title of the task to delete: ";
                cin.ignore();
                getline(cin, title);
                manager.deleteTask(title);
                break;
            }
            case 9: {
                manager.clearScreen();
                string title;
                cout << "Enter the title of the task to display dependency chain: ";
                cin.ignore();
                getline(cin, title);
                manager.displayDependencyChain(title);
                break;
            }
            case 10:
                return 0;
            default:
                manager.clearScreen();
                cout << "Invalid choice. Please try again.\n";
        }
    }
    return 0;
}

