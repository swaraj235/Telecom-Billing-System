#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

// Define a structure for customer
typedef struct {
    char name[50];
    char phone[15];
    double balance;
    double billHistory[100]; // Array to store bill history
    int billCount;           // Number of bills in history
} Customer;

Customer *customers = NULL;
int customerCount = 0;

// Function prototypes
void addCustomer();
void generateBill();
void viewCustomers();
int searchCustomerByPhone(const char *phone);
int isValidPhone(const char *phone);
void topUpBalance();
void deleteCustomer();
void updateCustomerDetails();
void saveCustomersToFile();
void loadCustomersFromFile();
void displayBillHistory();
void displayMenu();

int main() {
    loadCustomersFromFile(); // Load customers from file at startup

    int choice;
    while (true) {
        displayMenu();
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar(); // Clear buffer

        switch (choice) {
            case 1:
                addCustomer();
                break;
            case 2:
                generateBill();
                break;
            case 3:
                viewCustomers();
                break;
            case 4:
                topUpBalance();
                break;
            case 5:
                deleteCustomer();
                break;
            case 6:
                updateCustomerDetails();
                break;
            case 7:
                displayBillHistory();
                break;
            case 8:
                saveCustomersToFile(); // Save customers to file before exiting
                printf("Exiting program...\n");
                free(customers); // Free dynamically allocated memory
                return 0;
            default:
                printf("Invalid choice! Please try again.\n");
        }
    }
}

void displayMenu() {
    printf("\n--- Telecom Billing System ---\n");
    printf("1. Add New Customer\n");
    printf("2. Generate Bill\n");
    printf("3. View All Customers\n");
    printf("4. Top-Up Balance\n");
    printf("5. Delete Customer\n");
    printf("6. Update Customer Details\n");
    printf("7. View Bill History\n");
    printf("8. Exit\n");
}

void addCustomer() {
    customers = realloc(customers, (customerCount + 1) * sizeof(Customer));
    if (customers == NULL) {
        printf("Memory allocation failed!\n");
        return;
    }

    printf("Enter customer name: ");
    fgets(customers[customerCount].name, 50, stdin);
    customers[customerCount].name[strcspn(customers[customerCount].name, "\n")] = '\0'; // Remove newline

    printf("Enter phone number: ");
    fgets(customers[customerCount].phone, 15, stdin);
    customers[customerCount].phone[strcspn(customers[customerCount].phone, "\n")] = '\0'; // Remove newline

    if (!isValidPhone(customers[customerCount].phone)) {
        printf("The phone number is invalid. It must be a 10-digit number.\n");
        return;
    }

    printf("Enter initial balance: ");
    scanf("%lf", &customers[customerCount].balance);
    getchar(); // Clear buffer

    if (customers[customerCount].balance < 0) {
        printf("Initial balance cannot be negative.\n");
        return;
    }

    customers[customerCount].billCount = 0; // Initialize bill history
    customerCount++;
    printf("Customer added successfully!\n");
}

void generateBill() {
    char phone[15];
    double amount;

    printf("Enter customer's phone number: ");
    fgets(phone, 15, stdin);
    phone[strcspn(phone, "\n")] = '\0'; // Remove newline

    int index = searchCustomerByPhone(phone);
    if (index == -1) {
        printf("Customer not found!\n");
        return;
    }

    printf("Enter bill amount: ");
    scanf("%lf", &amount);
    getchar(); // Clear buffer

    if (amount <= 0) {
        printf("Bill amount must be positive.\n");
        return;
    }

    if (customers[index].balance < amount) {
        printf("Insufficient balance. Cannot generate bill.\n");
    } else {
        customers[index].balance -= amount;
        customers[index].billHistory[customers[index].billCount++] = amount; // Add to bill history
        printf("Bill generated successfully! Remaining balance: %.2lf\n", customers[index].balance);
    }
}

void viewCustomers() {
    if (customerCount == 0) {
        printf("No customers to display.\n");
        return;
    }

    printf("\n--- Customer Records ---\n");
    for (int i = 0; i < customerCount; i++) {
        printf("Name: %s, Phone: %s, Balance: %.2lf\n",
               customers[i].name, customers[i].phone, customers[i].balance);
    }
}

int searchCustomerByPhone(const char *phone) {
    for (int i = 0; i < customerCount; i++) {
        if (strcmp(customers[i].phone, phone) == 0) {
            return i;
        }
    }
    return -1;
}

