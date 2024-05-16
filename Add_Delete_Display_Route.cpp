
#include <iostream>
#include <string>
#include <cstdlib>

using namespace std;

// Function to add a route
void addRoute(string interface, string destination, string gateway) 
{
    string command = "route add -net " + destination + " gw " + gateway + " " + interface;
    system(command.c_str());
    cout << "Route added successfully." << endl;
}

// Function to delete a route
void deleteRoute(string destination) 
{
    string command = "route delete -net " + destination;
    system(command.c_str());
    cout << "Route deleted successfully." << endl;
}

// Function to display routes
void displayRoutes() 
{
    system("route print");
}

int main() 
{
    int choice;
    string interface, destination, gateway;

    do 
    {
        cout << "1. Add Route" << endl;
        cout << "2. Delete Route" << endl;
        cout << "3. Display Routes" << endl;
        cout << "4. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) 
        {
            case 1:
                cout << "Enter interface: ";
                cin >> interface;
                cout << "Enter destination: ";
                cin >> destination;
                cout << "Enter gateway: ";
                cin >> gateway;
                addRoute(interface, destination, gateway);
                break;
            case 2:
                cout << "Enter destination: ";
                cin >> destination;
                deleteRoute(destination);
                break;
            case 3:
                displayRoutes();
                break;
            case 4:
                cout << "Exiting..." << endl;
                break;
            default:
                cout << "Invalid choice. Try again." << endl;
        }
    } while (choice != 4);

    return 0;
}
