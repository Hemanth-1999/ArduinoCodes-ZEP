/**************************************************************
 * Project: [Project Name]
 * Description: [Brief description of the project]
 * Author: [Your Name]
 * Date: [Current Date]
 **************************************************************/

#include <TaskManager.h>

// Task IDs
#define TASK_ID_1 1
#define TASK_ID_2 2
#define TASK_ID_3 3

// Task intervals (in milliseconds)
#define TASK_INTERVAL_1 1000
#define TASK_INTERVAL_2 2000
#define TASK_INTERVAL_3 3000

TaskManager taskManager;

// Function prototypes
void task1();
void task2();
void task3();

void setup() {
  // Initialize Serial communication
  Serial.begin(9600);
  
  // Add tasks to the TaskManager
  taskManager.addTask(TASK_ID_1, TASK_INTERVAL_1, task1);
  taskManager.addTask(TASK_ID_2, TASK_INTERVAL_2, task2);
  taskManager.addTask(TASK_ID_3, TASK_INTERVAL_3, task3);
  
  // Start all tasks
  taskManager.startAllTasks();
}

void loop() {
  // Update the TaskManager
  taskManager.update();
}

// Task 1
void task1() {
  // Task 1 code here
  Serial.println("Task 1 executed");
}

// Task 2
void task2() {
  // Task 2 code here
  Serial.println("Task 2 executed");
}

// Task 3
void task3() {
  // Task 3 code here
  Serial.println("Task 3 executed");
}