int isValidPhone(const char *phone) {
    if (strlen(phone) != 10) return 0; // Ensure phone is exactly 10 characters
    for (int i = 0; i < 10; i++) {
        if (!isdigit(phone[i])) return 0; // Ensure all characters are digits
    }
    return 1;
}

void topUpBalance() {
    char phone[15];
    double amount;

    printf("\n--- Top-Up Balance ---\n");
    printf("Enter customer's phone number: ");
    fgets(phone, 15, stdin);
    phone[strcspn(phone, "\n")] = '\0'; // Remove newline

    int index = searchCustomerByPhone(phone);
    if (index == -1) {
        printf("Customer not found!\n");
        return;
    }

    printf("Enter top-up amount: ");
    scanf("%lf", &amount);
    getchar(); // Clear buffer

    if (amount <= 0) {
        printf("Invalid amount! Please enter a positive value.\n");
        return;
    }

    customers[index].balance += amount;
    printf("Balance topped up successfully! New balance: %.2lf\n", customers[index].balance);
}

void deleteCustomer() {
    char phone[15];

    printf("\n--- Delete Customer ---\n");
    printf("Enter customer's phone number: ");
    fgets(phone, 15, stdin);
    phone[strcspn(phone, "\n")] = '\0'; // Remove newline

    int index = searchCustomerByPhone(phone);
    if (index == -1) {
        printf("Customer not found!\n");
        return;
    }

    for (int i = index; i < customerCount - 1; i++) {
        customers[i] = customers[i + 1];
    }

    customerCount--;
    customers = realloc(customers, customerCount * sizeof(Customer));
    printf("Customer deleted successfully!\n");
}

void updateCustomerDetails() {
    char phone[15];

    printf("\n--- Update Customer Details ---\n");
    printf("Enter customer's phone number: ");
    fgets(phone, 15, stdin);
    phone[strcspn(phone, "\n")] = '\0'; // Remove newline

    int index = searchCustomerByPhone(phone);
    if (index == -1) {
        printf("Customer not found!\n");
        return;
    }

    printf("Enter new name: ");
    fgets(customers[index].name, 50, stdin);
    customers[index].name[strcspn(customers[index].name, "\n")] = '\0'; // Remove newline

    printf("Enter new phone number: ");
    fgets(customers[index].phone, 15, stdin);
    customers[index].phone[strcspn(customers[index].phone, "\n")] = '\0'; // Remove newline

    if (!isValidPhone(customers[index].phone)) {
        printf("The phone number is invalid. It must be a 10-digit number.\n");
        return;
    }

    printf("Customer details updated successfully!\n");
}

void displayBillHistory() {
    char phone[15];

    printf("\n--- Bill History ---\n");
    printf("Enter customer's phone number: ");
    fgets(phone, 15, stdin);
    phone[strcspn(phone, "\n")] = '\0'; // Remove newline

    int index = searchCustomerByPhone(phone);
    if (index == -1) {
        printf("Customer not found!\n");
        return;
    }

    if (customers[index].billCount == 0) {
        printf("No bill history available for this customer.\n");
        return;
    }

    printf("Bill History for %s:\n", customers[index].name);
    for (int i = 0; i < customers[index].billCount; i++) {
        printf("Bill %d: %.2lf\n", i + 1, customers[index].billHistory[i]);
    }
}

void saveCustomersToFile() {
    FILE *file = fopen("customers.dat", "wb");
    if (file == NULL) {
        printf("Failed to save customer data!\n");
        return;
    }

    fwrite(&customerCount, sizeof(int), 1, file);
    fwrite(customers, sizeof(Customer), customerCount, file);

    fclose(file);
    printf("Customer data saved successfully!\n");
}

void loadCustomersFromFile() {
    FILE *file = fopen("customers.dat", "rb");
    if (file == NULL) {
        printf("No existing customer data found.\n");
        return;
    }

    fread(&customerCount, sizeof(int), 1, file);
    customers = malloc(customerCount * sizeof(Customer));
    if (customers == NULL) {
        printf("Memory allocation failed!\n");
        fclose(file);
        return;
    }

    fread(customers, sizeof(Customer), customerCount, file);
    fclose(file);
    printf("Customer data loaded successfully!\n");
}
