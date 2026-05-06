#include <Arduino.h>
#include "esp32-hal-cpu.h"

#define SIZE 1000

// ======== QUICK SORT (шаблон) ========
template <typename T>
void quickSort(T arr[], int left, int right)
{
  int i = left;
  int j = right;
  T pivot = arr[(left + right) / 2];

  while (i <= j)
  {
    while (arr[i] < pivot)
      i++;
    while (arr[j] > pivot)
      j--;

    if (i <= j)
    {
      T temp = arr[i];
      arr[i] = arr[j];
      arr[j] = temp;
      i++;
      j--;
    }
  }

  if (left < j)
    quickSort(arr, left, j);
  if (i < right)
    quickSort(arr, i, right);
}

// ======== ДРУК ========
template <typename T>
void printArray(T arr[], int size)
{
  for (int i = 0; i < size; i++)
  {
    Serial.print(arr[i]);
    Serial.print(" ");
  }
  Serial.println();
}

// ======== МАСИВИ ========
int arrInt[SIZE];
float arrFloat[SIZE];
double arrDouble[SIZE];
char arrChar[SIZE];

// ======== ЗАПОВНЕННЯ ========
void generateArrays()
{
  for (int i = 0; i < SIZE; i++)
  {
    arrInt[i] = random(0, 1000);
    arrFloat[i] = random(0, 1000) / 10.0;
    arrDouble[i] = random(0, 1000) / 10.0;
    arrChar[i] = random(65, 90); // A-Z
  }
}

// ======== БІНАРНЕ ДЕРЕВО ========
template <typename T>
struct Node
{
  T data;
  Node *left;
  Node *right;
};

template <typename T>
Node<T> *createNode(T value)
{
  Node<T> *newNode = new Node<T>();
  newNode->data = value;
  newNode->left = nullptr;
  newNode->right = nullptr;
  return newNode;
}

template <typename T>
Node<T> *insertNode(Node<T> *root, T value)
{
  if (root == nullptr)
    return createNode(value);

  if (value < root->data)
    root->left = insertNode(root->left, value);
  else
    root->right = insertNode(root->right, value);

  return root;
}

template <typename T>
Node<T> *buildTree(T arr[], int size)
{
  Node<T> *root = nullptr;
  for (int i = 0; i < size; i++)
  {
    root = insertNode(root, arr[i]);
  }
  return root;
}

template <typename T>
void inorder(Node<T> *root)
{
  if (root == nullptr)
    return;

  inorder(root->left);
  Serial.print(root->data);
  Serial.print(" ");
  inorder(root->right);
}

void setup()
{
  Serial.begin(115200);
  delay(2000);

  setCpuFrequencyMhz(240); // Можливі 80, 160, 240
  Serial.print("ГЦ: ");
  Serial.println(getCpuFrequencyMhz());

  randomSeed(esp_random());
  generateArrays();

  Serial.println("=== ВХІДНІ МАСИВИ 50 ЕЛЕМЕНТІВ ===");
  Serial.println("INT:");
  printArray(arrInt, SIZE);
  Serial.println("FLOAT:");
  printArray(arrFloat, SIZE);
  Serial.println("DOUBLE:");
  printArray(arrDouble, SIZE);
  Serial.println("CHAR:");
  printArray(arrChar, SIZE);

  // ======== ЧАС СТВОРЕННЯ ДЕРЕВА ========
  unsigned long startTree = micros();
  Node<int> *treeInt = buildTree(arrInt, SIZE);
  Node<float> *treeFloat = buildTree(arrFloat, SIZE);
  Node<double> *treeDouble = buildTree(arrDouble, SIZE);
  Node<char> *treeChar = buildTree(arrChar, SIZE);
  unsigned long endTree = micros();

  Serial.print("\nЧас створення бінарного дерева (мкс): ");
  Serial.println(endTree - startTree);

  // ======== ПЕРЕВІРКА (inorder) ========
  Serial.println("\n=== INORDER TREE ===");
  Serial.println("INT TREE:");
  inorder(treeInt);
  Serial.println();
  Serial.println("FLOAT TREE:");
  inorder(treeFloat);
  Serial.println();
  Serial.println("DOUBLE TREE:");
  inorder(treeDouble);
  Serial.println();
  Serial.println("CHAR TREE:");
  inorder(treeChar);
  Serial.println();

  // ======== ЧАС СОРТУВАННЯ ========
  unsigned long startSort = micros();
  quickSort(arrInt, 0, SIZE - 1);
  quickSort(arrFloat, 0, SIZE - 1);
  quickSort(arrDouble, 0, SIZE - 1);
  quickSort(arrChar, 0, SIZE - 1);
  unsigned long endSort = micros();

  Serial.print("\nЧас сортування масивів (мкс): ");
  Serial.println(endSort - startSort);

  // ======== ВИВІД ПІСЛЯ СОРТУВАННЯ ========
  Serial.println("\n=== СОРТОВАНІ МАСИВИ ===");
  Serial.println("INT:");
  printArray(arrInt, SIZE);
  Serial.println("FLOAT:");
  printArray(arrFloat, SIZE);
  Serial.println("DOUBLE:");
  printArray(arrDouble, SIZE);
  Serial.println("CHAR:");
  printArray(arrChar, SIZE);
}

void loop() {}